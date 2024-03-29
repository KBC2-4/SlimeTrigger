#pragma once
#include"Enemy.h"
#include"EnemyBullet.h"
#include"Stage.h"
#include<math.h>

#define GURAFRU_W 80
#define GURAFRU_H 80


class GRAPEFRUIT :
	public ENEMY
{
private:
	bool flag[3];                   //弾が存在しているかどうかのフラグ
	int shootCount;                 //弾を撃つ感覚
	int animationTimer;            //アニメーションの時間
	int animationType;             //アニメーションの種類
	int spawnMapX;                //マップ上のX
	int spawnMapY;                //マップ上のY
	int targetX;                   //グレポンのターゲット座標
	int bulletCount;               //弾を撃つタイミング
	bool deleteFlg;                //消えるフラグ
	int faceImage[2];				//顔の画像
	int fruitImage[3];             //果実の画像
	double fruitImageSize;		//果実の画像の大きさ
	int damageSe;					//ダメージを受けた時のSE
	int pressSe;					//プレスアニメーション時のSE
	int splashSe;					//弾けるときのSE

	ENEMY_BULLET* bullet[3];       //弾のクラス
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GRAPEFRUIT();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤーのアドレス</param>
	GRAPEFRUIT(PLAYER* player,STAGE* stage,int,int);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GRAPEFRUIT();
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 移動
	/// </summary>
	void Move() override;
	
	/// <summary>
	/// 当たり判定
	/// </summary>
	void Hit() override;
	/// <summary>
	/// 元の形に戻るアニメーション
	/// </summary>
	/// <returns>アニメーションの終了判定</returns>
	bool ReturnAnimation();
	/// <summary>
	/// 弾の発射時のアニメーション
	/// </summary>
	/// <returns>アニメーションの終了判定</returns>
	bool PressAnimation();
	/// <summary>
	/// 落下アニメーション
	/// </summary>
	void FallAnimation();
	/// <summary>
	/// 死亡アニメーション
	/// </summary>
	/// <returns>アニメーションの終了判定</returns>
	bool DethAnimation();
	/// <summary>
	/// 描画
	/// </summary>
    void Draw()const override;


	//ゲット関数
	bool GetDeleteFlg() { return deleteFlg; }
	int GetSpawnMapY() { return spawnMapY; }
	int GetSpawnMapX() { return spawnMapX; }
	int GetX();
};

