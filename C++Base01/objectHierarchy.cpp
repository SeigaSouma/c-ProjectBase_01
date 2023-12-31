//=============================================================================
// 
//  階層オブジェクト処理 [objectHierarchy.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectHierarchy.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CObjectHierarchy::Load CObjectHierarchy::m_aLoadData[mylib_const::MAX_MODEL] = {};	// 読み込みデータ
int CObjectHierarchy::m_nNumLoad = 0;	// 読み込んだ数

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectHierarchy::CObjectHierarchy(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	D3DXMatrixIdentity(&m_mtxWorld);			// ワールドマトリックス
	m_posOrigin = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 最初の位置
	m_fRadius = 0.0f;			// 半径
	m_nNumModel = 0;			// モデルの数
	m_nIdxFile = 0;				// ファイルのインデックス番号

	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;	// モデル(パーツ)のポインタ
	}
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectHierarchy::~CObjectHierarchy()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObjectHierarchy *CObjectHierarchy::Create(const std::string pTextFile)
{
	// 生成用のオブジェクト
	CObjectHierarchy *pObjChara = NULL;

	if (pObjChara == NULL)
	{// NULLだったら

		// メモリの確保
		pObjChara = DEBUG_NEW CObjectHierarchy;

		if (pObjChara != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			HRESULT hr = pObjChara->SetCharacter(pTextFile);
			if (FAILED(hr))
			{// 失敗していたら
				return NULL;
			}
		}

		return pObjChara;
	}

	return NULL;
}

//==========================================================================
// キャラ作成
//==========================================================================
HRESULT CObjectHierarchy::SetCharacter(const std::string pTextFile)
{
	// 初期化処理
	if (FAILED(CObjectHierarchy::Init()))
	{// 失敗していたら
		return E_FAIL;
	}

	// 読み込み確認
	for (int nCntData = 0; nCntData < m_nNumLoad; nCntData++)
	{
		if (m_aLoadData[nCntData].sTextFile == pTextFile)
		{// ファイル名が一致している

			// オブジェクト毎のデータ割り当て
			BindObjectData(nCntData);
			return S_OK;
		}
	}

	if (FAILED(CObjectHierarchy::ReadText(pTextFile)))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// オブジェクト毎のデータ割り当て
//==========================================================================
void CObjectHierarchy::BindObjectData(int nCntData)
{
	// モデル数設定
	m_nNumModel = m_aLoadData[nCntData].nNumModel;

	// 最初の位置設定
	m_posOrigin = m_aLoadData[nCntData].posOrigin;

	// 半径
	m_fRadius = m_aLoadData[nCntData].fRadius;

	// ファイルのインデックス番号
	m_nIdxFile = nCntData;

	for (int nCntParts = 0; nCntParts < m_aLoadData[nCntData].nNumModel; nCntParts++)
	{// パーツ分繰り返し

		// モデルの生成
		m_apModel[nCntParts] = CModel::Create(
			m_aLoadData[nCntData].LoadData[m_aLoadData[nCntData].LoadData[nCntParts].nType].pModelFile.c_str(),
			m_aLoadData[nCntData].LoadData[nCntParts].pos,
			m_aLoadData[nCntData].LoadData[nCntParts].rot);

		if (m_apModel[nCntParts] == NULL)
		{// NULLだったら

			// モデルの終了処理
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = NULL;
		}

		// 親モデルの設定
		if (m_aLoadData[nCntData].LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[m_aLoadData[nCntData].LoadData[nCntParts].nParent]);
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(NULL);
		}

		// 初期配置の判定
		if (m_aLoadData[nCntData].LoadData[nCntParts].nStart != 1)
		{
			ChangeObject(nCntParts, -1);
		}
	}
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectHierarchy::Init(void)
{
	
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectHierarchy::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLじゃなかったら

			// モデルの終了処理
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectHierarchy::Update(void)
{

}

//==========================================================================
// モデル変更
//==========================================================================
void CObjectHierarchy::ChangeObject(int nDeleteParts, int nNewParts)
{
	// ファイル情報取得
	Load LoadData = GetLoadData(m_nIdxFile);

	// オブジェクトキャラクターの情報取得
	CObjectHierarchy *pObjChar = GetObjectHierarchy();

	int nNumAll = pObjChar->GetNumModel();

	if (m_apModel[nDeleteParts] != NULL)
	{// NULLじゃなかったら

		// モデルの終了処理
		m_apModel[nDeleteParts]->Uninit();
		delete m_apModel[nDeleteParts];
		m_apModel[nDeleteParts] = NULL;
	}

	// 新しいパーツを読み込む
	if (nNewParts >= 0 && m_apModel[nNewParts] == NULL)
	{
		if (m_apModel[nNewParts] == NULL)
		{
			// モデル作成
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}

		if (m_apModel[nNewParts] == NULL)
		{// NULLだったら

			// モデルの終了処理
			m_apModel[nNewParts]->Uninit();
			delete m_apModel[nNewParts];
			m_apModel[nNewParts] = NULL;
		}
	}

	// 親モデルの設定
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (m_apModel[nCntParts] == NULL)
		{// NULLだったら
			continue;
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(NULL);
		}
	}
}

//==========================================================================
// 切り替えの種類
//==========================================================================
void CObjectHierarchy::ChangeObject(int nSwitchType)
{
	// ファイル情報取得
	Load LoadData = GetLoadData(m_nIdxFile);

#if _DEBUG
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (m_apModel[nCntParts] != NULL)
		{
			// モデルの終了処理
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = NULL;
		}

		// モデル作成
		if (m_apModel[nCntParts] == NULL)
		{
			m_apModel[nCntParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nCntParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nCntParts].pos,
				LoadData.LoadData[nCntParts].rot);
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(NULL);
		}

		if (LoadData.LoadData[nCntParts].nStart != 1)
		{
			ChangeObject(nCntParts, -1);
		}
	}
