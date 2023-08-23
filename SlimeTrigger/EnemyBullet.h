#pragma once
#include"Enemy.h"
#include<math.h>
#include"DxLib.h"
#include"Player.h"
#include"Stage.h"
#define BULLET_SPEED 12

class ENEMY_BULLET
{
private:
    int myX, myY;                   //この敵の座標
    int drawX;                       //壁に当たった時に描画場所をずらすための変数
    double bulletX, bulletY, hypote;//弾の細かい座標と三角形の斜辺
    double playerX, playerY;        //プレイヤーの座標
    double disX, disY;              //プレイヤーと敵の距離
    double bulletSx, bulletSy;      //弾のX、Yそれぞれのスピード
    int mapX, mapY;                 //弾のマップ上のX, Y（int型）
    float mapdX, mapdY;             //弾のマップ上のX,Y（double）
    int oldMapX, oldMapY;             //弾のマップ上の古い座標
    bool endFlg;                     //弾の消えるアニメーションをするかどうか
    double radX;                     //角度変更用変数
    float scrollX;                   //スクロールの値(x)
    float scrollY;                   //スクロールの値(y)
    bool deleteFlg;                  //デリートするためのフラグ
    bool hitFlg;                     //プレイヤーの投げたボールに当たったかどうかのフラグ
    bool rightSideHit;              //物体の右側面に当たったかどうかのフラグ
    bool leftSideHit;               //物体の左側面にあたったかどうかのフラグ
    double hitRad;                   //弾が物体に当たった時にでるアニメーションの角度の変数
    int bulletImages[4];             //弾のアニメーション用の変数
    int bulletEndImages[20];        //弾が地面に当たった時にでるアニメーションの変数
    double rad;                       //角度の変数
    int imageIndex;                  //画像の判別用の変数
    int image;                        //画像の変数
    int animationTimer;              //アニメーションの時間
    int animationType;               //アニメーションの判別
    int splashSe;                    //弾けるときのSE

    PLAYER* player;                   //プレイヤーのクラスの変数
    STAGE* stage;                     //ステージのクラスの変数

    const int ANIMATION_TIMER = 5;    //アニメーションの時間
    const int IMAGE_Y_SIZE = 20;      //画像の縦のサイズ

private:
    float GetDrawX() const;           //スクロールの値を計算する関数
    float GetDrawY() const;           //スクロールの値を計算する関数

public:
    ENEMY_BULLET();                   //最初のコンストラクタ
    ENEMY_BULLET(PLAYER* player, STAGE* augStage, int x, int y, double dis,double,int); //引数アリのコンストラクタ
    ~ENEMY_BULLET();               //デストラクタ

    void Update();                    //弾のアップデート
    void Move();                      //弾の動き
    void Hit();                       //弾の当たり判定
    /// <summary>
    /// 弾が消えるアニメーション
    /// </summary>
    /// <returns>アニメーションの終了判定</returns>
    bool EndAnimation();
    /// <summary>
    /// 移動アニメーション
    /// </summary>
    void MoveAnimation();
    void Draw() const;                 //弾の描画
    bool GetDeleteFlg(){return deleteFlg;}//デリートフラグのゲット
};

