#pragma once
#include "Option.h"

class PAUSE
{
private:
	int pauseGraph;
	int pauseEffectTimer;
	int cursorMoveSe,okSe, menuOpenSe, menuCloseSe;
	int menuFont, titleFont, buttonGuidFont;
	int selectMenu;
	int inputMargin;
	int nextMenu;
	bool pauseFlag;	//ポーズフラグ

	Option* option;

public:
	PAUSE();
	~PAUSE();
	int Update(void);
	void Draw();
	bool IsPause(void) { return pauseFlag; }
	void SetPause(void) { pauseFlag = !pauseFlag; if (pauseFlag == false) { selectMenu = 0; } }
	int GetSelectMenu(void) { return nextMenu; }

	int GetDrawCenterX(const char* string, int font_handle)const;



	enum class MENU
	{
		RETURN,
		RESTART,
		OPTION,
		STAGE_SELECT
	};
};

