#include "MenuSelectScene.h"

#include "DrawRanking.h"
#include "Title.h"
#include "GameMain.h"
#include "DxLib.h"
#include "Option.h"
#include "PadInput.h"
#include "StageSelect.h"

MenuSelectScene::MenuSelectScene()
    :selected(0), closestIndex(0), inputMargin(0)
{
    backGraundImage = LoadGraph("Resource/Images/Scene/menu_select/background.png");
    if ((backGraundMusic = LoadSoundMem("Resource/Sounds/BGM/title.wav")) == -1)
    {
        throw "Resource/Sounds/BGM/title.wav";
    }

    // 草 画像の読み込み
    LoadDivGraph("Resource/Images/Scene/menu_select/grass.png", 2, 2 , 1, 640, 720,  grassImage);

    if ((cursorMoveSe = LoadSoundMem("Resource/Sounds/SE/cursor_move.wav")) == -1)
    {
        throw "Resource/Sounds/SE/cursor_move.wav";
    }

    if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1)
    {
        throw "Resource/Sounds/SE/ok.wav";
    }

    const int seRandom = GetRand(1);
    char disExitSe[30];
    sprintf_s(disExitSe, sizeof(disExitSe), "Resource/Sounds/SE/exit0%d.wav", seRandom + 1);

    if ((exitSe = LoadSoundMem(disExitSe)) == -1) {
        throw disExitSe;
    }

    const char* imageFiles[IMAGE_NUM] = {
        "Resource/Images/Scene/menu_select/slime.png",
        "Resource/Images/Scene/menu_select/crown.png",
        "Resource/Images/Scene/menu_select/setting.png",
        "Resource/Images/Scene/menu_select/button.png",
    };

    for (int i = 0; i < IMAGE_NUM; ++i)
    {
        menuImages[i] = LoadGraph(imageFiles[i]);
        if (menuImages[i] == -1)
        {
            // エラー（画像が読み込めなかった場合）
        }
    }

    guidFont = CreateFontToHandle("メイリオ", 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

    // タイマーを初期化
    startTime = std::chrono::high_resolution_clock::now();

    guidTimer = 0;

    PlaySoundMem(backGraundMusic, DX_PLAYTYPE_LOOP);

    //BGM
    ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic);
    ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.2), okSe);

    option = new Option();
}

MenuSelectScene::~MenuSelectScene()
{
    DeleteFontToHandle(guidFont);
    DeleteFontToHandle(buttonGuidFont);
    StopSoundMem(backGraundMusic);
    DeleteSoundMem(backGraundMusic);
    DeleteSoundMem(cursorMoveSe);
    DeleteSoundMem(okSe);
    DeleteSoundMem(exitSe);
    DeleteGraph(backGraundImage);
    for (int i = 0; i < IMAGE_NUM; ++i)
    {
        DeleteGraph(menuImages[i]);
    }

    for(int i = 0; i < 2; ++i)
    {
        DeleteGraph(grassImage[i]);
    }
}

AbstractScene* MenuSelectScene::Update()
{
    if (option->GetOptionFlg() == true) {
        option->Update();
        //BGM
        ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic);

        //SE
        ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.6), cursorMoveSe);
        ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.2), okSe);
        ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.3), exitSe);
    }
    else
    {
        //BACKボタンでタイトルへ戻る
        if (PAD_INPUT::OnButton(XINPUT_BUTTON_BACK))
        {
            PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
            //ok_seが鳴り終わってから画面推移する。
            while (CheckSoundMem(okSe)){}
            StartJoypadVibration(DX_INPUT_PAD1, OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
            return new Title();
        }
    

        //ガイドの表示タイマー
        if (guidTimer < 100) { guidTimer++; }
        else { guidTimer = 0; }

        // 操作間隔時間
        constexpr int maxInputMargin = 20;
    
        
        if (inputMargin < maxInputMargin)
        {
            inputMargin++;
        
        } else
        {

            // スティックの感度
            constexpr int stickSensitivity = 20000;

            int stickX = 0;
            int stickY = 0;

            
            if (std::abs(PAD_INPUT::GetPadThumbLX()) > stickSensitivity)
            {
                stickX = PAD_INPUT::GetPadThumbLX();   
            }

            if (std::abs(PAD_INPUT::GetPadThumbLY()) > stickSensitivity)
            {
                stickY = PAD_INPUT::GetPadThumbLY();   
            }

            if (std::abs(stickX) > stickSensitivity || std::abs(stickY) > stickSensitivity ||
                PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_UP) || PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_DOWN))
            {
                PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
                StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
                
                // スティックの絶対角度を計算
                const double stickAngle = std::atan2(-stickY, stickX);
                UpdateMenuSelection(closestIndex, selected, stickAngle);
                inputMargin = 0;
            }
        }
    

        if ((PAD_INPUT::OnRelease(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)))
        {
            PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
            //ok_seが鳴り終わってから画面推移する。
            while (CheckSoundMem(okSe)){}
            StartJoypadVibration(DX_INPUT_PAD1, OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);

            // intからenumへの変換
            const auto state = static_cast<MENU>(selected);

            switch (state)
            {
            case MENU::PLAY:
                return new STAGE_SELECT(0);
                break;
            case MENU::RANKING:
                return new DRAW_RANKING();
                break;
            case MENU::OPTION:
                option->ChangeOptionFlg();
                break;
            case MENU::EXIT:
                return new Title();
                break;
            default:
                break;
            }
        }

        return this;
    }
}

