#pragma once
#include "AbstractScene.h"


class GAMEMAIN_RESTART : public AbstractScene
{
private:
	int nowGraph;
	bool reStart;
	int halfwayTime;
	const char* stageName;

public:
	//コンストラクタ
	GAMEMAIN_RESTART(bool reStart = false, int halfwayTime = 0, const char* stageName = "StageSelect", int nowGraph = 0);
	~GAMEMAIN_RESTART();

	AbstractScene* Update() override;

	void Draw() const override;
};

