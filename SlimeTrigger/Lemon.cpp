#include "DxLib.h"
#include "Lemon.h"
#include "Option.h"

LEMON::LEMON()
{
	//画像の取得
	damageSe = 0;

	shootCount = 0;
	hitFlg = false;
	deleteFlg = false;
	x = 0;
	spawnMapX = 0;
	spawnMapY = 0;
	nowImage = 0;

	state = ENEMY_STATE::IDOL;
	bullet = nullptr;
}


LEMON::LEMON(PLAYER* player, STAGE* stage, int spawn_y, int spawn_x)
{
	//スポーン場所の設定
	spawnMapX = spawn_x;
	spawnMapY = spawn_y;
	x = (spawnMapX * MAP_CEllSIZE + MAP_CEllSIZE / 2);
	y = spawnMapY * MAP_CEllSIZE;
	hitFlg = false;
	deleteFlg = false;

	//画像の取得
	images.resize(2);
	images[0].resize(9);
	images[1].resize(6);
	if (LoadDivGraph("Resource/Images/Enemy/lemon.png", 9, 9, 1, 80, 80, &images[0][0]) == -1)
	{
		throw "Resource/Images/Enemy/lemon.png";
	}
	if (LoadDivGraph("Resource/Images/Enemy/lemon_break.png", 6, 6, 1, 80, 80, &images[1][0]) == -1)
	{
		throw "Resource/Images/Enemy/lemon_break.png";
	}
	//SEの取得
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

	bullet = nullptr;
	this->player = player;
	this->stage = stage;


	nowImage = images[0][0];
	state = ENEMY_STATE::IDOL;
	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.2), damageSe);
	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 0.9), pressSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), splashSe);
}


LEMON::~LEMON()
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
	if (bullet != nullptr)
	{
		delete bullet;
	}

	DeleteSoundMem(damageSe);
	DeleteSoundMem(pressSe);
	DeleteSoundMem(splashSe);
}

void LEMON::Update()
{

	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.2), damageSe);
	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 0.9), pressSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), splashSe);

	if (animationTimer < 80) {
		++animationTimer;
	}

	switch (state)
	{
	case ENEMY_STATE::IDOL:
		break;
	case ENEMY_STATE::MOVE:
		ChangeAngle();
		if (player->GetMapY() > mapY)
		{
			if (++shootCount % 60 == 0)
			{
				animationTimer = 0;
				state = ENEMY_STATE::PRESS;

			}
		}
		break;
	case ENEMY_STATE::RETURN:
		ChangeAngle();
		if (ReturnAnimation())
		{
			animationTimer = 0;
			animationType = 0;
			state = ENEMY_STATE::MOVE;
		}
		break;
	case ENEMY_STATE::PRESS:
		ChangeAngle();
		if (PressAnimation())
		{
			bullet = new ENEMY_BULLET(player, stage, x, y, 0, rad, 1);
			animationTimer = 0;
			animationType = 0;
			state = ENEMY_STATE::RETURN;
			PlaySoundMem(pressSe, DX_PLAYTYPE_BACK);
		}
		break;
	case ENEMY_STATE::FALL:
		Move();
		FallAnimation();
		break;
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
	if (bullet != nullptr)
	{
		bullet->Update();

		//弾が画面外に出るか、何かに当たったら弾を削除
		if (bullet->GetDeleteFlg())
		{
			delete bullet;
			bullet = nullptr;
		}
	}

	Hit();

	if (((x + stage->GetScrollX() < -IMAGE_SIZE) || (x + stage->GetScrollX() > 1280 + IMAGE_SIZE) || (y + stage->GetScrollY() < 0)
		|| (y + stage->GetScrollY() > 720)) && (state != ENEMY_STATE::FALL && state != ENEMY_STATE::DETH))		//画面外に出るとアイドル状態にする
	{
		state = ENEMY_STATE::IDOL;	//ステートをアイドル状態へ
		//アイドル状態の画像に変更
		nowImage = images[0][0];
	}
	else if (state == ENEMY_STATE::IDOL)	//画面内にいて、アイドル状態のとき敵の方向を向くようにする
	{
		// アニメーション時間をリセットし、ステートをムーブへ
		animationTimer = 0;
		state = ENEMY_STATE::MOVE;
	}
	else {}
}


void LEMON::Move()
{
	y += 10;
}

void LEMON::Hit()
{
	ThrowSlime throwSlime;

	float bx1, by1, bx2, by2;
	float gx1, gy1, gx2, gy2;
	//プレイヤーが投げた体の一部との当たり判定
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
			//レモナーの当たり判定
			gx1 = x - IMAGE_SIZE / 2.5;
			gy1 = y - IMAGE_SIZE / 2;
			gx2 = gx1 + IMAGE_SIZE / 1.25;
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
			animationType = 0;
			PlaySoundMem(splashSe, DX_PLAYTYPE_BACK);
		}
	}
}

bool LEMON::PressAnimation()
{

	bool ret = false;
	if (animationTimer < 40) //40フレーム間アニメーションをする
	{
		if (animationTimer % (ANIMATION_TIME * 2) == 0)
		{
			if (animationType < 2)
			{
				nowImage = images[0][(animationType++ % 3) + 2];
			}
			

		}
	}
	else //アニメーションの終了
	{
		ret = true;
	}
	return ret;
}

bool LEMON::ReturnAnimation()
{
	bool ret = false;
	if (animationTimer < 30) //50フレーム間アニメーションをする
	{
		if (animationTimer % (ANIMATION_TIME * 2) == 0)
		{
			nowImage = images[0][(animationType++ % 3) + 2];
		}
		
	}
	else //アニメーションの終了
	{
		nowImage = images[0][0];
		ret = true;
	}
	return ret;
}

void LEMON::FallAnimation()
{
	if (animationTimer % ANIMATION_TIME == 0)
	{
		nowImage = images[0][(++animationType % 2) + 7];
	}
}

bool LEMON::DethAnimation()
{
	bool ret = false;
	if (animationTimer < 30) //30フレーム間アニメーションをする
	{
		//アニメーション
		if (animationTimer % ANIMATION_TIME == 0)
		{
			nowImage = images[1][(++animationType % 6)];
		}
	}
	else //アニメーションの終了
	{
		ret = true;
	}
	return ret;
}

void LEMON::Draw() const
{
	float gx1, gy1, gx2, gy2;

	if (bullet != nullptr)		//弾が存在するとき弾を描画する
	{
		bullet->Draw();
	}
	DrawRotaGraphF(x + stage->GetScrollX(), y + stage->GetScrollY(), 1, rad + (-90 * (PI / 180)), nowImage, TRUE);
}