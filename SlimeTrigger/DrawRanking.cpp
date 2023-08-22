#include "DrawRanking.h"
#include "Ranking.h"
#include "PadInput.h"
#include "Title.h"

DRAW_RANKING::DRAW_RANKING()
{
	waitTime = 0;
	RANKING::ReadRanking();
	for (int i = 0; i < 3; i++)
	{
		bestTime[i] = RANKING::GetBestTime(i);
	}

	title_font = CreateFontToHandle("メイリオ", 60, 1, DX_FONTTYPE_ANTIALIASING_8X8);
	timeFont = LoadFontDataToHandle("Resource/Fonts/TimeAttack_BestTime.dft", 0);

	if ((image = (LoadGraph("Resource/Images/Result/Best_time_Image.png"))) == -1)
	{
		throw "Resource/Images/Result/Best_time_Image.png";
	}

	if ((backgraundImage = LoadGraph("Resource/Images/Stage/BackImage1.png")) == -1)
	{
		throw "Resource/Images/Stage/BackImage1.png";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1) {
		throw "Resource/Sounds/SE/ok.wav";
	}

	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);
}

DRAW_RANKING::~DRAW_RANKING() {

	DeleteFontToHandle(title_font);
	DeleteFontToHandle(timeFont);
	DeleteGraph(image);
	DeleteSoundMem(okSe);

}

AbstractScene* DRAW_RANKING::Update()
{

	if (waitTime < 120) {
		++waitTime;
	}
	else { waitTime = 0; }

	if ((PAD_INPUT::GetNowKey() == (Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) && (PAD_INPUT::GetPadState() == PAD_STATE::ON))
	{
		PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
		//ok_seが鳴り終わってから画面推移する。
		while (CheckSoundMem(okSe)) {}
		StartJoypadVibration(DX_INPUT_PAD1,  OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
		return new Title();
	}

	return this;
}

void DRAW_RANKING::Draw() const
{
	DrawGraph(0, 0, backgraundImage, false);

	DrawRotaGraph(640, 200, 1, 0, image, TRUE);


	for (int i = 0; i < 3; i++)
	{
		DrawFormatStringToHandle(330, 320 + (75 * i), 0xFF8C00, title_font,  "%dステージ :", i+1);
		if (bestTime[i] != -1)
		{
			if (bestTime[i] / 1000 >= 60)
			{
				DrawFormatStringToHandle(660, 325 + (75 * i), 0xFF8C00, timeFont, "%4d:%02d.%.3d", (bestTime[i] / 1000) / 60, (bestTime[i] / 1000) % 60, bestTime[i] % 1000);
			}
			else 
			{
				DrawFormatStringToHandle(715, 325 + (75 * i), 0xFF8C00, timeFont, "%4d.%.3d", bestTime[i] / 1000, bestTime[i] % 1000);
			}
		}
		
	}
	if (waitTime % 120 < 60)
	{
		DrawCircleAA(393.0f, 630.0f, 30, 28, 0xFFFFFF, 1);
		DrawStringToHandle(375, 602, Option::GetInputMode() ? "B" : "A", Option::GetInputMode() ? B_COLOR : A_COLOR, title_font);
		DrawStringToHandle(430, 600, "でタイトルに戻る", 0xFFFFFF,title_font);
	}
	SetFontSize(-1);
}