//=============================================================================
// 
//  数字処理 [multinumber.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "multinumber.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "objectBillboard.h"
#include "texture.h"
#include "calculation.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define TEXTURE			"data\\TEXTURE\\number_blackclover_01.png"	// テクスチャのファイル
#define TEX_U			(0.1f)							// Uの分割

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CMultiNumber::CMultiNumber(int nPriority)
{
	// 値のクリア
	m_nNum = 0;				// 数字
	m_nNumNumber = 0;		// 数字の数
	m_nTexIdx = 0;			// テクスチャのインデックス番号
	m_nPriority = 0;		// 優先順位
	m_ppMultiNumber = NULL;	// 数字のオブジェクト
	m_pos = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 位置
	m_col = mylib_const::DEFAULT_COLOR;		// 色
	m_size = D3DXVECTOR2(0.0f, 0.0f);	// 数字のサイズ
	m_objType = CNumber::OBJECTTYPE_2D;	// オブジェクトの種類
	m_bDigitDraw = false;				// 桁数描画
}

//==========================================================================
// デストラクタ
//==========================================================================
CMultiNumber::~CMultiNumber()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CMultiNumber *CMultiNumber::Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, bool bDigitDraw, int nPriority)
{
	// 生成用のオブジェクト
	CMultiNumber *pNumber = NULL;

	if (pNumber == NULL)
	{// NULLだったら

	 // メモリの確保
		pNumber = DEBUG_NEW CMultiNumber;

		if (pNumber != NULL)
		{// メモリの確保が出来ていたら

			// オブジェクトの種類
			pNumber->m_objType = objtype;

			// サイズ
			pNumber->m_size = size;

			// 位置
			pNumber->m_pos = pos;

			// 数字の数
			pNumber->m_nNumNumber = nNum;

			// 桁数描画
			pNumber->m_bDigitDraw = bDigitDraw;

			// 優先順位
			pNumber->m_nPriority = nPriority;

			// 初期化処理
			pNumber->Init();
		}
		else
		{
			delete pNumber;
			pNumber = NULL;
		}

		return pNumber;
	}

	return NULL;
}


//==========================================================================
// 生成処理(オーバーロード)
//==========================================================================
CMultiNumber *CMultiNumber::Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, const char *pTextureFile, bool bDigitDraw, int nPriority)
{
	// 生成用のオブジェクト
	CMultiNumber *pNumber = NULL;

	if (pNumber == NULL)
	{// NULLだったら

	 // メモリの確保
		pNumber = DEBUG_NEW CMultiNumber;

		if (pNumber != NULL)
		{// メモリの確保が出来ていたら

		 // オブジェクトの種類
			pNumber->m_objType = objtype;

			// サイズ
			pNumber->m_size = size;

			// 位置
			pNumber->m_pos = pos;

			// 数字の数
			pNumber->m_nNumNumber = nNum;

			// 桁数描画
			pNumber->m_bDigitDraw = bDigitDraw;

			// 優先順位
			pNumber->m_nPriority = nPriority;

			// テクスチャ読み込み
			pNumber->m_nTexIdx = CTexture::GetInstance()->Regist(pTextureFile);

			// 初期化処理
			pNumber->Init();
		}
		else
		{
			delete pNumber;
			pNumber = NULL;
		}

		return pNumber;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMultiNumber::Init(void)
{
	// 数分メモリ確保
	m_ppMultiNumber = DEBUG_NEW(CNumber*[m_nNumNumber]);

	// 生成処理
	if (m_nTexIdx == 0)
	{
		m_nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	}

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		// 生成処理
		m_ppMultiNumber[nCntNum] = CNumber::Create(m_objType, m_nPriority);

		// 各種変数の初期化
		m_ppMultiNumber[nCntNum]->SetSize(m_size);	// サイズ
		m_ppMultiNumber[nCntNum]->SetPosition(MyLib::Vector3(m_pos.x + m_size.y * nCntNum, m_pos.y, m_pos.z));	// 位置

																											// 種類の設定
		if (m_objType == CNumber::OBJECTTYPE_3D)
		{
			m_ppMultiNumber[nCntNum]->SetType(CObject::TYPE_OBJECT3D);
			m_ppMultiNumber[nCntNum]->SetSize3D(MyLib::Vector3(m_size.x, m_size.y, 0.0f));
			m_ppMultiNumber[nCntNum]->SetRotation(m_rot);
		}

		else
		{
			m_ppMultiNumber[nCntNum]->SetType(CObject::TYPE_OBJECT2D);
		}
		// テクスチャの割り当て
		m_ppMultiNumber[nCntNum]->BindTexture(m_nTexIdx);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMultiNumber::Uninit(void)
{
	// 終了処理
	if (m_ppMultiNumber != NULL)
	{// NULLじゃなかったら

	 // 各数字オブジェクトの破棄
		for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
		{
			if (m_ppMultiNumber[nCntNum] != NULL)
			{
				m_ppMultiNumber[nCntNum]->Uninit();
				delete m_ppMultiNumber[nCntNum];
				m_ppMultiNumber[nCntNum] = NULL;
			}
		}
		delete[] m_ppMultiNumber;
		m_ppMultiNumber = NULL;
	}
}

//==========================================================================
// 解放処理
//==========================================================================
void CMultiNumber::Release(void)
{
	// 終了処理
	if (m_ppMultiNumber != NULL)
	{// NULLじゃなかったら

	 // 各数字オブジェクトの破棄
		for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
		{
			if (m_ppMultiNumber[nCntNum] != NULL)
			{
				m_ppMultiNumber[nCntNum]->Release();
				delete m_ppMultiNumber[nCntNum];
				m_ppMultiNumber[nCntNum] = NULL;
			}
		}
		delete m_ppMultiNumber;
		m_ppMultiNumber = NULL;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CMultiNumber::Update(void)
{
	if (m_bDigitDraw == false)
	{// 桁数描画じゃない場合
		return;
	}

	int nNumberDigit = UtilFunc::Calculation::GetDigit(m_nNum);

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] == NULL)
		{// NULLだったら
			continue;
		}

		// 右詰めの描き方
		switch (m_objType)
		{
		case CNumber::OBJECTTYPE_2D:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// 桁数
				m_ppMultiNumber[nCntNum]->GetObject2D()->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->GetObject2D()->SetEnableDisp(false);
			}
			break;

		case CNumber::OBJECTTYPE_3D:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// 桁数
				m_ppMultiNumber[nCntNum]->GetObject3D()->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->GetObject3D()->SetEnableDisp(false);
			}
			break;

		case CNumber::OBJECTTYPE_BILLBOARD:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// 桁数
				m_ppMultiNumber[nCntNum]->GetObjectBillboard()->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->GetObjectBillboard()->SetEnableDisp(false);
			}
			break;
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CMultiNumber::Draw(void)
{
	int nNumNumber = m_nNumNumber;
	if (m_bDigitDraw == true)
	{// 桁数描画だったら
		nNumNumber = UtilFunc::Calculation::GetDigit(m_nNum);
	}

	for (int nCntNum = 0; nCntNum < nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] == NULL)
		{// NULLだったら
			continue;
		}
		m_ppMultiNumber[nCntNum]->Draw();
	}
}

