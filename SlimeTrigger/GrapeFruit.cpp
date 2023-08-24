#include"DxLib.h"
#include "GrapeFruit.h"
#include<math.h>
#define _USE_MATH_DEFINES
#include <vector>
#include"Option.h"

#define DETH_FRUIT_SIZE 1.5
//コンストラクタ
GRAPEFRUIT::GRAPEFRUIT()
{
	if (images[0][0] = LoadGraph("Resource/Images/Enemy/gurepon.png") == -1)
		throw "Resource/Images/Enemy/gurepon.png";
	shootCount = 0;
	deleteFlg = false;
	rad = 0.0;
	for (int i = 0; i < 3; i++)
	{
		flag[i] = false;
	}
	x = 0;
	spawnMapX = 0;
	spawnMapY = 0;
	targetX = 200;
	animationTimer = 0;
	animationType = 0;
	for (int i = 0; i < 2; i++)
	{
		faceImage[i] = 0;
		fruitImage[i] = 0;
	}
	for (int i = 0; i < 3; i++) {
		bullet[i] = nullptr;
	}
	damageSe = 0;
	bulletCount = 3;
	fruitImageSize = 1.0;
}

//引数付のコンストラクタ

GRAPEFRUIT::GRAPEFRUIT(PLAYER* player, STAGE* stage, int spawn_y, int spawn_x)
{
	spawnMapX = spawn_x;
	spawnMapY = spawn_y;
	x = (spawnMapX * MAP_CEllSIZE + MAP_CEllSIZE / 2);
	y = (spawnMapY * MAP_CEllSIZE + MAP_CEllSIZE / 2) - 40;
	for (int i = 0; i < 3; i++)
	{
		flag[i] = false;
	}
	deleteFlg = false;

	images.resize(1);
	images[0].resize(24);
	if (LoadDivGraph("Resource/Images/Enemy/gurepon.png", 24, 6, 4, 80, 80, &images[0][0]) == -1)
	{
		throw "Resource/Images/Enemy/gurepon.png";
	}
	if ((damageSe = LoadSoundMem("Resource/Sounds/SE/Enemy/damage.wav")) == -1) {
		throw "Resource/Sounds/SE/Enemy/damage.wav";
	}
	if ((pressSe = LoadSoundMem("Resource/Sounds/SE/Enemy/press.wav")) == -1) {
		throw "Resource/Sounds/SE/Enemy/press.wav";
	}
	if ((splashSe = LoadSoundMem("Resource/Sounds/SE/Enemy/splash.wav")) == -1) {
		throw "Resource/Sounds/SE/Enemy/splash.wav";
	}
	shootCount = 0;
	targetX = 200;

	for (int i = 0; i < 2; i++)
	{
		faceImage[i] = images[0][i];
		fruitImage[i] = images[0][(i + 1) * 6];
	}
	for (int i = 0; i < 3; i++)
	{
		bullet[i] = nullptr;
	}
	this->player = player;
	this->stage = stage;
	bulletCount = 3;

	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.2), damageSe);
	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 0.9), pressSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), splashSe);
	fruitImageSize = 1.0;
}

