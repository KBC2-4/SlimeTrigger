#pragma once
#include "Stage.h"
#include <vector>

#define LIFT_SIZE 160

class PLAYER;
class STAGE;

class ELEMENT :
    public STAGE
{
public:
	struct ELEMENT_DATA
	{
		float x, y;
		int type;
		bool flag = false;
		int animTimer = 0;
		
		float leftInitX, leftInitY;		//�������̏������W
		float leftGoalX, leftGoalY;		//�������̖ڕW�ʒu
		int leftVectorX;					//�������̈ړ�����(��)
		int leftVectorY;					//�������̈ړ�����(�c)
		int liftWaitTimer;					//�������ҋ@����
		
		int pairNum;						//�{�^���ƃh�A�̃y�A�ԍ�
		int image;							//�摜
	};
	
private:
	int guidFont,guidTimer;
	std::vector<ELEMENT_DATA> hook;			//�t�b�N
	std::vector<ELEMENT_DATA> button;		//�{�^��
	std::vector<ELEMENT_DATA> door;			//�h�A
	std::vector<ELEMENT_DATA> lift;			//������
	std::vector<ELEMENT_DATA> manhole;		//�}���z�[��
	std::vector<ELEMENT_DATA> acidrainPuddles;			//�_���J�̐����܂�
	float playerMapX, playerMapY;
	int playerState;
	
	//mutable int animTimer;
	bool keepPushing;						//�{�^�������������Ă��邩�t���O
	bool hookFlag; //�߂��ɂ���t�b�N�ɃK�C�h�\��������ׂ̃t���O�B
	int doorCloseSe, pressTheButtonSe, switchSe, walkPuddleSe, manholeOpenedSe;
	int acidrainPuddlesAniTimer;		//�_���J�̐����܂�̐؂�ւ��A�j���[�V�����p�^�C�}�[
	int underGroundEffects;			//�n���G�t�F�N�g�p�^�C�}�[

	//�_���܂�̉摜
	int acidImage[12];
	//�_���܂�̉t�̂̃A�j���[�V����
	int acidAnimation;
	//�_���܂�̖A�̃A�j���[�V����
	int foamAnimation;

	//�������̉摜
	int moveFloorImage[3];
	//�������̃A�j���[�V����
	int moveFloorAnimation;

	//�v���C���[���n���ɂ��邩�ǂ���
	bool isInUnder;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ELEMENT(const char* stageName = "StageSelect");
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ELEMENT();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(STAGE* stage, PLAYER* player);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update(PLAYER* player,STAGE*stage);
	/// <summary>
	/// 3��ނ̃{�^���̏���
	/// </summary>
	void Button(PLAYER* player);
	/// <summary>
	/// �h�A�̏���
	/// </summary>
	void Door(STAGE* stage);
	/// <summary>
	/// �������̏���
	/// </summary>
	void Lift(PLAYER* player, STAGE* stage);
	/// <summary>
	/// �������̓����蔻��
	/// </summary>
	bool HitLift(PLAYER* player);
	/// <summary>
	/// �}���z�[���̏���
	/// </summary>
	void Manhole(PLAYER* player, STAGE* stage);
	/// <summary>
	/// �_���J�̐����܂�̏���
	/// </summary>
	void Acidrain_puddles(PLAYER* player);
	/// <summary>
	/// �t�b�N�̃K�C�h�\���p�����v�Z
	/// </summary>
	void Hook_Distance(PLAYER* player, STAGE* stage);


	/// <summary>
	/// �t�b�N�\���̂�Getter
	/// </summary>
	std::vector<ELEMENT_DATA> GetHook() { return hook; }
	/// <summary>
	/// �{�^���\���̂�Getter
	/// </summary>
	std::vector<ELEMENT_DATA>GetButton() { return button; }
	/// <summary>
	/// �h�A�\���̂�Getter
	/// </summary>
	std::vector<ELEMENT_DATA>GetDoor() { return door; }
	/// <summary>
	/// �������\���̂�Getter
	/// </summary>
	std::vector<ELEMENT_DATA>GetLift() { return lift; }
	/// <summary>
	/// �}���z�[���\���̂�Getter
	/// </summary>
	std::vector<ELEMENT_DATA>GetManhole() { return manhole; }
	/// <summary>
	/// �_���J�̐����܂�\���̂�Getter
	/// </summary>
	std::vector<ELEMENT_DATA>GetAcidrain_puddles() { return acidrainPuddles; }

	bool GetIsInUnder() { return isInUnder; }
};

