#pragma once
#include"Enemy.h"
#include"EnemyBullet.h"
#include"Stage.h"

class LEMON :
    public ENEMY
{
private:
	bool hitFlg;		//ダメージ受けたかどうか
	bool deleteFlg;	//敵を削除するかどうか
	int shootCount;		//弾を撃つ感覚
	
	int nowImage;		//使用している画像

	int damageSe;		//ダメージを受けた時のSE
	int pressSe;		//プレスアニメーション時のSE
	int splashSe;		//弾けるときのSE

	int spawnMapX;	//スポーン地点(x)
	int spawnMapY;	//スポーン地点(y)

	ENEMY_BULLET* bullet;

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    LEMON();

	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤーのアドレス</param>
	/// <param name="stage">ステージのアドレス</param>
	/// <param name="spawnY">マップ上のスポーン地点(y)</param>
	/// <param name="spawnX">マップ上のスポーン地点(x)</param>
	LEMON(PLAYER* player, STAGE* stage, int spawnY, int spawnX);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LEMON();
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

	bool GetDeleteFlag() { return deleteFlg; }
	int GetMapX() { return mapX; }
	int GetMapY() { return mapY; }
};

