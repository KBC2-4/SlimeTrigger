#pragma once
#include "Stage.h"
#include <vector>

#define LIFT_SIZE 160

class PLAYER;
class STAGE;

class ELEMENT :
    public STAGE
{
public:
	struct ELEMENT_DATA
	{
		float x, y;
		int type;
		bool flag = false;
		int animTimer = 0;
		
		float leftInitX, leftInitY;		//動く床の初期座標
		float leftGoalX, leftGoalY;		//動く床の目標位置
		int leftVectorX;					//動く床の移動方向(横)
		int leftVectorY;					//動く床の移動方向(縦)
		int liftWaitTimer;					//動く床待機時間
		
		int pairNum;						//ボタンとドアのペア番号
		int image;							//画像
	};
	
private:
	int guidFont,guidTimer;
	std::vector<ELEMENT_DATA> hook;			//フック
	std::vector<ELEMENT_DATA> button;		//ボタン
	std::vector<ELEMENT_DATA> door;			//ドア
	std::vector<ELEMENT_DATA> lift;			//動く床
	std::vector<ELEMENT_DATA> manhole;		//マンホール
	std::vector<ELEMENT_DATA> acidrainPuddles;			//酸性雨の水たまり
	float playerMapX, playerMapY;
	int playerState;
	
	//mutable int animTimer;
	bool keepPushing;						//ボタンを押し続けているかフラグ
	bool hookFlag; //近くにあるフックにガイド表示させる為のフラグ。
	int doorCloseSe, pressTheButtonSe, switchSe, walkPuddleSe, manholeOpenedSe;
	int acidrainPuddlesAniTimer;		//酸性雨の水たまりの切り替えアニメーション用タイマー
	int underGroundEffects;			//地下エフェクト用タイマー

	//酸溜まりの画像
	int acidImage[12];
	//酸溜まりの液体のアニメーション
	int acidAnimation;
	//酸溜まりの泡のアニメーション
	int foamAnimation;

	//動く床の画像
	int moveFloorImage[3];
	//動く床のアニメーション
	int moveFloorAnimation;

	//プレイヤーが地下にいるかどうか
	bool isInUnder;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ELEMENT(const char* stageName = "StageSelect");
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ELEMENT();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(STAGE* stage, PLAYER* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(PLAYER* player,STAGE*stage);
	/// <summary>
	/// 3種類のボタンの処理
	/// </summary>
	void Button(PLAYER* player);
	/// <summary>
	/// ドアの処理
	/// </summary>
	void Door(STAGE* stage);
	/// <summary>
	/// 動く床の処理
	/// </summary>
	void Lift(PLAYER* player, STAGE* stage);
	/// <summary>
	/// 動く床の当たり判定
	/// </summary>
	bool HitLift(PLAYER* player);
	/// <summary>
	/// マンホールの処理
	/// </summary>
	void Manhole(PLAYER* player, STAGE* stage);
	/// <summary>
	/// 酸性雨の水たまりの処理
	/// </summary>
	void Acidrain_puddles(PLAYER* player);
	/// <summary>
	/// フックのガイド表示用距離計算
	/// </summary>
	void Hook_Distance(PLAYER* player, STAGE* stage);


	/// <summary>
	/// フック構造体のGetter
	/// </summary>
	std::vector<ELEMENT_DATA> GetHook() { return hook; }
	/// <summary>
	/// ボタン構造体のGetter
	/// </summary>
	std::vector<ELEMENT_DATA>GetButton() { return button; }
	/// <summary>
	/// ドア構造体のGetter
	/// </summary>
	std::vector<ELEMENT_DATA>GetDoor() { return door; }
	/// <summary>
	/// 動く床構造体のGetter
	/// </summary>
	std::vector<ELEMENT_DATA>GetLift() { return lift; }
	/// <summary>
	/// マンホール構造体のGetter
	/// </summary>
	std::vector<ELEMENT_DATA>GetManhole() { return manhole; }
	/// <summary>
	/// 酸性雨の水たまり構造体のGetter
	/// </summary>
	std::vector<ELEMENT_DATA>GetAcidrain_puddles() { return acidrainPuddles; }

	bool GetIsInUnder() { return isInUnder; }
};

