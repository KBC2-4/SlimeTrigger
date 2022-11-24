#pragma once
#include "AbstractScene.h"

class RESULT : public AbstractScene
{
private:
	int Result_Image;
	int count_se, ok_se,good_se[4],bad_se[4];
	int title_font, menu_font,time_font;
	int timer, guide_timer;
	bool win;
	int clear_time;
	int se_randnum;
	int effect_timer[2];

public:
	RESULT(bool issue , int clear_time = -1);
	~RESULT();
	AbstractScene* Update() override;
	void Draw() const override;
};

