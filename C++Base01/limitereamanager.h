//=============================================================================
// 
//  エリア制限マネージャヘッダー [limitereamanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _LIMITEREAMANAGER_H_
#define _LIMITEREAMANAGER_H_	// 二重インクルード防止

#include "constans.h"
#include "limitarea.h"

//==========================================================================
// クラス定義
//==========================================================================
// エリア制限のマネージャクラス定義
class CLimitAreaManager
{
public:

	CLimitAreaManager();
	~CLimitAreaManager();

	HRESULT Init(void);
	void Uninit(void);

	static CLimitAreaManager *Create(void);

	int Regist(CLimitArea *pBullet);	// 割り当て
	void Delete(int nIdx);			// 削除
	int GetNumAll(void);			// エリア制限の総数取得
	CLimitArea **GetLimitErea(void);		// エリア制限のオブジェクト取得
protected:


private:

	CLimitArea *m_apLimitErea[mylib_const::MAX_LIMITEREA];	// エリア制限のオブジェクト
	int m_nNumAll;		// エリア制限の総数

};



#endif