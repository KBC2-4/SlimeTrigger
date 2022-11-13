#include "STAGE.h"
#include "Element.h"
#include"DxLib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "PLAYER.h"
#include "RESULT.h"

int STAGE::map_data[MAP_HEIGHT][MAP_WIDTH];
float STAGE::scroll_x = 0;
float STAGE::scroll_y = 0;

STAGE::STAGE() {
	**map_data = 0;
	*block_image1 = 0;
	*stage_image = 0;
	//scroll_x = -8640;
	scroll_x = 0;
	scroll_y = 0;
	if (LoadDivGraph("Resource/Images/Stage/map_chips.png", 100, 10, 10, 80, 80, block_image1) == -1) {
		throw "Resource/Images/Stage/map_chips.png";
	}
	//InitStage();
	LoadMapData();
	clearflg = false;
	*clearbox = 0;

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			//クリア座標を代入
			if (map_data[i][j] == 73) { clearbox[0] = j * MAP_CEllSIZE; clearbox[1] = i * MAP_CEllSIZE; }
		}
	}
}
	

void STAGE::Update(PLAYER* player) {
	StageClear(player);
}

void STAGE::Draw()const {
	//デバッグ
	DrawFormatString(200, 200, 0xffffff, "scroll_y:%f", scroll_y);
	//ゲームクリア時
	if (clearflg == true) {DrawExtendString(30, 200, 5.5f, 5.5f, "ゲームクリアおめでとう！！！", 0xE2FE47);}
	
	//printfDx("%f",scroll_x);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			//画面外は描画しない
			if (j * MAP_CEllSIZE + scroll_x >= -80 && j * MAP_CEllSIZE + scroll_x <= 1280 && j * MAP_CEllSIZE + scroll_y >= -300) {
				if (map_data[i][j] < 90/* && map_data[i][j] != 68*/) { DrawGraph(j * MAP_CEllSIZE + scroll_x, i * MAP_CEllSIZE, block_image1[map_data[i][j] - 1], TRUE); }
			}
			
		}
	}
	
}

void STAGE::CameraWork(PLAYER *player) {
	//if (player->GetPlayerY() < 6)scroll_y = -60;
}



void STAGE::HookProcess() {

}

void STAGE::PuddleProcess(){

}

/// <summary>
/// ステージのスクロール
/// </summary>
bool STAGE::SetScrollPos(int move_x) {
	scroll_x -= 5 * move_x;
	if (scroll_x >= 0 || scroll_x <= -8080) {
		scroll_x += 5 * move_x;
		return true;
	}
	return false;
}

/// <summary>
/// プレイヤーとブロックの当たり判定
/// </summary>

bool STAGE::HitMapDat(int y, int x) {
	if (CheckHitKey(KEY_INPUT_Z))return false;		//デバッグ用
	int block_type = GetMapDat(y, x);
	if (
		block_type == -1 //範囲外
		|| block_type == 0	//水玉草
		|| block_type == 15 //フロー木
		|| block_type == 14 //アカシア木
		|| block_type == 13 //オーク木
		|| block_type == 64	//ドア 
		|| block_type == 65	//ドア 
		|| block_type == 62	//ボタン(感圧式)
		|| block_type == 68	//マンホールの蓋
		|| block_type == 69	//マンホールの中
		|| block_type == 73	//ゴール
		|| block_type == 91 
		|| block_type == 92 
		|| block_type == 93
		|| block_type == 95	//動く床

		//応急処置
		|| block_type == 19	//New アカシア木
		) {
		return false;
	}
	return true;
}
/// <summary>
/// スライムのかけらの当たり判定
/// </summary>
bool STAGE::HitThrowSlime(int y, int x) {
	int block_type = GetMapDat(y, x);
	if (
		block_type == -1 //範囲外
		|| block_type == 0	//水玉草
		|| block_type == 62	//ボタン(感圧式)
		|| block_type == 68	//マンホールの蓋
		|| block_type == 69	//マンホールの中
		|| block_type == 73	//ゴール
		) {
	return false;
	}
		return true;
}

/// <summary>
/// マップデータの読み込み
/// </summary>
void STAGE::LoadMapData(void) {

		std::ifstream ifs("Resource/Map_Data/MapData1.csv");

		std::string str = "";
		int i = 0, j = 0;
		while (std::getline(ifs,str))
		{
			std::string tmp = "";
			std::istringstream stream(str);

			while (std::getline(stream,tmp,','))
			{
				map_data[i][j] = std::stoi(tmp);
				j++;
			}
			j = 0;
			i++;
		}
}


/// <summary>
/// ステージクリア時
/// </summary>
void STAGE::StageClear(PLAYER *player) {
	int player_map_x = static_cast<int>(roundf(player->GetPlayerX() - STAGE::GetScrollX()));
	int player_map_y = static_cast<int>(floorf(player->GetPlayerY()));
	DrawFormatString(100, 200, 0xffffff, "x:%dy:%d", clearbox[0], clearbox[1]);

	//旗に触れるとゲームクリア
	if ((player_map_x >= clearbox[0] - MAP_CEllSIZE / 2 + 50) && (player_map_x <= clearbox[0] + MAP_CEllSIZE + 30) && (player_map_y >= clearbox[1] - MAP_CEllSIZE / 2) && (player_map_y <= clearbox[1] + MAP_CEllSIZE / 2)) {
		clearflg = true;
	}

	if (clearflg == true) {
		static int count = GetNowCount();
		if ((GetNowCount() - count) > 3000) {
			clearflg = false;
			count = GetNowCount();
		}
		/*if (GetNowCount() % 30 == 0)printfDx("%d:::::%d\n", count, GetNowCount());*/
	}
	
}