void MenuSelectScene::Draw() const
{
    // 背景描画
    DrawGraph(0, 0, backGraundImage, FALSE);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);
    DrawBoxAA(0,80,1280,640,0xFFFFFF,TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 現在の経過時間
    const auto currentTime = std::chrono::high_resolution_clock::now();

    // 経過時間（実数値の秒単位）
    const auto elapsed = std::chrono::duration<double>(currentTime - startTime).count();

    const double moveTime = 2.0;
    if (elapsed < moveTime) {
        const int moveDistance1 = static_cast<int>(-640 + (640 * (elapsed / moveTime)));
        const int moveDistance2 = 1280 - static_cast<int>(640 * (elapsed / moveTime));

        DrawGraph(moveDistance1, 0, grassImage[0], TRUE);
        DrawGraph(moveDistance2, 0, grassImage[1], TRUE);
    }  else {
        // 2秒以上たったら固定位置に
        DrawGraph(0, 0, grassImage[0], TRUE);
        DrawGraph(640, 0, grassImage[1], TRUE);
    }

    //オプション画面へ入る
    if (option->GetOptionFlg() == true) {
        option->Draw();
    }
    else
    {
        //ガイド表示

        constexpr unsigned int guidColor = 0xFFFFFF;

        if (PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::XINPUT_GAMEPAD) || PAD_INPUT::GetInputMode()
            == static_cast<int>(PAD_INPUT::InputMode::DIRECTINPUT_GAMEPAD))
        {
            const std::vector<guideElement> gamepadGuides = {
                guideElement({"L"}, "移動", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,
                             0xFFFFFF, 0xFFFFFF),
                guideElement({Option::GetInputMode() ? "B" : "A"}, "決定", GUIDE_SHAPE_TYPE::FIXED_CIRCLE, buttonGuidFont,
                             guidColor,
                             Option::GetInputMode() ? B_COLOR : A_COLOR, guidColor)
            };
            DrawGuides(gamepadGuides, 560.0f, 668.0f, 5.0f, 60.0f);
        }
        else if (PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::KEYBOARD))
        {
            const std::vector<guideElement> keyboardGuides = {
                guideElement({"W",  "S"}, "移動", GUIDE_SHAPE_TYPE::FIXED_BOX, buttonGuidFont, 0xFFFFFF,
                             buttonGuidFont),
                guideElement({Option::GetInputMode() ? "SPACE" : "Z"}, "決定", GUIDE_SHAPE_TYPE::DYNAMIC_BOX, buttonGuidFont,
                             guidColor,
                             Option::GetInputMode() ? A_COLOR : B_COLOR, guidColor)
            };
            DrawGuides(keyboardGuides, 480.0f, 668.0f, 5.0f, 60.0f);
        }

        // メニュー項目の描画

        for (int i = 0; i < 4; ++i)
        {
        }

        for (int i = 0; i < sizeof(menuItems) / sizeof(MenuItem); ++i)
        {
            const int stringWidth = GetDrawStringWidthToHandle(menuItems[i].name, strlen(menuItems[i].name), guidFont);
            const int centerX = menuItems[i].x - stringWidth / 2;
        
            if (i == selected)
            {

                DrawRotaGraph(menuItems[i].x, menuItems[i].y, 1.0, 0.0, menuImages[i], TRUE);
                DrawStringToHandle(centerX,
                                   menuItems[i].y, menuItems[i].name, 0xEB7415, guidFont, 0xFFFFFF);
            }
            else
            {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
                DrawRotaGraph(menuItems[i].x, menuItems[i].y, 1.0, 0.0, menuImages[i], TRUE);
                DrawStringToHandle(centerX,
                                   menuItems[i].y, menuItems[i].name, 0xFFFFFF, guidFont, 0xFFFFFF);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }
        }
    }
}

void MenuSelectScene::UpdateMenuSelection(int& closestIndex, int& selected, double stickAngle) const
{

    if(PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::XINPUT_GAMEPAD) || PAD_INPUT::GetInputMode()
        == static_cast<int>(PAD_INPUT::InputMode::DIRECTINPUT_GAMEPAD))
    {
        // 現在選択されているメニュー項目の座標
        const int currentX = menuItems[selected].x;
        const int currentY = menuItems[selected].y;
    
        // 初期値として最大の角度（360度）を設定
        double minAngleDiff = 3.14159 * 2;

        // 許容誤差（角度のマージン）

        for (int i = 0; i < sizeof(menuItems) / sizeof(MenuItem); ++i)
        {
            
            // 現在選択されている項目はスキップ
            if (i == selected) continue;

            // 現在の選択から見た相対角度を計算
            const double relativeX = menuItems[i].x - currentX;
            const double relativeY = menuItems[i].y - currentY;
            const double itemAngle = std::atan2(relativeY, relativeX);

            double angleDiff = std::abs(stickAngle - itemAngle);
            angleDiff = min(angleDiff, 2 * 3.14159 - angleDiff);  // 角度の差を最小化

            // 角度の許容誤差
            constexpr double margin = 1.2;
            if (angleDiff < margin)
            {
                // 許容誤差内で一番最初に見つかった項目を選択
                closestIndex = i;
                break;
            }
        
            // 最も近いメニュー項目を更新
            if (angleDiff < minAngleDiff) {
                closestIndex = i;
                minAngleDiff = angleDiff;
            }

        }
    }else if (PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::KEYBOARD))
    {
        // キーボード

        // UP
        if (PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_UP))
        {
            closestIndex = (closestIndex + IMAGE_NUM - 1) % IMAGE_NUM;   
        }
        // DOWN
        else if (PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_DOWN))
        {
            closestIndex = (closestIndex + 1) % IMAGE_NUM;
        }
    }
    // 選択を更新    
    selected = closestIndex;
}
