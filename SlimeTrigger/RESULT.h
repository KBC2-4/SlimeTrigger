#pragma once
#include "AbstractScene.h"

class RESULT : public AbstractScene
{
private:
	int timeFont;
	int titleImage, backGroundImage;
	int backGraundMusic;
	int countSe, okSe, goodSe[4];
	int menuFont, guidFont, buttonGuidFont;
	int timer, guideTimer;
	bool win;
	int clearTime;
	int serandNum;
	int effectTimer[2];
	float stringEffectTimer;
	bool highScore;
	
	//前回のステージ番号 0 == Title
	short lastStageNum;

public:
	RESULT(bool issue, int clearTime = -1, const char* stageName = "StageSelect");
	~RESULT();
	AbstractScene* Update() override;
	void Draw() const override;
};

