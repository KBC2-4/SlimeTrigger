#include "Title.h"
#include "GameMain.h"
#include "StageSelect.h"
#include "DrawRanking.h"
#include "DxLib.h"
#include "Option.h"

#define _USE_MATH_DEFINES
#include <math.h>

//�^�C�g���A�j���[�V����������N�����̂ݗL�������邽�߂̃t���O
static bool animation_flg = false;


Title::Title()
{

	if ((backGraundImage = LoadGraph("Resource/Images/Stage/TitleBackImage.png")) == -1)
	{
		throw "Resource/Images/Stage/TitleBackImage.png";
	}

	if ((backGraundMusic = LoadSoundMem("Resource/Sounds/BGM/title.wav")) == -1) {
		throw "Resource/Sounds/BGM/title.wav";
	}

	if ((cursorMoveSe = LoadSoundMem("Resource/Sounds/SE/cursor_move.wav")) == -1)
	{
		throw "Resource/Sounds/SE/cursor_move.wav";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1)
	{
		throw "Resource/Sounds/SE/ok.wav";
	}

	int se_random = GetRand(1);
	char dis_exit_se[30];
	sprintf_s(dis_exit_se, sizeof(dis_exit_se), "Resource/Sounds/SE/exit0%d.wav", se_random + 1);

	if ((exitSe = LoadSoundMem(dis_exit_se)) == -1) {
		throw dis_exit_se;
	}

	titleFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 120, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 8);
	menuFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 80, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	guidFont = CreateFontToHandle("���C���I", 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	selectMenu = 0;
	inputMargin = 0;
	timer = 0;
	exitFlag = false;

	titleAnitimer[0] = 0;

	//�^�C�g���A�j���[�V����������N�����̂ݗL����
	if (animation_flg == false) {
		titleAnitimer[1] = 180;
		animation_flg = true;
	}
	else { titleAnitimer[1] = 0; }

	option = new Option();

	

	//BGM
	ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic);

	//SE
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.6, cursorMoveSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.3, exitSe);
}

Title::~Title()
{
	delete option;

	DeleteGraph(backGraundImage);
	StopSoundMem(backGraundMusic);
	DeleteSoundMem(backGraundMusic);
	DeleteSoundMem(cursorMoveSe);
	DeleteSoundMem(okSe);
	DeleteSoundMem(exitSe);
	DeleteFontToHandle(titleFont);
	DeleteFontToHandle(menuFont);
	DeleteFontToHandle(guidFont);
}

AbstractScene* Title::Update()
{
	if (option->GetOptionFlg() == true) {
		option->Update();
		//BGM
		ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic);

		//SE
		ChangeVolumeSoundMem(Option::GetSEVolume() * 1.6, cursorMoveSe);
		ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);
		ChangeVolumeSoundMem(Option::GetSEVolume() * 1.3, exitSe);
	}
	else {

		if (inputMargin < 20) {
			inputMargin++;
		}
		else {

			//�^�C�g���A�j���[�V�������I����Ă���BGM���Đ�����
			if (titleAnitimer[1] == 0 && !CheckSoundMem(backGraundMusic)) {
				PlaySoundMem(backGraundMusic, DX_PLAYTYPE_LOOP);
			}


			if (titleAnitimer[1] <= 0) {
				if (PAD_INPUT::GetPadThumbLY() > 20000)
				{

					selectMenu = (selectMenu + 3) % 4;
					inputMargin = 0; PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
					StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
				}

				if (PAD_INPUT::GetPadThumbLY() < -20000)
				{

					selectMenu = (selectMenu + 1) % 4; inputMargin = 0;
					PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
					StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
				}
			}
		}

		if ((PAD_INPUT::GetNowKey() == (Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) && (PAD_INPUT::GetPadState() == PAD_STATE::ON))
		{

			if (titleAnitimer[1] <= 0) {
				PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
				//ok_se����I����Ă����ʐ��ڂ���B
				while (CheckSoundMem(okSe)) {}
				StartJoypadVibration(DX_INPUT_PAD1,  OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);

				switch (static_cast<MENU>(selectMenu))
				{

				case MENU::GAME_SELECT:
					return new STAGE_SELECT();
					break;

				case MENU::RANKING:
					return new DRAW_RANKING();
					break;

				case MENU::OPTION:
					option->ChangeOptionFlg();
					break;

				case MENU::END:
					exitFlag = true;
					PlaySoundMem(exitSe, DX_PLAYTYPE_BACK, FALSE);
					break;

				default:
					break;
				}

			}
			else { titleAnitimer[1] = 0; }
		}
		timer++;

		if (exitFlag == true && !CheckSoundMem(exitSe)) { return nullptr; }

		//���v�t���[��
		if (titleAnitimer[1] > 0) { titleAnitimer[1]--; }

		//��]
		if (titleAnitimer[0] < 180 && titleAnitimer[1] > 0) { titleAnitimer[0]++; }
		else { titleAnitimer[0] = 0; }
	}

	return this;
}

