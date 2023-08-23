#pragma once
#include "AbstractScene.h"
#include "Stage.h"
#include "GameOver.h"
#include "PadInput.h"
#include "Option.h"

enum class MENU
{
	GAME_SELECT = 0,
	RANKING,
	OPTION,
	END
};
class Title : public AbstractScene
{
private:

	int backGraundImage;
	int backGraundMusic;
	int cursorMoveSe, okSe,exitSe;
	int titleFont, menuFont, guidFont;
	int selectMenu;
	int inputMargin;
	int timer;
	bool exitFlag;

	int titleAnitimer[2];

	Option* option;

public:

	//コンストラクタ
	Title();
	//デストラクタ
	~Title();
	//描画以外の更新を実行
	AbstractScene* Update() override;
	//描画に関することを実装
	void Draw() const override;
};