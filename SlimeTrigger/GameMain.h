#pragma once
#include "AbstractScene.h"
#include "Player.h"
#include "Stage.h"
#include "Lemon.h"
#include "GrapeFruit.h"
#include "Tomato.h"
#include "Element.h"
#include "Result.h"
#include "Pause.h"
#include "Itemball.h"
class GAMEMAIN :
    public AbstractScene
{
private:
	//背景画像
	int backGraundImage[3];
	int backGraundMusic[3];		//BGM
	int startTimeFont, timeFont;	//フォント
	int cursorMoveSe, okSe, countSe, startSe, clearSe;	//SE
	int startTime;		//開始までの遅延時間
	int startEffectTimer;
	int startAddTime;
	int elapsedTime;	//経過時間
	const char* stageName;	//リスタート時のステージ選択用
	int nowGraph;		//リスタート時の今現在の画面
	int clearInterval;	//クリア時の画面推移までの時間

	//プレイヤーのライフの画像
	int hpImage;

	//レモナーの数用の変数
	int lemonerCount;

	//とまとんの数用の変数
	int tomatonCount;

	//グレポンの数用の変数
	int gureponCount;

	//アイテムの数用変数
	int itemCount;
	int itemNum;
	int itemRand;

	bool reStart;	//復活フラグ

	PLAYER* player;	//プレイヤー
	STAGE* stage;	//ステージ
	PAUSE* pause;	//ポーズ
	LEMON** lemoner;	//レモナー
	GRAPEFRUIT** gurepon;	//グレぽん
	TOMATO** tomaton;	//とまトン
	ITEMBALL** item;     //アイテム
	ELEMENT* element;	//ステージ内要素


	//PV制作用（完成次第即座に消去）
	int inputMargin;
	int scrollSpeed;
	bool playerVisible;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GAMEMAIN(bool reStart = false, int halfwayTime = 0, const char* stageName = "StageSelect");
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GAMEMAIN();
	/// <summary>
	/// 描画以外の更新を実行
	/// </summary>
	/// <returns>シーンのアドレスを返す</returns>
	AbstractScene* Update() override;
	/// <summary>
	/// 描画に関することを実装
	/// </summary>
	void Draw() const override;
};

