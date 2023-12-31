//=============================================================================
// 
// テクスチャヘッダー [texture.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TEXTURE_H_
#define _TEXTURE_H_	// 二重インクルード防止

#include "constans.h"
#include "constans.h"

//==========================================================================
// クラス定義
//==========================================================================
//テクスチャクラス定義
class CTexture
{
private:
	struct STexture
	{
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャのポインタ
		D3DXIMAGE_INFO imageInfo;		// テクスチャ素材情報
		std::string filename;			// ファイル名
		int nFileNameLen;				// ファイル名の文字数
	};

public:

	CTexture();
	~CTexture();

	void Init(void);	// 初期化処理
	HRESULT Load(void);
	HRESULT LoadAll(void);
	void Unload(void);
	int Regist(std::string file);
	LPDIRECT3DTEXTURE9 GetAdress(int nIdx);
	D3DXVECTOR2 GetImageSize(int nIdx);		// テクスチャ素材のサイズ取得

	int GetNumAll(void);		// 読み込んだ総数
	STexture GetTextureInfo(std::string file);	// テクスチャ情報取得
	STexture GetTextureInfo(int nIdxTex);	// テクスチャ情報取得

	static CTexture* Create(void);
	static CTexture* GetInstance(void) { return m_pTexture; }
private:

	// メンバ関数
	HRESULT LoadTex(std::string file);

	// メンバ変数
	std::vector<STexture> m_TexInfo;	// テクスチャの情報
	static CTexture* m_pTexture;	// 自身のポインタ
};



#endif