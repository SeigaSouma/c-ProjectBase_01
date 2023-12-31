//=============================================================================
// 
//  テクスチャ処理 [texture.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

// 読み込むテクスチャのヘッダー
#include "map.h"
#include "3D_effect.h"
#include "effect_slashhit.h"
#include "effect_thunderring.h"
#include "effect_enemyspawn.h"
#include "effect_startupeye.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CTexture* CTexture::m_pTexture = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CTexture::CTexture()
{
	m_TexInfo.clear();
}

//==========================================================================
// デストラクタ
//==========================================================================
CTexture::~CTexture()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTexture* CTexture::Create(void)
{
	if (m_pTexture == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		m_pTexture = DEBUG_NEW CTexture;
		m_pTexture->Init();
	}
	else
	{
		// インスタンス取得
		m_pTexture->GetInstance();
	}

	return m_pTexture;
}

//==========================================================================
// 初期化処理
//==========================================================================
void CTexture::Init(void)
{
	STexture init = {};
	m_TexInfo.emplace_back();

	// 読み込み
	Load();

	// 全て読み込み
	LoadAll();
}

//==========================================================================
// テクスチャの読み込み
//==========================================================================
HRESULT CTexture::LoadAll(void)
{
	// マップ用の読み込み
	if (FAILED(MyMap::ReadTexture()))
	{// 失敗した場合
		return E_FAIL;
	}

	// 3Dエフェクト
	CEffect3D::LoadTexture();

	// 斬撃ヒット
	CEffectSlashHit::LoadTexture();

	// 雷のリング
	CThunderRing::LoadTexture();

	// 敵スポーン
	CEffectEnemySpawn::LoadTexture();

	// 起動時の目
	CEffectStartupEye::LoadTexture();

	return S_OK;
}

//==========================================================================
// 既定テクスチャの読み込み
//==========================================================================
HRESULT CTexture::Load(void)
{
	return S_OK;
}

//==========================================================================
// 全てのテクスチャの破棄
//==========================================================================
void CTexture::Unload(void)
{

	for (const auto& texture : m_TexInfo)
	{
		if (texture.pTexture == nullptr)
		{
			continue;
		}
		texture.pTexture->Release();
	}

	m_TexInfo.clear();
	m_TexInfo.shrink_to_fit();
}

//==========================================================================
// テクスチャの割り当て処理
//==========================================================================
int CTexture::Regist(std::string file)
{
	if (file == "")
	{
		return 0;
	}

	int nNumAll = GetNumAll();	// 最大数取得
	int nIdx = -1;	// 今回の番号保存
	int nNowLen = 0;	// 今回のファイル名長さ
	nNowLen = file.length();

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (m_TexInfo[nCntData].nFileNameLen != nNowLen)
		{// ファイル名の長さが同じだったら
			continue;
		}

		if (m_TexInfo[nCntData].filename.length() != nNowLen)
		{// ファイル名の長さが違ったら
			continue;
		}

		// 既にテクスチャが読み込まれてないかの最終確認
		if (m_TexInfo[nCntData].filename == file)
		{// ファイル名が一致している

			// 番号割り当て
			nIdx = nCntData;
			return nIdx;
		}
	}

	// テクスチャ読み込み
	HRESULT hr = LoadTex(file);
	if (FAILED(hr))
	{// 失敗していたら
		return 0;
	}

	// 番号割り当て
	nIdx = nNumAll;
	return nIdx;
}

//==========================================================================
// テクスチャの読み込み処理
//==========================================================================
HRESULT CTexture::LoadTex(std::string file)
{
	HRESULT hr;
	int nIdx = static_cast<int>(m_TexInfo.size());

	// 割り当て
	m_TexInfo.emplace_back();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile(pDevive,
		file.c_str(),
		&m_TexInfo[nIdx].pTexture);

	if (hr == D3DXERR_INVALIDDATA)
	{// 失敗したとき

		// 割り当て
		m_TexInfo.erase(m_TexInfo.end() - 1);
		return E_FAIL;
	}

	// テクスチャ素材情報
	D3DXGetImageInfoFromFile(file.c_str(), &m_TexInfo[nIdx].imageInfo);

	// ファイル名と長さ保存
	m_TexInfo[nIdx].filename = file;
	m_TexInfo[nIdx].nFileNameLen = m_TexInfo[nIdx].filename.length();

	return S_OK;
}

//==========================================================================
// テクスチャのアドレス取得
//==========================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAdress(int nIdx)
{
	return m_TexInfo[nIdx].pTexture;
}

//==========================================================================
// テクスチャの総数取得
//==========================================================================
int CTexture::GetNumAll(void)
{
	return static_cast<int>(m_TexInfo.size());
}

//==========================================================================
// テクスチャ情報取得
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(std::string file)
{
	// 最大数取得
	int nNumAll = GetNumAll() + 1;

	if (file == "")
	{
		return m_TexInfo[nNumAll];
	}

	int nNowLen = file.length();	// 今回のファイル名長さ

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (m_TexInfo[nCntData].nFileNameLen != nNowLen)
		{// ファイル名の長さが違ったら
			continue;
		}

		// 既にテクスチャが読み込まれてないかの最終確認
		if (m_TexInfo[nCntData].filename == file)
		{// ファイル名が一致している

			return m_TexInfo[nCntData];
		}
	}

	int nIdx = nNumAll + 1;
	return m_TexInfo[nIdx];
}

//==========================================================================
// テクスチャ情報取得
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(int nIdxTex)
{
	return m_TexInfo[nIdxTex];
}

//==========================================================================
// テクスチャ素材のサイズ取得
//==========================================================================
D3DXVECTOR2 CTexture::GetImageSize(int nIdx)
{
	if (static_cast<int>(m_TexInfo.size()) >= nIdx)
	{
		return mylib_const::DEFAULT_VECTOR2;
	}
	return D3DXVECTOR2((float)m_TexInfo[nIdx].imageInfo.Width, (float)m_TexInfo[nIdx].imageInfo.Height);
}
