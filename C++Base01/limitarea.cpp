//=============================================================================
// 
//  エリア制限処理 [limitarea.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "limitarea.h"
#include "limitereamanager.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"
#include "tutorial.h"


//==========================================================================
// 無名ネームスペース
//==========================================================================
namespace
{
	const char* TEXTURE_DEFAULT = "data\\TEXTURE\\cyberwall_03.png";	// デフォルトテクスチャ
	const float MAX_ALPHA = 0.3f;	// 最大不透明度
	const float TIME_APPEARANCE = 0.5f;	// 出現時間
	const float TIME_FADEOUT = 0.5f;	// フェードアウト時間
}

//==========================================================================
// 関数リスト
//==========================================================================
CLimitArea::STATE_FUNC CLimitArea::m_StateFuncList[] =
{
	&CLimitArea::StateNone,			// 通常
	&CLimitArea::StateAppearance,	// 出現状態
	&CLimitArea::StateFadeout,		// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CLimitArea::CLimitArea(int nPriority) : CObject(nPriority)
{
	m_state = STATE_NONE;			// 状態
	m_fTimeState = 0.0f;			// 状態カウンター
	m_nIdxEreaManager = 0;			// エリア制限マネージャのインデックス番号
	memset(&m_pMeshWall[0], NULL, sizeof(m_pMeshWall));			// メッシュウォールのオブジェクト
	memset(&m_sLimitEreaInfo, NULL, sizeof(m_sLimitEreaInfo));	// エリア制限情報
}

//==========================================================================
// デストラクタ
//==========================================================================
CLimitArea::~CLimitArea()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CLimitArea *CLimitArea::Create(sLimitEreaInfo info)
{
	// 生成用のオブジェクト
	CLimitArea *pObjMeshField = NULL;

	if (pObjMeshField == NULL)
	{// NULLだったら

		// メモリの確保
		pObjMeshField = DEBUG_NEW CLimitArea;

		if (pObjMeshField != NULL)
		{// メモリの確保が出来ていたら

			// エリア制限情報
			pObjMeshField->m_sLimitEreaInfo = info;

			// 初期化処理
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CLimitArea::Init(void)
{

	// 種類設定
	SetType(TYPE_ELEVATION);

	// 割り当て
	m_nIdxEreaManager = CGame::GetLimitEreaManager()->Regist(this);

	// 各種変数初期化
	D3DXVECTOR3 WallPos[mylib_const::SHAPE_LIMITEREA];
	float fWidthLen[mylib_const::SHAPE_LIMITEREA];

	// 奥
	WallPos[WALL_UP] = D3DXVECTOR3(
		(m_sLimitEreaInfo.fMinX + m_sLimitEreaInfo.fMaxX) * 0.5f,
		0.0f,
		m_sLimitEreaInfo.fMaxZ);
	fWidthLen[WALL_UP] = GetPosLength(D3DXVECTOR3(m_sLimitEreaInfo.fMinX, 0.0f, 0.0f), D3DXVECTOR3(m_sLimitEreaInfo.fMaxX, 0.0f, 0.0f));

	// 右
	WallPos[WALL_RIGHT] = D3DXVECTOR3(
		m_sLimitEreaInfo.fMinX,
		0.0f,
		(m_sLimitEreaInfo.fMinZ + m_sLimitEreaInfo.fMaxZ) * 0.5f);
	fWidthLen[WALL_RIGHT] = GetPosLength(D3DXVECTOR3(m_sLimitEreaInfo.fMinZ, 0.0f, 0.0f), D3DXVECTOR3(m_sLimitEreaInfo.fMaxZ, 0.0f, 0.0f));

	// 手前
	WallPos[WALL_DOWN] = D3DXVECTOR3(
		(m_sLimitEreaInfo.fMinX + m_sLimitEreaInfo.fMaxX) * 0.5f,
		0.0f,
		m_sLimitEreaInfo.fMinZ);
	fWidthLen[WALL_DOWN] = GetPosLength(D3DXVECTOR3(m_sLimitEreaInfo.fMinX, 0.0f, 0.0f), D3DXVECTOR3(m_sLimitEreaInfo.fMaxX, 0.0f, 0.0f));

	// 左
	WallPos[WALL_LEFT] = D3DXVECTOR3(
		m_sLimitEreaInfo.fMaxX,
		0.0f,
		(m_sLimitEreaInfo.fMinZ + m_sLimitEreaInfo.fMaxZ) * 0.5f);
	fWidthLen[WALL_LEFT] = GetPosLength(D3DXVECTOR3(m_sLimitEreaInfo.fMinZ, 0.0f, 0.0f), D3DXVECTOR3(m_sLimitEreaInfo.fMaxZ, 0.0f, 0.0f));

	float fRot = (D3DX_PI * 2.0f) / (float)mylib_const::SHAPE_LIMITEREA;
	int nBlock = 4;
	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		m_pMeshWall[i] = CMeshWall::Create(WallPos[i], D3DXVECTOR3(0.0f, D3DX_PI + fRot * i, 0.0f), fWidthLen[i] / (float)nBlock, fWidthLen[i] / (float)nBlock, nBlock, 1, 7, TEXTURE_DEFAULT);
		m_pMeshWall[i]->SetType(CObject::TYPE_NONE);
	}

	// 出現状態
	m_state = STATE_APPEARANCE;

	return E_FAIL;
}

//==========================================================================
// 終了処理
//==========================================================================
void CLimitArea::Uninit(void)
{
	// 削除
	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME && CGame::GetLimitEreaManager() != NULL)
	{// 弾マネージャの削除
		CGame::GetLimitEreaManager()->Delete(m_nIdxEreaManager);
	}

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == NULL)
		{
			continue;
		}
		m_pMeshWall[i]->Uninit();
		m_pMeshWall[i] = NULL;
	}

	// 解放処理
	CObject::Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CLimitArea::Kill(void)
{
	// 削除
	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME && CGame::GetLimitEreaManager() != NULL)
	{// 弾マネージャの削除
		CGame::GetLimitEreaManager()->Delete(m_nIdxEreaManager);
	}

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == NULL)
		{
			continue;
		}
		m_pMeshWall[i]->Uninit();
		m_pMeshWall[i] = NULL;
	}

	// 解放処理
	CObject::Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CLimitArea::Update(void)
{
	// 状態更新
	(this->*(m_StateFuncList[m_state]))();

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == NULL)
		{
			continue;
		}

		m_pMeshWall[i]->Update();
	}
}

