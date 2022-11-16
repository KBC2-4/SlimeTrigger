#pragma once
#include<math.h>
#include"DxLib.h"
#include"STAGE.h"
#include"PLAYER.h"
#define _BALL_RADIUS 20

class ITEMBALL
{
private:
	int map_x, map_y;
	float item_x, item_y;
	int item;
	bool delete_flg;
	PLAYER* player;
	STAGE* stage;

public:
	ITEMBALL();
	ITEMBALL(float, float,int,int, PLAYER*,STAGE*);
	~ITEMBALL();

	void Draw()const;
	void Update();
	void PlayerHit();
	bool GetDeleteFlag() { return delete_flg; }
};
