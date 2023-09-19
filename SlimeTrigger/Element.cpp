#include "Element.h"
#include "Player.h"
#include "Option.h"

/// <summary>
/// コンストラクタ
/// </summary>
ELEMENT::ELEMENT(const char* stageName) : STAGE(stageName) {

	guidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	keyboardGuidFont = CreateFontToHandle("メイリオ", 9, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	if ((doorCloseSe = LoadSoundMem("Resource/Sounds/SE/Stage/door_close.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/door_close.wav";
	}

	if ((pressTheButtonSe = LoadSoundMem("Resource/Sounds/SE/Stage/press_the_button.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/press_the_button.wav";
	}

	if ((switchSe = LoadSoundMem("Resource/Sounds/SE/Stage/switch.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/switch.wav";
	}

	if ((walkPuddleSe = LoadSoundMem("Resource/Sounds/SE/Stage/walk_puddle.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/walk_puddle.wav";
	}

	if ((manholeOpenedSe = LoadSoundMem("Resource/Sounds/SE/Stage/manhole_opened.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/manhole_opened.wav";
	}

	//酸溜まりの画像読み込み
	LoadDivGraph("Resource/Images/Stage/acid.png",12,12,1,80,80,acidImage);
	//泡アニメーションの初期化
	foamAnimation = 4;

	//動く床の画像読み込み
	LoadDivGraph("Resource/Images/Stage/MoveFloor.png", 3, 3, 1, 80, 80, moveFloorImage);
	//泡アニメーションの初期化
	moveFloorAnimation = 0;

	ELEMENT_DATA data;
	for (int i = 0; i < mapData.size(); i++)
	{
		for (int j = 0; j < mapData.at(0).size(); j++)
		{
			int search_vector = 1;
			//探索座標
			int lift_pos = i + 1;
			int button_num_1 = 0;
			int button_num_2 = 0;
			int door_num = 0;
			switch (mapData.at(i).at(j))
			{
				//ボタン横
			case 61:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2) + 25);
				data.y = static_cast<float>(i * MAP_CEllSIZE + MAP_CEllSIZE / 2);
				data.type = 1;
				data.flag = false;
				data.animTimer = 0;

				//ボタンとドアの連携番号を格納
				while (button_num_1 < buttonInfo.size()) {
					if (buttonInfo.at(button_num_1).at(0) == i && buttonInfo.at(button_num_1).at(1) == j) {
						data.pairNum = buttonInfo.at(button_num_1).at(2);
						break;
					}
					button_num_1++;
				}

				button.push_back(data);
				break;

				//ボタン(下)
			case 62:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.type = 2;
				data.flag = false;
				data.animTimer = 0;

				//ボタンとドアの連携番号を格納
				while (button_num_2 < buttonInfo.size()) {
					if (buttonInfo.at(button_num_2).at(0) == i && buttonInfo.at(button_num_2).at(1) == j) {
						data.pairNum = buttonInfo.at(button_num_2).at(2);
						break;
					}
					button_num_2++;

				}

				button.push_back(data);
				break;


				//ボタン(ones)下
			case 63:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2) + 25);
				data.type = 3;
				data.flag = false;
				data.animTimer = 0;
				button.push_back(data);
				break;

				//閉まっているドアの下
			case 66:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.type = 1;
				data.flag = false;
				data.animTimer = 0;

				//ボタンとドアの連携番号を格納
				while (door_num < doorInfo.size()) {
					if (doorInfo.at(door_num).at(0) == i && doorInfo.at(door_num).at(1) == j) {
						data.pairNum = doorInfo.at(door_num).at(2);
						break;
					}
					door_num++;
				}

				door.push_back(data);
				break;

				//マンホールの蓋
			case 68:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.leftInitX = j;		//配列データ
				data.leftInitY = i;		//配列データ
				data.pairNum = 0;			//演算完了フラグ
				data.liftWaitTimer = 0;	//中間地点の数カウント用
				data.type = 1;
				data.flag = false;
				data.animTimer = 0;
				data.liftWaitTimer = 0;	//暗闇エフェクト
				manhole.push_back(data);
				break;

				//マンホール(中間)
			case 69:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.leftInitX = j;		//配列データ
				data.leftInitY = i;		//配列データ
				data.pairNum = 0;			//演算完了フラグ
				data.type = 2;
				data.flag = false;
				data.animTimer = 0;
				manhole.push_back(data);
				break;

				//マンホール(出口)
			case 97:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.leftInitX = j;		//配列データ
				data.leftInitY = i;		//配列データ
				data.liftWaitTimer = 0;	//中間地点の数カウント用
				data.type = 3;
				data.flag = false;
				data.animTimer = 0;
				manhole.push_back(data);
				break;

				//酸性雨の水たまり(左端)
			case 74:
			case 75:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.type = 1;
				data.animTimer = 0;
				data.flag = false;
				acidrainPuddles.push_back(data);
				break;

				//酸性雨の水たまり(中央)
			case 76:
			case 77:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.type = 2;
				data.animTimer = 0;
				data.flag = false;
				data.image = 99;
				acidrainPuddles.push_back(data);
				break;

				//酸性雨の水たまり(右端)
			case 78:
			case 79:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.type = 3;
				data.animTimer = 0;
				data.flag = false;
				acidrainPuddles.push_back(data);
				break;

				//フック
			case 72:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				hook.push_back(data);
				break;

				//動く床(縦移動)
			case 51:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.leftInitX = data.x;
				data.leftInitY = data.y;
				data.leftVectorX = 0;
				data.leftVectorY = 0;
				data.liftWaitTimer = 0;
				
				if (mapData.size() < lift_pos)
				{
					lift_pos = i - 1;
					search_vector = -1;
				}
				//ゴール位置を格納
				for (lift_pos; lift_pos >= 0 && lift_pos < mapData.size(); lift_pos += search_vector) {
					if (mapData.at(lift_pos).at(j) == 53) {
						data.leftGoalX = static_cast<float>((j * MAP_CEllSIZE));
						data.leftGoalY = static_cast<float>((lift_pos * MAP_CEllSIZE));
						break;
					}
					else if (mapData.at(lift_pos).at(j) > 0 && (mapData.at(i).at(lift_pos) < 21 || 32 < mapData.at(i).at(lift_pos) && mapData.at(i).at(lift_pos) != 71 && mapData.at(i).at(lift_pos) != 72 && mapData.at(i).at(lift_pos) < 91 && 93 < mapData.at(i).at(lift_pos)))
					{
						if (search_vector < 0) { break; }
						lift_pos = i - 1;
						search_vector = -1;
					}
					else if (mapData.size() <= lift_pos + 1)
					{
						lift_pos = i - 1;
						search_vector = -1;
					}
				}
				data.type = 1;
				data.flag = false;
				data.animTimer = 0;
				data.image = 0;
				lift.push_back(data);
				break;
				//動く床(横移動)
			case 52:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.leftInitX = data.x;
				data.leftInitY = data.y;
				data.leftVectorX = 0;
				data.leftVectorY = 0;
				data.liftWaitTimer = 0;
				//ゴール位置を格納
				for (int lift_pos = j + 1; lift_pos >= 0 && lift_pos < mapData.at(0).size(); lift_pos += search_vector) {
					if (mapData.at(i).at(lift_pos) == 54) {
						data.leftGoalX = static_cast<float>((lift_pos * MAP_CEllSIZE));
						data.leftGoalY = static_cast<float>((i * MAP_CEllSIZE));
						break;
					}//進路方向に障害物があればゴール探索位置を逆にする
					if (0 < mapData.at(i).at(lift_pos) && (mapData.at(i).at(lift_pos) < 21 || 32 < mapData.at(i).at(lift_pos) && mapData.at(i).at(lift_pos) != 71 && mapData.at(i).at(lift_pos) != 72 && mapData.at(i).at(lift_pos) < 91 && 93 < mapData.at(i).at(lift_pos))) {
						if (search_vector < 0) { break; }
						lift_pos = j - 1;
						search_vector *= -1;
						continue;
					}
				}
				data.type = 2;
				data.flag = false;
				data.animTimer = 0;
				data.image = 0;
				lift.push_back(data);
				break;

			default:
				break;
			}


		}

	}

	if (0 < acidrainPuddles.size())
	{
		acidrainPuddles.at(0).image = 1;
	}

	playerMapX = 0;
	playerMapY = 0;
	keepPushing = false;

	playerState = 0;
	guidTimer = 0;
	acidrainPuddlesAniTimer = 0;

	hookFlag = false;
	isInUnder = false;

	//SE
	ChangeVolumeSoundMem(Option::GetSEVolume(), doorCloseSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), pressTheButtonSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), switchSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), walkPuddleSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), manholeOpenedSe);

}

/// <summary>
/// デストラクタ
/// </summary>
ELEMENT::~ELEMENT() {
	DeleteFontToHandle(guidFont);
	DeleteFontToHandle(keyboardGuidFont);
	DeleteSoundMem(doorCloseSe);
	DeleteSoundMem(pressTheButtonSe);
	DeleteSoundMem(switchSe);
	DeleteSoundMem(walkPuddleSe);
	DeleteSoundMem(manholeOpenedSe);

	//逆に1MB程メモリ喰うのでやらない事
	/*hook.clear();
	hook.shrink_to_fit();*/

	underGroundEffects = 0;
}

/// <summary>
/// 描画
/// </summary>
void ELEMENT::Draw(STAGE* stage, PLAYER* player) {
	//DrawFormatString(100, 50, 0xffffff, "%2f %2f", scrollX, scrollY);
	//DrawFormatString(100,50,0xffffff,"mapData:%d",mapData[int(playerMapY) / MAP_CEllSIZE + 1][int(playerMapX) / MAP_CEllSIZE]);
	//static int animTimer = 0;
	//printfDx("%d", animTimer);
	//DrawFormatString(200, 100, 0xFFFFFF, "acidrainPuddles.x%f\acidrainPuddles.y%f", acidrainPuddles[1].x, acidrainPuddles[1].y);
	//DrawFormatString(200, 200, 0xFFFFFF, "x%f\nY%f", playerMapX, playerMapY);
	//デバッグ用



	//フックのガイド表示
	for (int i = 0; i < hook.size(); i++)
	{
		if (hook[i].flag == true)
		{
			if (playerState != static_cast<int>(PLAYER_MOVE_STATE::HOOK))
			{
				DrawGuide(hook[i].x, hook[i].y, stage);
			}
		}
	}

	//ボタン
	for (int i = 0; i < button.size(); i++) {

		if (button[i].type == 2) {

			if (button[i].flag == false) {
				DrawOvalAA(button[i].x + stage->GetScrollX(), button[i].y + stage->GetScrollY() + 30, 25, 10, 20, 0xbfcb4e, TRUE, 1.0f);
			}
			if (button[i].flag == true && button[i].animTimer < 30) {
				SetDrawArea(static_cast<int>(button[i].x + stage->GetScrollX()) - MAP_CEllSIZE / 2, static_cast<int>( button[i].y + stage->GetScrollY()) - MAP_CEllSIZE / 2, static_cast<int>( button[i].x + stage->GetScrollX()) + MAP_CEllSIZE / 2, static_cast<int>(button[i].y + stage->GetScrollY()) + MAP_CEllSIZE / 2);
				DrawOvalAA(button[i].x + stage->GetScrollX(), button[i].y + stage->GetScrollY() + 30 + button[i].animTimer, 25, 10, 20, 0xbfcb4e, TRUE, 1.0f);
				SetDrawAreaFull();
			}

			DrawGraphF(button[i].x + stage->GetScrollX() - MAP_CEllSIZE / 2, button[i].y + stage->GetScrollY() - MAP_CEllSIZE / 2, blockImage1[61], TRUE);
		}
	}

	//動く床
	for (int i = 0; i < lift.size(); i++) {
		//DrawFormatString(100+i*100, 400, 0xffffff, "%d", lift[i].leftVectorY);
		//DrawFormatString(100 + i * 100, 400, 0xffffff, "%f", lift[i].x);
		/*DrawFormatString(100+i*100, 400, 0xffffff, "%f", lift_goal_X[i].x);
		DrawBox(lift_goal_X[i].x + stage->GetScrollX(), lift_goal_X[i].y + stage->GetScrollY(), lift_goal_X[i].x + MAP_CEllSIZE * 2 + stage->GetScrollX(), lift_goal_X[i].y + MAP_CEllSIZE / 2 + stage->GetScrollY(),0xff0000,FALSE);*/
		//DrawExtendGraphF(lift[i].x + stage->GetScrollX(), lift[i].y - 31 + stage->GetScrollY(), lift[i].x + LIFT_SIZE + stage->GetScrollX(), lift[i].y + 70 + stage->GetScrollY(), blockImage1[51], TRUE);
		DrawExtendGraphF(lift[i].x + stage->GetScrollX(), lift[i].y - 50 + stage->GetScrollY(), lift[i].x + LIFT_SIZE + stage->GetScrollX(), lift[i].y + 70 + stage->GetScrollY(), moveFloorImage[lift[i].image], TRUE);
	}

	//ドア
	for (int i = 0; i < door.size(); i++) {
		//if (button[i].type == 2 && button[i].flag == false)DrawOvalAA(button[i].x + scrollX, button[i].y + scrollY + 30, 25, 10, 20, 0xbfcb4e, TRUE, 1.0f);
		if (door[i].flag == true) {

		}
	}

	//マンホール
	for (int i = 0; i < manhole.size(); i++) {

		if (manhole[i].type == 1) {
			if (manhole[i].flag == true)
			{
				if (manhole[i].animTimer < 20)
				{
					DrawModiGraphF(manhole[i].x + stage->GetScrollX(),
					               manhole[i].y + stage->GetScrollY() - manhole[i].animTimer * (288 / 20),
					               manhole[i].x + stage->GetScrollX() + MAP_CEllSIZE,
					               manhole[i].y + stage->GetScrollY() - manhole[i].animTimer * (288 / 20),
					               manhole[i].x + stage->GetScrollX() + MAP_CEllSIZE,
					               manhole[i].y + stage->GetScrollY() + MAP_CEllSIZE,
					               manhole[i].x + stage->GetScrollX(),
					               manhole[i].y + stage->GetScrollY() + MAP_CEllSIZE,
					               blockImage1[67], TRUE);
				}
				else
				{
					DrawGraphF(manhole[i].x + stage->GetScrollX(), manhole[i].y + stage->GetScrollY(), blockImage1[97],
					           TRUE);
				}
			}
			else
			{
				DrawGraphF(manhole[i].x + stage->GetScrollX(), manhole[i].y + stage->GetScrollY(), blockImage1[67],
				           TRUE);

				//マンホールのガイド表示
				DrawGuide(manhole[i].x + MAP_CEllSIZE / 2, manhole[i].y + MAP_CEllSIZE - 20, stage);
			}
		}

		if (manhole[i].type == 3) {
			if (manhole[i].flag == true) {
				DrawGraphF(manhole[i].x + stage->GetScrollX() - MAP_CEllSIZE / 2, manhole[i].y + stage->GetScrollY() - MAP_CEllSIZE / 2, blockImage1[97], TRUE);
			}
			else {
				DrawGraphF(manhole[i].x + stage->GetScrollX() - MAP_CEllSIZE / 2, manhole[i].y + stage->GetScrollY() - MAP_CEllSIZE / 2, blockImage1[67], TRUE);
			}
			//マンホールのガイド表示

			int x = static_cast<int>(manhole[i].leftInitX) * MAP_CEllSIZE + MAP_CEllSIZE / 2;
			int y = static_cast<int>(manhole[i].leftInitY) * MAP_CEllSIZE + MAP_CEllSIZE / 2;
			DrawGuide(x, y - 20, stage);
		}
	}


	//酸性雨の水たまり
	for (int i = 0; i < acidrainPuddles.size(); i++) {

		/*if (stageName == "Stage02") { DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[7], TRUE); }
		else if (stageName == "Stage03") { DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[11], TRUE); }
		else { DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[3], TRUE); }*/

		int imageNum = acidrainPuddles.at(i).image;
		if (imageNum == 99)
		{
			imageNum = 0;
		}

		switch (acidrainPuddles[i].type)
		{
		//case 1:		//左端
		//	if (acidrainPuddlesAniTimer > 5) {
		//		DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[74], TRUE);
		//	}
		//	else {
		//		DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[73], TRUE);
		//	}
		//	break;

		case 2:		//中央
			/*if (acidrainPuddlesAniTimer > 5) {
				DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[76], TRUE);
			}
			else {
				DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[75], TRUE);
			}*/

			DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), acidImage[imageNum], TRUE);
			DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), acidImage[foamAnimation], TRUE);
			DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY()+80.0f, acidImage[0], TRUE);
			//DrawBoxAA(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY() + 80.0f, acidrainPuddles[i].x + stage->GetScrollX()+80.0f, acidrainPuddles[i].y + stage->GetScrollY() + 160.0f, 0xc731e8, TRUE);
			//DrawGraphF(acidrain_puddles[i].x + stage->GetScrollX(), acidrain_puddles[i].y + stage->GetScrollY(), acid, TRUE);
			break;

		//case 3:		//右端
		//	if (acidrainPuddlesAniTimer > 5) {
		//		DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[78], TRUE);
		//	}
		//	else {
		//		DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[77], TRUE);
		//	}
		//	break;
		default:
			break;
		}
	}


	//地下エフェクト
	if (underGroundEffects != 0) {
		DrawCircleAA(player->GetPlayerX(), player->GetPlayerY(), 1200.0F - underGroundEffects * 3, 32, 0x000000, FALSE, 1200.0F);

	}

}

