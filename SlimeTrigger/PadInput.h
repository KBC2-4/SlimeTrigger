#pragma once
#include"DxLib.h"
#include <bitset>

class PLAYER;
	
constexpr auto BUTTONS = 16; //�{�^���̐�

class PAD_INPUT
{
public:
	// ���̓f�o�C�X�̐؂�ւ����
	enum class InputMode {
		KEYBOARD,			//�L�[�{�[�h
		XINPUT_GAMEPAD,		// Xinput�̃Q�[���p�b�h
		DIRECTINPUT_GAMEPAD	//DirectInput�̃Q�[���p�b�h
	};

private:
	static std::bitset<BUTTONS> nowKey;
	static std::bitset<BUTTONS> oldKey;
	static XINPUT_STATE input;
	static DINPUT_JOYSTATE dInput;
	// ���̓f�o�C�X�̐؂�ւ����
	static InputMode currentInputMode;
	// �v���C���[����������
	static bool isThrowing;

public:
	PAD_INPUT(); //�R���X�g���N�^
	void UpdateKey();	//�p�b�h���͂̍X�V
	/**
	* @brief	DirectInput����Xinput�ւ̕ϊ���
	* @param dInput DINPUT_JOYSTATE dInput�\����
	*/
	void InputConverter(const DINPUT_JOYSTATE& dInput) const;

	/**
	 * \brief Xinput����L�[�{�[�h���͂ւ̕ϊ���
	 * \param input XINPUT_STATE input�\����
	 */
	void KeyInput(XINPUT_STATE& input) const;

	/**
	 * \brief �E�X�e�B�b�N�̓��͂��v���C���[�̍��W�ƃ}�E�X�J�[�\���̍��W�̊p�x�ɕϊ�
	 * \param player �v���C���[
	 */
	static void ConvertStickInputToMouseCursorAngle(PLAYER& player);
	static int GetPadThumbLX(){ return input.ThumbLX; }	 //���X�e�B�b�N�̉����l
	static int GetPadThumbLY() { return input.ThumbLY; }	//���X�e�B�b�N�̏c���l
	static int GetPadThumbRX() { return input.ThumbRX; }	//�E�X�e�B�b�N�̉����l
	static int GetPadThumbRY() { return input.ThumbRY; }	//�E�X�e�B�b�N�̏c���l
	/**
	* @brief	���̓f�o�C�X�̐؂�ւ���Ԃ��擾����
	* @return ���̓f�o�C�X�̐؂�ւ����
	*/
	static int GetInputMode() { return static_cast<int>(currentInputMode); }
	/**
	* @brief	�{�^���������ꂽ���ǂ������擾����(1�t���[���̂�)
	* @param button �{�^���̔ԍ�(XINPUT_BUTTON_A���̃}�N��)
	* @return �{�^���̏��
	*/
	static bool OnButton(int button) {
		return nowKey[button] && !oldKey[button];
	}

	/**
	* @brief	�{�^����������Ă��邩�ǂ������擾����
	* @param button �{�^���̔ԍ�(XINPUT_BUTTON_A���̃}�N��)
	* @return �{�^���̏��
	*/
	static bool OnPressed(int button) {
		return nowKey[button];
	}

	/**
	* @brief	�{�^���������ꂽ���ǂ������擾����
	* @param button �{�^���̔ԍ�(XINPUT_BUTTON_A���̃}�N��)
	* @return �{�^���̏��
	*/
	static bool OnRelease(int button) {
		return !nowKey[button] && oldKey[button];
	}
	/**
	* @brief ���̓f�o�C�X�̐؂�ւ���ԍX�V
	*/
	void UpdateInputMode() const;

	/**
	* \brief �Q�[���p�b�h���͂ɐ؂�ւ��ׂ̓��̓`�F�b�N�w���p�[ 
	*/
	bool CheckInput() const;

};

