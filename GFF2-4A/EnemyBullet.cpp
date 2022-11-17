#include "EnemyBullet.h"
#include <math.h>

ENEMYBULLET::ENEMYBULLET()
{
	player = nullptr;
	stage = nullptr;
	player_x = 0.0;
	player_y = 0.0;
	bullet_x = 0.0;
	bullet_y = 0.0;
	hypote = 0.0;
	my_x = 0.0;
	my_y = 0.0;
	dis_x = 0.0;
	dis_y = 0.0;
	bullet_sx = 0.0;
	bullet_sy = 0.0;
	delete_flg = false;
	hit_flg = false;
	rad_x = 0.0;
	map_x = 0;
	map_y = 0;
	image_indx = 0;
	for (int i = 0; i < 4; i++)
	{
		bullet_images[i] = 0;
	}
}

ENEMYBULLET::ENEMYBULLET(PLAYER* argu_player, STAGE* aug_stage, int x, int y, double dis, float scroll,double p_rad,int index)
{
	if (LoadDivGraph("Resource/images/Enemy/Enemy_Bullet.png",4,4,1,20,20,bullet_images)== -1)
		throw "Resource/Images/Enemy/Enemy_Bullet.png";
	player = argu_player;
	player_x = player->GetPlayerX();
	player_y = player->GetPlayerY();
	bullet_x = x;
	bullet_y = y;
	hypote = 0.0;
	my_x = x;
	my_y = y;
	dis_x = 0.0;
	dis_y = 0.0;
	bullet_sx = 0.0;
	bullet_sy = 0.0;
	delete_flg = false;
	hit_flg = false;
	rad_x = dis;
	scroll_x = abs(scroll);
	map_x = 0;
	map_y = 0;
	rad = p_rad;
	image_indx = index;

	stage = aug_stage;
	dis_x = (player_x + rad_x) - (my_x - static_cast<double>(scroll_x));
	dis_y = player_y - my_y;

	hypote = sqrt((dis_x * dis_x) + (dis_y * dis_y));

	bullet_sx = dis_x / hypote * 5;
	bullet_sy = dis_y / hypote * 5;
}

void ENEMYBULLET::Draw() const
{
	DrawRotaGraph(static_cast<int>(GetDrawX()), bullet_y+stage->GetScrollY(), 2, rad + (-90 * (PI / 180)), bullet_images[image_indx], TRUE);
}

void ENEMYBULLET::Update()
{
	Move();
	Hit();

	if (hit_flg)
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

}

void ENEMYBULLET::Move()
{
	//弾の移動
	bullet_x += bullet_sx;
	bullet_y += bullet_sy;

	if (GetDrawX() < 0 || GetDrawX() > 1280) 
	{
		delete_flg = true;
	}

	//弾が画面外に行ったら消えるフラグを真に
	mapd_x = bullet_x / MAP_CEllSIZE;
	mapd_y = bullet_y / MAP_CEllSIZE;

	map_x = (int)floor(mapd_x);
	map_y = (int)floor(mapd_y);

}

void ENEMYBULLET::Animation()
{

}

void ENEMYBULLET::Hit()
{
	
	float px1, py1, px2, py2;
	float bx1, by1, bx2, by2;

	px1 = player->GetPlayerX() - 30;
	px2 = px1 + 60;
	py1 = player_y;
	py2 = py1 + 40;

	bx1 = GetDrawX();
	bx2 = bx1 + 20;
	by1 = bullet_y;
   	by2 = by1 + 20;

	if (px1 < bx2 && bx1 < px2 && py1 < by2 && by1 < py2)
	{
		delete_flg = true;
		hit_flg = true;
	}
	if (stage->HitMapDat(map_y, map_x))
	{
		delete_flg = true;
		stage->HitMapDat(map_y, map_x);
	}
}

float ENEMYBULLET::GetDrawX() const
{
	float ret = (bullet_x - scroll_x) + (static_cast<double>(scroll_x) + stage->GetScrollX());
	return ret;
}
