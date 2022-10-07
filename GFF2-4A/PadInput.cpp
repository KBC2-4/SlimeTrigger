#include "PadInput.h"


int PAD_INPUT::NowKey = -1;
int PAD_INPUT::OldKey = -1;
PADSTATE PAD_INPUT::state = PADSTATE::NOT;
XINPUT_STATE PAD_INPUT::Input;

void PAD_INPUT::UpdateKey()
{
	// 入力キー取得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &Input);
	if (NowKey != -1)
	{
		OldKey = NowKey;
	}
	for (int i = 0; i < BUTTONS; i++)
	{
		if (Input.Buttons[i])
		{
			NowKey = i;
			break;
		}
		NowKey = -1;
	}
	if (NowKey != -1)
	{
		//押された瞬間
		if (NowKey != OldKey)
		{
			state = PADSTATE::ON;
		}

		//押されている間
		if (NowKey == OldKey)
		{
			state = PADSTATE::DOWN;
		}
	}
	else
	{
		//押されていない
		state = PADSTATE::NOT;
	}
	
}
