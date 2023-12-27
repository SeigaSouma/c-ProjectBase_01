//=============================================================================
// 
//  弾マネージャヘッダー [bulletmanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _BULLETMANAGER_H_
#define _BULLETMANAGER_H_	// 二重インクルード防止

#include "constans.h"
#include "bullet.h"
#include "listmanager.h"

// 前方宣言
class CBullet;

//==========================================================================
// クラス定義
//==========================================================================
// 弾のマネージャクラス定義
class CBulletManager
{
public:

	CBulletManager();
	~CBulletManager();

	HRESULT Init(void);
	void Uninit(void);

	void Regist(CBullet *pItem);			// 割り当て
	void Delete(CBullet *pItem);			// 削除
	void KillAll(void);					// 全て削除
	int GetNumAll(void);				// アイテムの総数取得
	std::list<CBullet*> GetList(void);	// アイテムのリスト取得
	static CBulletManager *Create(void);	// 生成

private:
	int m_nNumAll;		// 弾の総数
	CListManager<CBullet> m_List;	// リスト

};



#endif