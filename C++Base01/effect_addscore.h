//=============================================================================
// 
//  弾ヘッダー [effect_addscore.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EFFECT_ADDSCORE_H_
#define _EFFECT_ADDSCORE_H_	// 二重インクルード防止

#include "main.h"
#include "object2D.h"


#define ADDSCORE_CIRCLE	(20)

//==========================================================================
// クラス定義
//==========================================================================
// 弾クラス定義
class CEffectAddScore : public CObject
{
public:

	CEffectAddScore(int nPriority = 7);
	~CEffectAddScore();

	// オーバーライドされた関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CEffectAddScore *Create(const MyLib::Vector3 pos);

private:
	enum STATE
	{
		STATE_NONE = 0,	// なにもなし
		STATE_DMG,		// 跳ね返し
		STATE_MAX
	};

	void UpdatePos(void);		// 移動

	STATE m_state;				// 状態
	int m_nCntState;			// 状態遷移カウンター
	int m_nLife;				// 寿命
	int m_nLifeMax;				// 寿命の最大値
	int m_nTexIdx;				// テクスチャのインデックス番号
	CObject2D *m_pEffect[ADDSCORE_CIRCLE];	// 見た目だけの弾
};


#endif