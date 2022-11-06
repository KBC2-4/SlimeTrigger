#include "EnemyBullet.h"

ENEMYBULLET::ENEMYBULLET() 
{
	player = nullptr;
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
	bullet_flag = false;
}

ENEMYBULLET::ENEMYBULLET(PLAYER* player ,int x,int y) 
{
	this->player = new PLAYER;
	player_x = player->GetPlayerX();
	player_y = player->GetPlayerY();
	bullet_x = 0.0;
	bullet_y = 0.0;
	hypote = 0.0;
	my_x = x;
	my_y = y;
	dis_x = 0.0;
	dis_y = 0.0;
	bullet_sx = 0.0;
	bullet_sy = 0.0;
	bullet_flag = true;
}

void ENEMYBULLET::Draw(float scroll_x) const 
{
	DrawBox(bullet_x, bullet_y, bullet_x + 40, bullet_y + 40, 0xff00ff, TRUE);
}

void ENEMYBULLET::Update() 
{
	dis_x = my_x - player_x;
	dis_y = my_y - player_y;
	
	hypote = sqrt(pow(dis_x, 2) + pow(dis_y, 2));

	bullet_sx = dis_x / hypote * 20;
	bullet_sy = dis_y / hypote * 20;
}

void ENEMYBULLET::Move() 
{
	//弾の移動
	bullet_x += bullet_sx;
	bullet_y += bullet_sy;

	//弾が画面外に行ったら消えるフラグを真に
	if (bullet_x < 0 || bullet_x>1280 || bullet_y < 0 || bullet_y>720)
		bullet_flag = true;
}

