#include "GameMain.h"
#include "GameMainRestart.h"
#include "Title.h"
#include <vector>
#include "Option.h"
#include "StageSelect.h"

//#define DEBUG_PV

GAMEMAIN::GAMEMAIN(bool reStart, int halfwayTime, const char* stageName)
{
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_4X4);
	std::vector<std::vector<int>> spawnPoint;
	hpImage = LoadGraph("Resource/Images/Player/hp.png");

	if (stageName == "Stage01") {

		if ((backGraundImage[0] = LoadGraph("Resource/Images/Stage/BackImage1.png")) == -1) {
			throw "Resource/Images/Stage/BackImage1.png";
		}

		if ((backGraundMusic[0] = LoadSoundMem("Resource/Sounds/BGM/stage1.wav")) == -1) {
			throw "Resource/Sounds/BGM/stage1.wav";
		}
	}
	else if (stageName == "Stage02") {

		if ((backGraundImage[1] = LoadGraph("Resource/Images/Stage/BackImage2.png")) == -1) {
			throw "Resource/Images/Stage/BackImage2.png";
		}

		if ((backGraundMusic[1] = LoadSoundMem("Resource/Sounds/BGM/stage2.wav")) == -1) {
			throw "Resource/Sounds/BGM/stage2.wav";
		}
	}
	else if (stageName == "Stage03") {

		if ((backGraundImage[2] = LoadGraph("Resource/Images/Stage/BackImage3.png")) == -1) {
			throw "Resource/Images/Stage/BackImage2.png";
		}

		if ((backGraundMusic[2] = LoadSoundMem("Resource/Sounds/BGM/stage3.wav")) == -1) {
			throw "Resource/Sounds/BGM/stage3.wav";
		}
	}
	else {
		if ((backGraundImage[0] = LoadGraph("Resource/Images/Stage/BackImage1.png")) == -1) {
			throw "Resource/Images/Stage/BackImage1.png";
		}
	}

	if ((cursorMoveSe = LoadSoundMem("Resource/Sounds/SE/cursor_move.wav")) == -1) {
		throw "Resource/Sounds/SE/cursor_move.wav";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}

	if ((countSe = LoadSoundMem("Resource/Sounds/SE/321.wav")) == -1) {
		throw "Resource/Sounds/SE/321.wav";
	}

	if ((startSe = LoadSoundMem("Resource/Sounds/SE/start.wav")) == -1) {
		throw "Resource/Sounds/SE/start.wav";
	}

	int clear_se_random = GetRand(4);
	char dis_clear_se[30];
	sprintf_s(dis_clear_se, sizeof(dis_clear_se), "Resource/Sounds/SE/clear%d.wav", clear_se_random + 1);

	if ((clearSe = LoadSoundMem(dis_clear_se)) == -1) {
		throw dis_clear_se;
	}

	nowGraph = 0;

	startTimeFont = CreateFontToHandle("メイリオ", 160, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	timeFont = LoadFontDataToHandle("Resource/Fonts/TimeAttack_HUD.dft", 2);
	this->stageName = stageName;
	elapsedTime = halfwayTime;
	startAddTime = 0;
	lemonerCount = 0;
	tomatonCount = 0;
	itemCount = 0;
	itemNum = 0;
	itemRand = 0;

	startTime = 180;
	startEffectTimer = 120;
	clearInterval = 480;

	this->reStart = reStart;


	stage = new STAGE(stageName, this->reStart);
	player = new PLAYER(stage);
	pause = new PAUSE;
	lemoner = nullptr;
	gurepon = nullptr;
	tomaton = nullptr;


	//とまトン生成する数を数える
	for (int i = 0, point = 0; i < stage->GetMapSize().x; i++)
	{
		for (int j = 0; j < stage->GetMapSize().y; j++)
		{
			if (stage->GetMapData(i, j) == 93)
			{
				tomatonCount++;
				itemCount++;
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

	//アイテムのカウント＆生成
	itemCount = gureponCount + lemonerCount;
	if (itemCount > 0)
	{
		item = new ITEMBALL * [itemCount];
		for (int i = 0; i < itemCount; i++) {
			item[i] = nullptr;
		}
	}
	element = new ELEMENT(stageName);



	if (reStart == true) {
		int scrollx = -(stage->GetHalfwayPoint().x - 500);
		stage->SetScrollX(scrollx);	//スポーン地点をセット
		stage->SetScrollY(-(stage->GetHalfwayPoint().y - MAP_CEllSIZE - 400));
		POINT buf = { stage->GetHalfwayPoint().y, stage->GetHalfwayPoint().x };
		player->SetPlayer_Screen(buf);

	}
	else {
		//スポーン地点をセット
		stage->SetScrollX(-(stage->GetSpawnPoint().y - MAP_CEllSIZE));
		stage->SetScrollY(-(stage->GetSpawnPoint().x - MAP_CEllSIZE - 400/* - ((stage->GetMapSize().x - 14) * MAP_CEllSIZE))*/));
		//stage->SetScrollY(-((stage->GetMapSize().x - 14) * MAP_CEllSIZE));
		player->SetPlayer_Screen(stage->GetSpawnPoint());
	}

	//BGM
	if (stageName == "Stage01") {
		PlaySoundMem(backGraundMusic[0], DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic[0]);
	}
	else if (stageName == "Stage02") {
		PlaySoundMem(backGraundMusic[1], DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(Option::GetBGMVolume() * 0.9, backGraundMusic[1]);
	}
	else if (stageName == "Stage03") {
		PlaySoundMem(backGraundMusic[2], DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic[2]);
	}


	//SE
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.6, cursorMoveSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.5, okSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.5, countSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.5, startSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.5, clearSe);

	PlaySoundMem(countSe, DX_PLAYTYPE_BACK, TRUE);


	//PV制作用（完成次第即座に消去）
	inputMargin = 0;
	scrollSpeed = 4;
	playerVisible = true;

}

GAMEMAIN::~GAMEMAIN()
{
	DeleteGraph(hpImage);

	if (stageName == "Stage01") {
		StopSoundMem(backGraundMusic[0]);
		DeleteSoundMem(backGraundMusic[0]);
		DeleteGraph(backGraundImage[0]);
	}
	else if (stageName == "Stage02") {
		StopSoundMem(backGraundMusic[1]);
		DeleteSoundMem(backGraundMusic[1]);
		DeleteGraph(backGraundImage[1]);
	}
	else if (stageName == "Stage03") {
		StopSoundMem(backGraundMusic[2]);
		DeleteSoundMem(backGraundMusic[2]);
		DeleteGraph(backGraundImage[2]);
	}
	else {
		DeleteGraph(backGraundImage[0]);
	}

	DeleteFontToHandle(startTimeFont);
	DeleteFontToHandle(timeFont);

	DeleteSoundMem(cursorMoveSe);
	DeleteSoundMem(okSe);
	DeleteSoundMem(countSe);
	DeleteSoundMem(startSe);
	DeleteSoundMem(clearSe);

	delete player;
	delete stage;
	delete pause;
	delete element;

	//レモナーの削除
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

	for (int i = 0; i < itemCount; i++)
	{
		if (item[i] != nullptr)
		{
			delete item[i];
		}
	}
	delete[] item;
}

AbstractScene* GAMEMAIN::Update()
{

	if (startTime > 0) {

		if (reStart == true) { SetDrawBright(255 - startTime, 255 - startTime, 255 - startTime); }
		//カウント音再生
		//if (startTime % 60 == 0) {
			//PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
		//}

		startTime--;

		char dis_start_time[2];	//文字列合成バッファー

		sprintf_s(dis_start_time, sizeof(dis_start_time), "%d", startTime / 60);

		//プレイヤーのX座標の動きを止める
		player->Update(element, stage, tomaton, tomatonCount, true);

		//START音再生
		if (startTime == 0) {
			if (reStart == false) { startAddTime = GetNowCount(); }
			else { startAddTime = GetNowCount() - elapsedTime; }
			PlaySoundMem(startSe, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	else if (startEffectTimer > 0) {

		startEffectTimer--;
	}


	if (startTime <= 0) {

		pause->Update();

		if (pause->IsPause() == false) {

			//経過時間の加算
			if (!stage->GetClearFlg()) { elapsedTime = GetNowCount() - startAddTime; }

			player->Update(element, stage, tomaton, tomatonCount);
			stage->Update(player, element);	//ステージクリア用
			element->Update(player, stage);
			for (int i = 0; i < lemonerCount; i++)
			{
				if (lemoner[i] != nullptr)
				{
					lemoner[i]->Update();
					if (lemoner[i]->GetDeleteFlag())
					{
						itemRand = GetRand(5);
						//アイテムを生成
						if (itemRand == 0)
						{
							item[itemNum++] = new ITEMBALL(lemoner[i]->GetX(), lemoner[i]->GetY(), lemoner[i]->GetMapX(), lemoner[i]->GetMapY(), player, stage, stage->GetScrollX(), stage->GetScrollY());
						}
						delete lemoner[i];
						lemoner[i] = nullptr;
					}
				}
			}
			for (int i = 0; i < tomatonCount; i++)
			{
				tomaton[i]->Update();
			}
			for (int i = 0; i < gureponCount; i++)
			{
				if (gurepon[i] != nullptr && gurepon[i]->GetDeleteFlg())
				{
					itemRand = GetRand(5);
					//アイテムを生成
					if (itemRand == 0)
					{
						item[itemNum++] = new ITEMBALL(gurepon[i]->GetX(), gurepon[i]->GetY(), gurepon[i]->GetSpawnMapX(), gurepon[i]->GetSpawnMapY(), player, stage, stage->GetScrollX(), stage->GetScrollY());
					}
					delete gurepon[i];
					gurepon[i] = nullptr;
				}
				else if (gurepon[i] != nullptr && !gurepon[i]->GetDeleteFlg())
				{
					gurepon[i]->Update();
				}
				else
				{
				}
			}
			//アイテムのアップデート
			for (int i = 0; i < itemCount; i++)
			{
				if (item[i] != nullptr)
				{
					if (item[i]->GetDeleteFlag())
					{
						delete item[i];
						item[i] = nullptr;
					}
					else
					{
						if ((item[i]->GetItemX() + stage->GetScrollX()) > 0 && (item[i]->GetItemX() + stage->GetScrollX()) < 1280)
						{
							item[i]->Update();
						}
					}
				}



				//ゲームオーバー
				if (player->IsDeath()) {
					if (!reStart && stage->GetHalfwayPointFlg()) {
						nowGraph = MakeGraph(1280, 720);
						GetDrawScreenGraph(0, 0, 1280, 720, nowGraph);
						return new GAMEMAIN_RESTART(true, elapsedTime, stageName, nowGraph);
					}
					return new GameOver(stageName);
				}

				//ステージクリア
				if (stage->GetClearFlg())
				{
					if (clearInterval == 480) { PlaySoundMem(clearSe, DX_PLAYTYPE_BACK, TRUE); }
					if (clearInterval > 0) { clearInterval--; }
					player->SetPlayerX(player->GetOldPlayerX());
					if (clearInterval <= 0 && !CheckSoundMem(clearSe)) {
						return new RESULT(true, elapsedTime, stageName);
					}
				}
			}
		}
		else {	//ポーズ画面のセレクター

			if (static_cast<PAUSE::MENU>(pause->GetSelectMenu()) == PAUSE::MENU::STAGE_SELECT) { return new STAGE_SELECT(); }
			else if (static_cast<PAUSE::MENU>(pause->GetSelectMenu()) == PAUSE::MENU::RESTART) {
				nowGraph = MakeGraph(1280, 720);
				GetDrawScreenGraph(0, 0, 1280, 720, nowGraph);
				return new GAMEMAIN_RESTART(false, 0, stageName, nowGraph);
			}
			else if (static_cast<PAUSE::MENU>(pause->GetSelectMenu()) == PAUSE::MENU::OPTION) {
				//BGM
				if (stageName == "Stage01") {
					ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic[0]);
				}
				else if (stageName == "Stage02") {
					ChangeVolumeSoundMem(Option::GetBGMVolume() * 0.9, backGraundMusic[1]);
				}
				else if (stageName == "Stage03") {
					ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic[2]);
				}

				//SE
				ChangeVolumeSoundMem(Option::GetSEVolume() * 1.6, cursorMoveSe);
				ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);
				ChangeVolumeSoundMem(Option::GetSEVolume() * 1.5, countSe);
				ChangeVolumeSoundMem(Option::GetSEVolume() * 1.5, startSe);
				ChangeVolumeSoundMem(Option::GetSEVolume() * 1.5, clearSe);
			}
		}

		//デバッグ
		//if (CheckHitKey(KEY_INPUT_F)) {
		//	int scrollx = -(7800 - 500);
		//	stage->SetScrollX(scrollx);	//スポーン地点をセット
		//	player->SetPlayerX(500); //プレイヤーの画面内座標をセット
		//}
	}

	if (inputMargin < 10) {
		inputMargin++;
	}
	else {

#ifdef DEBUG_PV
		//PV制作用（完成次第即座に消去）

			//プレイヤー表示・非表示
			if (CheckHitKey(KEY_INPUT_N)) {
				playerVisible = !playerVisible;

				if (playerVisible) {
					player->SetVisible(true);
				}
				else {
					player->SetVisible(false);
				}
			}

		//スクロールスピードダウン
			else if (CheckHitKey(KEY_INPUT_M)) {
				if (scrollSpeed > 0) { scrollSpeed--; }
			}

		//スクロールスピードアップ
			else if (CheckHitKey(KEY_INPUT_L)) {
				scrollSpeed++;
			}
		inputMargin = 0;
	}

	//スクロール移動
	if (CheckHitKey(KEY_INPUT_UP)) {
		stage->SetScrollY(stage->GetScrollY() + scrollSpeed);
	}
	else if (CheckHitKey(KEY_INPUT_DOWN)) {
		stage->SetScrollY(stage->GetScrollY() - scrollSpeed);
	}
	else if (CheckHitKey(KEY_INPUT_LEFT)) {
		stage->SetScrollX(stage->GetScrollX() + scrollSpeed * 3);
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT)) {
		if (stage->GetScrollX() > 0 || stage->GetScrollX() <= -(80 * static_cast<int>(stage->GetMapSize().x - 1280))) {
			stage->SetScrollX(stage->GetScrollX() - scrollSpeed * 3);
		}
#endif // DEBUG_PV
	}


	return this;
	}

void GAMEMAIN::Draw() const
{

	//ステージ背景
	if (stageName == "Stage01") {
		DrawGraph(static_cast<int>(stage->GetScrollX()) % 2560 + 2560, /*scrollY*/0, backGraundImage[0], FALSE);
		DrawGraph(static_cast<int>(stage->GetScrollX()) % 2560, /*scrollY*/0, backGraundImage[0], FALSE);
	}
	else if (stageName == "Stage02") {
		DrawGraph(static_cast<int>(stage->GetScrollX()) % 2560 + 2560, /*scrollY*/0, backGraundImage[1], FALSE);
		DrawGraph(static_cast<int>(stage->GetScrollX()) % 2560, /*scrollY*/0, backGraundImage[1], FALSE);
	}
	else if (stageName == "Stage03") {
		DrawGraph(static_cast<int>(stage->GetScrollX()) % 2560 + 2560, /*scrollY*/0, backGraundImage[2], FALSE);
		DrawGraph(static_cast<int>(stage->GetScrollX()) % 2560, /*scrollY*/0, backGraundImage[2], FALSE);
	}
	else {
		DrawGraph(static_cast<int>(stage->GetScrollX()) % 2560 + 2560, /*scrollY*/0, backGraundImage[0], FALSE);
		DrawGraph(static_cast<int>(stage->GetScrollX()) % 2560, /*scrollY*/0, backGraundImage[0], FALSE);
	}


	//地下背景描画
	if (stageName == "Stage01") {
		//ステージ１
		DrawBoxAA(stage->GetScrollX() + 6880, stage->GetScrollY() + 1380, stage->GetScrollX() + 10640, stage->GetScrollY() + 1900, 0x20251F, TRUE);
	}
	else if (stageName == "Stage03") {
		//ステージ3
		DrawBoxAA(0, stage->GetScrollY() + 1380, stage->GetScrollX() + 5120, stage->GetScrollY() + 4800, 0x20251F, TRUE);
	}

	/*if (stageName == "Stage03" && stage->GetScrollY() < -960) {
		DrawBox(0, 0, 25600, 1280, 0x20251F, TRUE);
	}*/

	for (int i = 0; i < player->GetLife(); i++) {
		DrawRotaGraph(30 + 50 * i, 20, 1, 0, hpImage, TRUE);
	}

	//ステージの描画
	stage->Draw(element);
	element->Draw(stage, player);



	//プレイヤーの描画
	player->Draw(stage);

	//レモナーの描画
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
	//アイテムの描画
	for (int i = 0; i < itemCount; i++)
	{
		if (item[i] != nullptr)
		{
			if ((item[i]->GetItemX() + stage->GetScrollX()) > 0 && (item[i]->GetItemX() + stage->GetScrollX()) < 1280)
			{
				item[i]->Draw();
			}
		}
	}


	//ゲームクリア時の描画
	if (stage->GetClearFlg()) {
		if (clearInterval >= 0) {

			//DrawStringToHandle(GetDrawCenterX("ゲームクリアおめでとう！！！", startTimeFont), 200, "ゲームクリアおめでとう！！！", 0xE2FE47, startTimeFont);

			SetDrawBlendMode(DX_BLENDMODE_ADD, 255 - clearInterval);
			//DrawTriangleAA(0, 0, 0, 720, 640, 720, 0xFFFFFF, TRUE);
			//DrawTriangleAA(1280, 0, 1280, 720, 640, 720, 0xFFFFFF, TRUE);

			DrawBox(0, 0, 1280, 720, 0xFFFFFF, TRUE);

			//DrawTriangleAA(0, 0, 0, 720, player->GetPlayerX() - stage->GetScrollX(), player->GetPlayerY(), 0xFFFFFF, TRUE);
			//DrawTriangleAA(1280, 0, 1280, 720, player->GetPlayerX() - stage->GetScrollX(), player->GetPlayerY(), 0xFFFFFF, TRUE);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	//HUD

	//プレイヤーのライフの描画
	for (int i = 0; i < player->GetLife(); i++) {
		DrawRotaGraph(30 + 50 * i, 20, 1, 0, hpImage, TRUE);
	}

	//経過時間の描画
	char dis_clear_time[20];	//文字列合成バッファー

	//文字列合成
	if (elapsedTime / 1000 >= 60)
	{

		sprintf_s(dis_clear_time, sizeof(dis_clear_time), "%4d:%02d.%.3d",
			(elapsedTime / 1000) / 60, (elapsedTime / 1000) % 60, elapsedTime % 1000);
	}
	else
	{

		sprintf_s(dis_clear_time, sizeof(dis_clear_time),
			"%5d.%.3d", elapsedTime / 1000, elapsedTime % 1000);
	}

	int str_width = GetDrawFormatStringWidthToHandle(timeFont, dis_clear_time);

	DrawStringToHandle(1260 - str_width, 10, dis_clear_time, 0x1aff00, timeFont, 0xFFFFFF);


	//デバッグ
	//if (CheckHitKey(KEY_INPUT_A)) {
	//	DrawFormatString(100, 150, 0xF77D0A, "ScrollX:%f", stage->GetScrollX());
	//	DrawFormatString(100, 200, 0xF77D0A, "ScrollY:%f", stage->GetScrollY());
	//	DrawFormatString(100, 250, 0xE04D02, "MapData:%d", stage->GetMapData((player->GetPlayerY() / MAP_CEllSIZE) + 1, player->GetPlayerX() / MAP_CEllSIZE));
	//	DrawFormatString(100, 300, 0x02F896, "PlayerX%f", player->GetPlayerX());
	//	DrawFormatString(100, 350, 0x02F896, "PlayerY%f", player->GetPlayerY());
	//	DrawFormatString(100, 400, 0x02F896, "SpawnPointY:%d", stage->GetSpawnPoint().y);
	//	DrawFormatString(100, 450, 0x02F896, "Jump:%f", player->GetJumpVelocity());
	//}

	if (startTime > 0) {

		char dis_start_time[2];	//文字列合成バッファー

		sprintf_s(dis_start_time, sizeof(dis_start_time), "%d", startTime / 60 + 1);

		DrawStringToHandle(GetDrawCenterX(dis_start_time, startTimeFont), 300, dis_start_time, 0xEBA05E, startTimeFont, 0xFFFFFF);
	}
	else if (startEffectTimer > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0 + startEffectTimer * 3 - 5 % 255);

		if (reStart == false) {
			DrawStringToHandle(GetDrawCenterX("START", startTimeFont), 300, "START", 0xF5E03D, startTimeFont, 0xFFFFFF);
		}
		else {
			DrawStringToHandle(GetDrawCenterX("RESTART", startTimeFont), 300, "RESTART", 0xE66500, startTimeFont, 0xFFFFFF);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}



	//常に最前面
	if (pause->IsPause() == true) { //ポーズ画面へ

		pause->Draw();
	}
}
