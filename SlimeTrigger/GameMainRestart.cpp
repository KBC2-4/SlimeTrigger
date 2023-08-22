#include "GameMainRestart.h"
#include "GameMain.h"
#include "DxLib.h"

GAMEMAIN_RESTART::GAMEMAIN_RESTART(bool reStart, int halfway_time, const char* stageName, int nowGraph) : AbstractScene(false) {
	this->reStart = reStart;
	this->halfway_time = halfway_time;
	this->stageName = stageName;
	this->nowGraph = nowGraph;
}

GAMEMAIN_RESTART::~GAMEMAIN_RESTART()
{
	DeleteGraph(nowGraph);

}

AbstractScene* GAMEMAIN_RESTART::Update() {
		return new GAMEMAIN(reStart, halfway_time, stageName);
}


void GAMEMAIN_RESTART::Draw()const {
	DrawGraph(0, 0, nowGraph, FALSE);
}