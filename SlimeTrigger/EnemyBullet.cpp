#include "EnemyBullet.h"
#include <math.h>
#include"Option.h"

//コンストラクタ
ENEMY_BULLET::ENEMY_BULLET()
{
	player = nullptr;
	stage = nullptr;
	playerX = 0.0;
	playerY = 0.0;
	bulletX = 0.0;
	bulletY = 0.0;
	hypote = 0.0;
	myX = 0.0;
	myY = 0.0;
	disX = 0.0;
	disY = 0.0;
	drawX = 0;
	bulletSx = 0.0;
	bulletSy = 0.0;
	hitRad = 0.0;
	endFlg = false;
	deleteFlg = false;
	hitFlg = false;
	rightSideHit = false;
	leftSideHit = false;
	image = 0;
	radX = 0.0;
	mapX = 0;
	mapY = 0;
	mapdX = 0.0;
	mapdY = 0.0;
	oldMapX = 0;
	oldMapY = 0;
	imageIndex = 0;
	animationTimer = 0;
	animationType = 0;
	splashSe = 0;
	for (int i = 0; i < 4; i++)
	{
		bulletImages[i] = 0;
	}
	for (int i = 0; i < 20; i++)
	{
		bulletEndImages[i] = 0;
	}
}

//引数付きコンストラクタ
ENEMY_BULLET::ENEMY_BULLET(PLAYER* argu_player, STAGE* aug_stage, int x, int y, double dis, double p_rad, int index)
{
	if (LoadDivGraph("Resource/images/Enemy/Enemy_Bullet.png", 4, 4, 1, 20, 20, bulletImages) == -1)
	{
		throw "Resource/Images/Enemy/Enemy_Bullet.png";
	}
	if (LoadDivGraph("Resource/images/Enemy/Bullet_End.png", 20, 10, 2, 60, 30, bulletEndImages) == -1)
	{
		throw "Resource/Images/Enemy/Bullet_End.png";
	}
	if ((splashSe = LoadSoundMem("Resource/Sounds/SE/Enemy/bullet.wav")) == -1) {
		throw "Resource/Sounds/SE/Enemy/bullet.wav";
	}
	player = argu_player;
	if (player->GetMoveX() > 0)
	{
		playerX = player->GetPlayerX();
	}
	else
	{
		playerX = player->GetPlayerX();
	}

	playerY = player->GetPlayerY();
	bulletX = x;
	bulletY = y;
	hypote = 0.0;
	myX = x;
	myY = y;
	disX = 0.0;
	disY = 0.0;
	drawX = 0;
	bulletSx = 0.0;
	bulletSy = 0.0;
	hitRad = -90 * (PI / 180);
	endFlg = false;
	deleteFlg = false;
	hitFlg = false;
	rightSideHit = false;
	leftSideHit = false;
	radX = dis;
	stage = aug_stage;
	this->scrollX = abs(stage->GetScrollX());
	this->scrollY = abs(stage->GetScrollY());

	mapX = 0;
	mapY = 0;
	mapdX = 0.0;
	mapdY = 0.0;
	oldMapX = 0;
	oldMapY = 0;
	rad = p_rad;
	imageIndex = index;
	image = 0;
	animationTimer = 0;
	animationType = 0;
	disX = (playerX + radX) - (myX - static_cast<double>(scrollX));
	disY = playerY - (myY - static_cast<double>(scrollY) - stage->GetSpawnPoint().y);

	hypote = sqrt((disX * disX) + (disY * disY));

	bulletSx = disX / hypote * BULLET_SPEED;
	bulletSy = disY / hypote * BULLET_SPEED;

	ChangeVolumeSoundMem(Option::GetSEVolume(), splashSe);
}

ENEMY_BULLET::~ENEMY_BULLET() {

	for (int i = 0; i < 4; i++) {
		DeleteGraph(bulletImages[i]);
	}

	for (int i = 0; i < 20; i++) {
		DeleteGraph(bulletEndImages[i]);
	}

	DeleteSoundMem(splashSe);
}