#endif

	// モデルの切り替え
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (LoadData.LoadData[nCntParts].nSwitchType != nSwitchType)
		{// 切り替えの種類が違うとき
			continue;
		}

		// 削除するインデックス番号
		int nDeleteIdx = LoadData.LoadData[nCntParts].nIDSwitchModel;

		if (nDeleteIdx >= 0 && m_apModel[nDeleteIdx] != NULL)
		{// NULLじゃなかったら

			// モデルの終了処理
			m_apModel[nDeleteIdx]->Uninit();
			delete m_apModel[nDeleteIdx];
			m_apModel[nDeleteIdx] = NULL;
		}

		// 生成するインデックス番号
		int nNewIdx = nDeleteIdx;

		if (nNewIdx < 0)
		{
			nNewIdx = nCntParts;
		}

		// モデル作成
		if (m_apModel[nNewIdx] == NULL)
		{
			m_apModel[nNewIdx] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nCntParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nCntParts].pos,
				LoadData.LoadData[nCntParts].rot);
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nNewIdx]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// 自分が親の時
			m_apModel[nNewIdx]->SetParent(NULL);
		}
	}

}

//==========================================================================
// モデル設定
//==========================================================================
void CObjectHierarchy::SetObject(int nNewParts)
{
	// ファイル情報取得
	Load LoadData = GetLoadData(m_nIdxFile);

	// 新しいパーツを読み込む
	if (nNewParts >= 0 && m_apModel[nNewParts] == NULL)
	{
		if (m_apModel[nNewParts] == NULL)
		{
			// モデル作成
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}

		if (m_apModel[nNewParts] == NULL)
		{// NULLだったら

			// モデルの終了処理
			m_apModel[nNewParts]->Uninit();
			delete m_apModel[nNewParts];
			m_apModel[nNewParts] = NULL;
		}
	}

	// 親モデルの設定
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (m_apModel[nCntParts] == NULL)
		{// NULLだったら
			continue;
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(NULL);
		}
	}
}

//==========================================================================
// モデル削除
//==========================================================================
void CObjectHierarchy::DeleteObject(int nSwitchType)
{
	// ファイル情報取得
	Load LoadData = GetLoadData(m_nIdxFile);

	// モデルの切り替え
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (LoadData.LoadData[nCntParts].nSwitchType != nSwitchType)
		{// 切り替えの種類が違うとき
			continue;
		}

		// 削除するインデックス番号
		int nDeleteIdx = LoadData.LoadData[nCntParts].nIDSwitchModel;

		if (nDeleteIdx >= 0 && m_apModel[nDeleteIdx] != NULL)
		{// NULLじゃなかったら

			// モデルの終了処理
			m_apModel[nDeleteIdx]->Uninit();
			delete m_apModel[nDeleteIdx];
			m_apModel[nDeleteIdx] = NULL;
		}
	}
}

