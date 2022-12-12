#pragma once
#include "AbstractScene.h"



class Option
{
private:
    int menu_font;
    int background_image;
    int background_music;
    int cursor_move_se;
    int input_margin;
    static int bgm_vol;        //BGMの音量
    static int se_vol;         //SEの音量
    int selectmenu;     //選択しているメニュー
    bool option_flg;	//オプション画面表示フラグ

public:

    //コンストラクタ
    Option();
    //デストラクタ
    ~Option();
    //描画以外の更新を実行
    void Update();
    //描画に関することを実装
    void Draw();

    static int GetBGMVolume(void) { return bgm_vol; }
    static int GetSEVolume(void) { return se_vol; }
    bool GetOptionFlg(void) { return option_flg; }
    void ChangeOptionFlg(void) { option_flg = !option_flg; }
};

