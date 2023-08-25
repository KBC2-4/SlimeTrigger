#include "DxLib.h"
#include "Tomato.h"
#include "Option.h"

TOMATO::TOMATO()
{
	x = 0;
	imageRate = 0.1;
	spawnMapX = 0;
	spawnMapY = 0;

	nowImage = 0;
	
	if (LoadDivGraph("Resource/Images/Enemy/tomaton.png", 3, 3, 1, 80, 80, &images[0][0]) == -1)
	{
		throw "Resource/Images/Enemy/tomaton.png";
	}
}

TOMATO::TOMATO(PLAYER* player, STAGE* stage, int spawn_y, int spawn_x)
{
	//スポーン場所の設定
	spawnMapX = spawn_x;
	spawnMapY = spawn_y;
	x = spawnMapX * MAP_CEllSIZE + MAP_CEllSIZE / 2;
	y = spawnMapY * MAP_CEllSIZE + MAP_CEllSIZE / 2;

	this->player = player;
	this->stage = stage;

	imageRate = 0.1;
	state = ENEMY_STATE::IDOL;

	nowImage = 0;
	images.resize(2);
	images[0].resize(3);
	images[1].resize(6);
	if (LoadDivGraph("Resource/Images/Enemy/tomaton.png", 3, 3, 1, 80, 80, &images[0][0]) == -1)
	{
		throw "Resource/Images/Enemy/tomaton.png";
	}
	if (LoadDivGraph("Resource/Images/Enemy/tomaton_Break.png", 6, 6, 1, 80, 80, &images[1][0]) == -1)
	{
		throw "Resource/Images/Enemy/tomaton_Break.png";
	}

	if ((splashSe = LoadSoundMem("Resource/Sounds/SE/Enemy/splash.wav")) == -1) {
		throw "Resource/Sounds/SE/Enemy/splash.wav";
	}

	ChangeVolumeSoundMem(Option::GetSEVolume(), splashSe);

}

TOMATO::~TOMATO()
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

	DeleteSoundMem(splashSe);

}
void TOMATO::Update()
{
	ChangeVolumeSoundMem(Option::GetSEVolume(), splashSe);

	if (animationTimer < 80) {
		++animationTimer;
	}
	switch (state)
	{
	case ENEMY_STATE::IDOL:
		//プレイヤーが一定範囲以内に入っている間落ちる
		if ((IdolAnimation()) && (fabsf(player->GetPlayerX() - (x + stage->GetScrollX())) < 240) && (y + stage->GetScrollY() > 0))
		{
			animationTimer = 0;
			state = ENEMY_STATE::FALL;
		}
		break;
	case ENEMY_STATE::FALL:
		Move();
		Hit();
		FallAnimation();
		if (y + stage->GetScrollY() > 720)
		{
			state = ENEMY_STATE::DETH;
		}
		break;
	case ENEMY_STATE::DETH:
		//爆発し終え時または、画面外に出たらアイドル状態にする
		if(DethAnimation() || (y + stage->GetScrollY() > 720))
		{
			state = ENEMY_STATE::IDOL;
			imageRate = 0;
			//スポーン地点に移動
			x = spawnMapX * MAP_CEllSIZE + MAP_CEllSIZE / 2;
			y = spawnMapY * MAP_CEllSIZE + MAP_CEllSIZE / 2;

		}
		break;
	default:
		break;
	}
	//マップ上の座標の設定
	mapX = x / MAP_CEllSIZE;
	mapY = (y - IMAGE_SIZE / 2) / MAP_CEllSIZE;

}

void TOMATO::Move()
{
	y += FALL_SPEED;
}


void TOMATO::Hit()
{
	float px1, py1, px2, py2;
	float bx1, by1, bx2, by2;

	px1 = player->GetPlayerX() - (20 * (player->GetPlayerScale()));
	px2 = px1 + (50 * (player->GetPlayerScale()));
	py1 = player->GetPlayerY() + (20 * (2 - player->GetPlayerScale()));
	py2 = py1 + (25 * (player->GetPlayerScale()));

	bx1 = x - IMAGE_SIZE / 2;
	bx2 = bx1 + IMAGE_SIZE;
	by1 = y + stage->GetScrollY() - IMAGE_SIZE / 2.5;
	by2 = by1 + IMAGE_SIZE / 2;

	//プレイヤーとの当たり判定
	if ((px1 < bx2) && (bx1 < px2) && (py1 < by2) && (by1 < py2))
	{
		player->SetLife(player->GetLife() - 2);
	}

	//地面やブロックとの当たり判定
	if (stage->HitMapDat(mapY + 1,mapX))
	{
		state = ENEMY_STATE::DETH;
		animationTimer = 0;
		animationType = 00;
		PlaySoundMem(splashSe, DX_PLAYTYPE_BACK);

	}
}

bool TOMATO::IdolAnimation()
{
	bool ret = false;
	//アニメーション
	if (animationTimer % ANIMATION_TIME == 0)
	{
		if (imageRate < 1.0) //とまとんのサイズを大きくする
		{
			imageRate += 0.1;
		}
		else //とまとんのサイズが元のサイズより大きくなったら元のサイズに戻す
		{
			imageRate = 1.0;
			ret = true;
		}
		nowImage = images[0][0];
	}
	return ret;
}
void TOMATO::FallAnimation()
{
	//アニメーション
	if (animationTimer % ANIMATION_TIME == 0)
	{
		nowImage = images[0][(++animationType % 2) + 1];
	}
}


bool TOMATO::DethAnimation()
{
	bool ret = false;
	
	if (animationTimer < 15)	//15フレーム間アニメーション
	{
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


void TOMATO::Draw()const
{
	//画面外に出たら描画しない
	if ((x + stage->GetScrollX() > -IMAGE_SIZE) && (x + stage->GetScrollX() < 1280 + IMAGE_SIZE))
	{
		DrawRotaGraphF(x + stage->GetScrollX(), y + stage->GetScrollY(), imageRate, 0, nowImage, TRUE);
	}

}