/// <summary>
/// 更新
/// </summary>
void ELEMENT::Update(PLAYER* player, STAGE* stage) {

	ChangeVolumeSoundMem(Option::GetSEVolume(), doorCloseSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), pressTheButtonSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), switchSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), walkPuddleSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), manholeOpenedSe);

	playerState = static_cast<int>(player->GetPlayerMoveState());
	//プレイヤーのマップ内座標を設定
	playerMapX = roundf(player->GetPlayerX() - stage->GetScrollX());
	playerMapY = floorf(player->GetPlayerY() - stage->GetScrollY());

	Button(player);
	Door(stage);
	Lift(player, stage);
	Manhole(player, stage);
	Acidrain_puddles(player);

	if (guidTimer < 100) { guidTimer++; }
	else { guidTimer = 0; }

	Hook_Distance(player, stage);	//フックのガイド表示用距離計算
}

void ELEMENT::DrawGuide(const float elementX, const float elementY, STAGE* stage, const int offsetX, const int offsetY) const
{
	float x1 = elementX + offsetX + stage->GetScrollX();
	float y1 = elementY + offsetY + stage->GetScrollY();
	const int currentInputMode = PAD_INPUT::GetInputMode();

	if (currentInputMode == static_cast<int>(PAD_INPUT::InputMode::XINPUT_GAMEPAD) ||
		currentInputMode == static_cast<int>(PAD_INPUT::InputMode::DIRECTINPUT_GAMEPAD))
	{
		int color = (guidTimer < 50) ? 0xFFFFFF : 0xFFCB33;
		DrawCircleAA(x1, y1, 15, 20, color, 1);

		std::string buttonText = Option::GetInputMode() ? "B" : "A";
		int textColor = Option::GetInputMode() ? B_COLOR : A_COLOR;
		DrawStringToHandle(static_cast<int>(x1) - 7, static_cast<int>(y1) - 12, buttonText.c_str(), textColor, guidFont, 0xFFFFFF);
	}
	else if (currentInputMode == static_cast<int>(PAD_INPUT::InputMode::KEYBOARD))
	{
		float width = 20;
		float height = 0;

		if (!Option::GetInputMode())
		{
			x1 += 6;
			width = 10;
			height = 10;
		}

		int color = (guidTimer < 50) ? 0xFFFFFF : 0xFFCB33;
		DrawBoxAA(x1 - 20, y1 - 20, x1 + width, y1 + height, color, 1);

		if (Option::GetInputMode())
		{
			DrawStringToHandle(static_cast<int>(x1) - 20, static_cast<int>(y1) - 15, "SPACE", B_COLOR, keyboardGuidFont, 0xFFFFFF);
		}
		else
		{
			DrawStringToHandle(static_cast<int>(x1) - 12, static_cast<int>(y1) - 17, "Z", A_COLOR, guidFont, 0xFFFFFF);
		}
	}
}

