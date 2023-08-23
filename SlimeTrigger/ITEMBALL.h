#pragma once
#include<math.h>
#include"DxLib.h"
#include"Stage.h"
#include"Player.h"
#define _BALL_RADIUS 20

class ITEMBALL
{
private:
	int mapX, mapY;
	float itemX, itemY;
	int item;
	bool deleteFlag;
	PLAYER* player;
	STAGE* stage;
	float scroolX;
	float scroolY;

public:
	ITEMBALL();
	ITEMBALL(float, float,int,int, PLAYER*,STAGE* ,float,float);
	~ITEMBALL();

	void Draw()const;
	void Update();
	void PlayerHit();
	float GetDrawX() const;
	float GetDrawY() const;
	bool GetDeleteFlag() { return deleteFlag; }
	float GetItemX() { return itemX; }

};

