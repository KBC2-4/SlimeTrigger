#include "GameOver.h"
#include "GameMain.h"
#include "StageSelect.h"
#include "DxLib.h"

GameOver::GameOver(const char* stageName)
{

	if ((title_image = (LoadGraph("Resource/Images/Result/GameOver_title.png"))) == -1)
	{
		throw "Resource/Images/Result/GameOver_title.png";
	}

	if ((backgraundImage = (LoadGraph("Resource/Images/Result/GameOvar_background.png"))) == -1)
	{
		throw "Resource/Images/Result/GameOvar_background.png";
	}

	if ((backgroundMusic = LoadSoundMem("Resource/Sounds/BGM/gameover02.wav")) == -1) {
		throw "Resource/Sounds/BGM/gameover02.wav";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}

	if ((cursormoveSe = LoadSoundMem("Resource/Sounds/SE/cursor_move.wav")) == -1)
	{
		throw "Resource/Sounds/SE/cursor_move.wav";
	}

	se_randnum = GetRand(3);
	char dis_bad_se[30];
	sprintf_s(dis_bad_se, sizeof(dis_bad_se), "Resource/Sounds/SE/bad%d.wav", se_randnum + 1);

	if ((bad_se[se_randnum] = LoadSoundMem(dis_bad_se)) == -1) {
		throw dis_bad_se;
	}


	menu_font = CreateFontToHandle("UD デジタル 教科書体 N-B", 90, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	guid_font = CreateFontToHandle("UD デジタル 教科書体 N-B", 40, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);


	inputMargin= 0;
	selectmenu = 0;

	timer=0;

	this->stageName = stageName;

	ChangeVolumeSoundMem(Option::GetBGMVolume(), backgroundMusic);

	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.3, bad_se[se_randnum]);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.6, cursormoveSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);

	PlaySoundMem(backgroundMusic, DX_PLAYTYPE_BACK, FALSE);
	PlaySoundMem(bad_se[se_randnum], DX_PLAYTYPE_BACK, FALSE);
}

GameOver::~GameOver()
{

	DeleteGraph(title_image);
	DeleteGraph(backgraundImage);
	DeleteFontToHandle(menu_font);
	DeleteFontToHandle(guid_font);
	DeleteSoundMem(backgroundMusic);
	DeleteSoundMem(okSe);
	DeleteSoundMem(cursormoveSe);
	DeleteSoundMem(bad_se[se_randnum]);
}

AbstractScene* GameOver::Update()
{

	//WaitTimeを加算
	if (inputMargin < 20) {
		++inputMargin;
	}
	else {

		/*上入力かつWaitTimeが20より大きい時cursorを上に、
		最上の場合は下へ*/
		if (PAD_INPUT::GetPadThumbLY() > 20000)
		{
			PlaySoundMem(cursormoveSe, DX_PLAYTYPE_BACK, TRUE);
			StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
			selectmenu = (selectmenu + 1) % 2;
			inputMargin = 0;
		}

		/*下入力かつWaitTimeが20より大きい時cursorを下に、
		最上の場合は上へ*/
		if (PAD_INPUT::GetPadThumbLY() < -20000)
		{
			PlaySoundMem(cursormoveSe, DX_PLAYTYPE_BACK, TRUE);
			StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
			selectmenu = (selectmenu + 1) % 2;
			inputMargin = 0;
		}

	}


	if ((PAD_INPUT::GetNowKey() == (Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) && (PAD_INPUT::GetPadState() == PAD_STATE::ON))
	{
		PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
		//ok_seが鳴り終わってから画面推移する。
		while (CheckSoundMem(okSe)) {}
		StartJoypadVibration(DX_INPUT_PAD1,  OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);

		switch (static_cast<GAMEOVER_MENU>(selectmenu))
		{

		case GAMEOVER_MENU::NewGame:
			return new GAMEMAIN(false, 0, stageName);
			break;

		case  GAMEOVER_MENU::ReSelect:
			return new STAGE_SELECT();
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

	DrawGraph(0, 0, backgraundImage, FALSE);
	DrawGraph(185, 100, title_image, TRUE);

	//Select用String
	DrawStringToHandle(GetDrawCenterX("ステージ選択画面へ",menu_font), 480, "ステージ選択画面へ", selectmenu == 0 ? 0x0a6500 : 0x1aff00, menu_font, 0x000000);
	DrawStringToHandle(GetDrawCenterX("リスタート",menu_font), 360, "リスタート", selectmenu == 1 ? 0x0a6500 : 0x1aff00, menu_font, 0x000000);

	if (timer % 120 < 60)
	{

		//Bボタンを押すことを促す(表示非表示を切り替え)
		DrawCircleAA(580.5f, 627.5f, 20, 20, 0x000000, 1);
		DrawStringToHandle(572, 610,  Option::GetInputMode() ? "B" : "A", Option::GetInputMode() ? B_COLOR : A_COLOR, guid_font, 0x000000);
		DrawStringToHandle(600, 610,  "で決定", 0x95ff89, guid_font, 0x000000);
	}
}