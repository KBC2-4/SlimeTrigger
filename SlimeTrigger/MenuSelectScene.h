#pragma once
#include "AbstractScene.h"
#include "Option.h"
#include <cmath>
#include <chrono>

constexpr int IMAGE_NUM = 4;

class MenuSelectScene :
    public AbstractScene
{
private:
    //背景画像
    int backGraundImage;

    // 草画像
    int grassImage[2];

    // アセット
    int menuImages[IMAGE_NUM];


    //BGM
    int backGraundMusic;

    // SE
    int cursorMoveSe;
    int okSe;
    int exitSe;

    //フォント
    int guidFont, buttonGuidFont;
    int menuFont;
    int MenuBigFont;

    //ガイド表示タイマー
    int guidTimer;

    Option* option;

    // タイマー
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

    enum class MENU
    {
        PLAY,
        RANKING,
        OPTION,
        EXIT
    };

    struct MenuItem
    {
        int x, y;
        const char* name;
        MENU menu;
    };

    // メニュー項目の定義
    MenuItem menuItems[4] = {
        {350, 400, "あそぶ", MENU::PLAY},
        {830, 260, "ランキング", MENU::RANKING},
        {690, 480, "せってい", MENU::OPTION},
        {980, 480, "おわる", MENU::EXIT}
    };

    // 現在選択されているメニュー項目
    int selected;

    // 最も近いメニュー要素
    int closestIndex;

    //操作間隔時間
    int inputMargin;

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    MenuSelectScene();
    /// <summary>
    /// デストラクタ
    /// </summary>
    ~MenuSelectScene() override;
    /// <summary>
    /// 描画以外の更新を実行
    /// </summary>
    /// <returns>シーンのアドレスを返す</returns>
    AbstractScene* Update() override;
    /// <summary>
    /// 描画に関することを実装
    /// </summary>
    void Draw() const override;

    /**
     * \brief メニューをスティックの角度によって選択する
     * \param closestIndex メニュー要素
     * \param selected 選択されているメニュー要素
     * \param stickAngle スティックの角度
     */
    void UpdateMenuSelection(int& closestIndex, int& selected, double stickAngle) const;
};
