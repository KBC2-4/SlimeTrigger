#pragma once
#include "AbstractScene.h"
#include "PadInput.h"
#include "Option.h"
#include <chrono>

// タイトルロゴアニメーションの最大フレーム数
#define MAX_ANIMATION_FRAMES 8

class Title : public AbstractScene
{
private:

	// 画像ハンドル
	int backGraundImage;
	int titleLogoAnimationImage[MAX_ANIMATION_FRAMES];
	int titleLogo;

	// BGMハンドル
	int backGraundMusic;

	// SEハンドル
	int cursorMoveSe, okSe,exitSe;

	// フォントハンドル
	int titleFont, menuFont, guidFont;

	// タイトルロゴアニメーション

	// タイマー
	std::chrono::steady_clock::time_point stateStartTime;
	double scaleFactor; 
	int titleLogoAnimationY;
	int titleAnimationFrame;
	int timer;
	bool exitFlag;
	int titleAniTimer[2];

	enum class AnimationState {
		PLAYING,	// 開始
		MOVING_UP,	// 上に移動
		SHRINKING,	// 縮小
		GROWING,	// 拡大
		DONE		// 完了
	};

	AnimationState animationState;

	// 操作間隔時間
	int inputMargin;

	Option* option;

	/**
	 * \brief タイトルロゴアニメーションの状態推移
	 * \param newState 新しい状態
	 * \param currentTime 現在の経過時間
	 */
	void TransitState(AnimationState newState, std::chrono::steady_clock::time_point currentTime) {
		animationState = newState;
		stateStartTime = currentTime;
	}

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