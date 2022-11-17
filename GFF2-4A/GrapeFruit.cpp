#include"DxLib.h"
#include "GrapeFruit.h"
#include<math.h>
#define _USE_MATH_DEFINES

#include <vector>



GRAPEFRUIT::GRAPEFRUIT()
{
	image = new int[100];
	if (image[0] = LoadGraph("Resource/Images/Enemy/gurepon.png") == -1)
		throw "Resource/Images/Enemy/gurepon.png";
	shootcount = 0;
	hitflg = false;
	delete_flg = false;
	rad = 0.0;
	for (int i = 0; i < 2; i++)
		rads[i] = 0.0;
	flag[3] = false;
	x = 0;
	spawn_map_x = 0;
	spawn_map_y = 0;
	target_x = 200;
	animation_timer = 0;
	animation_type = 0;
	check_hit_count = 0;
	hit_flg = false;
	for (int i = 0; i < 2; i++)
	{
		face_image[i] = 0;
		fruit_image[i] = 0;
	}
	for (int i = 0; i < 3; i++) {
		bullet[i] = nullptr;
	}
	bullet_count = 3;
}

GRAPEFRUIT::GRAPEFRUIT(PLAYER* player, STAGE* stage, int spawn_y, int spawn_x)
{
	spawn_map_x = spawn_x;
	spawn_map_y = spawn_y;
	x = (spawn_map_x * MAP_CEllSIZE + MAP_CEllSIZE / 2);
	y = (spawn_map_y * MAP_CEllSIZE + MAP_CEllSIZE / 2) - 40;
	for (int i = 0; i < 3; i++)
	{
		flag[i] = false;
	}
	delete_flg = false;
	image = new int[24];
	if (LoadDivGraph("Resource/Images/Enemy/gurepon.png", 24, 6, 4, 80, 80, image) == -1)
		throw "Resource/Images/Enemy/gurepon.png";
	shootcount = 0;
	target_x = 200;

	for (int i = 0; i < 2; i++)
	{
		face_image[i] = image[i];
		fruit_image[i] = image[(i + 1) * 6];
	}
	for (int i = 0; i < 2; i++)
	{
		rads[i] = 0.0;
	}
	for (int i = 0; i < 3; i++)
	{
		bullet[i] = nullptr;
	}
	this->player = player;
	this->stage = stage;
	bullet_count = 3;

	hit_flg = false;
}

void GRAPEFRUIT::Update()
{
	++animation_timer;
	switch (state)
	{
	case ENEMY_STATE::IDOL:
		break;
	case ENEMY_STATE::MOVE:
		ChangeAngle();
		if ((x + stage->GetScrollX() > 0) && (x + stage->GetScrollX() < 1280))
		{
			if (++shootcount % 300 == 0) 
			{
				animation_timer = 0;
				state = ENEMY_STATE::PRESS;
			}
		}

		break;
	case ENEMY_STATE::RETURN:
		ChangeAngle();
		if (ReturnAnimation())
		{
			animation_timer = 0;
			animation_type = 0;
			state = ENEMY_STATE::MOVE;
		}
		break;
	case ENEMY_STATE::PRESS:
		ChangeAngle();
		if (PressAnimation())
		{
			for (int i = 0; i < 3; i++)
			{
				if (flag[i] == false)
				{
					bullet[i] = new ENEMY_BULLET(player, stage, x, y, i * 100.0, stage->GetScrollX(), rad, 0);
					flag[i] = true;
				}
			}
			animation_timer = 0;
			animation_type = 0;
			state = ENEMY_STATE::RETURN;
		}
		break;
	case ENEMY_STATE::FALL:
		Move();
		FallAnimation();
		break;
	case ENEMY_STATE::DETH:
		if (DethAnimation())
		{
			delete_flg = true;
		}
		break;
	default:
		break;
	}

	//マップ上の座標の設定
	map_x = x / MAP_CEllSIZE;
	map_y = (y - IMAGE_SIZE / 2) / MAP_CEllSIZE;

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

	Hit();

	if ((x + stage->GetScrollX() < -IMAGE_SIZE) || (x + stage->GetScrollX() > 1280 + IMAGE_SIZE))		//画面外に出るとアイドル状態にする
	{
		state = ENEMY_STATE::IDOL;
	}
	else if (state == ENEMY_STATE::IDOL)	//画面内にいて、アイドル状態のとき敵の方向を向くようにする
	{
		animation_timer = 0;
		state = ENEMY_STATE::MOVE;
	}
	else {}
}

void GRAPEFRUIT::Move()
{
	y += 5;
	for (int i = 0; i < 3; i++)
	{
		flag[i] = false;
	}
	
}



