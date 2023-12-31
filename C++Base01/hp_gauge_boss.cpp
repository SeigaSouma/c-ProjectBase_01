//=============================================================================
// 
//  ボスの体力ゲージ処理 [hp_gauge_boss.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "hp_gauge_boss.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"

//==========================================================================
// マクロ定義
//==========================================================================
namespace
{
	const char* TEXTURE[3] =	// テクスチャのファイル
	{
		"data\\TEXTURE\\bossgauge\\bossHP_base.png",
		"data\\TEXTURE\\bossgauge\\bossHP_gauge.png",
		"data\\TEXTURE\\bossgauge\\bossHP_fram.png",
	};
}

#define WIDTH			(100.0f)	// 横幅
#define HEIGHT			(25.0f)		// 縦幅

//==========================================================================
// コンストラクタ
//==========================================================================
CHP_GaugeBoss::CHP_GaugeBoss(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		m_HPGauge[nCntGauge].pObj2D = NULL;
		m_HPGauge[nCntGauge].fMaxWidth = 0.0f;		// 幅の最大値
		m_HPGauge[nCntGauge].fMaxHeight = 0.0f;		// 高さの最大値
		m_HPGauge[nCntGauge].fWidthDest = 0.0f;		// 幅の差分
	}

	m_nLife = 0;	// 体力
	m_nMaxLife = 0;	// 最大体力
}

//==========================================================================
// デストラクタ
//==========================================================================
CHP_GaugeBoss::~CHP_GaugeBoss()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CHP_GaugeBoss *CHP_GaugeBoss::Create(MyLib::Vector3 pos, int nMaxLife)
{
	// 生成用のオブジェクト
	CHP_GaugeBoss *pHPGauge = NULL;

	if (pHPGauge == NULL)
	{// NULLだったら

		// メモリの確保
		pHPGauge = DEBUG_NEW CHP_GaugeBoss;

		if (pHPGauge != NULL)
		{// メモリの確保が出来ていたら

			// 最大体力
			pHPGauge->m_nMaxLife = nMaxLife;

			// 位置設定
			pHPGauge->SetPosition(pos);

			// 初期化処理
			pHPGauge->Init();
		}

		return pHPGauge;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CHP_GaugeBoss::Init(void)
{
	// 種類設定
	SetType(CObject::TYPE_OBJECT2D);

	// 最大体力
	m_nLife = m_nMaxLife;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		// 生成処理
		m_HPGauge[nCntGauge].pObj2D = CObject2D::Create(8);
		if (m_HPGauge[nCntGauge].pObj2D == NULL)
		{// NULLだったら
			return E_FAIL;
		}

		// テクスチャの割り当て
		int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE[nCntGauge]);

		// テクスチャの割り当て
		m_HPGauge[nCntGauge].pObj2D->BindTexture(nTexIdx);

		// サイズ
		D3DXVECTOR2 texsize = CTexture::GetInstance()->GetImageSize(nTexIdx);
		m_HPGauge[nCntGauge].pObj2D->SetSize(texsize * 0.3f);

		// 各種変数の初期化
		m_HPGauge[nCntGauge].fMaxWidth = m_HPGauge[nCntGauge].pObj2D->GetSize().x;		// 幅の最大値
		m_HPGauge[nCntGauge].fMaxHeight = m_HPGauge[nCntGauge].pObj2D->GetSize().y;		// 高さの最大値
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CHP_GaugeBoss::Uninit(void)
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObj2D != NULL)
		{// NULLじゃなかったら

			// 終了処理
			m_HPGauge[nCntGauge].pObj2D->Uninit();
			m_HPGauge[nCntGauge].pObj2D = NULL;
		}
	}

	// 情報削除
	Release();
}

//==========================================================================
// 終了処理
//==========================================================================
void CHP_GaugeBoss::Kill(void)
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObj2D != NULL)
		{// NULLじゃなかったら

			// 終了処理
			m_HPGauge[nCntGauge].pObj2D->Uninit();
			m_HPGauge[nCntGauge].pObj2D = NULL;
		}
	}


	// 情報削除
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CHP_GaugeBoss::Update(void)
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObj2D == NULL)
		{
			continue;
		}

		if (nCntGauge == VTXTYPE_GAUGE)
		{// ゲージ部分だけ

			// 減少処理
			GaugeDecrement(nCntGauge);
		}

		// 位置設定
		m_HPGauge[nCntGauge].pObj2D->SetPosition(pos);

		// 更新処理
		m_HPGauge[nCntGauge].pObj2D->Update();

		// 頂点座標設定
		SetVtx(nCntGauge);
	}

}

//==================================================================================
// 位置更新
//==================================================================================
void CHP_GaugeBoss::SetLife(int nLife)
{
	// 現在の体力設定
	m_nLife = nLife;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObj2D != NULL)
		{// NULLじゃなかったら

			//目標の幅設定
			m_HPGauge[nCntGauge].fWidthDest = m_HPGauge[nCntGauge].fMaxWidth * ((float)m_nLife / (float)m_nMaxLife);
		}
	}
}

//==================================================================================
// 減少処理
//==================================================================================
void CHP_GaugeBoss::GaugeDecrement(int nCntGauge)
{

	// サイズ取得
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObj2D->GetSize();

	// 差分で徐々に減らしていく
	size.x += (m_HPGauge[nCntGauge].fWidthDest - size.x) * 0.15f;

	// サイズ設定
	m_HPGauge[nCntGauge].pObj2D->SetSize(size);

	// 頂点座標設定
	SetVtx(nCntGauge);
}

//==========================================================================
// 描画処理
//==========================================================================
void CHP_GaugeBoss::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		// 描画処理
		m_HPGauge[nCntGauge].pObj2D->Draw();
	}
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CHP_GaugeBoss::SetVtx(int nCntGauge)
{
	// 頂点設定
	m_HPGauge[nCntGauge].pObj2D->SetVtx();

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	D3DXVECTOR2 *pTex = m_HPGauge[nCntGauge].pObj2D->GetTex();

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_HPGauge[nCntGauge].pObj2D->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// サイズ取得
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObj2D->GetSize();

	// 頂点座標の設定
	pVtx[0].pos = MyLib::Vector3(pos.x + -size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x),	pos.y + -size.y, 0.0f);
	pVtx[1].pos = MyLib::Vector3(pos.x + size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x),	pos.y + -size.y, 0.0f);
	pVtx[2].pos = MyLib::Vector3(pos.x + -size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x),	pos.y + size.y, 0.0f);
	pVtx[3].pos = MyLib::Vector3(pos.x + size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x),	pos.y + size.y, 0.0f);

	pTex[0] = D3DXVECTOR2(0.0f, 0.0f);
	pTex[1] = D3DXVECTOR2(size.x / m_HPGauge[nCntGauge].fMaxWidth, 0.0f);
	pTex[2] = D3DXVECTOR2(0.0f, 1.0f);
	pTex[3] = D3DXVECTOR2(size.x / m_HPGauge[nCntGauge].fMaxWidth, 1.0f);

	// 頂点バッファをアンロックロック
	m_HPGauge[nCntGauge].pObj2D->GetVtxBuff()->Unlock();

}
