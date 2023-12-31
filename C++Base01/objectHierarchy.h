//=============================================================================
// 
//  階層オブジェクトヘッダー [objectHierarchy.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTHIERARCHY_H_
#define _OBJECTHIERARCHY_H_	// 二重インクルード防止

#include "object.h"

class CModel;

//==========================================================================
// クラス定義
//==========================================================================
// 階層オブジェクトクラス定義
class CObjectHierarchy : public CObject
{
public:

	// 構造体定義
	struct LoadData
	{
		std::string pModelFile;	// モデルファイル名
		int nType;				// モデル種類
		int nParent;			// 親のインデックス
		int nStart;				// 最初からあるかどうか
		int nSwitchType;		// 切り替えの種類
		int nIDSwitchModel;		// 切り替えモデルのID
		MyLib::Vector3 pos;		// 位置
		MyLib::Vector3 rot;		// 向き
	};

	struct Load
	{
		std::string sTextFile;	// テキストファイル名
		int nNumModel;			// モデル数
		float fVelocity;		// 移動速度
		float fRadius;			// 半径
		int nLife;				// 体力
		int nMotionStartIdx;	// モーション開始のインデックス番号
		int nAddScore;			// スコア加算量
		MyLib::Vector3 posOrigin;	// 最初の位置
		LoadData LoadData[mylib_const::MAX_MODEL];
	};

	// 列挙型定義
	typedef enum
	{
		STATE_NONE = 0,	// なにもない
		STATE_MAX
	}STATE;

	CObjectHierarchy(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectHierarchy();

	// オーバーライドされた関数
	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Draw(D3DXCOLOR col);
	virtual void Draw(float fAlpha);

	void SetmtxWorld(const D3DXMATRIX mtxWorld);
	D3DXMATRIX GetmtxWorld(void) const;			// ワールドマトリックス取得
	MyLib::Vector3 GetCenterPosition(void) const;		// 中心の位置取得
	void SetOriginPosition(const MyLib::Vector3 pos);	// 最初の位置設定
	MyLib::Vector3 GetOriginPosition(void) const;		// 最初の位置取得
	void SetRadius(const float fRadius);		// 半径設定
	float GetRadius(void) const;				// 半径取得

	HRESULT ReadText(const std::string pTextFile);	// 外部ファイル読み込み処理
	virtual HRESULT SetCharacter(const std::string pTextFile);	// キャラクター設定

	void ChangeObject(int nDeleteParts, int nNewParts);
	void ChangeObject(int nSwitchType);		// 切り替えの種類
	void SetObject(int nNewParts);			// モデルの設定
	void DeleteObject(int nSwitchType);		// モデル削除

	int GetNumModel(void);					// モデル数取得
	void SetNumModel(int nNumModel);		// モデル数設定
	int GetIdxFile(void);					// ファイルのインデックス番号取得
	Load GetLoadData(int nIdx);				// 読み込み情報取得
	CModel **GetModel(void);				// モデル取得
	CObjectHierarchy* GetObjectHierarchy(void);	// 階層オブジェクト取得
	static CObjectHierarchy *Create(const std::string pTextFile);

protected:

	virtual void LoadObjectData(FILE* pFile, const char* pComment);	// オブジェクト毎のデータ読み込み
	virtual void LoadPartsData(FILE* pFile, const char* pComment, int *pCntParts);	// パーツ毎のデータ読み込み
	virtual void BindObjectData(int nCntData);	// オブジェクト毎のデータ割り当て

	CModel* m_apModel[mylib_const::MAX_MODEL];	// モデル(パーツ)のポインタ
	static Load m_aLoadData[mylib_const::MAX_MODEL];
	static int m_nNumLoad;	// 読み込んだ数
private:

	// メンバ関数
	void CalWorldMtx(void);		// ワールドマトリックスの計算処理

	// メンバ変数
	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	MyLib::Vector3 m_posOrigin;	// 最初の位置
	float m_fRadius;			// 半径
	int m_nNumModel;			// モデルの数
	int m_nIdxFile;				// ファイルのインデックス番号

};



#endif