#include "PadInput.h"
#include <array>
#include <cmath>


std::bitset<BUTTONS> PAD_INPUT::nowKey;
std::bitset<BUTTONS> PAD_INPUT::oldKey;
XINPUT_STATE PAD_INPUT::input;
DINPUT_JOYSTATE PAD_INPUT::dInput;
PAD_INPUT::InputMode PAD_INPUT::currentInputMode = InputMode::KEYBOARD;


PAD_INPUT::PAD_INPUT() = default;

void PAD_INPUT::UpdateKey()
{

	oldKey = nowKey;
	nowKey.reset();

	// 入力状態をリセット
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &input);

	// 入力状態を更新
	UpdateInputMode();

	for (int i = 0; i < BUTTONS; ++i) {
		nowKey[i] = input.Buttons[i] == 1;
	}
}

void PAD_INPUT::InputConverter(const DINPUT_JOYSTATE& dInput) const
{

	// スティックの変換
	std::array<short*, 4> thumbValues = { &input.ThumbLX, &input.ThumbLY, &input.ThumbRX, &input.ThumbRY };
	std::array<int, 4> dInputSticks = { dInput.X, -dInput.Y, dInput.Z, -dInput.Rz };

	for (int i = 0; i < 4; ++i) {
		*thumbValues[i] = static_cast<short>(dInputSticks[i] * 32767 / 1000);
	}

	// POVスイッチの変換
	const std::array<unsigned int, 4> povValues = { 0, 18000, 27000, 9000 };
	for (int i = 0; i < 4; ++i) {
		input.Buttons[i] = (dInput.POV[0] == povValues[i]) ? 1 : 0;
	}

	// ボタンの変換
	const std::array <int, 10> dInputButtonIndices = { 1, 2, 3, 4, 6, 7, 8, 9, 10, 11 };
	const std::array <int, 10> xInputButtonIndices = { XINPUT_BUTTON_A, XINPUT_BUTTON_B, XINPUT_BUTTON_Y, XINPUT_BUTTON_X,
			XINPUT_BUTTON_LEFT_SHOULDER, XINPUT_BUTTON_RIGHT_SHOULDER,
			XINPUT_BUTTON_BACK, XINPUT_BUTTON_START,
			XINPUT_BUTTON_LEFT_THUMB, XINPUT_BUTTON_RIGHT_THUMB };

	for (int i = 0; i < 10; ++i) {
		input.Buttons[xInputButtonIndices[i]] = (dInput.Buttons[dInputButtonIndices[i]] == 128) ? 1 : 0;
	}

	// トリガーの変換
	std::array <unsigned char*, 2> triggers = { &input.LeftTrigger, &input.RightTrigger };
	for (int i = 0; i < 2; ++i) {
		*triggers[i] = static_cast<unsigned char>(dInput.Slider[i] * 255 / 65535);
	}
}

void PAD_INPUT::KeyInput(XINPUT_STATE& input) const
{

	const std::array <int, 14> KeyIndices = { 16, 8192, 3, 4, 6, 7, 4096, 9, 10, 11, 1, 2, 4, 8 };
	const std::array <int, 14> xInputButtonIndices = { XINPUT_BUTTON_A, XINPUT_BUTTON_B, XINPUT_BUTTON_Y, XINPUT_BUTTON_X,
		XINPUT_BUTTON_LEFT_SHOULDER, XINPUT_BUTTON_RIGHT_SHOULDER,
		XINPUT_BUTTON_BACK, XINPUT_BUTTON_START,
		XINPUT_BUTTON_LEFT_THUMB, XINPUT_BUTTON_RIGHT_THUMB,
		XINPUT_BUTTON_DPAD_DOWN, XINPUT_BUTTON_DPAD_LEFT, XINPUT_BUTTON_DPAD_RIGHT, XINPUT_BUTTON_DPAD_UP
	};

	for (int i = 0; i < 14; ++i) {
		input.Buttons[xInputButtonIndices[i]] = (GetJoypadInputState(DX_INPUT_KEY) & KeyIndices[i]) ? 1 : 0;
	}

	//printfDx("%d\n", GetJoypadInputState(DX_INPUT_KEY));
	//printfDx("%d\n", GetJoypadInputState(DX_INPUT_PAD1));
}

void PAD_INPUT::UpdateInputMode()
{
	// スティックの感度
	const int stick_sensitivity = 20000;

	if (CheckHitKeyAll(DX_CHECKINPUT_PAD)) {
		// XInputが使用可能かチェック
		if (CheckJoypadXInput(DX_INPUT_KEY_PAD1)) {

			// 入力キー取得
			GetJoypadXInputState(DX_INPUT_KEY_PAD1, &input);

			if (std::abs(GetPadThumbLX()) > stick_sensitivity || std::abs(GetPadThumbLY()) > stick_sensitivity
				|| std::abs(GetPadThumbRX()) > stick_sensitivity || std::abs(GetPadThumbRY()) > stick_sensitivity) {
				currentInputMode = InputMode::XINPUT_GAMEPAD;
				//printfDx("XINPUT_GAMEPAD\n");
			}
		}
		else {

			GetJoypadDirectInputState(DX_INPUT_KEY_PAD1, &dInput);
			InputConverter(dInput);

			if (std::abs(GetPadThumbLX()) > stick_sensitivity || std::abs(GetPadThumbLY()) > stick_sensitivity
				|| std::abs(GetPadThumbRX()) > stick_sensitivity || std::abs(GetPadThumbRY()) > stick_sensitivity) {
				currentInputMode = InputMode::DIRECTINPUT_GAMEPAD;
				//printfDx("DIRECTINPUT_GAMEPAD\n");
			}
		}

	}
	if (CheckHitKeyAll(DX_CHECKINPUT_KEY)) {
		// キーボード入力
		KeyInput(input);
		currentInputMode = InputMode::KEYBOARD;
		//printfDx("KEYBOARD\n");
	}
}