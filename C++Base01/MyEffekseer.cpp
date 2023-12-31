//=============================================================================
// 
//  エフェクシア処理 [MyEffekseer.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "MyEffekseer.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include <Effekseer.h>

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CMyEffekseer* CMyEffekseer::m_pMyEffekseer = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CMyEffekseer::CMyEffekseer()
{
	// 変数のクリア
	time = 0;
	efkHandle = 0;
}

//==========================================================================
// デストラクタ
//==========================================================================
CMyEffekseer::~CMyEffekseer()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CMyEffekseer* CMyEffekseer::Create(void)
{
	if (m_pMyEffekseer == NULL)
	{// まだ生成していなかったら

		// インスタンス生成
		m_pMyEffekseer = DEBUG_NEW CMyEffekseer;

		// 初期化処理
		m_pMyEffekseer->Init();
	}
	else
	{
		// インスタンス取得
		m_pMyEffekseer->GetInstance();
	}

	return m_pMyEffekseer;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMyEffekseer::Init(void)
{
	time = 0;
	efkHandle = 0;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Effekseerのオブジェクトはスマートポインタで管理される。変数がなくなると自動的に削除される。

	// エフェクトのマネージャーの作成
	efkManager = ::Effekseer::Manager::Create(8000);

	// Effekseerのモジュールをセットアップする
	SetupEffekseerModules(efkManager);
	auto efkRenderer = GetEffekseerRenderer();

	// 座標系を設定する。アプリケーションと一致させる必要がある。
	efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// 視点位置を確定
	viewerPosition = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// 投影行列を設定
	projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// カメラ行列を設定
	cameraMatrix.LookAtRH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// エフェクトの設定
//==========================================================================
void CMyEffekseer::SetEffect(std::string efkpath, MyLib::Vector3 pos, MyLib::Vector3 rot, MyLib::Vector3 move, float scale)
{
	MyEffekseerInfo loacalInfo = {};

	// char16_tに変換
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(efkpath);

	// エフェクトの読込
	auto effect = Effekseer::Effect::Create(efkManager, string16t.c_str());
	m_LoadEffect = effect;

	onLostDevice = [effect]() -> void
	{
		// 読み込んだエフェクトのリソースは全て破棄する。
		if (effect != nullptr)
		{
			effect->UnloadResources();
		}
	};

	onResetDevice = [effect]() -> void
	{
		// エフェクトのリソースを再読み込みする。
		if (effect != nullptr)
		{
			effect->ReloadResources();
		}
	};

	// エフェクトの再生
	loacalInfo.handle = efkManager->Play(m_LoadEffect, 0.0f, 0.0f, 0.0f);

	// 引数情報設定
	loacalInfo.move = move;
	efkManager->SetLocation(loacalInfo.handle, pos.x, pos.y, pos.z);
	efkManager->SetRotation(loacalInfo.handle, rot.x, rot.y, rot.z);
	efkManager->SetScale(loacalInfo.handle, scale, scale, scale);

	// リストに割り当て
	m_EffectObj.push_back(loacalInfo);

}

//==========================================================================
// 終了処理
//==========================================================================
void CMyEffekseer::Uninit(void)
{
	efkManager->StopAllEffects();

	delete m_pMyEffekseer;
	m_pMyEffekseer = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CMyEffekseer::Update(void)
{
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	float size = 10.0f;

	for (int i = 0; i < static_cast<int>(m_EffectObj.size()); i++)
	{
		Effekseer::Handle loacalhandle = m_EffectObj[i].handle;

		if (!efkManager->Exists(loacalhandle))
		{// 再生終了

			efkManager->StopEffect(loacalhandle);

			// 削除
			m_EffectObj.erase(m_EffectObj.begin() + i);
		}
		else
		{
			// エフェクトの移動
			efkManager->AddLocation(
				loacalhandle,
				Effekseer::Vector3D(m_EffectObj[i].move.x, m_EffectObj[i].move.y, m_EffectObj[i].move.z));
		}
	}

	// レイヤーパラメータの設定
	Effekseer::Manager::LayerParameter layerParameter;
	::Effekseer::Matrix44 invEfkCameraMatrix;
	::Effekseer::Matrix44::Inverse(invEfkCameraMatrix, cameraMatrix);
	layerParameter.ViewerPosition = ::Effekseer::Vector3D(invEfkCameraMatrix.Values[3][0], invEfkCameraMatrix.Values[3][1], invEfkCameraMatrix.Values[3][2]);
	efkManager->SetLayerParameter(0, layerParameter);

	// マネージャーの更新
	Effekseer::Manager::UpdateParameter updateParameter;
	efkManager->Update(updateParameter);

	// 描画処理
	Draw();
	time++;
}

//==========================================================================
// 描画処理
//==========================================================================
void CMyEffekseer::Draw(void)
{
	// 時間を更新する
	efkRenderer->SetTime(time / 60.0f);

	// 投影行列を設定
	efkRenderer->SetProjectionMatrix(projectionMatrix);

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXMATRIX appProjectionMatrix = pCamera->GetMtxProjection();

	// EffekseerのCameraMatrixは、カメラの姿勢行列の逆行列を指す。カメラ行列がカメラの姿勢行列である場合は、逆行列化しておく。
	auto invAppCameraMatrix = pCamera->GetMtxView();

	// 自分の環境とEffekseerの、投影(射影)行列とカメラ行列を同期
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			projectionMatrix.Values[i][j] = appProjectionMatrix.m[i][j];
			cameraMatrix.Values[i][j] = invAppCameraMatrix.m[i][j];
		}
	}

	// カメラ行列を設定
	efkRenderer->SetCameraMatrix(cameraMatrix);

	// エフェクトの描画開始処理を行う。
	efkRenderer->BeginRendering();

	// エフェクトの描画を行う。
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = efkRenderer->GetCameraProjectionMatrix();
	efkManager->Draw(drawParameter);

	// エフェクトの描画終了処理
	efkRenderer->EndRendering();
}

#if 0
void CMyEffekseer::ClearScreen()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pDevice->BeginScene();
}

