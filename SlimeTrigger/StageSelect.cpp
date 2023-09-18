#include "StageSelect.h"
#include "Title.h"
#include "GameMain.h"
#include "DxLib.h"
#include "Option.h"

#define callout_backcolor 0xFFFFFF

//#define DEBUG_STAGE

STAGE_SELECT::STAGE_SELECT(short lastStageNum)
{
	//backGraundImage[0] = LoadGraph("Resource/Images/Stage/BackImpause_cash.bmp");
	backGraundImage[0] = LoadGraph("Resource/Images/Stage/BackImage1.png");
	if ((backGraundMusic = LoadSoundMem("Resource/Sounds/BGM/title.wav")) == -1) {
		throw "Resource/Sounds/BGM/title.wav";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}

	guidFont = CreateFontToHandle("メイリオ", 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	keyboardGuidFont = CreateFontToHandle("メイリオ", 9, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	moveToTitleFont = CreateFontToHandle("メイリオ", 18, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	stageNameFont = CreateFontToHandle("メイリオ", 31, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 4);
	stage = new STAGE("StageSelect");
	player = new PLAYER(stage);
	element = new ELEMENT();
	lemoner = nullptr;
	lemonerCount = 0;
	gurepon = nullptr;
	gureponCount = 0;
	tomaton = nullptr;
	tomatonCount = 0;

	std::vector<std::vector<int>> spawnPoint;

	//とまトン生成する数を数える
	for (int i = 0, point = 0; i < stage->GetMapSize().x; i++)
	{
		for (int j = 0; j < stage->GetMapSize().y; j++)
		{
			if (stage->GetMapData(i, j) == 93)
			{
				tomatonCount++;
				spawnPoint.push_back(std::vector<int>(2));
				spawnPoint[point][0] = i;
				spawnPoint[point][1] = j;
				point++;
			}
		}
	}
	//とまトンの生成
	if (tomatonCount > 0)
	{
		tomaton = new TOMATO * [tomatonCount];
		for (int i = 0; i < tomatonCount; i++)
		{
			tomaton[i] = new TOMATO(player, stage, spawnPoint[i][0], spawnPoint[i][1]);
		}
	}

	//スポーンポイントを削除
	spawnPoint.clear();

	//グレポンを生成する数を数える
	for (int i = 0, point = 0; i < stage->GetMapSize().x; i++)
	{
		for (int j = 0; j < stage->GetMapSize().y; j++)
		{
			if (stage->GetMapData(i, j) == 92)
			{
				gureponCount++;
				spawnPoint.push_back(std::vector<int>(2));
				spawnPoint[point][0] = i;
				spawnPoint[point][1] = j;
				point++;
			}
		}
	}

	//グレポンの生成
	if (gureponCount > 0)
	{
		gurepon = new GRAPEFRUIT * [gureponCount];
		for (int i = 0; i < gureponCount; i++)
		{
			gurepon[i] = new GRAPEFRUIT(player, stage, spawnPoint[i][0], spawnPoint[i][1]);
		}
	}

	//スポーンポイントを削除
	spawnPoint.clear();

	//レモナー生成する数を数える
	for (int i = 0, point = 0; i < stage->GetMapSize().x; i++)
	{
		for (int j = 0; j < stage->GetMapSize().y; j++)
		{
			if (stage->GetMapData(i, j) == 91)
			{
				lemonerCount++;
				spawnPoint.push_back(std::vector<int>(2));
				spawnPoint[point][0] = i;
				spawnPoint[point][1] = j;
				point++;
			}
		}
	}
	//レモナーの生成
	if (lemonerCount > 0)
	{
		lemoner = new LEMON * [lemonerCount];
		for (int i = 0; i < lemonerCount; i++)
		{
			lemoner[i] = new LEMON(player, stage, spawnPoint[i][0], spawnPoint[i][1]);
		}
	}

	playerMapX = 0;
	playerMapY = 0;

	guidTimer = 0;
	*effectTimer = 0;
	stageReturn = { 0,0 };
	*stageMove = { 0,0 };
	effectDelta = false;


	joysAniTimer = 0;
	joyStickDelta = false;

	for (int i = 0; i < stage->GetMapSize().x; i++)
	{
		for (int j = 0; j < stage->GetMapSize().y; j++)
		{
			switch (stage->GetMapData(i, j))
			{

			case 101:
				stageReturn.x = j * MAP_CEllSIZE;
				stageReturn.y = i * MAP_CEllSIZE;

			case 102:
			case 103:
				stageMove[1].x = j * MAP_CEllSIZE;
				stageMove[1].y = i * MAP_CEllSIZE;
				break;

			case 104:
			case 105:
				stageMove[2].x = j * MAP_CEllSIZE;
				stageMove[2].y = i * MAP_CEllSIZE;
				break;

			case 106:
			case 107:
				stageMove[3].x = j * MAP_CEllSIZE;
				stageMove[3].y = i * MAP_CEllSIZE;
				break;
			}
		}
	}

	//プレイヤーのスポーン地点をセット
	POINT spawnPointPlayer = stage->GetSpawnPoint();

	if (lastStageNum != 0)
	{
		spawnPointPlayer.x = stageMove[lastStageNum].y;
		spawnPointPlayer.y = stageMove[lastStageNum].x - MAP_CEllSIZE;
	}

	stage->SetScrollX(-(spawnPointPlayer.y - MAP_CEllSIZE * 4));
	stage->SetScrollY(-(spawnPointPlayer.x - MAP_CEllSIZE - 400.0f));
	player->SetPlayer_Screen(spawnPointPlayer);

	PlaySoundMem(backGraundMusic, DX_PLAYTYPE_LOOP);

	//BGM
	ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);
}

STAGE_SELECT::~STAGE_SELECT()
{
	DeleteFontToHandle(guidFont);
	DeleteFontToHandle(keyboardGuidFont);
	DeleteFontToHandle(buttonGuidFont);
	DeleteFontToHandle(moveToTitleFont);
	DeleteFontToHandle(stageNameFont);
	StopSoundMem(backGraundMusic);
	DeleteSoundMem(backGraundMusic);
	DeleteSoundMem(okSe);
	DeleteGraph(backGraundImage[0]);
	delete player;
	delete stage;
	delete element;

	for (int i = 0; i < lemonerCount; i++)
	{
		delete lemoner[i];
	}
	delete[] lemoner;

	//とまトンの削除
	for (int i = 0; i < tomatonCount; i++)
	{
		delete tomaton[i];
	}
	delete[] tomaton;
	//グレポンの削除
	for (int i = 0; i < gureponCount; i++) {
		delete gurepon[i];

	}
	delete[] gurepon;
}

AbstractScene* STAGE_SELECT::Update()
{
	//BACKボタンでタイトルへ戻る
	if (PAD_INPUT::OnButton(XINPUT_BUTTON_BACK)) {
		PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
		//ok_seが鳴り終わってから画面推移する。
		while (CheckSoundMem(okSe)) {}
		StartJoypadVibration(DX_INPUT_PAD1, OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
		return new Title();
	}

	player->Update(element, stage, nullptr, 0);
	stage->Update(player, element);
	element->Update(player, stage);

	//プレイヤーを死なせない。
	if (player->GetLife() < 2) { player->SetLife(2); }

	//レモナーUpdate
	for (int i = 0; i < lemonerCount; i++)
	{
		if (lemoner[i] != nullptr)
		{
			lemoner[i]->Update();
			if (lemoner[i]->GetDeleteFlag())
			{
				delete lemoner[i];
				lemoner[i] = nullptr;
			}
		}
	}

	//とまとんUpdate
	for (int i = 0; i < tomatonCount; i++)
	{
		tomaton[i]->Update();
	}

	//グレポンUpdate
	for (int i = 0; i < gureponCount; i++)
	{
		if (gurepon[i] != nullptr && gurepon[i]->GetDeleteFlg())
		{
			delete gurepon[i];
			gurepon[i] = nullptr;
		}
		else if (gurepon[i] != nullptr && !gurepon[i]->GetDeleteFlg())
		{
			gurepon[i]->Update();
		}
	}

	playerMapX = roundf(player->GetPlayerX() - stage->GetScrollX());
	playerMapY = floorf(player->GetPlayerY());

	//落ちたらリスタート
	if (player->IsDeath() == true) {
		return new STAGE_SELECT(0);
	}

	//戻る
	if ((playerMapX >= stageReturn.x - (MAP_CEllSIZE * 3) / 2) && (playerMapX <= stageReturn.x + (MAP_CEllSIZE * 3) / 2)) {
		if (PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) { StageIn(); return new Title(); }
	}

	//ステージ1
	if ((playerMapX >= stageMove[1].x - MAP_CEllSIZE / 2) && (playerMapX <= stageMove[1].x + (MAP_CEllSIZE * 3) / 2)) {
		if (PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) { StageIn(); return new GAMEMAIN(false, 0, "Stage01"); }
	}


	//ステージ2
	if ((playerMapX >= stageMove[2].x - MAP_CEllSIZE / 2) && (playerMapX <= stageMove[2].x + (MAP_CEllSIZE * 3) / 2)) {
		if (PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) { StageIn(); return new GAMEMAIN(false, 0, "Stage02"); }
	}


	//ステージ3
	if ((playerMapX >= stageMove[3].x - MAP_CEllSIZE / 2) && (playerMapX <= stageMove[3].x + (MAP_CEllSIZE * 3) / 2)) {
		if (PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) { StageIn(); return new GAMEMAIN(false, 0, "Stage03"); }
	}

#ifdef DEBUG_STAGE
	//旧ステージ1
	if ((playerMapX >= 11 * MAP_CEllSIZE - MAP_CEllSIZE / 2) && (playerMapX <= 11 * MAP_CEllSIZE + (MAP_CEllSIZE * 3) / 2)) {
		if (PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) { StageIn(); return new GAMEMAIN(false, 0, "Stage04"); }
	}


	//デバッグステージ
	if ((playerMapX >= 2 * MAP_CEllSIZE - MAP_CEllSIZE / 2) && (playerMapX <= 2 * MAP_CEllSIZE + (MAP_CEllSIZE * 3) / 2)) {
		if (PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) { StageIn(); return new GAMEMAIN(false, 0, "DebugStage"); }
	}
#endif // DEBUG_STAGE

	//ガイドの表示タイマー
	if (guidTimer < 100) { guidTimer++; }
	else { guidTimer = 0; }

	//ポータルのアニメーションタイマー
	if (effectTimer[1] >= 255) { effectDelta = true; }
	if (effectTimer[1] <= 0) { effectDelta = false; }

	if (effectDelta) { effectTimer[1]--; }
	else if (!effectDelta) { effectTimer[1]++; }


	//ジョイスティックのアニメーションタイマー
	if (joysAniTimer >= 15) { joyStickDelta = true; }
	if (joysAniTimer <= -15) { joyStickDelta = false; }

	if (effectTimer[1] % 2 == 0) {
		if (joyStickDelta) { joysAniTimer--; }
		else if (!joyStickDelta) { joysAniTimer++; }
	}


	return this;
}

void STAGE_SELECT::Draw() const
{
	//ステージの描画
	stage->Draw(element);
	element->Draw(stage, player);

	for (int i = 0; i < lemonerCount; i++)
	{
		if (lemoner[i] != nullptr)
		{
			lemoner[i]->Draw();

		}
	}

	//とまトンの描画
	for (int i = 0; i < tomatonCount; i++)
	{
		tomaton[i]->Draw();
	}
	//グレポンの描画
	for (int i = 0; i < gureponCount; i++)
	{
		if (gurepon[i] != nullptr && gurepon[i]->GetDeleteFlg() == false)
		{
			gurepon[i]->Draw();
		}
	}

	//ガイド表示

	const int guid_color = 0xFFFFFF;

	const int guid_x = 600;
	//BACKボタン：タイトルへ戻る

		const int start_x = guid_x - 600;
		const int start_y = 665;


		const int y = 10;
		DrawBoxAA(start_x, start_y + y, start_x + 70, start_y + 30 + y, 0xFFFFFF, TRUE, 1.0F);
		DrawCircleAA(start_x + 5, start_y + 14.6 + y, 15, 20, 0xFFFFFF, TRUE, 1.0F);	//左端
		DrawCircleAA(start_x + 65, start_y + 14.6 + y, 15, 20, 0xFFFFFF, TRUE, 1.0F);	//右端

		DrawStringToHandle(start_x, start_y - 18, "タイトルへ", guid_color, moveToTitleFont, 0x000000);

	// ガイド表示
	if (PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::XINPUT_GAMEPAD) || PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::DIRECTINPUT_GAMEPAD)) {

		DrawStringToHandle(start_x + 2, start_y + 3 + y, "BACK", BACK_COLOR, buttonGuidFont, 0xFFFFFF);


		std::vector<guideElement> gamepadGuides = {
					guideElement({"L"}, "移動", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,
						 0xFFFFFF, 0xFFFFFF),
						 //GuideElement({"X"},"説明",GUIDE_SHAPE_TYPE::DYNAMIC_BOX),
						 guideElement({Option::GetInputMode() ? "A" : "B"}, "ジャンプ", GUIDE_SHAPE_TYPE::FIXED_CIRCLE, buttonGuidFont, guid_color,
									  Option::GetInputMode() ? A_COLOR : B_COLOR, guid_color),
						 guideElement({Option::GetInputMode() ? "B" : "A"}, "アクション", GUIDE_SHAPE_TYPE::DYNAMIC_CIRCLE, buttonGuidFont, guid_color,
									  Option::GetInputMode() ? B_COLOR : A_COLOR, guid_color),
						 guideElement({"START"}, "[ゲーム中]ポーズ", GUIDE_SHAPE_TYPE::ROUNDED_BOX, buttonGuidFont, guid_color, START_COLOR,
									  guid_color, 0x000000,10.f,30.f,30.f, 0, 2.f)
		};
		DrawGuides(gamepadGuides, 280.0f, 668.0f, 5.0f, 60.0f);
	}
	else if (PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::KEYBOARD)) {

		DrawStringToHandle(start_x + 12, start_y + 3 + y, "ESC", BACK_COLOR, buttonGuidFont, 0xFFFFFF);

		std::vector<guideElement> keyboardGuides = {
			guideElement({"W", "A", "S", "D"}, "移動", GUIDE_SHAPE_TYPE::FIXED_BOX, buttonGuidFont, 0xFFFFFF,
			             buttonGuidFont),
			//GuideElement({"X"},"説明",GUIDE_SHAPE_TYPE::DYNAMIC_BOX),	
			guideElement({Option::GetInputMode() ? "Z" : "SPACE"}, "ジャンプ", GUIDE_SHAPE_TYPE::DYNAMIC_BOX, buttonGuidFont, guid_color,
			             Option::GetInputMode() ? A_COLOR : B_COLOR, guid_color),
			guideElement({Option::GetInputMode() ? "SPACE" : "Z"}, "アクション", GUIDE_SHAPE_TYPE::DYNAMIC_BOX, buttonGuidFont, guid_color,
			             Option::GetInputMode() ? B_COLOR : A_COLOR, guid_color),
			guideElement({"Q"}, "[ゲーム中]ポーズ", GUIDE_SHAPE_TYPE::FIXED_BOX, buttonGuidFont, guid_color, START_COLOR,
			             guid_color),
		};
		DrawGuides(keyboardGuides, 200.0f, 668.0f, 5.0f, 60.0f);
	}
	

	//戻る
	if ((playerMapX >= (stageReturn.x) - (MAP_CEllSIZE * 3) / 2) && (playerMapX <= stageReturn.x + (MAP_CEllSIZE * 3) / 2)) {
		DrawStageGuid("タイトルへ", stageReturn.x - MAP_CEllSIZE / 2, stageReturn.y - 30, stageNameFont, 0x1FCF6E, callout_backcolor, 0, -10, -1, "戻る", 55, 30);
	}

	//ステージ1 ポータル描画
	SetDrawBright(0, 255, 255 - effectTimer[1]);
	DrawGraph(stageMove[1].x + stage->GetScrollX(), stageMove[1].y + -MAP_CEllSIZE + stage->GetScrollY(), stage->GetMapImage(101), TRUE);
	DrawGraph(stageMove[1].x + stage->GetScrollX(), stageMove[1].y + stage->GetScrollY(), stage->GetMapImage(102), TRUE);
	SetDrawBright(255, 255, 255);

	//ステージ2 ポータル描画
	SetDrawBright(85 + (155 - 85) * effectTimer[1] / 255, 177 + (153 - 177) * effectTimer[1] / 255, 82 + (148 - 82) * effectTimer[1] / 255);
	DrawGraph(stageMove[2].x + stage->GetScrollX(), stageMove[2].y + -MAP_CEllSIZE + stage->GetScrollY(), stage->GetMapImage(103), TRUE);
	DrawGraph(stageMove[2].x + stage->GetScrollX(), stageMove[2].y + stage->GetScrollY(), stage->GetMapImage(104), TRUE);
	SetDrawBright(255, 255, 255);

	//ステージ3 ポータル描画
	//SetDrawBright(255 - effectTimer[1], 255, 255);
	DrawGraph(stageMove[3].x + stage->GetScrollX(), stageMove[3].y + -MAP_CEllSIZE + stage->GetScrollY(), stage->GetMapImage(105), TRUE);
	DrawGraph(stageMove[3].x + stage->GetScrollX(), stageMove[3].y + stage->GetScrollY(), stage->GetMapImage(106), TRUE);
	SetDrawBright(255, 255, 255);

	//ステージ1
	if ((playerMapX >= stageMove[1].x - MAP_CEllSIZE / 2) && (playerMapX <= stageMove[1].x + (MAP_CEllSIZE * 3) / 2)) {
		DrawStageGuid("１ステージ", stageMove[1].x, stageMove[1].y, stageNameFont, 0xFA5A47, -1, 0, 0, callout_backcolor);
	}

	//ステージ2
	if ((playerMapX >= stageMove[2].x - MAP_CEllSIZE / 2) && (playerMapX <= stageMove[2].x + (MAP_CEllSIZE * 3) / 2)) {
		DrawStageGuid("２ステージ", stageMove[2].x, stageMove[2].y, stageNameFont, 0xBF700F, -1, 0, 0, callout_backcolor);
	}

	//ステージ3
	if ((playerMapX >= stageMove[3].x - MAP_CEllSIZE / 2) && (playerMapX <= stageMove[3].x + (MAP_CEllSIZE * 3) / 2)) {
		DrawStageGuid("３ステージ", stageMove[3].x, stageMove[3].y, stageNameFont, 0x9511D9, -1, 0, 0, callout_backcolor);
	}
#ifdef DEBUG_STAGE
	//旧ステージ1
	if ((playerMapX >= 11 * MAP_CEllSIZE - MAP_CEllSIZE / 2) && (playerMapX <= 11 * MAP_CEllSIZE + (MAP_CEllSIZE * 3) / 2)) {
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
		//DrawOvalAA(11 * MAP_CEllSIZE + MAP_CEllSIZE / 2 + stage->GetScrollX(), stageMove[1].y - MAP_CEllSIZE + stage->GetScrollY(), 100, 80, 30, 0x000000, FALSE, 1.0F);
		//DrawOvalAA(11 * MAP_CEllSIZE + MAP_CEllSIZE / 2 + stage->GetScrollX(), stageMove[1].y - MAP_CEllSIZE + stage->GetScrollY(), 99, 79, 30, callout_backcolor, TRUE, 0.0F);
		////DrawString(stageMove[1].x + MAP_CEllSIZE / 2, stageMove[1].y + MAP_CEllSIZE, "STAGE 1" , 0x6AF6C5, 0x000000);
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//DrawExtendStringToHandle(11 * MAP_CEllSIZE + stage->GetScrollX() - 55, stageMove[1].y - MAP_CEllSIZE - 10 + stage->GetScrollY(), 0.4f, 0.4f, "４ステージ", 0x856AF6, guidFont, 0x000000);

		//DrawCircleAA(11 * MAP_CEllSIZE + stage->GetScrollX(), stageMove[1].y + stage->GetScrollY(), 15, 20, 0xFFFFFF, 1);
		//DrawStringToHandle(11 * MAP_CEllSIZE + stage->GetScrollX() - 7, stageMove[1].y + stage->GetScrollY() - 12, Option::GetInputMode() ? "B" : "A", Option::GetInputMode() ? B_COLOR : A_COLOR, buttonGuidFont, 0xFFFFFF);
		DrawStageGuid("４ステージ", 11 * MAP_CEllSIZE, stageMove[1].y, stageNameFont, 0x4F56F8, -1, 0, 0, callout_backcolor);
	}


	//デバッグステージ
	if ((playerMapX >= 2 * MAP_CEllSIZE - MAP_CEllSIZE / 2) && (playerMapX <= 2 * MAP_CEllSIZE + (MAP_CEllSIZE * 3) / 2)) {
		DrawStageGuid("５ステージ", 2 * MAP_CEllSIZE, stageMove[1].y, stageNameFont, 0xD90B8B, -1, 0, 0, callout_backcolor);
	}
#endif // DEBUG_STAGE

	//チュートリアル
	if (lemoner[0] != nullptr) {
		const int x = 18;
		const int x_map = x * MAP_CEllSIZE + +stage->GetScrollX();
		const int y_map = stageMove[1].y - MAP_CEllSIZE + stage->GetScrollY();
		if ((playerMapX >= x * MAP_CEllSIZE - MAP_CEllSIZE / 2) && (playerMapX <= x * MAP_CEllSIZE + (MAP_CEllSIZE * 3) / 2)) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			DrawTriangleAA(x_map + 100, y_map - 62, x_map + 140, y_map, x_map + 300, y_map - 200, callout_backcolor, TRUE, 2.0F);
			DrawOvalAA(x * MAP_CEllSIZE + MAP_CEllSIZE / 2 + stage->GetScrollX(), stageMove[1].y - MAP_CEllSIZE + stage->GetScrollY(), 100, 80, 30, 0x000000, FALSE, 1.0F);
			DrawOvalAA(x * MAP_CEllSIZE + MAP_CEllSIZE / 2 + stage->GetScrollX(), stageMove[1].y - MAP_CEllSIZE + stage->GetScrollY(), 99, 79, 30, callout_backcolor, TRUE, 0.0F);
			//DrawString(stageMove[1].x + MAP_CEllSIZE / 2, stageMove[1].y + MAP_CEllSIZE, "STAGE 1" , 0x6AF6C5, 0x000000);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			//DrawCircleAA(x * MAP_CEllSIZE + stage->GetScrollX(), stageMove[1].y + stage->GetScrollY(), 15, 20, 0xFFFFFF, 1);
			//DrawStringToHandle(x * MAP_CEllSIZE + stage->GetScrollX() - 7, stageMove[1].y + stage->GetScrollY() - 12, Option::GetInputMode() ? "B" : "A", Option::GetInputMode() ? B_COLOR : A_COLOR, buttonGuidFont, 0xFFFFFF);

			const int joystick_x = x * MAP_CEllSIZE + +stage->GetScrollX() - 10;
			const int joystick_y = stageMove[1].y - MAP_CEllSIZE + stage->GetScrollY() - 10;
			const int joystick_size = 15;


			DrawOvalAA(joystick_x + joysAniTimer * 0.8, joystick_y + 6 + abs(joysAniTimer * 0.6), joystick_size + 8, 20, joystick_size, 0x000000, TRUE);
			//DrawBox(joystick_x - 5, joystick_y, joystick_x + 7, joystick_y + 23, 0x000000, 1);
			DrawQuadrangle(joystick_x - 5 + joysAniTimer, joystick_y + abs(joysAniTimer * 0.5), joystick_x + 7 + joysAniTimer, joystick_y + abs(joysAniTimer * 0.5), joystick_x + 7, joystick_y + 23, joystick_x - 5, joystick_y + 23, 0x000000, TRUE);
			DrawOval(joystick_x, joystick_y + joystick_size + 13, joystick_size + 12, 8, 0x000000, 1);
			DrawStringToHandle(joystick_x - 8 + joysAniTimer, joystick_y + -8 + abs(joysAniTimer * 0.5), "R", 0xFFFFFF, buttonGuidFont);

			DrawStringToHandle(joystick_x + 30, joystick_y, " + ", 0xFFFFFF, buttonGuidFont, 0x000000);
			//RBボタン
			DrawBoxAA(joystick_x + 70, joystick_y - 10, joystick_x + 130, joystick_y + 30, 0x000000, TRUE, 1.0F);
			DrawStringToHandle(joystick_x + 83, joystick_y + 0, "RB", 0xFFFFFF, buttonGuidFont);
		}
	}



	//プレイヤーの描画
	player->Draw(stage);
}

