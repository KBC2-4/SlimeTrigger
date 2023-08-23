#pragma once
#include "AbstractScene.h"

//Aボタンの文字色
#define A_COLOR 0x16F002

//Bボタンの文字色
#define B_COLOR 0xEB7415

//STARTボタンの文字色
#define START_COLOR 0x16F2AC

//BACK(SELECT)ボタンの文字色
#define BACK_COLOR 0x16F2AC

//OK時のバイブレーション設定
#define OK_VIBRATION_POWER	130
#define OK_VIBRATION_TIME	150

class Option
{

private:

	enum class MENU
	{
		WindowMode,
		BGM,
		SE,
		RETURN
	};

	int menuFont, buttonGuidFont;
	int cursorMoveSe, okSe, muteSe;
	int inputMargin;
	static int bgmVolume;        //BGMの音量
	static int seVolume;         //SEの音量
	static bool inputMode;	//入力方式の切り替え
	bool windowMode;//ウィンドウモードの切り替え
	int oldBgmVolume, oldSeVolume;
	int selectMenu;     //選択しているメニュー
	bool optionFlag;	//オプション画面表示フラグ

public:

	//コンストラクタ
	Option();
	//デストラクタ
	~Option();
	//描画以外の更新を実行
	void Update();
	//描画に関することを実装
	void Draw();

	static int GetBGMVolume(void) { return bgmVolume; }
	static int GetSEVolume(void) { return seVolume; }
	bool GetOptionFlg(void) { return optionFlag; }
	void ChangeOptionFlg(void) { optionFlag = !optionFlag; }
	static bool GetInputMode(void) { return inputMode; }

	void LoadData(void);
	void SaveData(void);


	int GetDrawCenterX(const char* string, int font_handle)const;
};