GRAPEFRUIT::~GRAPEFRUIT()
{

	if (!images.empty())
	{
		for (int i = 0; i < images.size(); i++)
		{
			for (int j = 0; j < images[i].size(); j++)
			{
				DeleteGraph(images[i][j]);
			}

			images[i].clear();
		}
		images.clear();
	}

	for (int i = 0; i < 3; i++)
	{
		delete bullet[i];
	}

	DeleteSoundMem(damageSe);
	DeleteSoundMem(pressSe);
	DeleteSoundMem(splashSe);

}
//アップデート
void GRAPEFRUIT::Update()
{

	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.2), damageSe);
	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 0.9), pressSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), splashSe);

	//アニメーションの時間を加算
	if (animationTimer < 80) {
		++animationTimer;
	}

	//動きのステート
	switch (state)
	{
		//待機状態
	case ENEMY_STATE::IDOL:
		break;

		//動いている
	case ENEMY_STATE::MOVE:
		ChangeAngle();
		if ((x + stage->GetScrollX() > 0) && (x + stage->GetScrollX() < 1280))
		{
			if (player->GetMapY() > mapY)
			{

				if (++shootCount % 90 == 0)
				{
					animationTimer = 0;
					state = ENEMY_STATE::PRESS;
				}
			}
		}
		break;

		//アニメーションをリセットする
	case ENEMY_STATE::RETURN:
		ChangeAngle();
		if (ReturnAnimation())
		{
			animationTimer = 0;
			animationType = 0;
			state = ENEMY_STATE::MOVE;
		}
		break;

		//弾を撃つ
	case ENEMY_STATE::PRESS:
		ChangeAngle();
		if (PressAnimation())
		{
			for (int i = 0; i < 3; i++)
			{
				if (flag[i] == false)
				{
					bullet[i] = new ENEMY_BULLET(player, stage, x, y, i * 100.0, rad, 0);
					flag[i] = true;
				}
			}
			animationTimer = 0;
			animationType = 0;
			state = ENEMY_STATE::RETURN;
			PlaySoundMem(pressSe, DX_PLAYTYPE_BACK);
		}
		break;

		//落ちる時
	case ENEMY_STATE::FALL:
		Move();
		FallAnimation();
		break;

		//死
	case ENEMY_STATE::DETH:
		if (DethAnimation())
		{
			deleteFlg = true;
		}
		break;
	default:
		break;
	}

	//マップ上の座標の設定
	mapX = x / MAP_CEllSIZE;
	mapY = (y - IMAGE_SIZE / 2) / MAP_CEllSIZE;

	//弾が存在しているときに弾の処理を行う
	for (int i = 0; i < 3; i++)
	{
		if (bullet[i] != nullptr)
		{
			bullet[i]->Update();
			if (bullet[i]->GetDeleteFlg())
			{
				delete bullet[i];
				bullet[i] = nullptr;
				flag[i] = false;
			}
		}
	}

	//当たり判定
	Hit();

	//画面内にいるかどうか
	if (((x + stage->GetScrollX() < -IMAGE_SIZE) || (x + stage->GetScrollX() > 1280 + IMAGE_SIZE) || (y + stage->GetScrollY() < 0) 
		|| (y + stage->GetScrollY() > 720)) && (state != ENEMY_STATE::FALL && state != ENEMY_STATE::DETH))		//画面外に出るとアイドル状態にする
	{
		state = ENEMY_STATE::IDOL;		//ステートをアイドル状態へ
		//アイドル状態の画像に変更
		for (int i = 0; i < 2; i++)
		{
			faceImage[i] = images[0][i];
			fruitImage[i] = images[0][(i + 1) * 6];
		}
	}
	else if (state == ENEMY_STATE::IDOL)	//画面内にいて、アイドル状態のとき敵の方向を向くようにする
	{
		//アニメーション時間をリセットし、ステートをムーブへ
		animationTimer = 0;
		state = ENEMY_STATE::MOVE;
	}
	else {}
}

//おちたときの動き
void GRAPEFRUIT::Move()
{
	//下へ落ちる
	y += 10;
	//弾のフラグをFALSEへ
	for (int i = 0; i < 3; i++)
	{
		if (flag[i])
		{
			flag[i] = false;
		}
	}

}

//当たり判定

void GRAPEFRUIT::Hit()
{
	//呼ばれた時にスライムのクラス変数を作成
	ThrowSlime throwSlime;

	float bx1, by1, bx2, by2;
	float gx1, gy1, gx2, gy2;
	//プレイヤーが投げた体一部との当たり判定
	if ((state != ENEMY_STATE::FALL) && (state != ENEMY_STATE::DETH))
	{
		for (int i = 0; i < player->GetThrowCnt(); i++)
		{
			throwSlime = player->GetThrowSlime(i);
			//スライムのボールの当たり判定
			bx1 = throwSlime.GetThrowX();
			by1 = throwSlime.GetThrowY();
			bx2 = throwSlime.GetThrowX() + BALL_W;
			by2 = throwSlime.GetThrowY() - BALL_H;
			//グレープフルーツの当たり判定
			gx1 = x - IMAGE_SIZE / 2;
			gy1 = y - IMAGE_SIZE / 2;
			gx2 = gx1 + IMAGE_SIZE;
			gy2 = gy1 + IMAGE_SIZE;
			if (((bx2 >= gx1 && bx2 <= gx2) || (bx1 <= gx2 && bx1 >= gx1)) && ((by1 >= gy2 && by1 <= gy1) || (by2 >= gy1 && by2 <= gy2)))
			{
				rad = 90 * (PI / 180);
				state = ENEMY_STATE::FALL;
				PlaySoundMem(damageSe, DX_PLAYTYPE_BACK);

			}
		}
	}

	//地面やブロックとの当たり判定
	if (state == ENEMY_STATE::FALL)
	{
		if (stage->HitMapDat(mapY + 1, mapX))
		{
			state = ENEMY_STATE::DETH;
			animationTimer = 0;
			fruitImageSize = DETH_FRUIT_SIZE;
			animationType = 0;
			PlaySoundMem(splashSe, DX_PLAYTYPE_BACK);
		}
	}
}

