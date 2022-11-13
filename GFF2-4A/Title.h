#pragma once
#include "AbstractScene.h"
#include "STAGE.h"
#include "PadInput.h"

class Title :
    public AbstractScene
{
private:
    int background_image;
    int title_font,menu_font;
    int selectmenu;
    
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

