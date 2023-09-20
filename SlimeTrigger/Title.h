#pragma once
#include "AbstractScene.h"
#include "PadInput.h"
#include "Option.h"
#include <chrono>

// �^�C�g�����S�A�j���[�V�����̍ő�t���[����
#define MAX_ANIMATION_FRAMES 8

class Title : public AbstractScene
{
private:

	// �摜�n���h��
	int backGraundImage;
	int titleLogoAnimationImage[MAX_ANIMATION_FRAMES];
	int titleLogo;

	// BGM�n���h��
	int backGraundMusic;

	// SE�n���h��
	int cursorMoveSe, okSe,exitSe;

	// �t�H���g�n���h��
	int titleFont, menuFont, guidFont;

	// �^�C�g�����S�A�j���[�V����

	// �^�C�}�[
	std::chrono::steady_clock::time_point stateStartTime;
	double scaleFactor; 
	int titleLogoAnimationY;
	int titleAnimationFrame;
	int timer;
	bool exitFlag;
	int titleAniTimer[2];

	enum class AnimationState {
		PLAYING,	// �J�n
		MOVING_UP,	// ��Ɉړ�
		SHRINKING,	// �k��
		GROWING,	// �g��
		DONE		// ����
	};

	AnimationState animationState;

	// ����Ԋu����
	int inputMargin;

	Option* option;

	/**
	 * \brief �^�C�g�����S�A�j���[�V�����̏�Ԑ���
	 * \param newState �V�������
	 * \param currentTime ���݂̌o�ߎ���
	 */
	void TransitState(AnimationState newState, std::chrono::steady_clock::time_point currentTime) {
		animationState = newState;
		stateStartTime = currentTime;
	}

public:

	//�R���X�g���N�^
	Title();
	//�f�X�g���N�^
	~Title();
	//�`��ȊO�̍X�V�����s
	AbstractScene* Update() override;
	//�`��Ɋւ��邱�Ƃ�����
	void Draw() const override;

	
};