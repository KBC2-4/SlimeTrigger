#include "Stage.h"
#include "Element.h"
#include"DxLib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


#include "Player.h"
#include "Result.h"
#include "Option.h"
//円周率
#define PI    3.1415926535897932384626433832795f

//#define _DEBUG

/// <summary>
/// コンストラクタ
/// </summary>
STAGE::STAGE(const char* stageName, bool restert) {
	//**mapData = 0;
	*blockImage1 = 0;
	scrollX = 0;
	scrollY = 0;
	scrollSpeedX = 5;
	scrollSpeedY = 5;
	countTimer = 0;

	playerXOld = 0;
	playerYOld = 0;
	playerVectorX = 0;
	playerVectorY = 0;


	//背景画像読み込み
	if (stageName == "Stage01") {

		if ((backGraundImage[0] = LoadGraph("Resource/Images/Stage/BackImage1.png")) == -1) {
			throw "Resource/Images/Stage/BackImage1.png";
		}
	}
	else if (stageName == "Stage02") {

		if ((backGraundImage[1] = LoadGraph("Resource/Images/Stage/BackImage2.png")) == -1) {
			throw "Resource/Images/Stage/BackImage2.png";
		}
	}
	else if (stageName == "Stage03") {

		if ((backGraundImage[2] = LoadGraph("Resource/Images/Stage/BackImage3.png")) == -1) {
			throw "Resource/Images/Stage/BackImage2.png";
		}
	}
	else {
		if ((backGraundImage[0] = LoadGraph("Resource/Images/Stage/BackImage1.png")) == -1) {
			throw "Resource/Images/Stage/BackImage1.png";
		}
	}

	if (LoadDivGraph("Resource/Images/Stage/map_chips2.png", 110, 10, 11, 80, 80, blockImage1) == -1) {
		throw "Resource/Images/Stage/map_chips2.png";
	}

	if ((halfWayPointSe = LoadSoundMem("Resource/Sounds/SE/Stage/halfwaypoint.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/halfwaypoint.wav";
	}

	//mapData.resize(MAP_HEIGHT);

	//InitStage();

	LoadMapData(stageName);
	temporaryHit = 999;
	clearFlag = false;
	clearBox = { 0,0 };
	clearCount = 3000;
	halWayPointBox = { 0,0 };
	halfWayPoint = restert;
	halfWayTimer = 0;
	spawnPoint = { 0,0 };
	this->stageName = stageName;


	for (int i = 0; i < mapData.size(); i++) {
		for (int j = 0; j < mapData.at(0).size(); j++) {
			//クリア座標を代入
			if (mapData.at(i).at(j) == 88) { clearBox.x = j * MAP_CEllSIZE; clearBox.y = i * MAP_CEllSIZE; }
			//中間地点座標を代入
			if (mapData.at(i).at(j) == 90) { halWayPointBox.x = j * MAP_CEllSIZE; halWayPointBox.y = i * MAP_CEllSIZE; }

			//スポーン地点座標を代入
			if (mapData.at(i).at(j) == 777) { spawnPoint.x = i * MAP_CEllSIZE; spawnPoint.y = j * MAP_CEllSIZE; }
		}
	}
	//スポーン地点をセット
	if (spawnPoint.x == 0 && spawnPoint.y == 0) {
		MessageBox(NULL, "スポーン地点がセットされていません。", "マップ読み込みエラー", MB_OK | MB_ICONERROR);
		spawnPoint.x = 0;
		spawnPoint.y = 0;
	}

	ChangeVolumeSoundMem(Option::GetSEVolume(), halfWayPointSe);
}
/// <summary>
/// デストラクタ
/// </summary>
STAGE::~STAGE() {
	for (int i = 0; i < 110; i++) {
		DeleteGraph(blockImage1[i]);
	}
	DeleteSoundMem(halfWayPointSe);

	if (stageName == "Stage01") {
		DeleteGraph(backGraundImage[0]);
	}
	else if (stageName == "Stage02") {
		DeleteGraph(backGraundImage[1]);
	}
	else if (stageName == "Stage03") {
		DeleteGraph(backGraundImage[2]);
	}
	else {
		DeleteGraph(backGraundImage[0]);
	}
}

/// <summary>
/// ステージの更新
/// </summary>
void STAGE::Update(PLAYER* player, ELEMENT* element) {

	//デバッグ
	//if (CheckHitKey(KEY_INPUT_C))clearFlag = true;
	ChangeVolumeSoundMem(Option::GetSEVolume(), halfWayPointSe);

	StageClear(player);
	HalfwayPoint(player);
	CameraWork(player, element);
}
/// <summary>
/// ステージの描画
/// </summary>
void STAGE::Draw(ELEMENT* element)const {

	//ステージ背景
	if (stageName == "Stage01") {
		DrawGraph(static_cast<int>(scrollX) % 2560 + 2560, /*scrollY*/0, backGraundImage[0], FALSE);
		DrawGraph(static_cast<int>(scrollX) % 2560, /*scrollY*/0, backGraundImage[0], FALSE);
	}
	else if (stageName == "Stage02") {
		DrawGraph(static_cast<int>(scrollX) % 2560 + 2560, /*scrollY*/0, backGraundImage[1], FALSE);
		DrawGraph(static_cast<int>(scrollX) % 2560, /*scrollY*/0, backGraundImage[1], FALSE);
	}
	else if (stageName == "Stage03") {
		DrawGraph(static_cast<int>(scrollX) % 2560 + 2560, /*scrollY*/0, backGraundImage[2], FALSE);
		DrawGraph(static_cast<int>(scrollX) % 2560, /*scrollY*/0, backGraundImage[2], FALSE);
	}
	else {
		DrawGraph(static_cast<int>(scrollX) % 2560 + 2560, /*scrollY*/0, backGraundImage[0], FALSE);
		DrawGraph(static_cast<int>(scrollX) % 2560, /*scrollY*/0, backGraundImage[0], FALSE);
	}

	//地下背景描画
	if (stageName == "Stage01") {
		//ステージ１
		DrawBoxAA(scrollX + 6880, scrollY + 1380, scrollX + 10640, scrollY + 1900, 0x20251F, TRUE);
	}
	else if (stageName == "Stage03") {
		//ステージ3
		DrawBoxAA(0, scrollY + 1380, scrollX + 5120, scrollY + 4800, 0x20251F, TRUE);
	}


	for (int i = 0; i < mapData.size(); i++) {
		for (int j = 0; j < mapData.at(0).size(); j++) {
			//if (mapData.at(i).at(j) == 72)DrawFormatString(100 + j * 20, 50, 0xffffff, "%d %d", i, j);
			//画面外は描画しない
			if (j * MAP_CEllSIZE + scrollX >= -80 && j * MAP_CEllSIZE + scrollX <= 1280 && i * MAP_CEllSIZE + scrollY >= -80 && i * MAP_CEllSIZE + scrollY <= 720) {
				if (
					mapData.at(i).at(j) != 68		//マンホール(上)
					&& mapData.at(i).at(j) != 62	 //ボタン
					&& mapData.at(i).at(j) != 74	//酸性雨の水たまり
					&& mapData.at(i).at(j) != 75	//酸性雨の水たまり
					&& mapData.at(i).at(j) != 76	//酸性雨の水たまり
					&& mapData.at(i).at(j) != 77	//酸性雨の水たまり
					&& mapData.at(i).at(j) != 78	//酸性雨の水たまり
					&& mapData.at(i).at(j) != 79	//酸性雨の水たまり
					&& mapData.at(i).at(j) != 101		//戻る看板下で別途描画している為
					&& mapData.at(i).at(j) != 102	//ステージ1ゲート(上)
					&& mapData.at(i).at(j) != 103	//ステージ1ゲート(下)
					&& mapData.at(i).at(j) != 104	//ステージ2ゲート(上)
					&& mapData.at(i).at(j) != 105	//ステージ2ゲート(下)
					&& mapData.at(i).at(j) != 106	//ステージ3ゲート(上)
					&& mapData.at(i).at(j) != 107	//ステージ3ゲート(下)
					&& mapData.at(i).at(j) != 51	//動く床
					&& mapData.at(i).at(j) != 52	//動く床
					&& mapData.at(i).at(j) != 53	//動く床
					&& mapData.at(i).at(j) != 54	//動く床
					&& (mapData.at(i).at(j) <= 88		//クリア門等
						////89～90番台を描画しない
						|| mapData.at(i).at(j) >= 100 && mapData.at(i).at(j) != 777)
					) {
					
					int chipNum = mapData.at(i).at(j);

					//横に連なる気は回転させて描画
					if (chipNum == 22||chipNum==23|| chipNum == 25|| chipNum == 26|| chipNum == 28 || chipNum == 29 || chipNum == 31 || chipNum == 32)
					{
						//左側の設定
						//調整距離
						float shiftDistance = 9.0f;
						//画像回転角度
						float angle = PI / 180 * -90;
						//右側の設定
						if (chipNum == 23 || chipNum == 26 || chipNum == 29 || chipNum == 32)
						{
							shiftDistance = -9.0f;
							angle = PI / 180 * 90;
							chipNum -= 1;
						}

						DrawRotaGraphF(j * MAP_CEllSIZE + scrollX + MAP_CEllSIZE / 2 + shiftDistance, i * MAP_CEllSIZE + scrollY + MAP_CEllSIZE / 2, 1.0f, angle, blockImage1[chipNum - 2], TRUE);
					}
					else
					{
						DrawGraphF(j * MAP_CEllSIZE + scrollX, i * MAP_CEllSIZE + scrollY, blockImage1[mapData.at(i).at(j) - 1], TRUE);
					}
				}
			}
			//レモナーとグレポンはツルだけ描画する
			if (mapData.at(i).at(j) == 91 || mapData.at(i).at(j) == 92) { DrawGraphF(j * MAP_CEllSIZE + scrollX, (i - 1) * MAP_CEllSIZE + scrollY, blockImage1[mapData.at(i).at(j) - 1], TRUE); }
			if (mapData.at(i).at(j) == 101) { DrawExtendGraphF((j - 1) * MAP_CEllSIZE + scrollX, (i - 1) * MAP_CEllSIZE + scrollY, (j + 1) * MAP_CEllSIZE + scrollX, (i + 1) * MAP_CEllSIZE + scrollY, blockImage1[100], TRUE); }
		}
	}

	//中間地点　描画
	//中間地点がない場合は描画しない。
	if (halWayPointBox.x != 0) {
		if (halfWayPoint == false) { DrawGraphF(halWayPointBox.x + scrollX, halWayPointBox.y + scrollY, blockImage1[88], TRUE); }
		else { DrawGraphF(halWayPointBox.x + scrollX, halWayPointBox.y + scrollY, blockImage1[89], TRUE); }
	}
}

/// <summary>
/// マップデータの読み込み
/// </summary>
void STAGE::LoadMapData(const char* stageName) {

	char buf[37];
	sprintf_s(buf, sizeof(buf), "Resource/Map_Data/%s.csv", stageName);
	//std::ifstream ifs(buf);

	//mapData.clear();
	//mapData.shrink_to_fit();
	////std::string str = "";

	//int i = 0, j = 0;

	//while (std::getline(ifs, str))
	//{
	//	std::string tmp = "";
	//	std::istringstream stream(str);
	//	mapData.push_back(std::vector<int>());

	//	while (std::getline(stream,tmp,','))
	//	{
	//		//mapData.at(i).at(j) = std::stoi(tmp);
	//		mapData[i].push_back(std::stoi(tmp));
	//		j++;
	//	}
	//	j = 0;
	//	i++;
	//}



	//アーカイブ対応版


	int FileHandle;
	if ((FileHandle = FileRead_open(buf)) == 0) {
		exit(1);
	}

	char str[2000];		//一行の長さ
	char* context;
	int i = 0, j = 0;

	while (FileRead_gets(str, sizeof(str), FileHandle) != -1) {

		char* tmp = strtok_s(str, ",", &context);

		mapData.push_back(std::vector<int>());
		while (tmp != NULL) {
			//mapData[i][j] = atoi(tmp);

			std::string info[2];
			std::istringstream streamtmp(tmp);
			int k = 0;
			while (std::getline(streamtmp, info[k], ':')) {
				k++;
			}

			mapData[i].push_back(std::stoi(tmp));

			//ボタンとドアの連携番号を格納
			if ((mapData[i][j] == 61 || mapData[i][j] == 62) && info[1] != "") {
				buttonInfo.push_back(std::vector<int>());
				buttonInfo.at(buttonInfo.size() - 1).push_back(i);
				buttonInfo.at(buttonInfo.size() - 1).push_back(j);
				buttonInfo.at(buttonInfo.size() - 1).push_back(std::stoi(info[1]));
			}
			if (mapData[i][j] == 66 && info[1] != "") {
				doorInfo.push_back(std::vector<int>());
				doorInfo.at(doorInfo.size() - 1).push_back(i);
				doorInfo.at(doorInfo.size() - 1).push_back(j);
				doorInfo.at(doorInfo.size() - 1).push_back(std::stoi(info[1]));

			}

			tmp = strtok_s(NULL, ",", &context);
			j++;
		}
		j = 0;
		i++;
	}

	FileRead_close(FileHandle);

}

/// <summary>
/// マップ配列のGetter
/// </summary>
int STAGE::GetMapData(int y, int x) {
	if (y < 0 || y >= mapData.size()) { return -999; }
	if (x < 0 || x >= mapData[y].size()) { return -999; }
	return mapData.at(y).at(x);
}

/// <summary>
/// ステージスクロール関数
/// </summary>
void STAGE::CameraWork(PLAYER* player, ELEMENT* element) {
	int scroll_speedY = 7;
	//プレイヤーxベクトルの判定
	if (player->GetPlayerX() > playerXOld) {
		playerVectorX = 1;
	}
	else if (player->GetPlayerX() < playerXOld) {
		playerVectorX = -1;
	}

	//プレイヤーyベクトルの判定
	if (player->GetPlayerY() < playerYOld) {
		playerVectorY = 1;
	}
	else if (player->GetPlayerY() > playerYOld) {
		playerVectorY = -1;
		//scroll_speedY = 10;
	}
	scrollSpeedX = player->GetPlayerSpeed();
	if (player->GetPlayerMoveState() == PLAYER_MOVE_STATE::HOOK) {
		scrollSpeedX = fabsf(player->GetPlayerHookSpeed());
	}
	//x軸スクロール
	//if (element->HitLift(player)) { scrollSpeedX = element->GetLift_SpeedX(); }
	if ((playerVectorX > 0 && player->GetPlayerX() >= 620 || playerVectorX < 0 && player->GetPlayerX() <= 660) && playerXOld != player->GetPlayerX()) {
		scrollX -= scrollSpeedX * playerVectorX;
		if (scrollX > 0 || scrollX <= -(80 * static_cast<int>(mapData.at(0).size()) - 1280)) {
			scrollX += scrollSpeedX * playerVectorX;
		}
	}

	//y軸スクロール
	//if ((playerVectorY > 0 && player->GetPlayerY() <= 240 || playerVectorY < 0 && (scrollY > 0 && mapData.size() <= 14) || (scrollY > -720 && mapData.size() > 14)) && playerYOld != player->GetPlayerY()) {
	//	scrollY += scroll_speedY * playerVectorY;
	//	if (scrollY > 0/* || scrollX <= -(80 * static_cast<int>(mapData.size()) - 720)*/) {
	//		scrollY -= scroll_speedY * playerVectorY;
	//	}
	//}


	//x軸スクロールを元にy座標バージョンを作成
	//if ((playerVectorY > 0 && player->GetPlayerY() >= 620 || playerVectorY < 0 && player->GetPlayerY() <= 300)) {
	//	scrollY -= 5 * playerVectorY;
	//	//if (scrollY > 0 || scrollY <= -(80 * static_cast<int>(mapData.size()) - 720)) {
	//	//	scrollY += 5 * playerVectorY;
	//	//}
	//}

	if (++countTimer % 60 == 0)playerLongOld = player->GetPlayerY();

	//スポーン地点を基準に上げる位置を決める
	//if (scrollY + player->GetPlayerY() < 0 && player->GetPlayerY() <= spawnPoint.y - player->GetPlayerY() + 400 && player->GetPlayerMoveState() != PLAYER_MOVE_STATE::HOOK) { scrollY += scrollSpeedY; }
	//else if (scrollY + player->GetPlayerY() < player->GetPlayerY()) {
	//	if (scrollY >= (-MAP_CEllSIZE * static_cast<int>(mapData.size()) + 721) && (player->GetPlayerY() > GetSpawnPoint().y + 400)) {
	//		//プレイヤーの落下速度に応じてスクロールYを下げる
	//		if (player->GetJumpVelocity() > 0)scrollY -= player->GetJumpVelocity();
	//	}
	//	if ((player->GetPlayerMoveState() == PLAYER_MOVE_STATE::HOOK || player->GetPlayerMoveState() == PLAYER_MOVE_STATE::GROW_HOOK) && player->GetPlayerY() > 500.0f) {
	//		scrollY -= 5;
	//	}
	//}


	//マンホールの下にいった時	
	//if (-scrollY + player->GetPlayerY() > player->GetPlayerY())scrollY--;
	//for(unsigned int i=scrollY )

	//スクロールY-720とプレイヤーY520の誤差が200になるまで



	if (player->GetPlayerY() >= 560 &&player->GetPlayerY()-scrollY<=mapData.size()*MAP_CEllSIZE&& GetMapData((player->GetPlayerY() - scrollY) / MAP_CEllSIZE + 3, (player->GetPlayerX() - scrollX) / MAP_CEllSIZE) != -1) {
		if (player->GetPlayerMoveState() == PLAYER_MOVE_STATE::FALL) {
			//プレイヤーの落下速度に応じてスクロールYを下げる
			if (player->GetJumpVelocity() > 0)scrollY -= player->GetJumpVelocity() + 10.0f;
		}
		else {
			scrollY -= 10.0f;
		}
	}
	else if (player->GetPlayerY() <= 320) {
		scrollY += 5;
	}

	if (playerXOld != player->GetPlayerX()) {
		playerXOld = player->GetPlayerX();
	}
	else {
		playerVectorX = 0;
	}
	if (playerYOld != player->GetPlayerY()) {
		playerYOld = player->GetPlayerY();
	}
	else playerVectorY = 0;


	//カメラがステージ外へ行かないよう調整
	if (0 < scrollY)
	{
		scrollY = 0;
	}

	float stageHeight = mapData.size() * MAP_CEllSIZE - 720;

	if (scrollY < -stageHeight)
	{
		scrollY = -stageHeight;
	}
}

/// <summary>
/// 画面スクロール座標XのSetter
/// </summary>
bool STAGE::SetScrollPos(int moveX) {
	scrollX -= 5 * moveX;
	if (scrollX >= 0 || scrollX <= -(80 * static_cast<int>(mapData.at(0).size()) - 640)) {
		scrollX += 5 * moveX;
		return true;
	}
	return false;
}

/// <summary>
/// プレイヤーとブロックの当たり判定
/// </summary>
bool STAGE::HitMapDat(int y, int x) {
#ifdef _DEBUG
	if (PAD_INPUT::OnPressed(XINPUT_BUTTON_Y) || CheckHitKey(KEY_INPUT_Z))return false;		//デバッグ用
#endif
	int block_type = GetMapData(y, x);
	if (block_type == temporaryHit) { return true; }
	if (
		block_type == -1 //範囲外
		|| block_type == 0	//水玉草
		|| block_type == 21 //木1
		|| block_type == 22 //木1(横左)
		|| block_type == 23 //木1(横右)
		|| block_type == 24 //木2
		|| block_type == 25 //木2(横左)
		|| block_type == 26 //木2(横右)
		|| block_type == 27 //木3
		|| block_type == 28 //木3(横左)
		|| block_type == 39 //木3(横右)
		|| block_type == 30 //木4
		|| block_type == 31 //木4(横左)
		|| block_type == 32 //木4(横右)
		|| block_type == 51	//動く床(縦)
		|| block_type == 52	//動く床(横)
		|| block_type == 53	//動く床(ゴール縦)
		|| block_type == 54	//動く床(ゴール横)
		|| block_type == 64	//ドア 
		|| block_type == 65	//ドア 
		|| block_type == 62	//ボタン(感圧式)
		|| block_type == 68	//マンホールの蓋
		|| block_type == 69	//マンホールの中
		|| block_type == 71	//ツル中間
		|| block_type == 72	//ツル
		|| block_type == 73	//ゴール
		|| block_type == 76 //酸溜まり
		|| block_type == 85	//クリア門
		|| block_type == 86	//クリア門
		|| block_type == 87	//クリア門
		|| block_type == 88	//クリア門
		|| block_type == 90 //中間地点
		|| block_type == 91 //レモナー
		|| block_type == 92 //グレポン
		|| block_type == 93	//トマトン
		|| block_type == 97	//マンホールの蓋(出口)
		|| block_type == 98	//マンホールの開いている蓋
		|| block_type == 101//戻るブロック
		|| block_type == 102//ステージ1ブロック
		|| block_type == 103//ステージ2ブロック
		|| block_type == 104//ステージ3ブロック
		|| block_type == 105//ステージ3ブロック
		|| block_type == 106//ステージ3ブロック
		|| block_type == 107//ステージ3ブロック
		|| block_type == 777//スポーン地点ブロック
		) {
		return false;
	}

	return true;
}
/// <summary>
/// スライムのかけらの当たり判定
/// </summary>
bool STAGE::HitThrowSlime(int y, int x) {
	int block_type = GetMapData(y, x);
	if (
		block_type == -1 //範囲外
		|| block_type == 0	//水玉草
		|| block_type == 31 //葉っぱ
		|| block_type == 61	//壁ボタン(感圧式)
		|| block_type == 62	//ボタン(感圧式)
		|| block_type == 68	//マンホールの蓋
		|| block_type == 69	//マンホールの中
		|| block_type == 71	//ツタ
		|| block_type == 72	//ツタ(捕まる部分)
		|| block_type == 73	//ゴール
		|| block_type == 86	//クリア門
		|| block_type == 87	//クリア門
		|| block_type == 88	//クリア門
		|| block_type == 89	//クリア門
		|| block_type == 97	//マンホールの蓋End
		|| block_type == 98	//マンホールの開いている蓋
		|| block_type == 101//戻るブロック
		|| block_type == 102//ステージ1ブロック
		|| block_type == 103//ステージ2ブロック
		|| block_type == 104//ステージ3ブロック
		|| block_type == 777//スポーン地点ブロック
		) {
		return false;
	}
	return true;
}


/// <summary>
/// ステージクリア判定処理
/// </summary>
void STAGE::StageClear(PLAYER* player) {

	int playerMapX = static_cast<int>(roundf(player->GetPlayerX() - scrollX));
	int playerMapY = static_cast<int>(floorf(player->GetPlayerY()) - scrollY);

	//旗に触れるとゲームクリア
	if ((playerMapX >= clearBox.x - MAP_CEllSIZE / 2 + 50) && (playerMapX <= clearBox.x + MAP_CEllSIZE + 30) && (playerMapY >= clearBox.y - MAP_CEllSIZE * 8) && (playerMapY <= clearBox.y + MAP_CEllSIZE / 2)) {
		clearFlag = true;
	}

	if (clearFlag == true) {

		if (--clearCount <= 0) {
			clearFlag = false;
			clearCount = 3000;
		}
		/*if (GetNowCount() % 30 == 0)printfDx("%d:::::%d\n", count, GetNowCount());*/
	}

}
/// <summary>
/// 中間地点判定処理
/// </summary>
void STAGE::HalfwayPoint(PLAYER* player) {
	int playerMapX = roundf(player->GetPlayerX() - STAGE::GetScrollX());
	int playerMapY = floorf(player->GetPlayerY());
	if ((playerMapX >= halWayPointBox.x - MAP_CEllSIZE / 2) && (playerMapX <= halWayPointBox.x + MAP_CEllSIZE / 2)/* && (playerMapY >= halWayPointBox.y - MAP_CEllSIZE) && (playerMapY <= halWayPointBox.y + MAP_CEllSIZE)*/) {

		if (halfWayPoint == false) {
			PlaySoundMem(halfWayPointSe, DX_PLAYTYPE_BACK, TRUE);

			if (++halfWayTimer < 180) {
				DrawOvalAA(halWayPointBox.x + scrollX + MAP_CEllSIZE + aniTimer % 3, halWayPointBox.y + scrollY + 30 + aniTimer, 25, 10, 4, 0xbfcb4e, TRUE, 1.0f);
				DrawOvalAA(halWayPointBox.x + scrollX + MAP_CEllSIZE + aniTimer % 3, halWayPointBox.y + scrollY + 30 + aniTimer, 25, 10, 4, 0xbfcb4e, TRUE, 1.0f);
				DrawOvalAA(halWayPointBox.x + scrollX + aniTimer % 3, halWayPointBox.y + scrollY + 30 + aniTimer, 25, 10, 4, 0xbfcb4e, TRUE, 1.0f);
			}
			else if (180 <= halfWayTimer)halfWayTimer = 0;
		}
		halfWayPoint = true;
	}
}