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
	int titleFont, menuFont, guidFont, buttonGuidFont;

	// �^�C�g�����S�A�j���[�V����

	// �^�C�}�[
	std::chrono::steady_clock::time_point stateStartTime;
	double scaleFactor; 
	int titleLogoAnimationY;
	int titleAnimationFrame;
	// BOUNCING�Ŕ�ђ��˂���
	int bounceCount;
	// �����x
	double velocity;
	// ����ē��_�ŗp�^�C�}�[
	int timer;
	bool exitFlag;

	enum class AnimationState {
		BOUNCING,	// ��ђ��˂�
		PLAYING,	// �J�n
		MOVING_UP,	// ��Ɉړ�
		POP,	// �|�b�v
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

	/**
	 * \brief �񎟕�������p�����A�E�g�C�[�W���O�iOutQuad�j
	 * \param elapsedTime �o�ߎ��ԁi�~���b��b�P�ʂŃA�j���[�V�������J�n���Ă���̎��ԁj
	 * \param startValue �A�j���[�V�����̊J�n�l
	 * \param changeInValue �J�n�l����I���l�܂ł̕ω���
	 * \param totalDuration �A�j���[�V�����̑S�̂̎�������
	 * \return elapsedTime �ɂ�����A�j���[�V�����̌��݂̒l
	 */
	double OutQuad(double elapsedTime, double startValue, double changeInValue, double totalDuration) {
		// �o�ߎ��Ԃ𐳋K��
		elapsedTime /= totalDuration;
		return -changeInValue * elapsedTime * (elapsedTime - 2) + startValue;
	}
	
	/**
	 * \brief �O����������p�����A�E�g�C�[�W���O�iOutCubic�j
	 * \param elapsedTime �o�ߎ��ԁi�~���b��b�P�ʂŃA�j���[�V�������J�n���Ă���̎��ԁj
	 * \param startValue �A�j���[�V�����̊J�n�l
	 * \param changeInValue �J�n�l����I���l�܂ł̕ω���
	 * \param totalDuration �A�j���[�V�����̑S�̂̎�������
	 * \return elapsedTime �ɂ�����A�j���[�V�����̌��݂̒l
	 */
	double OutCubic(double elapsedTime, double startValue, double changeInValue, double totalDuration) {
		// �o�ߎ��Ԃ𐳋K������1������
		elapsedTime = elapsedTime / totalDuration - 1;
		return changeInValue * (elapsedTime * elapsedTime * elapsedTime + 1) + startValue;
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