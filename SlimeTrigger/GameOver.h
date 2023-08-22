#pragma once
#include "AbstractScene.h"
#include "Title.h"

enum class GAMEOVER_MENU
{


	NewGame = 0,		//Restart
	ReSelect			//GameSelect
};

class GameOver : public AbstractScene
{
private:
	int title_image,backgraundImage;
	int selectmenu;
	int menu_font;
	int guid_font;
	int backgroundMusic;
	int bad_se[4], okSe, cursormoveSe;
	int inputMargin;
	int timer,se_randnum;
	const char* stageName;	//リスタート時のステージ選択用
	
public:
	//コンストラクタ
	GameOver(const char* stageName);
	//デストラクタ
	~GameOver();
	//描画以外の処理
	AbstractScene* Update() override;
	//描画処理
	void Draw() const override;
};