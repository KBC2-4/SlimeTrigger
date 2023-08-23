#include "Pause.h"
#include "DxLib.h"
#include "PadInput.h"
#include "Title.h"
#include "Option.h"

PAUSE::PAUSE() {
	if ((cursorMoveSe = LoadSoundMem("Resource/Sounds/SE/cursor_move.wav")) == -1) {
		throw "Resource/Sounds/SE/cursor_move.wav";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}

	if ((menuOpenSe = LoadSoundMem("Resource/Sounds/SE/pause_open.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}

	if ((menuCloseSe = LoadSoundMem("Resource/Sounds/SE/pause_close.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}
	menuFont = CreateFontToHandle("UD デジタル 教科書体 N-B", 80, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	titleFont = CreateFontToHandle("UD デジタル 教科書体 N-B", 140, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 8);
	buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	selectMenu = 0;
	nextMenu = 0;
	inputMargin = 0;
	pauseGraph = 0;
	pauseEffectTimer = 0;
	pauseFlag = false;

	//SE
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.6, cursorMoveSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.3, menuOpenSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.3, menuCloseSe);

	option = new Option();
}

PAUSE::~PAUSE() {

	delete option;
	DeleteFontToHandle(titleFont);
	DeleteFontToHandle(menuFont);
	DeleteFontToHandle(buttonGuidFont);
	DeleteSoundMem(cursorMoveSe);
	DeleteSoundMem(okSe);
	DeleteSoundMem(menuOpenSe);
	DeleteSoundMem(menuCloseSe);

	DeleteGraph(pauseGraph);
}

int PAUSE::Update(void) {

	if (GetWindowActiveFlag() == FALSE || GetWindowMinSizeFlag() == TRUE) { pauseFlag = true; }

	//STARTボタンでポーズ
	if ((PAD_INPUT::GetNowKey() == XINPUT_BUTTON_START) && (PAD_INPUT::GetPadState() == PAD_STATE::ON)) {
		
		if (pauseFlag == true) {
			PlaySoundMem(menuCloseSe, DX_PLAYTYPE_BACK, TRUE);
			//デリート処理
			DeleteGraph(pauseGraph);
			pauseGraph = 0;
			pauseEffectTimer = 0;
		}else{ PlaySoundMem(menuOpenSe, DX_PLAYTYPE_BACK, TRUE); }
		SetPause();
	}

	if (pauseFlag == true)
	{



		if (pauseGraph == 0) {
			pauseGraph = MakeGraph(1280, 720);
			GetDrawScreenGraph(0, 0, 1280, 720, pauseGraph);
		}

		ChangeVolumeSoundMem(Option::GetSEVolume() * 1.6, cursorMoveSe);
		ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);
		ChangeVolumeSoundMem(Option::GetSEVolume() * 1.3, menuOpenSe);
		ChangeVolumeSoundMem(Option::GetSEVolume() * 1.3, menuCloseSe);

		if (option->GetOptionFlg() == true) {
			option->Update();
		}
		else {
			if (inputMargin < 20) {
				inputMargin++;
			}
			else {

				if ((PAD_INPUT::GetPadThumbLY() > 20000) || (PAD_INPUT::GetNowKey() == XINPUT_BUTTON_DPAD_UP))
				{
					selectmenu = (selectmenu + 3) % 4;
					input_margin = 0; PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);
					StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1); 
				}
				if ((PAD_INPUT::GetPadThumbLY() < -20000) || (PAD_INPUT::GetNowKey() == XINPUT_BUTTON_DPAD_DOWN))
				{
					selectmenu = (selectmenu + 1) % 4;
					input_margin = 0;
					PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE); StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
				}
			}

			if ((PAD_INPUT::GetNowKey() == (Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) && (PAD_INPUT::GetPadState() == PAD_STATE::ON)) {
				PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
				//ok_seが鳴り終わってから画面推移する。
				while (CheckSoundMem(okSe)) {}
				StartJoypadVibration(DX_INPUT_PAD1, OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);

				//デリート処理
				if (static_cast<MENU>(selectMenu) != MENU::OPTION) {
					DeleteGraph(pauseGraph);
					pauseGraph = 0;
					pauseEffectTimer = 0;
				}
				nextMenu = selectMenu;

				if (static_cast<MENU>(selectMenu) == MENU::RETURN) {
					pauseFlag = !pauseFlag;
				}
				else if (static_cast<MENU>(selectMenu) == MENU::OPTION) { option->ChangeOptionFlg(); }
				else if (static_cast<MENU>(selectMenu) == MENU::STAGE_SELECT) { option->~Option(); }
			}

			//Aボタンでもポーズを戻す
			if ((PAD_INPUT::GetNowKey() == (Option::GetInputMode() ? XINPUT_BUTTON_A : XINPUT_BUTTON_B)) && (PAD_INPUT::GetPadState() == PAD_STATE::ON)) {
				//デリート処理
				DeleteGraph(pauseGraph);
				selectMenu = 0;
				pauseGraph = 0;
				pauseEffectTimer = 0;
				pauseFlag = !pauseFlag;

				// Aボタンを一定時間無効化する
				int disable_time = 100;  // Aボタンを無効化する時間（単位：ミリ秒）
				int startTime = GetNowCount();  // Aボタンを押した時間
				while (!pauseFlag) {
					// Aボタンを無効化する時間を超えた場合、while文を抜ける
					if (GetNowCount() - startTime > disable_time) {
						break;
					}
				}
			}
		}
	}
	return 0;
}

void PAUSE::Draw() {

	if (pauseEffectTimer < 50) {
		pauseEffectTimer++;
		if (pauseEffectTimer % 5 == 0) {
			GraphFilter(pauseGraph, DX_GRAPH_FILTER_GAUSS, 16, 70);
		}
	}

	DrawGraph(0, 0, pauseGraph, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawFillBox(0, 0, 1280, 720, 0x000000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//オプション画面へ入る
	if (option->GetOptionFlg() == true) {
		option->Draw();
	}
	else {

		DrawStringToHandle(GetDrawCenterX("ポーズ", titleFont), 100, "ポーズ", 0x56F590, titleFont, 0xFFFFFF);
		//選択メニュー
		DrawStringToHandle(GetDrawCenterX("ゲームへ戻る", menuFont), 270, "ゲームへ戻る", static_cast<MENU>(selectMenu) == MENU::RETURN ? 0xB3E0F5 : 0xEB8F63, menuFont, 0xFFFFFF);
		DrawStringToHandle(GetDrawCenterX("リスタート", menuFont), 360, "リスタート", static_cast<MENU>(selectMenu) == MENU::RESTART ? 0xEBABDC : 0xEB8F63, menuFont, 0xFFFFFF);
		DrawStringToHandle(GetDrawCenterX("オプション", menuFont), 450, "オプション", static_cast<MENU>(selectMenu) == MENU::OPTION ? 0x5FEBB6 : 0xEB8F63, menuFont, 0xFFFFFF);
		DrawStringToHandle(GetDrawCenterX("ステージ選択へ", menuFont), 540, "ステージ選択へ", static_cast<MENU>(selectMenu) == MENU::STAGE_SELECT ? 0xF5E6B3 : 0xEB8F63, menuFont, 0xFFFFFF);

		//ガイド表示
		DrawStringToHandle(580, 668, "ゲームへ戻る", 0xFFA15C, buttonGuidFont, 0x000000);
		DrawCircleAA(560, 680, 15, 20, 0xFFFFFF, 1);
		DrawStringToHandle(553, 668, Option::GetInputMode() ? "A" : "B", Option::GetInputMode() ? A_COLOR : B_COLOR, buttonGuidFont, 0xFFFFFF);
	}
}

int PAUSE::GetDrawCenterX(const char* string, int font_handle)const {

	//画面幅
	const int screenX = 1280;

	const int w = screenX / 2 - GetDrawFormatStringWidthToHandle(font_handle, string) / 2;
	return w;
}