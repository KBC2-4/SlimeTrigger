#pragma once
#include "AbstractScene.h"

class DRAW_RANKING :
	public AbstractScene
{
private:
	int waitTime;
	int bestTime[3];
	int titleFont,timeFont;
	int image;
	int okSe;
	int backGraundImage;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DRAW_RANKING();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~DRAW_RANKING();
	/// <summary>
	/// 描画以外の更新を実行
	/// </summary>
	/// <returns>シーンのアドレスを返す</returns>
	AbstractScene* Update() override;
	/// <summary>
	/// 描画に関することを実装
	/// </summary>
	void Draw() const override;
};

