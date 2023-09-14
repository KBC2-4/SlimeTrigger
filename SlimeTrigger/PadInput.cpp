#include "PadInput.h"
#include <array>
#include <corecrt_math_defines.h>
#include "Player.h"


std::bitset<BUTTONS> PAD_INPUT::nowKey;
std::bitset<BUTTONS> PAD_INPUT::oldKey;
XINPUT_STATE PAD_INPUT::input;
DINPUT_JOYSTATE PAD_INPUT::dInput;
PAD_INPUT::InputMode PAD_INPUT::currentInputMode = InputMode::KEYBOARD;
bool PAD_INPUT::isThrowing = false;


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

	const std::array <int, 14> KeyIndices = { 16, 8192, 3, 4, 6, 7, 4096, 1096, 10, 11, 256, 128, 512, 2048 };
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

	// マウスの右クリックで右ショルダーを押す
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		input.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] = 1;
	}

}

void PAD_INPUT::ConvertStickInputToMouseCursorAngle(PLAYER& player)
{

	isThrowing = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;


	if(!isThrowing)return;;
	// 右スティック
	std::array<short*, 2> thumbValues = { &input.ThumbRX, &input.ThumbRY };

	struct PlayerPoint {
		float x;
		float y;
	};

	struct MousePoint {
		int x;
		int y;
	};

	PlayerPoint playerPoint = { player.GetPlayerX(), player.GetPlayerY() };
	MousePoint mousePoint = { 0, 0 };

	// マウスの位置を取得
	GetMousePoint(&mousePoint.x, &mousePoint.y);

	// プレイヤーとマウスカーソルの座標の距離差を計算
	float dx = static_cast<float>(mousePoint.x - playerPoint.x);
	float dy = static_cast<float>(mousePoint.y - playerPoint.y);

	// ベクトルの角度を計算
	float angle = atan2(-dy, dx);

	// 角度を度数法に変換
	//float angleInDegrees = angle * (180.0 / M_PI);

	// スケールを調整（-32768 ～ 32767）
	short thumbRX = static_cast<short>(cos(angle) * 32767);
	short thumbRY = static_cast<short>(sin(angle) * 32767);


	*thumbValues[0] = thumbRX;
	*thumbValues[1] = thumbRY;
}

void PAD_INPUT::UpdateInputMode() const
{

	if (CheckHitKeyAll(DX_CHECKINPUT_PAD)) {
		// マウスカーソルを非表示にする
		//SetMouseDispFlag(FALSE);
		// XInputが使用可能かチェック
		if (CheckJoypadXInput(DX_INPUT_KEY_PAD1)) {

			// 入力キー取得
			GetJoypadXInputState(DX_INPUT_KEY_PAD1, &input);

			if (CheckInput()) {
				currentInputMode = InputMode::XINPUT_GAMEPAD;
				//printfDx("XINPUT_GAMEPAD\n");
			}
		}
		else {

			GetJoypadDirectInputState(DX_INPUT_KEY_PAD1, &dInput);
			InputConverter(dInput);

			if (CheckInput()) {
				currentInputMode = InputMode::DIRECTINPUT_GAMEPAD;
				//printfDx("DIRECTINPUT_GAMEPAD\n");
			}
		}

	}
	if (CheckHitKeyAll(DX_CHECKINPUT_KEY) || CheckHitKeyAll(DX_CHECKINPUT_MOUSE)) {
		// マウスカーソルを表示する
		//SetMouseDispFlag(TRUE);
		// キーボード入力
		KeyInput(input);
		currentInputMode = InputMode::KEYBOARD;
		//printfDx("KEYBOARD\n");
	}
}

bool PAD_INPUT::CheckInput() const
{
	// ボタンのリスト
	std::array<int, 10> buttonList = {
		XINPUT_BUTTON_A,
		XINPUT_BUTTON_B,
		XINPUT_BUTTON_X,
		XINPUT_BUTTON_Y,
		XINPUT_BUTTON_BACK,
		XINPUT_BUTTON_START,
		XINPUT_BUTTON_DPAD_UP,
		XINPUT_BUTTON_DPAD_RIGHT,
		XINPUT_BUTTON_DPAD_DOWN,
		XINPUT_BUTTON_DPAD_LEFT
	};

	// スティックの感度
	const int stick_sensitivity = 20000;

	if (std::abs(GetPadThumbLX()) > stick_sensitivity || std::abs(GetPadThumbLY()) > stick_sensitivity
		|| std::abs(GetPadThumbRX()) > stick_sensitivity || std::abs(GetPadThumbRY()) > stick_sensitivity)
	{
		return true;
	}

	for (auto button : buttonList)
	{
		if (input.Buttons[button] == 1)
		{
			return true;
		}
	}

	return false;
}