/// <summary>
/// 3種類のボタンの処理
/// </summary>
void ELEMENT::Button(PLAYER* player) {


	for (int i = 0; i < button.size(); i++) {
		if (button[i].flag == true)button[i].animTimer++;
		if (button[i].animTimer > 180 && keepPushing == false) {
			button[i].animTimer = 0;
			button[i].flag = false;
		}

		if (button[i].type == 1) {		//横ボタン
			int max_ball_num = player->GetThrowCnt();
			for (int ball_num = 0; ball_num < max_ball_num; ball_num++) {
				if ((player->GetThrowSlime(ball_num).GetThrowX() >= button[i].x - MAP_CEllSIZE / 2 + 33) && (player->GetThrowSlime(ball_num).GetThrowX() <= button[i].x + MAP_CEllSIZE / 2 - 30) && (player->GetThrowSlime(ball_num).GetThrowY() >= button[i].y - MAP_CEllSIZE / 2) && (player->GetThrowSlime(ball_num).GetThrowY() <= button[i].y + MAP_CEllSIZE / 2)) {
					//デバッグ
					//printfDx("1番に入ってるよ！");
					if (CheckSoundMem(pressTheButtonSe) == FALSE)PlaySoundMem(pressTheButtonSe, DX_PLAYTYPE_BACK, TRUE);
					/*if (i < door.size() && i >= 0) {
						int j = i + 1;
						door[i].flag = true;
					}*/
					int k = 0;
					while (k < door.size()) {
						if (button[i].pairNum == door[k].pairNum) {
							door[k].flag = true;
							break;
						}
						k++;
					}

				}
			}
		}
		if (button[i].type == 2) {	//ボタン
			bool ball_flg = false;	//ボールの当たり判定フラグ
			for (int ball_num = 0; ball_num < player->GetThrowCnt(); ball_num++) {
				if ((player->GetThrowSlime(ball_num).GetThrowX() >= button[i].x - MAP_CEllSIZE / 2 + 10) && (player->GetThrowSlime(ball_num).GetThrowX() <= button[i].x + MAP_CEllSIZE / 2 - 10) && (player->GetThrowSlime(ball_num).GetThrowY() >= button[i].y - MAP_CEllSIZE / 2) && (player->GetThrowSlime(ball_num).GetThrowY() <= button[i].y + MAP_CEllSIZE / 2)) {
					ball_flg = true;
				}
				else { keepPushing = false; }
			}

			if ((ball_flg == true) || ((playerMapX >= button[i].x - MAP_CEllSIZE + 25) && (playerMapX <= button[i].x + MAP_CEllSIZE - 25) && (playerMapY >= button[i].y - MAP_CEllSIZE / 2) && (playerMapY <= button[i].y + MAP_CEllSIZE / 2))) {
				//デバッグ
				//printfDx("2番に入ってるよ！");
				// 当たり判定バグる為一時停止
				//if(ball_flg == false)player->SetPlayerY(button[i].y - 6.0f);
				if (button[i].flag == false) {
					if (CheckSoundMem(switchSe) == FALSE)PlaySoundMem(switchSe, DX_PLAYTYPE_BACK, TRUE);
				}
				keepPushing = true; //押し続けている
				button[i].flag = true;		//ボタンを押した

				/*if (i < door.size() && i >= 0) {
					int j = i - 1;
					door[i].flag = true;
				}*/
				int l = 0;
				while (l < door.size()) {
					if (button[i].pairNum == door[l].pairNum) {
						door[l].flag = true;
						break;
					}
					l++;
				}
			}
			else { keepPushing = false; }
		}

		//一回限り
		if (button[i].type == 3) {	//Onecボタン
			if ((playerMapX >= button[i].x - MAP_CEllSIZE / 2) && (playerMapX <= button[i].x + MAP_CEllSIZE / 2) && (playerMapY >= button[i].y - MAP_CEllSIZE / 2) && (playerMapY <= button[i].y + MAP_CEllSIZE / 2)) {
				//デバッグ
				//printfDx("3番に入ってるよ！");
			}
		}
	}
}