//描画
void ENEMY_BULLET::Draw() const
{
	DrawRotaGraph(static_cast<int>(GetDrawX()) - drawX, static_cast<int>(GetDrawY()), 2, rad + hitRad, image, TRUE);
}

//アップデート
void ENEMY_BULLET::Update()
{

	animationTimer++;        //アニメーションの時間を加算
	//アニメーションを終わらせる
	if (endFlg)
	{
		if (EndAnimation())
		{
			deleteFlg = true;
		}
	}
	else
	{
		Move();	//あにめーしょんのうごき
		Hit();  //プレイヤーとの当たり判定
	}
	//弾がプレイヤーに当たっていたら
	if (hitFlg)
	{
		if (player->GetLife() > 0)
		{
			player->SetLife(player->GetLife() - 1);
		}
		else
		{
			player->SetLife(0);
		}
	}

	if (GetDrawX() < 0 || GetDrawX() > 1280 || (bulletY + stage->GetScrollY()) < 0 || (bulletY + stage->GetScrollY()) > 720)
	{
		deleteFlg = true;
	}
}

void ENEMY_BULLET::Move()
{

	//弾の移動
	bulletX += bulletSx;
	bulletY += bulletSy;

	//マップ上の値を代入
	mapdX = bulletX / MAP_CEllSIZE;
	mapdY = (bulletY + IMAGE_Y_SIZE) / MAP_CEllSIZE;

	//自分が前いたマップ座標
	oldMapX = mapX;
	oldMapY = mapY;

	//ダブル型のマップ上の値をイント型に
	mapX = (int)(mapdX);
	mapY = (int)(mapdY);
	MoveAnimation();
}

//アニメーションを終わらせる
bool ENEMY_BULLET::EndAnimation()
{
	bool ret = false;           //アニメーションが終わっているかどうかのフラグ
	if (animationTimer < 50)	//50フレーム間アニメーションをする
	{
		if (animationTimer % ANIMATION_TIMER == 0)
		{
			image = bulletEndImages[(imageIndex * 10) + (animationType++ % 10)];
		}
	}
	else  //アニメーションの終了
	{
		ret = true;
	}

	return ret;
}
void ENEMY_BULLET::MoveAnimation()
{
	if (animationTimer % ANIMATION_TIMER == 0)
	{
		image = bulletImages[(imageIndex * 2) + (animationType++ % 2)];
	}
}

void ENEMY_BULLET::Hit()
{

	float px1, py1, px2, py2;
	float bx1, by1, bx2, by2;

	px1 = player->GetPlayerX() - (20 * (player->GetPlayerScale()));
	px2 = px1 + (50 * (player->GetPlayerScale()));
	py1 = player->GetPlayerY() + (20 * (2 - player->GetPlayerScale()));
	py2 = py1 + (25 * (player->GetPlayerScale()));

	bx1 = GetDrawX();
	bx2 = bx1 + 20;
	by1 = GetDrawY();
	by2 = by1 + 20;

	if (px1 < bx2 && bx1 < px2 && py1 < by2 && by1 < py2)
	{
		deleteFlg = true;
		hitFlg = true;
	}
	if (stage->HitMapDat(mapY, mapX))
	{
		PlaySoundMem(splashSe, DX_PLAYTYPE_BACK);
		endFlg = true;
		animationTimer = 0;
		animationType = 0;
		if (stage->GetMapData(oldMapY + 1, oldMapX) != stage->GetMapData(mapY, mapX))
		{
			if (rad > 90 * (PI / 180))
			{
				hitRad = 0;
				drawX = -40;
			}
			else {}
			if (rad < 90 * (PI / 180))
			{
				hitRad = 180 * (PI / 180);
				drawX = 40;
			}
			else {}
		}
		rad = 1.6;
	}
}

float ENEMY_BULLET::GetDrawX() const
{
	float ret = (bulletX - scrollX) + (static_cast<double>(scrollX) + stage->GetScrollX());
	return ret;
}

float ENEMY_BULLET::GetDrawY() const
{
	float ret = (bulletY - scrollY) + (static_cast<double>(scrollY) + stage->GetScrollY());

	return ret;
}

