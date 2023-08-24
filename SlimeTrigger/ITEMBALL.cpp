#include "ItemBall.h"




ITEMBALL::ITEMBALL()
{
	mapX = 0;
	mapY = 0;
	player = nullptr;
	itemX = 0;
	itemY = 0;
	item = 0;
	scroolX = 0;
}

ITEMBALL::ITEMBALL(float enemy_x, float enemy_y, int e_mapy, int e_mapx, PLAYER* r_player, STAGE* r_stage, float scroolx,float scrooly)
{
	if ((item = (LoadGraph("Resource/Images/Player/SlimeBullet2.png"))) == -1)
	{
		throw "Resource/Images/Enemy/ThrowSlime.png";
	}
	mapX = e_mapx;
	mapY = e_mapy;
	player = r_player;
	scroolX = scroolx;
	scroolY = scrooly;
	stage = r_stage;
	itemX = enemy_x;
	itemY = enemy_y;
}

ITEMBALL::~ITEMBALL() {
	DeleteGraph(item);
};

void ITEMBALL::Draw() const
{
	DrawRotaGraph((static_cast<int>(GetDrawX())), (static_cast<int>(GetDrawY())) + 30, 1, 0, item, TRUE);
}

void ITEMBALL::Update()
{
	PlayerHit();
}

void ITEMBALL::PlayerHit()
{
	float px1, py1, px2, py2;
	float bx1, by1, bx2, by2;

	px1 = player->GetPlayerX() - 30;
	px2 = px1 + 60;
	py1 = player->GetPlayerY();
	py2 = py1 + 40;

	bx1 = GetDrawX() - 10;
	bx2 = bx1 + _BALL_RADIUS;
	by1 = GetDrawY() + _BALL_RADIUS;
	by2 = by1 + _BALL_RADIUS;


	if (px1 < bx2 && bx1 < px2 && py1 < by2 && by1 < py2)
	{
		if (player->GetLife() < 5)
		{
			player->SetLife(player->GetLife() + 1);
			deleteFlag = true;
		}
	}

}

float ITEMBALL::GetDrawX() const
{
	float ret = (itemX - scroolX) + (static_cast<double>(scroolX) + stage->GetScrollX());
	return ret;
}

float ITEMBALL::GetDrawY() const
{
	float ret = (itemY - scroolY) + (static_cast<double>(scroolY) + stage->GetScrollY());

	return ret;
}