//==========================================================================
// 値の設定処理
//==========================================================================
void CMultiNumber::AddNumber(int nValue)
{
	m_nNum += nValue;

	// 値の設定処理
	SetValue();
}

//==========================================================================
// 値の設定処理
//==========================================================================
void CMultiNumber::SetValue(int nValue)
{
	m_nNum = nValue;

	// 値の設定処理
	SetValue();
}

//==========================================================================
// 値の取得処理
//==========================================================================
int CMultiNumber::GetValue(void)
{
	return m_nNum;
}

//==========================================================================
// 値の設定処理
//==========================================================================
void CMultiNumber::SetValue(void)
{
	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] == NULL)
		{// NULLだったら
			continue;
		}

		int aTexU = m_nNum % (int)std::pow(10, m_nNumNumber + 1 - nCntNum) / ((int)std::pow(10, m_nNumNumber - nCntNum) / 10);

		if (aTexU < 0)
		{
			aTexU = 0;
		}

		// テクスチャポインタ取得
		D3DXVECTOR2 *pTex = m_ppMultiNumber[nCntNum]->GetTex();

		// テクスチャ座標の設定
		pTex[0] = D3DXVECTOR2(aTexU * TEX_U, 0.0f);
		pTex[1] = D3DXVECTOR2(aTexU * TEX_U + TEX_U, 0.0f);
		pTex[2] = D3DXVECTOR2(aTexU * TEX_U, 1.0f);
		pTex[3] = D3DXVECTOR2(aTexU * TEX_U + TEX_U, 1.0f);

		// 頂点設定
		m_ppMultiNumber[nCntNum]->SetVtx();
	}
}

//==========================================================================
// 位置設定
//==========================================================================
void CMultiNumber::SetPosition(const MyLib::Vector3 pos)
{
	// 位置設定
	m_pos = pos;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != NULL)
		{
			m_ppMultiNumber[nCntNum]->SetPosition(MyLib::Vector3(
				m_pos.x + sinf(D3DX_PI / 2 + m_rot.y) * (m_size.y * nCntNum),
				m_pos.y,
				m_pos.z + cosf(D3DX_PI / 2 + m_rot.y) * (m_size.y * nCntNum)));	// 位置
		}
	}
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CMultiNumber::GetPosition(void) const
{
	return m_pos;
}

//==========================================================================
// 位置設定
//==========================================================================
void CMultiNumber::SetOriginPosition(const MyLib::Vector3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CMultiNumber::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// 向き設定
//==========================================================================
void CMultiNumber::SetRotation(const MyLib::Vector3 rot)
{
	m_rot = rot;
	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != NULL)
		{
			m_ppMultiNumber[nCntNum]->SetRotation(rot);	// 位置
		}
	}
}

//==========================================================================
// 向き取得
//==========================================================================
MyLib::Vector3 CMultiNumber::GetRotation(void) const
{
	return m_rot;
}

//==========================================================================
// 色設定
//==========================================================================
void CMultiNumber::SetColor(const D3DXCOLOR col)
{
	m_col = col;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != NULL)
		{
			m_ppMultiNumber[nCntNum]->SetColor(m_col);	// 色
		}
	}
}

//==========================================================================
// 色取得
//==========================================================================
D3DXCOLOR CMultiNumber::GetColor(void) const
{
	return m_col;
}

//==========================================================================
// サイズ設定
//==========================================================================
void CMultiNumber::SetSize(const D3DXVECTOR2 size)
{
	m_size = size;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != NULL)
		{
			m_ppMultiNumber[nCntNum]->SetSize(m_size);
		}
	}
}

//==========================================================================
// サイズ取得
//==========================================================================
D3DXVECTOR2 CMultiNumber::GetSize(void) const
{
	return m_size;
}

//==========================================================================
// サイズ設定
//==========================================================================
void CMultiNumber::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_sizeOrigin = size;
}

//==========================================================================
// サイズ取得
//==========================================================================
D3DXVECTOR2 CMultiNumber::GetSizeOrigin(void) const
{
	return m_sizeOrigin;
}