//==========================================================================
// ワールドマトリックスの計算処理
//==========================================================================
void CObjectHierarchy::CalWorldMtx(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス宣言

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectHierarchy::Draw(void)
{
	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// モデルの描画
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLじゃなかったら

			// パーツごとの描画
			m_apModel[nCntModel]->Draw();
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectHierarchy::Draw(D3DXCOLOR col)
{
	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// モデルの描画
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLじゃなかったら

			// パーツごとの描画
			m_apModel[nCntModel]->Draw(col);
		}
	}
}


//==========================================================================
// 描画処理
//==========================================================================
void CObjectHierarchy::Draw(float fAlpha)
{
	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// モデルの描画
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLじゃなかったら

			// パーツごとの描画
			m_apModel[nCntModel]->Draw(fAlpha);
		}
	}
}

//==========================================================================
// テキスト読み込み処理
//==========================================================================
HRESULT CObjectHierarchy::ReadText(const std::string pTextFile)
{
	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(pTextFile.c_str(), "r");

	if (pFile == NULL)
	{//ファイルが開けた場合
		return E_FAIL;
	}

	// ファイル名保存
	m_aLoadData[m_nNumLoad].sTextFile = pTextFile;

	// ファイルのインデックス番号
	m_nIdxFile = m_nNumLoad;

	char aComment[MAX_COMMENT];	// コメント
	int nCntFileName = 0;
	int nCntSetParts = 0;

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデル数の設定
		if (strcmp(aComment, "NUM_MODEL") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &m_nNumModel);	// モデル数
			m_aLoadData[m_nNumLoad].nNumModel = m_nNumModel;
		}

		while (nCntFileName != m_nNumModel)
		{// モデルの数分読み込むまで繰り返し

			// 文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			// モデル名の設定
			if (strcmp(aComment, "MODEL_FILENAME") == 0)
			{// NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// ファイル名保存
				m_aLoadData[m_nNumLoad].LoadData[nCntFileName].pModelFile = aComment;

				nCntFileName++;	// ファイル数加算
			}
		}

		// 各パーツの設定
		if (strcmp(aComment, "CHARACTERSET") == 0)
		{// キャラクター情報の読み込みを開始

			while (strcmp(aComment, "END_CHARACTERSET") != 0)
			{// END_CHARACTERSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	//確認する

				// オブジェクト毎のデータ読み込み
				LoadObjectData(pFile, aComment);

				// パーツ毎のデータ読み込み
				LoadPartsData(pFile, aComment, &nCntSetParts);
			}
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	// 読み込んだ数加算
	m_nNumLoad++;

	return S_OK;
}

//==========================================================================
// オブジェクト毎のデータ読み込み
//==========================================================================
void CObjectHierarchy::LoadObjectData(FILE *pFile, const char* pComment)
{
	char hoge[MAX_COMMENT];	// コメント
	if (strcmp(pComment, "RADIUS") == 0)
	{// RADIUSで半径

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fRadius);	// 半径
		m_fRadius = m_aLoadData[m_nNumLoad].fRadius;
	}
}

