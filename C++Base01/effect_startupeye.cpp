//=============================================================================
// 
//  起動時の目エフェクト処理 [effect_startupeye.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "effect_startupeye.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "sound.h"
#include "3D_Effect.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\anim\\effect_eye.png";
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CEffectStartupEye::m_nTexIdx = 0;	// テクスチャのインデックス番号

//==========================================================================
// コンストラクタ
//==========================================================================
CEffectStartupEye::CEffectStartupEye(int nPriority) : CObjectBillboardAnim(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CEffectStartupEye::~CEffectStartupEye()
{
	
}

//==========================================================================
// テクスチャ読み込み
//==========================================================================
void CEffectStartupEye::LoadTexture(void)
{
	// テクスチャの読み込み
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);
}

//==========================================================================
// 生成処理
//==========================================================================
CEffectStartupEye *CEffectStartupEye::Create(const D3DXVECTOR3 pos)
{
	// 生成用のオブジェクト
	CEffectStartupEye *pExplosion = NULL;

	if (pExplosion == NULL)
	{// NULLだったら

		// メモリの確保
		pExplosion = DEBUG_NEW CEffectStartupEye;

		if (pExplosion != NULL)
		{// メモリの確保が出来ていたら

			// 位置割り当て
			pExplosion->SetPosition(pos);

			// 初期化処理
			pExplosion->Init();
		}

		return pExplosion;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEffectStartupEye::Init(void)
{

	// 種類の設定
	SetType(TYPE_OBJECTBILLBOARD);

	// テクスチャの割り当て
	if (m_nTexIdx == 0)
	{
		m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);
	}

	// テクスチャの割り当て
	BindTexture(m_nTexIdx);

	// サイズ設定
	SetSize(D3DXVECTOR2(512.0f, 288.0f) * 0.2f);


	// 初期化処理
	CObjectBillboardAnim::Init(20, 3, 1);

	return S_OK;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEffectStartupEye::Update(void)
{
	for (int i = 0; i < 2; i++)
	{
		D3DXVECTOR3 pos = GetPosition();
		pos.x = (256.0f * 0.2f) - i * (512.0f * 0.2f);

		CEffect3D *pEffect = CEffect3D::Create(
			pos,
			mylib_const::DEFAULT_VECTOR3,
			D3DXCOLOR(1.0f, 0.2f, 0.7f, 1.0f),
			450.0f,
			2,
			CEffect3D::MOVEEFFECT_SUB,
			CEffect3D::TYPE_POINT);
	}

	// 更新処理
	CObjectBillboardAnim::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CEffectStartupEye::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//// αブレンディングを加算合成に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CObjectBillboardAnim::Draw();

	//// αブレンディングを元に戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
