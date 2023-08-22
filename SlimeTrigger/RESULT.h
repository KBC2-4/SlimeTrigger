#pragma once
#include "AbstractScene.h"

class RESULT : public AbstractScene
{
private:
	int timeFont;
	int title_image, backgroundImage;
	int backgroundMusic;
	int countSe, okSe, good_se[4];
	int menu_font, guid_font, button_guid_font;
	int timer, guide_timer;
	bool win;
	int clear_time;
	int se_randnum;
	int effect_timer[2];
	float string_effect_timer;
	bool high_score;

public:
	RESULT(bool issue, int clear_time = -1, const char* stageName = "StageSelect");
	~RESULT();
	AbstractScene* Update() override;
	void Draw() const override;
};

