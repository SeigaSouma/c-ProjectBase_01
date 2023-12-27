//=============================================================================
// 
//  起動時の目エフェクトヘッダー [effect_startupeye.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EFFECT_STARTUPEYE_H_
#define _EFFECT_STARTUPEYE_H_	// 二重インクルード防止

#include "objectBillboard_Anim.h"

//==========================================================================
// クラス定義
//==========================================================================
// 起動時の目エフェクトクラス定義
class CEffectStartupEye : public CObjectBillboardAnim
{
public:

	CEffectStartupEye(int nPriority = 8);
	~CEffectStartupEye();

	// オーバーライドされた関数
	HRESULT Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CEffectStartupEye *Create(const D3DXVECTOR3 pos);	// 生成処理
	static void LoadTexture(void);	// テクスチャ読み込み
private:

	// メンバ変数
	static int m_nTexIdx;	// テクスチャのインデックス番号
};



#endif