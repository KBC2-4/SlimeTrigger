#include "PadInput.h"


int PAD_INPUT::nowKey = -1;
int PAD_INPUT::oldKey = -1;
PAD_STATE PAD_INPUT::state = PAD_STATE::NOT;
XINPUT_STATE PAD_INPUT::Input;

void PAD_INPUT::UpdateKey()
{
	// ���̓L�[�擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &Input);
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