//弾を撃つときのつぶれるアニメーション
bool GRAPEFRUIT::PressAnimation()
{

	bool ret = false;
	if (animationTimer < 40) //30フレーム間アニメーションをする
	{
		if (animationTimer % (ANIMATION_TIME * 2) == 0)
		{
			animationType++;

			for (int i = 0; i < 2; i++)
			{
				faceImage[i] = images[0][i];
				if (animationType % 3 == 0)
				{
					fruitImage[i] = images[0][(6 * (i + 2) - 1)];
				}
				else
				{
					fruitImage[i] = images[0][((animationType % 3) * 2) + (6 * (i + 1))];
				}
			}

		}
	}
	else //アニメーションの終了
	{
		ret = true;
	}
	return ret;
}

//つぶれた状態から元の状態に戻すアニメーション
bool GRAPEFRUIT::ReturnAnimation()
{
	bool ret = false;
	if (animationTimer < 40) //30フレーム間アニメーションをする
	{
		if (animationTimer % (ANIMATION_TIME * 2) == 0)
		{
			animationType++;

			for (int i = 0; i < 2; i++)
			{
				faceImage[i] = images[0][i];
				if (animationType % 3 == 0)
				{
					fruitImage[i] = images[0][(i + 1) * 6];
				}
				else
				{
					fruitImage[i] = images[0][(6 * (i + 2) - 1) - (animationType % 6)];
				}
			}
		}
	}
	else //アニメーションの終了
	{
		for (int i = 0; i < 2; i++)
		{
			faceImage[i] = images[0][i];
			fruitImage[i] = images[0][(i + 1) * 6];
		}
		ret = true;
	}
	return ret;
}


//落ちるアニメーション
void GRAPEFRUIT::FallAnimation()
{
	if (animationTimer % ANIMATION_TIME == 0)
	{
		if (animationTimer % (ANIMATION_TIME * 2) == 0)
		{
			for (int i = 0; i < 2; i++)
			{
				faceImage[i] = images[0][(2 + (animationType % 2) +  (i  * 2))];
				fruitImage[i] = images[0][(i + 1) * 6];
			}
			animationType++;
		}
	}
}

//死ぬアニメーション
bool GRAPEFRUIT::DethAnimation()
{
	bool ret = false;
	if (animationTimer < 30) //30フレーム間アニメーションをする
	{
		//アニメーション
		if (animationTimer % ANIMATION_TIME == 0)
		{
			for (int i = 0; i < 2; i++)
			{
				faceImage[i] = 0;
			}
			fruitImage[0] = images[0][(animationType % 6) + (6 * 3)];
			fruitImage[1] = 0;
			animationType++;
		}
	}
	else //アニメーションの終了
	{
		ret = true;
	}
	return ret;
}

//描画
void GRAPEFRUIT::Draw() const
{
	for (int i = 0; i < 2; i++)
	{
		DrawRotaGraphF(x + stage->GetScrollX(), (y + 5 * i) + stage->GetScrollY(), fruitImageSize, rad + (-90 * (PI / 180)), fruitImage[i], TRUE);
		DrawRotaGraphF(x + stage->GetScrollX(), (y + 5 * i) + stage->GetScrollY(), 1.0, rad + (-90 * (PI / 180)), faceImage[i], TRUE);
	}

	if (flag)
	{
		for (int i = 0; i < bulletCount; i++)
		{
			if (bullet[i] != nullptr)
			{
				bullet[i]->Draw();
			}
		}
	}
}

//X座標をゲットする
int GRAPEFRUIT::GetX()
{
	return x;
}

