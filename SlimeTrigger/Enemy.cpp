#include "DxLib.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include "Enemy.h"

ENEMY::ENEMY()
{
	player = nullptr;
	stage = nullptr;
	item = nullptr;

	rad = 0.0;
	x = 0;
	y = 0;
	mapX = 0;
	mapY = 0;
	image = nullptr;
	animationTimer = 0;
	animationType = 0;

	State = ENEMY_STATE::IDOL;
}

void ENEMY::ChangeAngle()
{
	//���������_�Ƀv���C���[�Ƃ̊p�x���v�Z
	rad = atan2f(player->GetPlayerY() - (y + stage->GetScrollY()), player->GetPlayerX() - (x + stage->GetScrollX()));
	if (rad > 170 * (PI / 180))
	{
		rad = 170 * (PI / 180);
	}

	if (rad < 10 * (PI / 180))
	{
		rad = 10 * (PI / 180);
	}
}