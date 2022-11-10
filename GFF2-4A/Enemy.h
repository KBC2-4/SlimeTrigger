#pragma once
#include"PLAYER.h"
#include"STAGE.h"


#define BALL_W 10
#define BALL_H 10

enum class ENEMY_STATE
{
	IDOL = 0,	//アイドル状態
	MOVE,		//プレイヤーを標準を合わせ続けているとき
	RETURN,		//発射可能状態に戻る
	PRESS,		//発射状態
	FALL,		//落下状態
	DETH		//死亡状態
};

class ENEMY
{
protected:
	int x, y;			//座標
	int map_x, map_y;	//マップ内での座標
	int* image;			//画像保存用
	int now_image;		//使用している画像
	double rad;          //ラジアン
	ENEMY_STATE state;	//エネミーの状態
	STAGE* stage;		//ステージ
	PLAYER* player;		//ステージ

	const int IMAGE_SIZE = 80;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ENEMY();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ENEMY()
	{
	}
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 移動
	/// </summary>
	virtual void Move() = 0;
	/// <summary>
	/// プレイヤーとの角度を求める
	/// </summary>
	void ChangeAngle();
	/// <summary>
	/// 当たり判定
	/// </summary>
	virtual void Hit() = 0;
	/// <summary>
	/// アニメーション
	/// </summary>
	virtual void Animation() = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()const = 0;

	/// <summary>
	/// プレイヤーとの角度の取得
	/// </summary>
	/// <returns>角度</returns>
	double GetRadian()const { return rad; }
	/// <summary>
	/// X座標の取得
	/// </summary>
	/// <returns>x座標</returns>
	int GetX()const { return x; }
	/// <summary>
	/// Y座標の取得
	/// </summary>
	/// <returns>y座標</returns>
	int GetY()const { return y; }
};