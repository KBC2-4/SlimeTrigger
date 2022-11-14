#include "DxLib.h"
#include "Lemon.h"

LEMON::LEMON()
{
	//画像の取得
	image = nullptr;
	shootcount = 0;
	hitflg = false;
	rad = 0.0;
	for (int i = 0; i < 2; i++)
		rads[i] = 0.0;
	delete_flag = false;
	x = 0;
	spawn_map_x = 0;
	spawn_map_y = 0;
	animation_timer = 0;
	animation_type = 0;
	now_image = 0;

	state = ENEMY_STATE::IDOL;
	bullet = nullptr;
}


LEMON::LEMON(PLAYER* player, STAGE* stage, int spawn_y, int spawn_x)
{
	//スポーン場所の設定
	spawn_map_x = spawn_x;
	spawn_map_y = spawn_y;
	x = (spawn_map_x * MAP_CEllSIZE + MAP_CEllSIZE / 2);
	y = spawn_map_y * MAP_CEllSIZE;
	hitflg = false;
	delete_flag = false;
	
	//画像の取得
	image = new int[4];
	if (LoadDivGraph("Resource/Images/Enemy/lemon.png", 4, 4, 1, 80, 80, image) == -1)
	{
		throw "Resource/Images/Enemy/lemon.png";
	}
	shootcount = 0;

	for (int i = 0; i < 2; i++)
	{
		rads[i] = 0.0;
	}
	bullet = nullptr;
	this->player = player;
	this->stage = stage;

	animation_timer = 0;
	animation_type = 0;
	now_image = image[1];
	state = ENEMY_STATE::IDOL;

}


LEMON::~LEMON()
{
	delete[] image;
	if (bullet != nullptr)
	{
		delete bullet;
	}
}

void LEMON::Update()
{
	++animation_timer;
	switch (state)
	{
	case ENEMY_STATE::IDOL:
		break;
	case ENEMY_STATE::MOVE:
		ChangeAngle();
		if((x+stage->GetScrollX() > 0) && (x + stage->GetScrollX()<1280))
		{
			if (++shootcount % 180 == 0)
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
			state = ENEMY_STATE::MOVE;
		}
		break;
	case ENEMY_STATE::PRESS:
		ChangeAngle();
		bullet = new ENEMYBULLET(player, stage, x, y, 0, stage->GetScrollX(),rad,2);
		animation_timer = 0;
		state = ENEMY_STATE::RETURN;
		break;
	case ENEMY_STATE::FALL:
		Move();
		FallAnimation();
		break;
	case ENEMY_STATE::DETH:
		if (DethAnimation())
		{
			delete_flag = true;
		}
		break;
	default:
		break;
	}

	//マップ上の座標の設定
	map_x = x / MAP_CEllSIZE;
	map_y = (y - IMAGE_SIZE / 2) / MAP_CEllSIZE;

	//弾が存在しているときに弾の処理を行う
	if (bullet != nullptr)
	{
		bullet->Update();

		//弾が画面外に出るか、何かに当たったら弾を削除
		if (bullet->GetDeleteFlg())
		{
			delete bullet;
			bullet = nullptr;
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
	else{ }
}

void LEMON::Move()
{
	y += 5;
}

void LEMON::Hit()
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
		if (stage->HitMapDat(map_y + 1,map_x))
		{
			state = ENEMY_STATE::DETH;
			animation_timer = 0;
		}
	}
}

bool LEMON::ReturnAnimation()
{
	bool ret = false;
	if (animation_timer < 120) //120フレーム間アニメーションをする
	{
	}
	else //アニメーションの終了
	{
		ret = true;
	}
	return ret;
}

void LEMON::FallAnimation()
{
	if (animation_timer % ANIMATION_TIME == 0)
	{
		now_image = image[(++animation_type % 2) + 2];
	}
}
bool LEMON::DethAnimation()
{
	bool ret = false;
	if (animation_timer < 30) //120フレーム間アニメーションをする
	{

	}
	else //アニメーションの終了
	{
		ret = true;
	}
	return ret;
}

void LEMON::Draw() const
{
	if (bullet != nullptr)		//弾が存在するとき弾を描画する
	{
		bullet->Draw();
	}
	DrawRotaGraph(x + stage->GetScrollX(), y, 1, rad + (-90 * (PI / 180)), now_image, TRUE);

	
}