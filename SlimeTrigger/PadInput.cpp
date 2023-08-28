#include "PadInput.h"
#include <array>


int PAD_INPUT::nowKey = -1;
int PAD_INPUT::oldKey = -1;
PAD_STATE PAD_INPUT::state = PAD_STATE::NOT;
XINPUT_STATE PAD_INPUT::Input;
DINPUT_JOYSTATE PAD_INPUT::dInput;


PAD_INPUT::PAD_INPUT() = default;

void PAD_INPUT::UpdateKey()
{
	if (CheckJoypadXInput(DX_INPUT_KEY_PAD1)) {

		// ���̓L�[�擾
		GetJoypadXInputState(DX_INPUT_KEY_PAD1, &Input);
	}
	else {

		GetJoypadDirectInputState(DX_INPUT_KEY_PAD1, &dInput);
		InputConverter(dInput);
	}
	oldKey = nowKey;
	for (int i = 0; i < BUTTONS; i++)
	{
		if (Input.Buttons[i])
		{
			nowKey = i;
			break;
		}
		nowKey = -1;
	}
	if (nowKey != -1)
	{
		//�����ꂽ�u��
		if (nowKey != oldKey)
		{
			state = PAD_STATE::ON;
		}

		//������Ă����
		if (nowKey == oldKey)
		{
			state = PAD_STATE::DOWN;
		}
	}
	else
	{
		//������Ă��Ȃ�
		state = PAD_STATE::NOT;
	}

}

void PAD_INPUT::InputConverter(const DINPUT_JOYSTATE& dInput)
{

	// �X�e�B�b�N�̕ϊ�
	std::array<short*, 4> thumbValues = { &Input.ThumbLX, &Input.ThumbLY, &Input.ThumbRX, &Input.ThumbRY };
	std::array<int, 4> dInputSticks = { dInput.X, -dInput.Y, dInput.Z, -dInput.Rz };

	for (int i = 0; i < 4; ++i) {
		*thumbValues[i] = static_cast<short>(dInputSticks[i] * 32767 / 1000);
	}

	// POV�X�C�b�`�̕ϊ�
	const std::array<unsigned int, 4> povValues = { 0, 18000, 27000, 9000 };
	for (int i = 0; i < 4; ++i) {
		Input.Buttons[i] = (dInput.POV[0] == povValues[i]) ? 1 : 0;
	}

	// �{�^���̕ϊ�
	const std::array <int, 10> dInputButtonIndices = { 1, 2, 3, 4, 6, 7, 8, 9, 10, 11 };
	const std::array <int, 10> xInputButtonIndices = { XINPUT_BUTTON_A, XINPUT_BUTTON_B, XINPUT_BUTTON_Y, XINPUT_BUTTON_X,
			XINPUT_BUTTON_LEFT_SHOULDER, XINPUT_BUTTON_RIGHT_SHOULDER,
			XINPUT_BUTTON_BACK, XINPUT_BUTTON_START,
			XINPUT_BUTTON_LEFT_THUMB, XINPUT_BUTTON_RIGHT_THUMB };

	for (int i = 0; i < 10; ++i) {
		Input.Buttons[xInputButtonIndices[i]] = (dInput.Buttons[dInputButtonIndices[i]] == 128) ? 1 : 0;
	}

	// �g���K�[�̕ϊ�
	unsigned char* triggers[] = { &Input.LeftTrigger, &Input.RightTrigger };
	for (int i = 0; i < 2; ++i) {
		*triggers[i] = static_cast<unsigned char>(dInput.Slider[i] * 255 / 65535);
	}
}