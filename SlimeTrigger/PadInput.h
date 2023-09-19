#pragma once
#include"DxLib.h"
#include <bitset>

class PLAYER;
	
constexpr auto BUTTONS = 16; //ボタンの数

class PAD_INPUT
{
public:
	// 入力デバイスの切り替え状態
	enum class InputMode {
		KEYBOARD,			//キーボード
		XINPUT_GAMEPAD,		// Xinputのゲームパッド
		DIRECTINPUT_GAMEPAD	//DirectInputのゲームパッド
	};

private:
	static std::bitset<BUTTONS> nowKey;
	static std::bitset<BUTTONS> oldKey;
	static XINPUT_STATE input;
	static DINPUT_JOYSTATE dInput;
	// 入力デバイスの切り替え状態
	static InputMode currentInputMode;
	// プレイヤーが投げる状態
	static bool isThrowing;

public:
	PAD_INPUT(); //コンストラクタ
	void UpdateKey();	//パッド入力の更新
	/**
	* @brief	DirectInputからXinputへの変換器
	* @param dInput DINPUT_JOYSTATE dInput構造体
	*/
	void InputConverter(const DINPUT_JOYSTATE& dInput) const;

	/**
	 * \brief Xinputからキーボード入力への変換器
	 * \param input XINPUT_STATE input構造体
	 */
	void KeyInput(XINPUT_STATE& input) const;

	/**
	 * \brief 右スティックの入力をプレイヤーの座標とマウスカーソルの座標の角度に変換
	 * \param player プレイヤー
	 */
	static void ConvertStickInputToMouseCursorAngle(PLAYER& player);
	static int GetPadThumbLX(){ return input.ThumbLX; }	 //左スティックの横軸値
	static int GetPadThumbLY() { return input.ThumbLY; }	//左スティックの縦軸値
	static int GetPadThumbRX() { return input.ThumbRX; }	//右スティックの横軸値
	static int GetPadThumbRY() { return input.ThumbRY; }	//右スティックの縦軸値
	/**
	* @brief	入力デバイスの切り替え状態を取得する
	* @return 入力デバイスの切り替え状態
	*/
	static int GetInputMode() { return static_cast<int>(currentInputMode); }
	/**
	* @brief	ボタンが押されたかどうかを取得する(1フレームのみ)
	* @param button ボタンの番号(XINPUT_BUTTON_A等のマクロ)
	* @return ボタンの状態
	*/
	static bool OnButton(int button) {
		return nowKey[button] && !oldKey[button];
	}

	/**
	* @brief	ボタンが押されているかどうかを取得する
	* @param button ボタンの番号(XINPUT_BUTTON_A等のマクロ)
	* @return ボタンの状態
	*/
	static bool OnPressed(int button) {
		return nowKey[button];
	}

	/**
	* @brief	ボタンが離されたかどうかを取得する
	* @param button ボタンの番号(XINPUT_BUTTON_A等のマクロ)
	* @return ボタンの状態
	*/
	static bool OnRelease(int button) {
		return !nowKey[button] && oldKey[button];
	}
	/**
	* @brief 入力デバイスの切り替え状態更新
	*/
	void UpdateInputMode() const;

	/**
	* \brief ゲームパッド入力に切り替わる為の入力チェックヘルパー 
	*/
	bool CheckInput() const;

};