void Title::Draw()const
{

	DrawGraph(0, 0, backGraundImage, false);

	//�I�v�V������ʂ֓���
	if (option->GetOptionFlg() == true) {
		option->Draw();
	}
	else {

		DrawRotaStringToHandle(GetDrawCenterX("�X���C���g���K�[", titleFont, 600 - titleAnitimer[1] * 3), 200 + titleAnitimer[1] * 3, 1.0 - titleAnitimer[1] * 0.01, 1.0 - titleAnitimer[1] * 0.01, 600, 100, 10 * titleAnitimer[0] * (M_PI / 180), 0x56F590, titleFont, 0xFFFFFF, FALSE, "�X���C���g���K�[");
		//DrawStringToHandle(GetDrawCenterX("�X���C���A�N�V����",titleFont), 100, "�X���C���A�N�V����", 0x56F590, titleFont, 0xFFFFFF);

		//�{�b�N�X
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA,100);
		//DrawBoxAA(400.0f, 300.0f, 800.0f, 680.0f, 0xF3F589, TRUE, 5.0f);
		//SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL,0);

		//���
		//DrawCircleAA(475.0f, 398.0f + selectMenu * 90, 20, 3, 0xffffff, TRUE, 3.0f);

		if (titleAnitimer[1] > 0) { return; }

		//�I�����j���[
		DrawStringToHandle(GetDrawCenterX("�v���C", menuFont), 360, "�v���C", selectMenu == 0 ? 0xB3E0F5 : 0xEB8F63, menuFont, 0xFFFFFF);
		DrawStringToHandle(GetDrawCenterX("�����L���O", menuFont), 450, "�����L���O", selectMenu == 1 ? 0xF5E6B3 : 0xEB8F63, menuFont, 0xFFFFFF);
		DrawStringToHandle(GetDrawCenterX("�I�v�V����", menuFont), 540, "�I�v�V����", selectMenu == 2 ? 0x5FEBB6 : 0xEB8F63, menuFont, 0xFFFFFF);
		DrawStringToHandle(GetDrawCenterX("�I��", menuFont, 8), 630, "�I��", selectMenu == 3 ? 0xEBABDC : 0xEB8F63, menuFont, 0xFFFFFF);



		//����ē�
		if (timer % 120 < 60)
		{

			if (GetJoypadNum() == 0) {
				DrawStringToHandle(GetDrawCenterX("�R���g���[���[��ڑ����Ă�������", guidFont), 280, "�R���g���[���[��ڑ����Ă�������", 0xFF5446, guidFont, 0xF53E27);
			}
			else {

				DrawCircleAA(530.0f, 311.0f, 30, 20, 0xFFFFFF, 1);

				DrawStringToHandle(510, 283, Option::GetInputMode() ? "B" : "A", Option::GetInputMode() ? B_COLOR : A_COLOR, guidFont, 0xFFFFFF);
				DrawStringToHandle(570, 280, "�Ō���", 0xEBA05E, guidFont, 0xFFFFFF);
			}
		}
	}
}