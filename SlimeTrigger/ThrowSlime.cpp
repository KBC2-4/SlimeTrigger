#include "ThrowSlime.h"
#include "PadInput.h"
#include"Player.h"
#include<math.h>
#include "Tomato.h"

ThrowSlime::ThrowSlime(float playerX, float playerY, float throwRad, STAGE* stage) : ve(110.0f) {
	if ((image = LoadGraph("Resource/Images/Player/Slime_Bullet.png")) == -1) {
		throw "Resource/Images/Player/Slime_Bullet.png";
	}

	throw_del = false;

	vx0 = ve * cosf(throwRad);
	vy0 = ve * sinf(throwRad);

	x0 = playerX;
	y0 = playerY;

	vx = vx0;
	vy = vy0;

	time = 0.0f;
	maxY = stage->GetMapSize().y * MAP_CEllSIZE;
}

void ThrowSlime::Finalize() {
	DeleteGraph(image);
}

void ThrowSlime::Update(STAGE* stage, ELEMENT* element, TOMATO** tomaton, int tomatonCount) {
	x0 = x0 + vx * dt;
	y0 = y0 - vy * dt;
	vy = vy - g * dt;
	if (vy < 0) {
		g += 0.2f;
	}
	time += dt;

	if (maxY < y0) {
		throw_del = true;
	}
	else {
		Hit(stage, element, tomaton, tomatonCount);
	}
}

void ThrowSlime::Draw(STAGE *stage) const {
	DrawRotaGraph(x0 + stage->GetScrollX(), y0 + stage->GetScrollY(), 1.0, 0.0, image, TRUE);
}

void ThrowSlime::Hit(STAGE* stage, ELEMENT* element, TOMATO** tomaton, int tomatonCount) {
	//マップの上部を越えたら当たり判定を無視
	if (y0 <= 0) {
		return;
	}

	//リフトの当たり判定
	std::vector<ELEMENT::ELEMENT_DATA> lift = element->GetLift();
	for (int i = 0; i < lift.size(); i++) {
		if (x0 - BULLETRADIUS <= lift[i].x + LIFT_SIZE && x0 + BULLETRADIUS >= lift[i].x
			&& y0 - BULLETRADIUS <= lift[i].y + MAP_CEllSIZE / 2 && y0 + BULLETRADIUS >= lift[i].y) {
			throw_del = true;
			return;
		}
	}

	for (int i = 0; i < tomatonCount; i++) {
		float tomaton_x = tomaton[i]->GetX();
		float tomaton_y = tomaton[i]->GetY();
		if (x0 - BULLETRADIUS <= tomaton_x + MAP_CEllSIZE / 2 && x0 + BULLETRADIUS >= tomaton_x - MAP_CEllSIZE / 2 &&
			y0 - BULLETRADIUS <= tomaton_y + MAP_CEllSIZE / 2 && y0 + BULLETRADIUS >= tomaton_y - MAP_CEllSIZE / 2) {
			throw_del = true;
			return;
		}
	}
	
	int object = stage->GetMapData(static_cast<int>(floor((y0 / MAP_CEllSIZE))), static_cast<int>(x0 / MAP_CEllSIZE));
	if (stage->HitThrowSlime(static_cast<int>(floor(y0 / MAP_CEllSIZE)), static_cast<int>(x0 / MAP_CEllSIZE)) == true) {
		if (object == 91 || object == 92 || object == 93) { return; } //敵
		if (object == 51 || object == 52 || object == 53 || object == 54) return; //リフト
		if (21 <= object && object <= 32) { return; } //木
		if (object == 64 || object == 65) { return; } //開いたドア

		throw_del = true;
	}

	return;
}