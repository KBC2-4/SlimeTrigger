#include "Option.h"
#include "DxLib.h"
#include "PadInput.h"
#include "Title.h"

#include <fstream>
#include <string>
#include <sstream>

int Option::bgmVolume = 255 * 50 / 100;
int Option::seVolume = 255 * 50 / 100;
bool Option::inputMode = true;

Option::Option() {
	menuFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 80, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	buttonGuidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	if ((cursorMoveSe = LoadSoundMem("Resource/Sounds/SE/cursor_move.wav")) == -1)
	{
		throw "Resource/Sounds/SE/cursor_move.wav";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}

	if ((muteSe = LoadSoundMem("Resource/Sounds/SE/mute.wav")) == -1)
	{
		throw "Resource/Sounds/SE/cursor_move.wav";
	}

	windowMode = 1;
	LoadData();

	//PlaySoundMem(backGraundMusic, DX_PLAYTYPE_LOOP);

	oldBgmVolume = 0;
	oldSeVolume = 0;
	selectMenu = 0;
	inputMargin = 0;

	//SE
	ChangeVolumeSoundMem(GetSEVolume() * 1.6, cursorMoveSe);
	ChangeVolumeSoundMem(GetSEVolume() * 1.6, okSe);
	ChangeVolumeSoundMem(GetSEVolume() * 1.6, muteSe);

	optionFlag = false;

	ChangeWindowMode(windowMode);
}


Option::~Option() {
	SaveData();
	DeleteFontToHandle(menuFont);
	DeleteFontToHandle(buttonGuidFont);
	DeleteSoundMem(cursorMoveSe);
	DeleteSoundMem(okSe);
	DeleteSoundMem(muteSe);
	selectMenu = 0;
	optionFlag = false;
}


