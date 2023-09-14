#include "GameOver.h"
#include "GameMain.h"
#include "StageSelect.h"
#include "DxLib.h"

GameOver::GameOver(const char* stageName)
{

	if ((titleImage = (LoadGraph("Resource/Images/Result/GameOver_title.png"))) == -1)
	{
		throw "Resource/Images/Result/GameOver_title.png";
	}

	if ((backGraundImage = (LoadGraph("Resource/Images/Result/GameOvar_background.png"))) == -1)
	{
		throw "Resource/Images/Result/GameOvar_background.png";
	}

	if ((backGraundMusic = LoadSoundMem("Resource/Sounds/BGM/gameover02.wav")) == -1) {
		throw "Resource/Sounds/BGM/gameover02.wav";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}

	if ((cursorMoveSe = LoadSoundMem("Resource/Sounds/SE/cursor_move.wav")) == -1)
	{
		throw "Resource/Sounds/SE/cursor_move.wav";
	}

	serandNum = GetRand(3);
	char dis_bad_se[30];
	sprintf_s(dis_bad_se, sizeof(dis_bad_se), "Resource/Sounds/SE/bad%d.wav", serandNum + 1);

	if ((badSe[serandNum] = LoadSoundMem(dis_bad_se)) == -1) {
		throw dis_bad_se;
	}


	menuFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 90, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	guidFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 40, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);


	inputMargin= 0;
	selectMenu = 0;

	timer=0;

	this->stageName = stageName;

	ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic);

	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.3, badSe[serandNum]);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.6, cursorMoveSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);

	PlaySoundMem(backGraundMusic, DX_PLAYTYPE_BACK, FALSE);
	PlaySoundMem(badSe[serandNum], DX_PLAYTYPE_BACK, FALSE);
}

GameOver::~GameOver()
{

	DeleteGraph(titleImage);
	DeleteGraph(backGraundImage);
	DeleteFontToHandle(menuFont);
	DeleteFontToHandle(guidFont);
	DeleteSoundMem(backGraundMusic);
	DeleteSoundMem(okSe);
	DeleteSoundMem(cursorMoveSe);
	DeleteSoundMem(badSe[serandNum]);
}

AbstractScene* GameOver::Update()
{

	//WaitTime�����Z
	if (inputMargin < 20) {
		++inputMargin;
	}
	else {

		/*����͂���WaitTime��20���傫����cursor����ɁA
		�ŏ�̏ꍇ�͉���*/
		if ((PAD_INPUT::GetPadThumbLY() > 20000) || PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_UP))
		{
			PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
			StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
			selectMenu = (selectMenu + 1) % 2;
			inputMargin = 0;
		}

		/*�����͂���WaitTime��20���傫����cursor�����ɁA
		�ŏ�̏ꍇ�͏��*/
		if ((PAD_INPUT::GetPadThumbLY() < -20000) || PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_DOWN))
		{
			PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
			StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
			selectMenu = (selectMenu + 1) % 2;
			inputMargin = 0;
		}

	}


	if ((PAD_INPUT::OnPressed(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)))
	{
		PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
		//ok_se����I����Ă����ʐ��ڂ���B
		while (CheckSoundMem(okSe)) {}
		StartJoypadVibration(DX_INPUT_PAD1,  OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);

		//�O��̃X�e�[�W�ԍ�
		short lastStageNum = 0;

		switch (static_cast<GAMEOVER_MENU>(selectMenu))
		{

		case GAMEOVER_MENU::NewGame:
			return new GAMEMAIN(false, 0, stageName);
			break;

		case  GAMEOVER_MENU::ReSelect:
			if (stageName != "StageSelect")
			{
				lastStageNum = stageName[6] - '0';
			}

			return new STAGE_SELECT(lastStageNum);
			break;

		default:
			break;
		}
	}
	timer++;

	return this;
}

void GameOver::Draw() const
{

	DrawGraph(0, 0, backGraundImage, FALSE);
	DrawGraph(185, 100, titleImage, TRUE);

	//Select�pString
	DrawStringToHandle(GetDrawCenterX("�X�e�[�W�I����ʂ�",menuFont), 480, "�X�e�[�W�I����ʂ�", selectMenu == 0 ? 0x0a6500 : 0x1aff00, menuFont, 0x000000);
	DrawStringToHandle(GetDrawCenterX("���X�^�[�g",menuFont), 360, "���X�^�[�g", selectMenu == 1 ? 0x0a6500 : 0x1aff00, menuFont, 0x000000);

	if (timer % 120 < 60)
	{

		//B�{�^�����������Ƃ𑣂�(�\����\����؂�ւ�)
		DrawCircleAA(580.5f, 627.5f, 20, 20, 0x000000, 1);
		DrawStringToHandle(572, 610,  Option::GetInputMode() ? "B" : "A", Option::GetInputMode() ? B_COLOR : A_COLOR, guidFont, 0x000000);
		DrawStringToHandle(600, 610,  "�Ō���", 0x95ff89, guidFont, 0x000000);
	}
}