#pragma once
#include"DxLib.h"

#define BUTTONS 16

enum class PAD_STATE
{
	NOT = 0, //������Ă��Ȃ�
	ON,		//�����ꂽ�u��
	DOWN	//������Ă����
};

class PAD_INPUT
{
private:
	static int nowKey;
	static int oldKey;
	static PAD_STATE state;
	static XINPUT_STATE Input;
	static DINPUT_JOYSTATE dInput;

public:
	PAD_INPUT(); //�R���X�g���N�^
	void UpdateKey();	//�p�b�h���͂̍X�V
	/**
	* @brief	DirectInput����Xinput�ւ̕ϊ���
	* @param input DINPUT_JOYSTATE dInput�\����
	*/
	void InputConverter(const DINPUT_JOYSTATE& dInput);
	static int GetPadThumbLX(){ return Input.ThumbLX; }	 //���X�e�B�b�N�̉����l
	static int GetPadThumbLY() { return Input.ThumbLY; }	//���X�e�B�b�N�̏c���l
	static int GetPadThumbRX() { return Input.ThumbRX; }	//�E�X�e�B�b�N�̉����l
	static int GetPadThumbRY() { return Input.ThumbRY; }	//�E�X�e�B�b�N�̏c���l
	static int GetNowKey() { return nowKey; }
	static PAD_STATE GetPadState() { return state; }
};

