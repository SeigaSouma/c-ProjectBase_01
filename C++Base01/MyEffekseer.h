//=============================================================================
// 
//  エフェクシアヘッダー [MyEffekseer.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MYEFFEKSEER_H_
#define _MYEFFEKSEER_H_	// 二重インクルード防止

#include <EffekseerRendererDX9.h>
#include <XAudio2.h>
#include <d3d9.h>
#include <functional>
#include <wrl/client.h>
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// エフェクシアクラス定義
class CMyEffekseer
{
public:
	
	struct MyEffekseerInfo
	{
		MyLib::Vector3 move;	// 移動量
		Effekseer::Handle handle;	// エフェクトのオブジェクト
	};

	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	CMyEffekseer();
	~CMyEffekseer();

	void Uninit(void);
	void Update(void);

	/**
	@brief	エフェクトの設定
	@details パスの拡張子は[.efkefc]です
	@param	efkpath	[in]	読み込むエフェクトのパス
	@param	pos		[in]	位置
	@param	rot		[in]	向き
	@param	move	[in]	移動量
	@param	scale	[in]	拡大率
	@return	void
	*/
	void SetEffect(std::string efkpath, MyLib::Vector3 pos, MyLib::Vector3 rot, MyLib::Vector3 move, float scale);
	static CMyEffekseer* GetInstance() { return m_pMyEffekseer; }	// インスタンス取得
	static CMyEffekseer* Create(void);								// インスタンス生成

private:

	// メンバ関数
	HRESULT Init(void);
	void Draw(void);
	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);	// モジュールのセットアップ
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer() { return efkRenderer; }	// エフェクシアのレンダラー取得
	
	// メンバ変数
	int32_t time = 0;
	Effekseer::Handle efkHandle = 0;
	EffekseerRendererDX9::RendererRef efkRenderer;
	Effekseer::ManagerRef efkManager;
	Effekseer::Vector3D viewerPosition;
	Effekseer::Matrix44 projectionMatrix;
	Effekseer::Matrix44 cameraMatrix;
	std::function<void()> onLostDevice;
	std::function<void()> onResetDevice;

	// 自作変数
	Effekseer::EffectRef m_LoadEffect;				// 読み込んだエフェクトのスマートポインタ
	std::vector<MyEffekseerInfo> m_EffectObj;		// エフェクトのオブジェクト
	static CMyEffekseer* m_pMyEffekseer;			// 自身のポインタ
};



#endif