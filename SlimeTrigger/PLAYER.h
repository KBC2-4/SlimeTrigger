#pragma once
#include"PadInput.h"
#include "Element.h"
#include <vector>
#include "ThrowSlime.h"
#include "Stage.h"

class TOMATO;

#define MAX_LIFE				5		//プレイヤーの最大ライフ
#define SPEED					4.2f	//プレイヤーのスピード
#define DEVIATION				10000	//スティック入力の誤入力の範囲
#define JUMP_VELOCITY			-6.2f	//ジャンプスピード
#define HOOK_MAX_DISTANCE		280
#define ANIMATION_TYPE			7
#define THROW_INTERVAL			60		//投げるときのクールタイム
#define HOOK_INTERVAL			20

#define PI 3.1415926535897932384626433832795
#define LENGTH      200                 // 紐の長さ
#define CLENGTH     (LENGTH * 2 * PI)   // 紐を伸ばして一周させた場合に出来る円の円周の長さ
#define G           9.81                // 重力加速度

#define MIN_SIZE_SCALE		0.8f	//プレイヤーの最小サイズ(倍率)

//移動ステート
enum class PLAYER_MOVE_STATE {
	IDLE = 0,	//停止しているとき
	MOVE,		//動いてるとき
	JUMP,		//跳ぶ
	FALL,		//落下
	THROW,		//投げる時
	GROW_HOOK,	//フックに伸びるとき
	HOOK,		//振り子しているとき
	//DAMAGE,		//ダメージを受けた時
	DEAD,		//死んだとき
};

//アニメーションステート
enum class PLAYER_ANIM_STATE {
	IDLE = 0,//アイドルアニメーション
	MOVE,	 //移動アニメーション
	THROW,	 //投げるアニメーション
	HOOK,
	JUMP,	//ジャンプアニメーション
	FALL,	//落下アニメーション
	LANDING,//着地アニメーション
};

class PLAYER
{

private:
	bool isVisible;
	float playerX, playerY;
	float oldPlayerX, oldPlayerY;
	int mapX, mapY;
	float jumpMoveX;
	bool isGround;		//地面についてるかどうか
	bool hitCeil;
	float playerSpeed;

	int life;
	int images[ANIMATION_TYPE][10];		//アニメーションの画像
	int moveType;			//左か右の移動(反転用)
	float moveX;

	//Jump
	int jumpMode;			//停止ジャンプ(1)か移動ジャンプ(2)か
	float jumpPower;
	float jumpVelocity;
	bool isJump;
	bool jumpRequest;
	bool isGravity;

	//hook
	bool isHookMove;
	bool isHookPendulumMove; //フックまでの移動終了判定
	float hookMoveX, hookMoveY;
	float hookAngle;
	float hookDistance;
	float hookMoveDistance;
	float hookY, hookX;
	int hookIndex;
	int idleNobiImage;
	std::vector<int> grabbedHookArray;
	
	double x;     // 紐を伸ばして一周させた場合に出来る円の線上の座標、０は紐が軸の真下に伸びた位置
	double speed; // xの変化速度
	double angle;
	int nX, nY;
	double b = 8.0;		//デフォルト8
	double mass = 4.0;

	//Throw
	bool throwPreparation;
	int throwInterval;
	int throwBallImage;
	float throwRad;

	std::vector<ThrowSlime> throwSlime;
	std::vector<float>throwX;
	std::vector<float>throwY;
	int throwIndex = 0;

	//点滅用
	bool isDamage;
	int alphaParameter;	//パラーメーター
	int alphaTime;		//点滅時間

	bool isHear;

	//プレイヤーのサイズ(倍率)
	float playerScale;

	//サウンド
	int damageSE;		//ダメージを食らったとき
	int jumpSE;			//ジャンプしたとき
	int landingSE;		//着地したとき
	int hookMoveSe;	//フックへの移動時
	int hookPendulumSe;//振り子運動時
	int healSe;			//回復したとき
	int throwBallSe;	//投げるとき

	typedef struct Animation_old {
		//画像を切り替えるタイミング(フレーム)
		const int switchFrame;
		
		//int animationPlayType [10] = {};
		//アニメーションの再生の仕方;
		//-1: 固定
		// 0: 一枚目から再生したら逆再生する
		// 1: 一枚目から再生したら一枚目に戻す
		// 2: 最後までされたら最後の画像で固定
		const int playType;

		//アニメーション画像の枚数
		const int imageNum;

		//アニメーションの優先度
		const int priority;

		//アニメーションのフレームのカウント
		int frame = 0;

		//今のアニメーションの添え字
		int type = 0;

		//play_typeが0のアニメーションの段階(0: 前半, 1: 後半)
		int phase = 0;

		//ループ再生かどうか(0: ループ再生)
		int playMode;

		//アニメーションの終了判定
		bool endAnim;
	};
	Animation_old animation[ANIMATION_TYPE]{
		/*
		* 切り替えタイミング（フレーム）
		* アニメーションの再生方式
		* 画像の枚数
		* アニメーション切り替えの優先度
		*/

		{  3,  1,  9, 0 },	//アイドル
		{  1,  0, 10, 0 },	//移動
		{  2,  1,  7, 2 },	//投げる
		{  1, -1,  1, 0 },	//フック
		{ 20,  1,  4, 1 },	//ジャンプ
		{ 20,  2,  4, 1 },	//落下
		{  2,  1, 10, 1 },	//着地
	};