void Option::Update() {
	
	if (inputMargin < 20) {
		inputMargin++;
	}
	else {
		// ���쐧���̃J�E���^�[�����Z�b�g
		if ((PAD_INPUT::GetPadThumbLY() > 20000) || (PAD_INPUT::GetPadThumbLY() < -20000)
			|| (PAD_INPUT::GetPadThumbLX() > 20000) || (PAD_INPUT::GetPadThumbLX() < -20000)
			|| PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_UP) || PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_DOWN)
			|| PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_LEFT) || PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_RIGHT)) {
				inputMargin = 0;
		}


		if (((static_cast<MENU>(selectMenu) == MENU::BGM && bgmVolume < 255 * 90 / 100) || (static_cast<MENU>(selectMenu) == MENU::SE && seVolume < 255 * 90 / 100)) && PAD_INPUT::GetPadThumbLX() > 20000) {
			PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
			StartJoypadVibration(DX_INPUT_PAD1, 50, 100, -1);
		}

		if (((static_cast<MENU>(selectMenu) == MENU::BGM && bgmVolume > 255 * 10 / 100) || (static_cast<MENU>(selectMenu) == MENU::SE && seVolume > 255 * 10 / 100)) && PAD_INPUT::GetPadThumbLX() < -20000) {
			PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
			StartJoypadVibration(DX_INPUT_PAD1, 50, 100, -1);
		}

		if ((PAD_INPUT::GetPadThumbLY() > 20000) || PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_UP)) {
			selectMenu = (selectMenu + 3) % 4; PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE); StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
		}

		if ((PAD_INPUT::GetPadThumbLY() < -20000) || PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_DOWN)) {
			selectMenu = (selectMenu + 1) % 4; PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE); StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
		}

		if ((PAD_INPUT::GetPadThumbLX() > 20000) || (PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_RIGHT))) {
			if (static_cast<MENU>(selectMenu) == MENU::BGM && bgmVolume < 255 * 90 / 100) { bgmVolume += 255 * 10 / 100; }
			else if (static_cast<MENU>(selectMenu) == MENU::SE && seVolume < 255 * 90 / 100) { seVolume += 255 * 10 / 100; }

			ChangeVolumeSoundMem(GetSEVolume() * 1.6, cursorMoveSe);
		}

		if ((PAD_INPUT::GetPadThumbLX() < -20000) || (PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_LEFT))) {
			if (static_cast<MENU>(selectMenu) == MENU::BGM && bgmVolume > 255 * 10 / 100) { bgmVolume -= 255 * 10 / 100; }
			else if (static_cast<MENU>(selectMenu) == MENU::SE && seVolume > 255 * 10 / 100) { seVolume -= 255 * 10 / 100; }
		}

		////SE��~�E�o�C�u���[�V������~
		//if()

		//}
	}


	//A�{�^���Ń~���[�g�܂���50%�ɐݒ肷��
	if ((PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A))) {

		if (static_cast<MENU>(selectMenu) == MENU::WindowMode) {
			PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
			//ok_se����I����Ă����ʐ��ڂ���B
			while (CheckSoundMem(okSe)) {}
			StartJoypadVibration(DX_INPUT_PAD1, OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
			windowMode = !windowMode;
			ChangeWindowMode(windowMode);
		}
		else if (static_cast<MENU>(selectMenu) == MENU::BGM) {
			PlaySoundMem(muteSe, DX_PLAYTYPE_BACK, TRUE);
			while (CheckSoundMem(muteSe)) {}
			StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
			if (bgmVolume > 2) { oldBgmVolume = bgmVolume; bgmVolume = 2; }
			else { bgmVolume = oldBgmVolume; }
		}
		else if (static_cast<MENU>(selectMenu) == MENU::SE) {
			PlaySoundMem(muteSe, DX_PLAYTYPE_BACK, TRUE);
			//mute_se����I����Ă���~���[�g�ɂ���B
			while (CheckSoundMem(muteSe)) {}
			StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
			if (seVolume > 2) { oldSeVolume = seVolume; seVolume = 2; }
			else { seVolume = oldSeVolume; }
		}
		else if (static_cast<MENU>(selectMenu) == MENU::RETURN) {
			PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
			//ok_se����I����Ă����ʐ��ڂ���B
			while (CheckSoundMem(okSe)) {}
			StartJoypadVibration(DX_INPUT_PAD1,  OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
			selectMenu = 0;
			ChangeOptionFlg();
		}
	}


	//���͕����̐؂�ւ�
	if (PAD_INPUT::OnButton(XINPUT_BUTTON_BACK)) {
		PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
		//ok_se����I����Ă����ʐ��ڂ���B
		while (CheckSoundMem(okSe)) {}
		StartJoypadVibration(DX_INPUT_PAD1,  OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
		inputMode = !inputMode;
	}

	//�߂�(�߂郁�j���[�ɃJ�[�\�������킹�Ȃ��Ă�)
	if ((PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_A : XINPUT_BUTTON_B))) {
		PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
		//ok_se����I����Ă����ʐ��ڂ���B
		while (CheckSoundMem(okSe)) {}
		StartJoypadVibration(DX_INPUT_PAD1,  OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
		selectMenu = 0;
		ChangeOptionFlg();
	}

	ChangeVolumeSoundMem(GetSEVolume() * 1.6, cursorMoveSe);
	ChangeVolumeSoundMem(GetSEVolume(), muteSe);
	ChangeVolumeSoundMem(GetSEVolume(), okSe);
}


void Option::Draw() {

	//�I�����j���[

	//�E�B���h�E���[�h�ؑ�
	if (windowMode) {
		DrawStringToHandle(GetDrawCenterX("�S��ʕ\��", menuFont), 120, "�S��ʕ\��", static_cast<MENU>(selectMenu) == MENU::WindowMode ? 0x4572D9 : 0xEB8F63, menuFont, 0xFFFFFF);
	}else{
		DrawStringToHandle(GetDrawCenterX("�E�B���h�E�\��", menuFont), 120, "�E�B���h�E�\��", static_cast<MENU>(selectMenu) == MENU::WindowMode ? 0x4572D9 : 0xEB8F63, menuFont, 0xFFFFFF);
	}


	//BGM
	DrawStringToHandle(GetDrawCenterX("BGM", menuFont), 250, "BGM", static_cast<MENU>(selectMenu) == MENU::BGM ? 0x5FEBB6 : 0xEB8F63, menuFont, 0xFFFFFF);

	const int bgm_x = 640;
	const int bgm_y = 350;

	DrawOvalAA(bgm_x, bgm_y, 180, 10, 30, 0x000000, FALSE, 2.0F);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
	DrawOvalAA(bgm_x, bgm_y, 180, 10, 30, 0xFFFFFF, TRUE, 0.0F);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawOvalAA(bgm_x, bgm_y, 180 * bgmVolume / 255, 10 * bgmVolume / 255, 30, 0xFFEB91, TRUE, 0.0F);

	DrawFormatString(bgm_x, bgm_y - 6, 0x000000, "%d", ((110 * bgmVolume / 255) - 1) / 10);


	//SE
	DrawStringToHandle(GetDrawCenterX("SE", menuFont), 380, "SE", static_cast<MENU>(selectMenu) == MENU::SE ? 0xF5E6B3 : 0xEB8F63, menuFont, 0xFFFFFF);

	const int se_x = 640;
	const int se_y = 480;

	DrawOvalAA(se_x, se_y, 180, 10, 30, 0x000000, FALSE, 2.0F);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
	DrawOvalAA(se_x, se_y, 180, 10, 30, 0xFFFFFF, TRUE, 0.0F);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawOvalAA(se_x, se_y, 180 * seVolume / 255, 10 * seVolume / 255, 30, 0x11A7ED, TRUE, 0.0F);

	DrawFormatString(se_x, se_y - 6, 0x000000, "%d", ((110 * seVolume / 255) - 1) / 10);

	DrawStringToHandle(GetDrawCenterX("�߂�", menuFont), 540, "�߂�", static_cast<MENU>(selectMenu) == MENU::RETURN ? 0xEBABDC : 0xEB8F63, menuFont, 0xFFFFFF);

	//���͕����̐؂�ւ�

	//BACK�{�^��
	const int start_x = 220;
	const int start_y = 300;

	DrawBoxAA(start_x, start_y, start_x + 70, start_y + 30, 0xFFFFFF, TRUE, 1.0F);
	DrawCircleAA(start_x + 5, start_y + 14.6, 15, 20, 0xFFFFFF, TRUE, 1.0F);	//���[
	DrawCircleAA(start_x + 65, start_y + 14.6, 15, 20, 0xFFFFFF, TRUE, 1.0F);	//�E�[
	DrawStringToHandle(start_x + 2, start_y + 3, "BACK", BACK_COLOR, buttonGuidFont, 0xFFFFFF);

	DrawStringToHandle(100, 300 + 3, "���͕���", 0xEB8F63, buttonGuidFont, 0xFFFFFF);

	{//�{�^���̓�����e
		const int x = 110;

		DrawCircleAA(x + 7, 362, 15, 20, 0xFFFFFF, 1);
		DrawStringToHandle(x, 350, "A", A_COLOR, buttonGuidFont, 0xFFFFFF);
		DrawStringToHandle(x + 30, 350, Option::GetInputMode() ? "�߂�^�W�����v" : "����^�A�N�V����", B_COLOR, buttonGuidFont, 0xFFFFFF);
		DrawCircleAA(x + 7, 402, 15, 20, 0xFFFFFF, 1);
		DrawStringToHandle(x, 390, "B", B_COLOR, buttonGuidFont, 0xFFFFFF);
		DrawStringToHandle(x + 30, 390, Option::GetInputMode() ? "����^�A�N�V����" : "�߂�^�W�����v", B_COLOR, buttonGuidFont, 0xFFFFFF);
	}



	const int guid_center_x = 640;

	//�K�C�h�\��

	const int back_guid_x = 220;
	const int back_guid_y = 665;

	DrawBoxAA(back_guid_x, back_guid_y, back_guid_x + 70, back_guid_y + 30, 0xFFFFFF, TRUE, 1.0F);
	DrawCircleAA(back_guid_x + 5, back_guid_y + 14.6, 15, 20, 0xFFFFFF, TRUE, 1.0F);	//���[
	DrawCircleAA(back_guid_x + 65, back_guid_y + 14.6, 15, 20, 0xFFFFFF, TRUE, 1.0F);	//�E�[
	DrawStringToHandle(back_guid_x + 2, back_guid_y + 3, "BACK", BACK_COLOR, buttonGuidFont, 0xFFFFFF);
	DrawStringToHandle(back_guid_x + 85, 668, "���͕����ؑ�", 0xFFFFFF, buttonGuidFont, 0x000000);

	const int mute_guid_x = 560;
	DrawStringToHandle(mute_guid_x, 668, "�~���[�g�^�~���[�g����", 0xFFFFFF, buttonGuidFont, 0x000000);
	DrawCircleAA(mute_guid_x - 20, 680, 15, 20, 0xFFFFFF, 1);
	DrawStringToHandle(mute_guid_x - 27, 668, Option::GetInputMode() ? "B" : "A", Option::GetInputMode() ? B_COLOR : A_COLOR, buttonGuidFont, 0xFFFFFF);

	const int return_center_x = 940;
	DrawStringToHandle(return_center_x, 668, "�߂�", 0xFFFFFF, buttonGuidFont, 0x000000);
	DrawCircleAA(return_center_x - 20, 680, 15, 20, 0xFFFFFF, 1);
	DrawStringToHandle(return_center_x - 27, 668, Option::GetInputMode() ? "A" : "B", Option::GetInputMode() ? A_COLOR : B_COLOR, buttonGuidFont, 0xFFFFFF);
}



int Option::GetDrawCenterX(const char* string, int font_handle)const {

	//��ʕ�
	const int screenX = 1280;

	const int w = screenX / 2 - GetDrawFormatStringWidthToHandle(font_handle, string) / 2;
	return w;
}


void Option::LoadData(void) {

	std::string line;
	std::ifstream config_file("Resource/Option.config");
	if (config_file.is_open()) {
		while (getline(config_file, line)) {

			std::istringstream line_stream(line);
			std::string key;
			if (std::getline(line_stream, key, ':')) {

				int value;

				//�󔒂���������������ɏ���������
				key.erase(std::remove_if(key.begin(), key.end(), iswspace), key.end());

				if (key == "BGM") {
					line_stream >> value;
					if (value > 10 || value < 0) { continue; }
					bgmVolume = value * 25 +2;
				}
				else if (key == "SE") {
					line_stream >> value;
					if (value > 10 || value < 0) { continue; }
					seVolume = value * 25 + 2;
				}
				else if (key == "INPUT_MODE") {
					line_stream >> value;
					if (value != 0 && value != 1) { continue; }
					inputMode = value;
				}

				else if (key == "WINDOW_MODE") {
					line_stream >> value;
					if (value != 0 && value != 1) { continue; }
					windowMode = value;
				}
			}
		}
		config_file.close();
	}
}


void Option::SaveData(void) {

	std::ofstream config_file("Resource/Option.config");

	int bgm_buf = ((110 * bgmVolume / 255) - 1) / 10;
	int se_buf = ((110 * seVolume / 255) - 1) / 10;

	if (config_file.is_open()) {
		config_file << u8"���ʒ���(0 �` 10)" << std::endl;
		config_file << "BGM : " << bgm_buf << std::endl;
		config_file << " SE : " << se_buf << std::endl;
		config_file << u8"\n0[A:���� B:�߂�], 1[A:�߂� B:����]" << std::endl;
		config_file << "INPUT_MODE : " << inputMode << std::endl;
		config_file << u8"\n0[�S��ʕ\��], 1[�E�B���h�E�\��]" << std::endl;
		config_file << "WINDOW_MODE : " << windowMode << std::endl;
		config_file.close();
	}
}