#pragma once
#include "Enemy.h"
class SPLASH
{
private:
	int x, y;	//座標
	int w, h;	//幅、高さ
	int image;

	ENEMY* enemy;
public:

	virtual void Draw()const;
	virtual void Move();
};