void CMyEffekseer::PresentDevice()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->EndScene();

	{
		HRESULT hr;
		hr = pDevice->Present(NULL, NULL, NULL, NULL);

		switch (hr)
		{
		case D3DERR_DEVICELOST:
			while (FAILED(hr = pDevice->TestCooperativeLevel()))
			{
				switch (hr)
				{
				case D3DERR_DEVICELOST:
					::SleepEx(1000, true);
					break;

				case D3DERR_DEVICENOTRESET:

					// Call it before device lost
					// デバイスロストの処理を行う前に実行する
					if (onLostDevice)
					{
						onLostDevice();
					}

					efkRenderer->OnLostDevice();

					D3DPRESENT_PARAMETERS d3dpParams{};
					d3dpParams.BackBufferWidth = SCREEN_WIDTH;
					d3dpParams.BackBufferHeight = SCREEN_HEIGHT;
					d3dpParams.BackBufferFormat = D3DFMT_X8R8G8B8;
					d3dpParams.BackBufferCount = 1;
					d3dpParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
					d3dpParams.Windowed = TRUE;
					//d3dpParams.hDeviceWindow = (HWND)window->GetNativePtr(0);
					d3dpParams.EnableAutoDepthStencil = TRUE;
					d3dpParams.AutoDepthStencilFormat = D3DFMT_D16;
					pDevice->Reset(&d3dpParams);

					// Call it after device lost
					// デバイスロストの処理の後に実行する
					efkRenderer->OnResetDevice();

					if (onResetDevice)
					{
						onResetDevice();
					}

					break;
				}
			}
			break;
		}
	}
}
#endif

//==========================================================================
// モジュールのセットアップ
//==========================================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 描画デバイスの作成
	auto graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(pDevice);

	// エフェクトのレンダラーの作成
	efkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// 描画モジュールの設定
	efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	//// Specify sound modules
	//// サウンドモジュールの設定
	//efkSound = ::EffekseerSound::Sound::Create(GetIXAudio2(), 16, 16);

	//// Specify a metho to play sound from an instance of efkSound
	//// 音再生用インスタンスから再生機能を指定
	//efkManager->SetSoundPlayer(efkSound->CreateSoundPlayer());

	//// Specify a sound data loader
	//// It can be extended by yourself. It is loaded from a file on now.
	//// サウンドデータの読込機能を設定する。
	//// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	//efkManager->SetSoundLoader(efkSound->CreateSoundLoader());
}
