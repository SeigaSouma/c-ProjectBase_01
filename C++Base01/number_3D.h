//=============================================================================
// 
//  数字(3D)ヘッダー [number_3D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _NUMBER_3D_H_
#define _NUMBER_3D_H_	// 二重インクルード防止

#include "number.h"
#include "object3D.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObject3D;

//==========================================================================
// クラス定義
//==========================================================================
// 背景クラス定義
class CNumber3D : public CNumber
{
public:

	CNumber3D(int nPriority = 6);
	~CNumber3D();

	// メンバ関数
	HRESULT Init(int nPriority) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;	// 開放処理

	void SetPosition(const MyLib::Vector3 pos) override;	// 位置設定
	MyLib::Vector3 GetPosition(void) const override;		// 位置取得
	void SetMove(const MyLib::Vector3 move) override;		// 移動量設定
	MyLib::Vector3 GetMove(void) const override;			// 移動量取得
	void SetRotation(const MyLib::Vector3 rot) override;	// 向き設定
	MyLib::Vector3 GetRotation(void) const override;		// 向き取得

	void SetColor(const D3DXCOLOR col) override;			// 色設定
	D3DXCOLOR GetColor(void) const override;				// 色取得
	void SetSize3D(const MyLib::Vector3 size) override;		// サイズの設定
	MyLib::Vector3 GetSize3D(void) const override;			// サイズの取得
	void SetTex(D3DXVECTOR2 *tex) override;				// テクスチャ座標の設定
	D3DXVECTOR2 *GetTex(void) override;					// テクスチャ座標の取得

	void SetVtx(void) override;
	void BindTexture(int nIdx) override;
	void SetType(const CObject::TYPE type) override;
	CObject3D *GetObject3D(void) override;

private:
	CObject3D *m_aObject3D;				// オブジェクト3Dのオブジェクト
	bool m_bAddAlpha;
};

#endif