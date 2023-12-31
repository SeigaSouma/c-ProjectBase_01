//=============================================================================
// 
// ゲームマネージャ処理 [gamemanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "gamemanager.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "instantfade.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "blackframe.h"
#include "enemybase.h"
#include "enemymanager.h"
#include "stage.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CGameManager::CGameManager()
{
	// 値のクリア
	m_SceneType = SCENE_MAIN;	// シーンの種類
	m_bEndRush = false;			// ラッシュが終了したか
	m_bControll = false;		// 操作できるか
	m_bEndNormalStage = false;	// 通常ステージが終了したか
	m_bSetEvolusion = false;	// 進化設定してるか
	m_nNowStage = 0;			// 現在のステージ
	m_nNumStage = 0;			// ステージの総数
}

//==========================================================================
// デストラクタ
//==========================================================================
CGameManager::~CGameManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGameManager *CGameManager::Create(void)
{
	// 生成用のオブジェクト
	CGameManager *pManager = NULL;

	if (pManager == NULL)
	{// NULLだったら

		// メモリの確保
		pManager = DEBUG_NEW CGameManager;

		if (pManager != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			HRESULT hr = pManager->Init();

			if (FAILED(hr))
			{// 失敗していたら
				return NULL;
			}
		}

		return pManager;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGameManager::Init(void)
{
	m_bControll = true;			// 操作できるか
	m_bEndNormalStage = false;	// 通常ステージが終了したか
	m_nNowStage = 0;			// 現在のステージ
	m_bSetEvolusion = false;	// 進化設定してるか
	m_SceneType = SCENE_MAIN;	// シーンの種類

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGameManager::Uninit(void)
{
	
}

//==========================================================================
// 更新処理
//==========================================================================
void CGameManager::Update(void)
{
	if (CGame::GetEnemyBase()->GetNumStage() - 1 <= m_nNowStage)
	{// 総ステージ数を超えたら
		m_nNowStage = CGame::GetEnemyBase()->GetNumStage() - 1;

		// 通常ステージが終了判定
		m_bEndNormalStage = true;
	}

	// 操作状態
	switch (m_SceneType)
	{
	case CGameManager::SCENE_MAIN:
		m_bControll = true;
		break;

	case CGameManager::SCENE_TRANSITIONWAIT:
		m_bControll = false;
		break;

	case CGameManager::SCENE_TRANSITION:
		m_bControll = false;
		break;

	default:
		break;
	}


	// テキストの描画
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- ゲームマネージャ情報 ----------------\n"
		"【今のモード】[%d]\n", m_SceneType);


	if (m_SceneType == SCENE_TRANSITION)
	{// 遷移中

		// 遷移なしフェードの状態取得
		CInstantFade::STATE fadestate = CManager::GetInstance()->GetInstantFade()->GetState();

		if (fadestate == CInstantFade::STATE_FADECOMPLETION)
		{// 完了した瞬間

			// カメラ取得
			CCamera *pCamera = CManager::GetInstance()->GetCamera();
			if (pCamera != NULL)
			{
				pCamera->SetEnableFollow(true);
			}

			if (m_bEndNormalStage == false)
			{// 通常ステージが終わっていなかったら
				SetEnemy();
			}
			else
			{// ボスステージ
				//SetBoss();
			}

		}
	}

}

//==========================================================================
// 敵設定
//==========================================================================
void CGameManager::SetEnemy(void)
{
	// プレイヤー情報
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL)
		{
			continue;
		}

		// 位置設定
		pPlayer->SetPosition(MyLib::Vector3(-500.0f + nCntPlayer * 250.0f, 0.0f, 0.0f));
	}

	// カメラの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != NULL)
	{
		pCamera->Reset(CScene::MODE_GAME);
	}


	// 種類設定
	m_SceneType = SCENE_MAIN;

	// 敵の再配置
	CEnemyManager *pEnemyManager = CGame::GetEnemyManager();
	if (pEnemyManager != NULL)
	{
		// 敵の再配置
		pEnemyManager->SetStageEnemy();

		// 変更中じゃなくする
		pEnemyManager->SetEnableChangeStage(false);
	}
}

//==========================================================================
// ステージの加算
//==========================================================================
void CGameManager::AddNowStage(void)
{
	// 加算
	m_nNowStage++;

	if (CGame::GetEnemyBase()->GetNumStage() <= m_nNowStage)
	{// 総ステージ数を超えたら
		m_nNowStage = CGame::GetEnemyBase()->GetNumStage();

		// 通常ステージが終了判定
		m_bEndNormalStage = true;
	}

}

//==========================================================================
// 現在のステージ取得
//==========================================================================
int CGameManager::GetNowStage(void)
{
	return m_nNowStage;
}

//==========================================================================
// シーンの種類設定
//==========================================================================
void CGameManager::SetType(SceneType type)
{
	m_SceneType = type;
}

//==========================================================================
// シーンの種類取得
//==========================================================================
CGameManager::SceneType CGameManager::GetType(void)
{
	return m_SceneType;
}
