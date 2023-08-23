#pragma once
#include "AbstractScene.h"

//A�{�^���̕����F
#define A_COLOR 0x16F002

//B�{�^���̕����F
#define B_COLOR 0xEB7415

//START�{�^���̕����F
#define START_COLOR 0x16F2AC

//BACK(SELECT)�{�^���̕����F
#define BACK_COLOR 0x16F2AC

//OK���̃o�C�u���[�V�����ݒ�
#define OK_VIBRATION_POWER	130
#define OK_VIBRATION_TIME	150

class Option
{

private:

	enum class MENU
	{
		WindowMode,
		BGM,
		SE,
		RETURN
	};

	int menuFont, buttonGuidFont;
	int cursorMoveSe, okSe, muteSe;
	int inputMargin;
	static int bgmVolume;        //BGM�̉���
	static int seVolume;         //SE�̉���
	static bool inputMode;	//���͕����̐؂�ւ�
	bool windowMode;//�E�B���h�E���[�h�̐؂�ւ�
	int oldBgmVolume, oldSeVolume;
	int selectMenu;     //�I�����Ă��郁�j���[
	bool optionFlag;	//�I�v�V������ʕ\���t���O

public:

	//�R���X�g���N�^
	Option();
	//�f�X�g���N�^
	~Option();
	//�`��ȊO�̍X�V�����s
	void Update();
	//�`��Ɋւ��邱�Ƃ�����
	void Draw();

	static int GetBGMVolume(void) { return bgmVolume; }
	static int GetSEVolume(void) { return seVolume; }
	bool GetOptionFlg(void) { return optionFlag; }
	void ChangeOptionFlg(void) { optionFlag = !optionFlag; }
	static bool GetInputMode(void) { return inputMode; }

	void LoadData(void);
	void SaveData(void);


	int GetDrawCenterX(const char* string, int font_handle)const;
};