/// <summary>
/// ドアの処理
/// </summary>
void ELEMENT::Door(STAGE* stage) {
	for (int i = 0; i < door.size(); i++) {
		if (door[i].flag == true) {
			int x = floor(door[i].x / MAP_CEllSIZE);
			int y = floor(door[i].y / MAP_CEllSIZE);
			stage->SetMapData(y, x, 64);
			stage->SetMapData(y - 1, x, 65);
		}

		if (door[i].flag==true&&(playerMapX >= door[i].x + MAP_CEllSIZE) && (playerMapX <= door[i].x + MAP_CEllSIZE + 10) && (playerMapY >= door[i].y - MAP_CEllSIZE *1.5) && (playerMapY <= door[i].y + MAP_CEllSIZE / 2)) {
			int x = floor(door[i].x / MAP_CEllSIZE);
			int y = floor(door[i].y / MAP_CEllSIZE);
			stage->SetMapData(y, x, 66);
			stage->SetMapData(y - 1, x, 67);
			if (CheckSoundMem(doorCloseSe) == FALSE)PlaySoundMem(doorCloseSe, DX_PLAYTYPE_BACK, TRUE);
			door[i].flag = false;
		}
	}
}

/// <summary>
/// 動く床の処理
/// </summary>
void ELEMENT::Lift(PLAYER* player, STAGE* stage) {

	//プレイヤー座標取得
	float pX = player->GetPlayerX() - stage->GetScrollX();
	float pY = player->GetPlayerY() - stage->GetScrollY();

	for (int i = 0; i < lift.size(); i++) {

		if (lift[i].flag == false)
		{
			if (lift[i].x - MAP_CEllSIZE / 2 <= pX &&
				lift[i].y - MAP_CEllSIZE / 2 <= pY && pY <= lift[i].y + MAP_CEllSIZE)
			{
				lift[i].flag = true;
			}
		}
		else
		{
			//移動処理

			//動く床(縦)の動き
			if (lift[i].type == 1) {
				if (lift[i].y < lift[i].leftGoalY) { lift[i].leftVectorY = 1; }
				else if (lift[i].y > lift[i].leftGoalY) { lift[i].leftVectorY = -1; }
				else { lift[i].leftVectorY = 0; }
				if (lift[i].y != lift[i].leftGoalY) 
				{
					lift[i].y += lift[i].leftVectorY * 4;
				}
				else 
				{
					lift[i].liftWaitTimer++;
					if (lift[i].liftWaitTimer == 90) {
						float work = lift[i].leftGoalY;
						lift[i].leftGoalY = lift[i].leftInitY;
						lift[i].leftInitY = work;
						lift[i].liftWaitTimer = 0;
					}
				}
			}
			//動く床(横)の動き
			else if (lift[i].type == 2) 
			{
				if (lift[i].x < lift[i].leftGoalX) { lift[i].leftVectorX = 1; }
				else if (lift[i].x > lift[i].leftGoalX) { lift[i].leftVectorX = -1; }
				else { lift[i].leftVectorX = 0; }

				if (lift[i].liftWaitTimer != 60)
				{
					lift[i].liftWaitTimer++;
				}
				else
				{
					if (lift[i].x != lift[i].leftGoalX)
					{
						lift[i].x += lift[i].leftVectorX * 4;
					}
					else
					{
						lift[i].liftWaitTimer = 0;
						float work = lift[i].leftGoalX;
						lift[i].leftGoalX = lift[i].leftInitX;
						lift[i].leftInitX = work;
					}
				}
			}
		}
		//アニメーション処理
		if (lift[i].animTimer < 8)
		{
			lift[i].animTimer++;
		}
		else
		{
			if (lift[i].image < 2)
			{
				lift[i].image++;
			}
			else
			{
				lift[i].image = 0;
			}

			lift[i].animTimer = 0;
		}
	}
}

