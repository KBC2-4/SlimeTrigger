#include "GameMain.h"


GAMEMAIN::GAMEMAIN()
{
	player = new PLAYER;
	stage = new STAGE;
	lemonner = new LEMON(player);
	gurepon = new GRAPEFRUIT(player);
	tomaton = new TOMATO(player);
	element = new Element();
}

GAMEMAIN::~GAMEMAIN()
{
	delete player;
	delete stage;
}

AbstractScene* GAMEMAIN::Update()
{
	stage->Update();
	player->Update(element);

	return this;
}

void GAMEMAIN::Draw() const
{
	//ステージの描画
	stage->Draw();
	//プレイヤーの描画
	player->Draw();
	
}