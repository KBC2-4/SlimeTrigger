#include "Result.h"
#include "GameMain.h"
#include "DxLib.h"
#include "StageSelect.h"
#include "Option.h"
#include "Ranking.h"

RESULT::RESULT(bool issue, int clear_time, const char* stageName)
{

	menuFont = CreateFontToHandle("UD デジタル 教科書体 N-B", 90, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	guidFont = CreateFontToHandle("UD デジタル 教科書体 N-B", 70, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	button_guid_font = CreateFontToHandle("UD デジタル 教科書体 N-B", 40, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	timeFont = LoadFontDataToHandle("Resource/Fonts/TimeAttack.dft", 2);

	if ((titleImage = LoadGraph("Resource/Images/Result/GameClear_title.png")) == -1) {
		throw "Resource/Images/Result/GameClear_title.png";
	}

	if ((backgroundImage = LoadGraph("Resource/Images/Result/GameClear_background.png")) == -1) {
		throw "Resource/Images/Result/GameClear_background.png";
	}
	int bgm_randum = GetRand(1);
	char dis_good_bgm[36];
	sprintf_s(dis_good_bgm, sizeof(dis_good_bgm), "Resource/Sounds/BGM/gameclear0%d.wav", bgm_randum + 1);

	if ((backgroundMusic = LoadSoundMem(dis_good_bgm)) == -1) {
		throw dis_good_bgm;
	}

	if ((countSe = LoadSoundMem("Resource/Sounds/SE/321.wav")) == -1) {
		throw "Resource/Sounds/SE/321.wav";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}


	serandNum = GetRand(3);

	char dis_good_se[30];
	sprintf_s(dis_good_se, sizeof(dis_good_se), "Resource/Sounds/SE/good%d.wav", serandNum + 1);

	if ((good_se[serandNum] = LoadSoundMem(dis_good_se)) == -1) {
		throw dis_good_se;
	}



	if (issue == true) { timer = 10 * 60; }
	else { timer = 8 * 60; }

	win = issue;

	this->clear_time = clear_time;

	*effect_timer = 0;
	string_effect_timer = 0.0f;
	guide_timer = 0;

	ChangeVolumeSoundMem(Option::GetBGMVolume(), backgroundMusic);

	//SE
	ChangeVolumeSoundMem(Option::GetSEVolume(), countSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.5, good_se[serandNum]);

	PlaySoundMem(backgroundMusic, DX_PLAYTYPE_BACK, FALSE);
	PlaySoundMem(good_se[serandNum], DX_PLAYTYPE_BACK, FALSE);

	high_score = false;

	//ランキング登録
	if (stageName == "Stage01")
	{
		if (clear_time < RANKING::GetBestTime(0)) { high_score = true; }
		RANKING::Insert(this->clear_time, 1);
	}
	else if (stageName == "Stage02")
	{
		if (clear_time < RANKING::GetBestTime(1)) { high_score = true; }
		RANKING::Insert(this->clear_time, 2);
	}
	else if (stageName == "Stage03")
	{
		if (clear_time < RANKING::GetBestTime(2)) { high_score = true; }
		RANKING::Insert(this->clear_time, 3);
	}
	else {}
}

RESULT::~RESULT()
{

	DeleteFontToHandle(menuFont);
	DeleteFontToHandle(guidFont);
	DeleteFontToHandle(button_guid_font);
	DeleteFontToHandle(timeFont);
	DeleteGraph(titleImage);
	DeleteSoundMem(backgroundMusic);
	DeleteSoundMem(countSe);
	DeleteSoundMem(okSe);
	DeleteSoundMem(good_se[serandNum]);
}

AbstractScene* RESULT::Update()
{
	if (timer <= 5 * 60) { if (CheckSoundMem(countSe) == FALSE)PlaySoundMem(countSe, DX_PLAYTYPE_BACK, FALSE); }

	if (timer <= 60) { return new STAGE_SELECT(); }
	else { --timer; }

	//ガイド点滅表示
	if (guide_timer < 100)
	{

		guide_timer++;
	}
	else
	{

		guide_timer = 0;
	}

	if (PAD_INPUT::GetNowKey() == (Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A) && PAD_INPUT::GetPadState() == PAD_STATE::ON)
	{
		PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
		//ok_seが鳴り終わってから画面推移する。
		while (CheckSoundMem(okSe)) {}
		StartJoypadVibration(DX_INPUT_PAD1, OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
		return new STAGE_SELECT();
	}

	if (effect_timer[0] < 180) { effect_timer[0]++; }
	if (string_effect_timer < 10.0f) { string_effect_timer += 0.1f; }

	return this;
}


void RESULT::Draw() const {

	if (win == true)
	{

		DrawFillBox(0, 0, 1280, 720, 0x000000);
		//DrawExtendGraph(0, 0, 1280, 720, titleImage, true);
		DrawGraph(0, 0, backgroundImage, FALSE);
		DrawGraph(180, 90, titleImage, TRUE);

		char dis_clear_time[20];	//文字列合成バッファー

		//文字列合成
		if (clear_time / 1000 >= 60)
		{

			sprintf_s(dis_clear_time, sizeof(dis_clear_time), "%4d:%02d.%.3d",
				(clear_time / 1000) / 60, (clear_time / 1000) % 60, clear_time % 1000);
		}
		else
		{

			sprintf_s(dis_clear_time, sizeof(dis_clear_time),
				"%5d.%.3d", clear_time / 1000, clear_time % 1000);
		}

		//クリアタイム
		DrawStringToHandle(GetDrawCenterX("クリアタイム",menuFont), 300, "クリアタイム", 0x1aff00, menuFont, 0x000000);

		DrawStringToHandle(GetDrawCenterX(dis_clear_time, timeFont,-20), 410, dis_clear_time, 0x1aff00, timeFont, 0xFFFFFF);

		DrawFormatStringToHandle(GetDrawCenterX("%2d秒後にリスタートします",guidFont,120), 540, 0x56F590, guidFont, "%2d秒後にリスタートします", timer / 60);


		if (high_score == true) {
			const int x = 80;
			const int y = 260;
			const float size = 1.0f + string_effect_timer;
			/*DrawOvalAA(x, y, 100, 80, 30, 0x000000, FALSE, 1.0F);
			DrawOvalAA(x, y, 99, 79, 30, 0xFFFFFF, TRUE, 0.0F);*/
			DrawStringToHandle(x, y, "新記録", 0xFAF300, menuFont, 0xFFFFFF);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 75 + effect_timer[0]);
			//DrawExtendStringToHandle(x, y, size, size, "新記録", 0xFAF300, menuFont, 0xFFFFFF);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	if (guide_timer < 50)
	{

		DrawCircleAA(530.0f, 668.0f, 20, 20, 0xFFFFFF, 1);
		DrawStringToHandle(520, 650, Option::GetInputMode() ? "B" : "A", Option::GetInputMode() ? B_COLOR : A_COLOR, button_guid_font, 0xFFFFFF);
		DrawStringToHandle(560, 650, "でスキップ", 0x76F567, button_guid_font, 0xFFFFFF);
	}
}