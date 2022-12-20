#pragma once
#include"PadInput.h"
#include "Element.h"
#include <vector>
#include "ThrowSlime.h"
#include "STAGE.h"

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
	float player_x, player_y;
	float old_player_x, old_player_y;
	int map_x, map_y;
	float jump_move_x;
	bool is_ground;		//地面についてるかどうか
	bool hit_ceil;
	float player_speed;

	int life;
	int images[ANIMATION_TYPE][10];		//アニメーションの画像
	int move_type;			//左か右の移動(反転用)
	float move_x;
	int jump_mode;			//停止ジャンプ(1)か移動ジャンプ(2)か
	bool is_jump;
	bool jump_request;
	float jumppower;
	float jump_velocity;

	//hook
	bool is_hook_move;
	float hook_angle;
	float hook_distance;
	float hook_y, hook_x;
	int hook_index;
	int idle_nobi_img;
	std::vector<int> grabbed_hook_array;
	int hook_interval = 0;
	
	double x;     // 紐を伸ばして一周させた場合に出来る円の線上の座標、０は紐が軸の真下に伸びた位置
	double speed; // xの変化速度
	double angle;
	int nx, ny;
	double b = 8.0;		//デフォルト8
	double mass = 4.0;

	//Throw
	bool throw_preparation;
	int throw_interval;
	int throw_ball_image;
	float throw_rad;

	std::vector<ThrowSlime> throw_slime;
	std::vector<float>throw_x;
	std::vector<float>throw_y;
	int throw_index = 0;

	//点滅用
	bool is_damage;
	int alpha_param;	//パラーメーター
	int alpha_time;		//点滅時間

	bool is_heal;

	//プレイヤーのサイズ(倍率)
	float player_scale;

	//サウンド
	int damageSE;		//ダメージを食らったとき
	int jumpSE;			//ジャンプしたとき
	int landingSE;		//着地したとき
	int hook_moveSE;	//フックへの移動時
	int hook_pendulumSE;	//振り子運動時

	typedef struct Animation {
		//画像を切り替えるタイミング(フレーム)
		const int switch_frame;

		//アニメーションの再生の仕方
		//-1: 固定
		// 0: 一枚目から再生したら逆再生する
		// 1: 一枚目から再生したら一枚目に戻す
		// 2: 最後までされたら最後の画像で固定
		const int play_type;

		//アニメーション画像の枚数
		const int image_num;

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
	Animation animation[ANIMATION_TYPE]{
		{  3,  1,  9, 0 },	//アイドル
		{  1,  0, 10, 0 },	//移動
		{  2,  1,  7, 2 },	//投げる
		{  1, -1,  1, 0 },	//フック
		{ 20,  1,  4, 1 },	//ジャンプ
		{ 20,  2,  4, 1 },	//落下
		{  2,  1, 10, 1 },	//着地
	};

	//ステート変数
	PLAYER_MOVE_STATE player_state;
	PLAYER_ANIM_STATE animation_state;
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
	void Update(ELEMENT*element, STAGE* stage, TOMATO** tomaton, int tomaton_count);
	void HitBlock(ELEMENT* element, STAGE* stage);
	void ChangeAnimation(PLAYER_ANIM_STATE anim, bool compelChange = false);

	/*変数のセットとゲット*/
	int GetLife() { return life; };
	bool IsDeath() { return player_state == PLAYER_MOVE_STATE::DEAD; }
	/// <summary>
	/// プレイヤーのマップ内X座標のGetter
	/// </summary>
	/// <returns>float型：プレイヤーのマップ内X座標</returns>	
	float GetPlayerX() { return player_x + stage->GetScrollX(); }
	/// <summary>
	/// プレイヤーのマップ内Y座標のGetter
	/// </summary>
	/// <returns>float型：プレイヤーのマップ内Y座標</returns>	
	float GetPlayerY() { return player_y + stage->GetScrollY(); }
	/// <summary>
	/// プレイヤーのマップ内旧X座標のGetter
	/// </summary>
	/// <returns>float型：プレイヤーのマップ内旧X座標</returns>	
	float GetOldPlayerX() { return old_player_x + stage->GetScrollX(); }
	/// <summary>
	/// プレイヤーのマップ内旧Y座標のGetter
	/// </summary>
	/// <returns>float型：プレイヤーのマップ内旧Y座標</returns>	
	float GetOldPlayerY() { return old_player_y + stage->GetScrollY(); }
	/// <summary>
	/// プレイヤーのマップ内X座標のSetter
	/// </summary>
	void SetPlayerX(float x) { player_x = x - stage->GetScrollX(); }
	/// <summary>
	/// プレイヤーの画面内Y座標のSetter
	/// </summary>
	void SetPlayerY(float y) { player_y = y; }
	/// <summary>
	/// プレイヤーの画面内座標のSetter
	/// </summary>
	void SetPlayer_Screen(POINT screen) { player_x = screen.y; player_y = screen.x; }

	int GetThrowCnt() { return static_cast<int>(throw_slime.size()); }
	ThrowSlime GetThrowSlime(int index) { return throw_slime[index]; }

	float GetSpeed() { return player_speed; }
	float GetMoveX() { return move_x; }

	float GetPlayerScale() { return player_scale; }
	/// <summary>
	/// プレイヤーのジャンプ・落下速度のSetter
	/// </summary>
	/// <returns>-10 〜 10</returns>
	float GetJumpVelocity() { return jump_velocity; }

	float GetPlayerSpeed() { return player_speed; }
	float GetPlayerHookSpeed() { return static_cast<float>(speed); }

	int GetMapY() { return map_y; }
	/// <summary>
	/// プレイヤーのライフのSetter
	/// </summary>
	void SetLife(int a);
	/// <summary>
	/// プレイヤーの動作状態のSetter
	/// </summary>
	/// <returns>enum class型</returns>
	PLAYER_MOVE_STATE GetPlayerMoveState() { return player_state; }
};
