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
	menuFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 80, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	titleFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 140, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 8);
	buttonGuidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
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

	//START�{�^���Ń|�[�Y
	if ((PAD_INPUT::GetNowKey() == XINPUT_BUTTON_START) && (PAD_INPUT::GetPadState() == PAD_STATE::ON)) {
		
		if (pauseFlag == true) {
			PlaySoundMem(menuCloseSe, DX_PLAYTYPE_BACK, TRUE);
			//�f���[�g����
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
				//ok_se����I����Ă����ʐ��ڂ���B
				while (CheckSoundMem(okSe)) {}
				StartJoypadVibration(DX_INPUT_PAD1, OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);

				//�f���[�g����
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

			//A�{�^���ł��|�[�Y��߂�
			if ((PAD_INPUT::GetNowKey() == (Option::GetInputMode() ? XINPUT_BUTTON_A : XINPUT_BUTTON_B)) && (PAD_INPUT::GetPadState() == PAD_STATE::ON)) {
				//�f���[�g����
				DeleteGraph(pauseGraph);
				selectMenu = 0;
				pauseGraph = 0;
				pauseEffectTimer = 0;
				pauseFlag = !pauseFlag;

				// A�{�^������莞�Ԗ���������
				int disable_time = 100;  // A�{�^���𖳌������鎞�ԁi�P�ʁF�~���b�j
				int startTime = GetNowCount();  // A�{�^��������������
				while (!pauseFlag) {
					// A�{�^���𖳌������鎞�Ԃ𒴂����ꍇ�Awhile���𔲂���
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

	//�I�v�V������ʂ֓���
	if (option->GetOptionFlg() == true) {
		option->Draw();
	}
	else {

		DrawStringToHandle(GetDrawCenterX("�|�[�Y", titleFont), 100, "�|�[�Y", 0x56F590, titleFont, 0xFFFFFF);
		//�I�����j���[
		DrawStringToHandle(GetDrawCenterX("�Q�[���֖߂�", menuFont), 270, "�Q�[���֖߂�", static_cast<MENU>(selectMenu) == MENU::RETURN ? 0xB3E0F5 : 0xEB8F63, menuFont, 0xFFFFFF);
		DrawStringToHandle(GetDrawCenterX("���X�^�[�g", menuFont), 360, "���X�^�[�g", static_cast<MENU>(selectMenu) == MENU::RESTART ? 0xEBABDC : 0xEB8F63, menuFont, 0xFFFFFF);
		DrawStringToHandle(GetDrawCenterX("�I�v�V����", menuFont), 450, "�I�v�V����", static_cast<MENU>(selectMenu) == MENU::OPTION ? 0x5FEBB6 : 0xEB8F63, menuFont, 0xFFFFFF);
		DrawStringToHandle(GetDrawCenterX("�X�e�[�W�I����", menuFont), 540, "�X�e�[�W�I����", static_cast<MENU>(selectMenu) == MENU::STAGE_SELECT ? 0xF5E6B3 : 0xEB8F63, menuFont, 0xFFFFFF);

		//�K�C�h�\��
		DrawStringToHandle(580, 668, "�Q�[���֖߂�", 0xFFA15C, buttonGuidFont, 0x000000);
		DrawCircleAA(560, 680, 15, 20, 0xFFFFFF, 1);
		DrawStringToHandle(553, 668, Option::GetInputMode() ? "A" : "B", Option::GetInputMode() ? A_COLOR : B_COLOR, buttonGuidFont, 0xFFFFFF);
	}
}

int PAUSE::GetDrawCenterX(const char* string, int font_handle)const {

	//��ʕ�
	const int screenX = 1280;

	const int w = screenX / 2 - GetDrawFormatStringWidthToHandle(font_handle, string) / 2;
	return w;
}