	//ステート変数
	PLAYER_MOVE_STATE playerState;
	PLAYER_ANIM_STATE animationState;
	STAGE *stage;

public:
	PLAYER(STAGE* stage);
	~PLAYER();

	void Move();
	void Draw(STAGE* stage) const;
	void HookMove(ELEMENT* element, STAGE* stage);
	void JumpMove();
	void Throw(STAGE* stage);
	void MoveAnimation();
	void Update(ELEMENT* element, STAGE* stage, TOMATO** tomaton, int tomatonCount, bool is_stay = false);
	void Hit(ELEMENT* element, STAGE* stage);
	void ChangeAnimation(PLAYER_ANIM_STATE anim, bool compelChange = false);

	//変数のゲット

	/// <summary>
	/// 死亡判定のGetter
	/// </summary>
	bool IsDeath() { return playerState == PLAYER_MOVE_STATE::DEAD; }
	
	/// <summary>
	/// プレイヤーのライフのGetter
	/// </summary>
	int GetLife() { return life; };

	/// <summary>
	/// プレイヤーの弾の数のGetter
	/// </summary>
	int GetThrowCnt() { return static_cast<int>(throwSlime.size()); }

	/// <summary>
	/// プレイヤーのマップ配列YのGetter
	/// </summary>
	/// <returns>int型：配列の添え字</returns>
	int GetMapY() { return mapY; }

	/// <summary>
	/// プレイヤーの画面内X座標のGetter
	/// </summary>
	/// <returns>float型：プレイヤーの画面内X座標</returns>	
	float GetPlayerX() { return playerX + stage->GetScrollX(); }

	/// <summary>
	/// プレイヤーの画面内Y座標のGetter
	/// </summary>
	/// <returns>float型：プレイヤーの画面内Y座標</returns>	
	float GetPlayerY() { return playerY + stage->GetScrollY(); }

	/// <summary>
	/// プレイヤーの画面内旧X座標のGetter
	/// </summary>
	/// <returns>float型：プレイヤーの画面内旧X座標</returns>	
	float GetOldPlayerX() { return oldPlayerX + stage->GetScrollX(); }

	/// <summary>
	/// プレイヤーの画面内旧Y座標のGetter
	/// </summary>
	/// <returns>float型：プレイヤーの画面内旧Y座標</returns>	
	float GetOldPlayerY() { return oldPlayerY + stage->GetScrollY(); }

	/// <summary>
	/// プレイヤーの移動方向のGetter
	/// </summary>
	/// <returns>float型：-1.0 or 1.0</returns>
	float GetMoveX() { return moveX; }

	/// <summary>
	/// プレイヤーのサイズの倍率のGetter
	/// </summary>
	/// <returns>float型：倍率</returns>
	float GetPlayerScale() { return playerScale; }

	/// <summary>
	/// プレイヤーのジャンプ・落下速度のGetter
	/// </summary>
	/// <returns>-10 ～ 10</returns>
	float GetJumpVelocity() { return jumpVelocity; }

	/// <summary>
	/// プレイヤーの移動速度のGetter
	/// </summary>
	/// <returns>float型：プレイヤーの移動速度</returns>
	float GetPlayerSpeed() { return playerSpeed; }

	/// <summary>
	/// プレイヤーの振り子の速度のGetter
	/// </summary>
	/// <returns>float型：プレイヤーの振り子の速度</returns>
	float GetPlayerHookSpeed() { return static_cast<float>(speed); }

	/// <summary>
	/// プレイヤーの動作状態のGetter
	/// </summary>
	/// <returns>enum class型</returns>
	PLAYER_MOVE_STATE GetPlayerMoveState() { return playerState; }

	/// <summary>
	/// プレイヤーの弾のクラスのGetter
	/// </summary>
	ThrowSlime GetThrowSlime(int index) { return throwSlime[index]; }

	//変数のセット

	/// <summary>
	/// プレイヤーのライフのSetter
	/// </summary>
	void SetLife(int _life);

	/// <summary>
	/// プレイヤーの画面内X座標のSetter
	/// </summary>
	void SetPlayerX(float x) { playerX = x - stage->GetScrollX(); }

	/// <summary>
	/// プレイヤーのマップ内Y座標のSetter
	/// </summary>
	void SetPlayerY(float y) { playerY = y; }

	/// <summary>
	/// プレイヤーのマップ内座標のSetter
	/// </summary>
	void SetPlayer_Screen(POINT screen) { playerX = screen.y; playerY = screen.x; }

	/// <summary>
	/// プレイヤーの重力のSetter
	/// </summary>
	/// <param name="gravity">true:有効 false:無効</param>
	void SetGravity(bool gravity) { isGravity = gravity; }

	/// <summary>
	/// プレイヤーの表示非表示
	/// </summary>
	/// <param name="visible">true:有効 false:無効</param>
	void SetVisible(bool visible) { isVisible = visible; }
};