//==========================================================================
// 通常状態
//==========================================================================
void CLimitArea::StateNone(void)
{
	// 状態カウンターリセット
	m_fTimeState = 0.0f;
}

//==========================================================================
// 出現状態
//==========================================================================
void CLimitArea::StateAppearance(void)
{
	// 状態カウンター加算
	m_fTimeState += CManager::GetInstance()->GetDeltaTime();

	// 不透明度
	float fAlpha =  MAX_ALPHA * (m_fTimeState / TIME_APPEARANCE);

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == NULL)
		{
			continue;
		}

		// 頂点カラー取得
		D3DXCOLOR *pVtxCol = m_pMeshWall[i]->GetVtxCol();
		D3DXCOLOR newcolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// 全ての要素を書き換え
		std::fill(pVtxCol, pVtxCol + m_pMeshWall[i]->GetNumVertex(), newcolor);
	}

	if (m_fTimeState >= TIME_APPEARANCE)
	{// 規定値を超えたら出現状態解除
		m_state = STATE_NONE;
	}

}

//==========================================================================
// フェードアウト状態
//==========================================================================
void CLimitArea::StateFadeout(void)
{
	// 状態カウンター加算
	m_fTimeState += CManager::GetInstance()->GetDeltaTime();

	// 不透明度
	float fAlpha = MAX_ALPHA - (m_fTimeState / TIME_FADEOUT);

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == NULL)
		{
			continue;
		}

		// 頂点カラー取得
		D3DXCOLOR *pVtxCol = m_pMeshWall[i]->GetVtxCol();
		D3DXCOLOR newcolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// 全ての要素を書き換え
		std::fill(pVtxCol, pVtxCol + m_pMeshWall[i]->GetNumVertex(), newcolor);
	}

	if (m_fTimeState >= TIME_FADEOUT)
	{// 削除

		// 削除
		Kill();
		return;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CLimitArea::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 背面のカリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == NULL || i == WALL_DOWN)
		{
			continue;
		}

		m_pMeshWall[i]->Draw();
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリングデフォルト
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==========================================================================
// 情報取得
//==========================================================================
CLimitArea::sLimitEreaInfo CLimitArea::GetLimitEreaInfo(void)
{
	return m_sLimitEreaInfo;
}

//==========================================================================
// 状態取得
//==========================================================================
CLimitArea::STATE CLimitArea::GetState(void)
{
	return m_state;
}

//==========================================================================
// 状態取得
//==========================================================================
void CLimitArea::SetState(STATE state)
{
	m_state = state;
}