void GRAPEFRUIT::Hit()
{
	ThrowSlime throw_slime;

	float bx1, by1, bx2, by2;
	float gx1, gy1, gx2, gy2;
	//プレイヤーが投げた体一部との当たり判定
	if ((state != ENEMY_STATE::FALL) || (state != ENEMY_STATE::DETH))
	{
		for (int i = 0; i < player->GetThrowCnt(); i++)
		{
			throw_slime = player->GetThrowSlime(i);
			//スライムのボールの当たり判定
			bx1 = throw_slime.GetThrowX();
			by1 = throw_slime.GetThrowY();
			bx2 = throw_slime.GetThrowX() + BALL_W;
			by2 = throw_slime.GetThrowY() - BALL_H;
			//グレープフルーツの当たり判定
			gx1 = x;
			gy1 = y;
			gx2 = gx1 + IMAGE_SIZE;
			gy2 = gy1 + IMAGE_SIZE;
			if (((bx2 >= gx1 && bx2 <= gx2) || (bx1 <= gx2 && bx1 >= gx1)) && ((by1 >= gy2 && by1 <= gy1) || (by2 >= gy1 && by2 <= gy2)))
			{
				rad = 90 * (PI / 180);
				state = ENEMY_STATE::FALL;
			}
		}
	}

	//地面やブロックとの当たり判定
	if (state == ENEMY_STATE::FALL)
	{
		if (stage->HitMapDat(map_y + 1, map_x))
		{
			state = ENEMY_STATE::DETH;
			animation_timer = 0;
			animation_type = 0;

		}
	}
}

bool GRAPEFRUIT::PressAnimation()
{

	bool ret = false;
	if (animation_timer < 40) //30フレーム間アニメーションをする
	{
		if (animation_timer % (ANIMATION_TIME * 2) == 0)
		{
			animation_type++;

			for (int i = 0; i < 2; i++)
			{
				face_image[i] = image[i];
				if (animation_type % 3 == 0)
				{
					fruit_image[i] = image[(6 * (i + 2) - 1)];
				}
				else
				{
					fruit_image[i] = image[((animation_type % 3) * 2) + (6 * (i + 1))];
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
bool GRAPEFRUIT::ReturnAnimation()
{
	bool ret = false;
	if (animation_timer < 40) //30フレーム間アニメーションをする
	{
		if (animation_timer % (ANIMATION_TIME * 2) == 0)
		{
			animation_type++;

			for (int i = 0; i < 2; i++)
			{
				face_image[i] = image[i];
				if (animation_type % 3 == 0)
				{
					fruit_image[i] = image[(i + 1) * 6];
				}
				else
				{
					fruit_image[i] = image[(6 * (i + 2) - 1) - (animation_type % 6)];
				}
			}
		}
	}
	else //アニメーションの終了
	{
		for (int i = 0; i < 2; i++)
		{
			face_image[i] = image[i];
			fruit_image[i] = image[(i + 1) * 6];
		}
		ret = true;
	}
	return ret;
}

void GRAPEFRUIT::FallAnimation()
{
	if (animation_timer % ANIMATION_TIME == 0)
	{
		if (animation_timer % (ANIMATION_TIME * 2) == 0)
		{
			for (int i = 0; i < 2; i++)
			{
				face_image[i] = image[(2 + (animation_type % 2) +  (i  * 2))];
				fruit_image[i] = image[(i + 1) * 6];
			}
			animation_type++;
		}
	}
}
bool GRAPEFRUIT::DethAnimation()
{
	bool ret = false;
	if (animation_timer < 30) //30フレーム間アニメーションをする
	{
		//アニメーション
		if (animation_timer % ANIMATION_TIME == 0)
		{
			for (int i = 0; i < 2; i++)
			{
				face_image[i] = 0;
			}
			fruit_image[0] = image[(animation_type % 6) + (6 * 3)];
			fruit_image[1] = 0;
			animation_type++;
		}
	}
	else //アニメーションの終了
	{
		ret = true;
	}
	return ret;
}


void GRAPEFRUIT::Draw() const
{
	for (int i = 0; i < 2; i++)
	{
		DrawRotaGraph(x + stage->GetScrollX(), (y + 10 * i) + stage->GetScrollY(), 1, rad + (-90 * (PI / 180)), fruit_image[i], TRUE);
		DrawRotaGraph(x + stage->GetScrollX(), (y + 10 * i) + stage->GetScrollY(), 1, rad + (-90 * (PI / 180)), face_image[i], TRUE);
	}
	

	if (flag)
	{
		for (int i = 0; i < bullet_count; i++)
		{
			if (bullet[i] != nullptr)
			{
				bullet[i]->Draw();
			}
		}
	}
}

int GRAPEFRUIT::GetX()
{
	return x;
}