void STAGE_SELECT::StageIn(void)
{
	PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
	//ok_seが鳴り終わってから画面推移する。
	while (CheckSoundMem(okSe)) {}
	StartJoypadVibration(DX_INPUT_PAD1, OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);


}


void STAGE_SELECT::DrawStageGuid(const char* stageName, const float x, const float y, const int stageNameFont, const int text_color, const int textback_color,
	const int text_margin_x, const int text_margin_y, const int backcolor, const char* second_title, const int secont_margin_x, const int secont_margin_y) const {

	if (textback_color == -1) { const int textback_color = 0xFFFFFF; }
	if (backcolor == -1) { const int backcolor = 0xFFFFFF; }

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
	DrawOvalAA(x + MAP_CEllSIZE / 2 + stage->GetScrollX(), y - MAP_CEllSIZE + stage->GetScrollY(), 100, 80, 30, 0x000000, FALSE, 1.0F);
	DrawOvalAA(x + MAP_CEllSIZE / 2 + stage->GetScrollX(), y - MAP_CEllSIZE + stage->GetScrollY(), 99, 79, 30, backcolor, TRUE, 0.0F);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawStringToHandle(x + stage->GetScrollX() - 55 + text_margin_x, y - MAP_CEllSIZE - 10 + stage->GetScrollY() + text_margin_y, stageName, text_color, stageNameFont, textback_color);
	if (second_title != "") { DrawStringToHandle(x + stage->GetScrollX() - 55 + secont_margin_x, y - MAP_CEllSIZE - 10 + stage->GetScrollY() + secont_margin_y, second_title, text_color, stageNameFont, textback_color); }

	// DrawCircleAA(x + stage->GetScrollX(), y + stage->GetScrollY(), 15, 20, guidTimer < 50 ? 0xFFFFFF : 0xFFCB33, 1);
	// DrawStringToHandle(x + stage->GetScrollX() - 7, y + stage->GetScrollY() - 12, Option::GetInputMode() ? "B" : "A", Option::GetInputMode() ? B_COLOR : A_COLOR, buttonGuidFont, 0xFFFFFF);
	DrawGuide(x, y, stage);

}

void STAGE_SELECT::DrawGuide(float baseX, float baseY, STAGE* stage) const
{
	float x1 = baseX + stage->GetScrollX();
	float y1 = baseY + stage->GetScrollY();
	const int currentInputMode = PAD_INPUT::GetInputMode();

	if (currentInputMode == static_cast<int>(PAD_INPUT::InputMode::XINPUT_GAMEPAD) ||
		currentInputMode == static_cast<int>(PAD_INPUT::InputMode::DIRECTINPUT_GAMEPAD))
	{
		int color = (guidTimer < 50) ? 0xFFFFFF : 0xFFCB33;
		DrawCircleAA(x1, y1, 15, 20, color, 1);

		std::string buttonText = Option::GetInputMode() ? "B" : "A";
		int textColor = Option::GetInputMode() ? B_COLOR : A_COLOR;
		DrawStringToHandle(static_cast<int>(x1) - 7, static_cast<int>(y1) - 12, buttonText.c_str(), textColor, buttonGuidFont, 0xFFFFFF);
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
			DrawStringToHandle(static_cast<int>(x1) - 12, static_cast<int>(y1) - 17, "Z", A_COLOR, buttonGuidFont, 0xFFFFFF);
		}
	}
}
