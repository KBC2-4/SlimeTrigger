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
	int titleFont, menuFont, guidFont, buttonGuidFont;

	// タイトルロゴアニメーション

	// タイマー
	std::chrono::steady_clock::time_point stateStartTime;
	double scaleFactor; 
	int titleLogoAnimationY;
	int titleAnimationFrame;
	// BOUNCINGで飛び跳ねた回数
	int bounceCount;
	// 加速度
	double velocity;
	// 操作案内点滅用タイマー
	int timer;
	bool exitFlag;

	enum class AnimationState {
		BOUNCING,	// 飛び跳ねる
		PLAYING,	// 開始
		MOVING_UP,	// 上に移動
		POP,	// ポップ
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

	/**
	 * \brief 二次方程式を用いたアウトイージング（OutQuad）
	 * \param elapsedTime 経過時間（ミリ秒や秒単位でアニメーションが開始してからの時間）
	 * \param startValue アニメーションの開始値
	 * \param changeInValue 開始値から終了値までの変化量
	 * \param totalDuration アニメーションの全体の持続時間
	 * \return elapsedTime におけるアニメーションの現在の値
	 */
	double OutQuad(double elapsedTime, double startValue, double changeInValue, double totalDuration) {
		// 経過時間を正規化
		elapsedTime /= totalDuration;
		return -changeInValue * elapsedTime * (elapsedTime - 2) + startValue;
	}
	
	/**
	 * \brief 三次方程式を用いたアウトイージング（OutCubic）
	 * \param elapsedTime 経過時間（ミリ秒や秒単位でアニメーションが開始してからの時間）
	 * \param startValue アニメーションの開始値
	 * \param changeInValue 開始値から終了値までの変化量
	 * \param totalDuration アニメーションの全体の持続時間
	 * \return elapsedTime におけるアニメーションの現在の値
	 */
	double OutCubic(double elapsedTime, double startValue, double changeInValue, double totalDuration) {
		// 経過時間を正規化して1を引く
		elapsedTime = elapsedTime / totalDuration - 1;
		return changeInValue * (elapsedTime * elapsedTime * elapsedTime + 1) + startValue;
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