//==========================================================================
// パーツ毎のデータ読み込み
//==========================================================================
void CObjectHierarchy::LoadPartsData(FILE* pFile, const char* pComment, int *pCntParts)
{
	char CheckText[MAX_COMMENT];	// チェック用
	char hoge[MAX_COMMENT];	// コメント

	int nCntSetParts = *pCntParts;

	if (strcmp(pComment, "PARTSSET") == 0)
	{// PARTSSETでパーツ情報の読み込み開始

		while (strcmp(CheckText, "END_PARTSSET") != 0)
		{// END_PARTSSETが来るまで繰り返す

			fscanf(pFile, "%s", &CheckText[0]);	// 確認する

			if (strcmp(CheckText, "INDEX") == 0)
			{// INDEXが来たらモデル番号読み込み

				fscanf(pFile, "%s", &hoge[0]);	// =の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType);		// モデル種類の列挙
			}

			if (strcmp(CheckText, "PARENT") == 0)
			{// PARENTが来たら親の番号読み込み

				fscanf(pFile, "%s", &hoge[0]);	// =の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent);		// 親の番号
			}

			if (strcmp(CheckText, "POS") == 0)
			{// POSが来たら位置読み込み

				fscanf(pFile, "%s", &hoge[0]);		// =の分
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.x);	// X座標
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.y);	// Y座標
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.z);	// Z座標

				if (nCntSetParts == 0)
				{
					m_posOrigin = m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos;	// 最初の位置
					m_aLoadData[m_nNumLoad].posOrigin = m_posOrigin;	// 最初の位置
				}
			}

			if (strcmp(CheckText, "ROT") == 0)
			{// ROTが来たら向き読み込み

				fscanf(pFile, "%s", &hoge[0]);		//=の分
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.x);	// Xの角度
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.y);	// Yの角度
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.z);	// Zの角度
			}

			if (strcmp(CheckText, "START") == 0)
			{// STARTが来たら初期状態

				fscanf(pFile, "%s", &hoge[0]);		//=の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart);	// 最初からあるか
			}

			if (strcmp(CheckText, "SWITCHTYPE") == 0)
			{// SWITCHTYPEが来たら切り替えの種類

				fscanf(pFile, "%s", &hoge[0]);		//=の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nSwitchType);	// 切り替えの種類
			}

			if (strcmp(CheckText, "ID_SWITCHMODEL") == 0)
			{// ID_SWITCHMODELが来たら切り替えるモデルID

				fscanf(pFile, "%s", &hoge[0]);		//=の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nIDSwitchModel);	// 切り替えるモデルID
			}

		}// END_PARTSSETのかっこ

		// モデルの生成
		if (m_apModel[nCntSetParts] == NULL)
		{
			m_apModel[nCntSetParts] = CModel::Create(
				m_aLoadData[m_nNumLoad].LoadData[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType].pModelFile.c_str(),
				m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos,
				m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot);
		}

		if (m_apModel[nCntSetParts] == NULL)
		{// NULLだったら

			// モデルの終了処理
			m_apModel[nCntSetParts]->Uninit();
			delete m_apModel[nCntSetParts];
			m_apModel[nCntSetParts] = NULL;
		}

		// 親モデルの設定
		if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent >= 0)
		{
			m_apModel[nCntSetParts]->SetParent(m_apModel[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent]);
		}
		else
		{
			m_apModel[nCntSetParts]->SetParent(NULL);
		}

		if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart != 1)
		{
			ChangeObject(nCntSetParts, -1);
		}

		(*pCntParts)++;	// パーツのカウントを加算
	}
}

//==========================================================================
// マトリックス設定
//==========================================================================
void CObjectHierarchy::SetmtxWorld(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// マトリックス取得
//==========================================================================
D3DXMATRIX CObjectHierarchy::GetmtxWorld(void) const
{
	return m_mtxWorld;
}

//==========================================================================
// 中心の位置取得
//==========================================================================
MyLib::Vector3 CObjectHierarchy::GetCenterPosition(void) const
{
	if (m_apModel[0] == NULL)
	{
		return mylib_const::DEFAULT_VECTOR3;
	}
	return GetPosition() + m_apModel[0]->GetPosition();
}

//==========================================================================
// 位置設定
//==========================================================================
void CObjectHierarchy::SetOriginPosition(const MyLib::Vector3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CObjectHierarchy::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// 半径設定
//==========================================================================
void CObjectHierarchy::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//==========================================================================
// 半径取得
//==========================================================================
float CObjectHierarchy::GetRadius(void) const
{
	return m_fRadius;
}

//==========================================================================
// オブジェクトキャラクターオブジェクトの取得
//==========================================================================
CObjectHierarchy *CObjectHierarchy::GetObjectHierarchy(void)
{
	return this;
}

//==========================================================================
// モデル数取得
//==========================================================================
int CObjectHierarchy::GetNumModel(void) 
{
	return m_nNumModel;
}

//==========================================================================
// モデル数設定
//==========================================================================
void CObjectHierarchy::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//==========================================================================
// モデル情報の取得
//==========================================================================
CModel **CObjectHierarchy::GetModel(void)
{
	return &m_apModel[0];
}

//==========================================================================
// 読み込み情報取得
//==========================================================================
CObjectHierarchy::Load CObjectHierarchy::GetLoadData(int nIdx)
{
	return m_aLoadData[nIdx];
}

//==========================================================================
// ファイルのインデックス番号取得
//==========================================================================
int CObjectHierarchy::GetIdxFile(void)
{
	return m_nIdxFile;
}