/// <summary>
/// 動く床の当たり判定
/// </summary>
bool ELEMENT::HitLift(PLAYER* player) {
	for (int i = 0; i < lift.size(); i++) {
		if (playerMapX + player->GetPlayerScale() * 25 >= lift[i].x && playerMapX - player->GetPlayerScale() * 25 <= lift[i].x + LIFT_SIZE
			&& playerMapY >= lift[i].y - MAP_CEllSIZE && playerMapY <= lift[i].y + 10 && player->GetPlayerMoveState() != PLAYER_MOVE_STATE::JUMP
			/*&& (mapData[int(playerMapY) / MAP_CEllSIZE + 1][int(playerMapX) / MAP_CEllSIZE] == 0 || mapData[int(playerMapY) / MAP_CEllSIZE + 1][int(playerMapX) / MAP_CEllSIZE] >= 51)*/) {

			player->SetPlayerY(lift[i].y - MAP_CEllSIZE / 2);

			player->SetPlayerY(lift[i].y - MAP_CEllSIZE / 2 + lift[i].leftVectorY * 4);
			player->SetPlayerX(player->GetPlayerX() + lift[i].leftVectorX * 4);
			return true;
		}
	}
	return false;
}


/// <summary>
/// マンホールの処理
/// </summary>
void ELEMENT::Manhole(PLAYER* player, STAGE* stage) {
	for (int i = 0; i < manhole.size(); i++) {
		if (manhole[i].flag == true && manhole[i].animTimer < 20)manhole[i].animTimer++;
		if (manhole[i].animTimer > 20) {
			//manhole[i].animTimer = 0;
			//manhole[i].flag = false;
		}
		if (manhole[i].type == 1) {

			int mapdata_down = stage->GetMapData(manhole[i].leftInitY + 1, manhole[i].leftInitX);

			//中間地点を抜ける下の座標を取得する
			//中間地点の時にループ
			while (mapdata_down == 69 && manhole[i].pairNum == 0)
			{
				manhole[i].leftInitY++;
				mapdata_down = stage->GetMapData(manhole[i].leftInitY, manhole[i].leftInitY);
			}

			//演算完了フラグ
			manhole[i].pairNum = 1;

			if ((playerMapX >= manhole[i].x) && (playerMapX <= manhole[i].x + MAP_CEllSIZE) && (playerMapY <= manhole[i].y + MAP_CEllSIZE) && (playerMapY >= manhole[i].y)) {
				if (PAD_INPUT::OnPressed(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) {
					stage->SetTemporary_Hit(999);
					manhole[i].flag = true;
				}

				if (manhole[i].flag == false) {
					stage->SetTemporary_Hit(69);
				}

				if (manhole[i].animTimer == 19 && !CheckSoundMem(manholeOpenedSe))PlaySoundMem(manholeOpenedSe, DX_PLAYTYPE_BACK, TRUE);

				if (manhole[i].animTimer >= 20) {
					int x = floor(manhole[i].x / MAP_CEllSIZE);
					int y = floor(manhole[i].y / MAP_CEllSIZE);
					stage->SetMapData(y, x, 98);
				}
			}

			else if ((manhole[i].flag == true) && (playerMapX >= manhole[i].x - MAP_CEllSIZE * 4) && (playerMapX <= manhole[i].x + MAP_CEllSIZE * 5) && (playerMapY > manhole[i].y + MAP_CEllSIZE)) {
				if (underGroundEffects < 100) {
					underGroundEffects += 2;
				}

				const int speed = manhole[i].liftWaitTimer * 2;

				if ((manhole[i].flag == true) && player->GetPlayerY() + -stage->GetScrollY() < manhole[i].y) {
					player->SetGravity(false);
					stage->SetScrollY(stage->GetScrollY() - speed);
					player->SetPlayerY((player->GetPlayerY() - stage->GetScrollY()) + speed);
					//printfDx("遅くしています\n");
				}
			}

		}

		//中間地点
		if (manhole[i].type == 2) {
			if ((playerMapX >= manhole[i].x) && (playerMapX <= manhole[i].x + MAP_CEllSIZE) && (playerMapY <= manhole[i].y + MAP_CEllSIZE) && (playerMapY >= manhole[i].y)) {
				//プレイヤーの落下速度を遅くする
				//player->SetPlayerY(player->GetPlayerY() - 3.0f);
				//player->SetGravity(true);
				player->SetVisible(false);
			}
			else {
				//player->SetGravity(true);
			}


			int mapdata_down = stage->GetMapData(manhole[i].leftInitY, manhole[i].leftInitX);

			//中間地点を抜ける下の座標を取得する
			//中間地点の時にループ
			while (mapdata_down == 69 && manhole[i].pairNum == 0)
			{
				manhole[i].leftInitY++;
				mapdata_down = stage->GetMapData(manhole[i].leftInitY, manhole[i].leftInitY);
			}

			//演算完了フラグ
			manhole[i].pairNum = 1;

			int manhole_down_y;
			manhole_down_y = manhole[i].leftInitY * MAP_CEllSIZE + MAP_CEllSIZE;

			//中間地点の一番下に行った時
			if ((playerMapX >= manhole[i].x) && (playerMapX <= manhole[i].x + MAP_CEllSIZE) && (playerMapY <= manhole_down_y + MAP_CEllSIZE) && (playerMapY >= manhole_down_y)) {
				player->SetVisible(true);
				player->SetGravity(true);
				//背景を描画しない
				if (isInUnder == false)
				{
					isInUnder = true;
				}
			}
		}

		//出口
		if (manhole[i].type == 3) {

			int mapdata = stage->GetMapData(manhole[i].leftInitY + 1, manhole[i].leftInitX);

			//中間地点の時にループ
			while (mapdata == 69)
			{
				manhole[i].leftInitY++;
				mapdata = stage->GetMapData(manhole[i].leftInitY, manhole[i].leftInitX);

				//中間の数をカウント
				manhole[i].liftWaitTimer++;
			}

			//マンホールの出口の左端から右端までのx座標かつマンホールの出口よりも下にいる場合
			if ((playerMapX >= manhole[i].x - MAP_CEllSIZE / 2) && (playerMapX <= manhole[i].x + MAP_CEllSIZE / 2) && (playerMapY > manhole[i].y) && (playerMapY < manhole[i].leftInitY * MAP_CEllSIZE + MAP_CEllSIZE * 3)) {
				if (PAD_INPUT::OnPressed(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) { manhole[i].flag = true; }


				//Bボタンを押してflgがtrueになった時
				if (manhole[i].flag == true) {
					//player->SetGravity(false);
					//player->SetVisible(true);
					const int speed = manhole[i].liftWaitTimer * 5;

					if (player->GetPlayerY() + -stage->GetScrollY() > manhole[i].y) {

						stage->SetScrollY(stage->GetScrollY() + speed);
						player->SetPlayerY((player->GetPlayerY() - stage->GetScrollY()) - speed);
					}

					//背景を描画する
					if (isInUnder == true)
					{
						isInUnder = false;
					}

					if (underGroundEffects > 0) {
						underGroundEffects -= manhole[i].liftWaitTimer + 2;
					}
					//SetGravityを有効化するとGetterの値が変わらない
					//printfDx("%f\n", player->GetPlayerY());
					if ((player->GetPlayerY() + -stage->GetScrollY()) < manhole[i].y) {
						if (!CheckSoundMem(manholeOpenedSe)) { PlaySoundMem(manholeOpenedSe, DX_PLAYTYPE_BACK, TRUE); }
						//player->SetVisible(false);
						//一時的な当たり判定をつける。
						stage->SetTemporary_Hit(69);
						//player->SetGravity(true);
						player->SetVisible(true);
						manhole[i].flag = false;
						if (underGroundEffects != 0)
						{
							underGroundEffects = 0;
						}
					}
				}
			}
		}
	}
}

/// <summary>
/// 酸性雨の水たまりの処理
/// </summary>
void ELEMENT::Acidrain_puddles(PLAYER* player) {
	if (acidrainPuddlesAniTimer < 5) { acidrainPuddlesAniTimer++; }
	else
	{
		acidrainPuddlesAniTimer = 0;

		if (foamAnimation < 10)
		{
			foamAnimation++;
		}
		else
		{
			foamAnimation = 5;
		}
		
	}
	int acidSize = acidrainPuddles.size();
	for (int i = 0; i < acidSize; i++) {
		if (acidrainPuddles[i].image < 5)
		{
			if (acidrainPuddles[i].animTimer < 6)
			{
				acidrainPuddles[i].animTimer++;
			}
			else
			{
				acidrainPuddles.at(i).animTimer = 0;
				if (i < acidSize - 1)
				{
					if (acidrainPuddles.at(i + 1).image == 99)
					{
						acidrainPuddles.at(i + 1).image = 1;
					}
				}

				if (acidrainPuddles[i].image < 4)
				{
					acidrainPuddles[i].image++;

					/*else if (acidrain_puddles[i - 1].image < acidrain_puddles[i].image)
					{
						acidrain_puddles[i].image++;
					}*/
				}
				else
				{
					acidrainPuddles[i].image = 1;
				}

			}
		}

		/*if (acidrainPuddles[i].flag == false)acidrainPuddles[0].animTimer++;
		if (acidrainPuddles[0].animTimer > 120) {
			acidrainPuddles[0].animTimer = 0;
			acidrainPuddles[i].flag = true;
		}*/
		//酸性雨の水たまり
		if ((playerMapX >= acidrainPuddles[i].x) && (playerMapX <= acidrainPuddles[i].x + MAP_CEllSIZE) && (playerMapY >= acidrainPuddles[i].y - MAP_CEllSIZE / 2) && (playerMapY <= acidrainPuddles[i].y)) {
			if (CheckSoundMem(walkPuddleSe) == FALSE && acidrainPuddles[0].animTimer % 90 == 0)PlaySoundMem(walkPuddleSe, DX_PLAYTYPE_BACK, TRUE);
			acidrainPuddles[i].flag = true;				//強制的にtrueにしましたよ				
			if (acidrainPuddles[i].flag == true) {
				player->SetLife(player->GetLife() - 1);
				acidrainPuddles[i].flag = false;

			}
		}
	}
}

/// <summary>
/// フックのガイド表示用距離計算
/// </summary>
void ELEMENT::Hook_Distance(PLAYER* player, STAGE* stage) {
	float min_distance = HOOK_MAX_DISTANCE + 80;
	//フックのガイド表示用
	//フックまでの距離計算
	for (int i = 0; i < hook.size(); i++) {
		float diff_x = hook[i].x - (player->GetPlayerX() - stage->GetScrollX());
		float diff_y = hook[i].y - (player->GetPlayerY() - stage->GetScrollY());
		float distance = sqrtf(diff_x * diff_x + diff_y * diff_y);
		//距離が最短距離より近いとき
		if (distance <= min_distance) { min_distance = distance; hook[i].flag = true; }
		else { hook[i].flag = false; }
	}
}