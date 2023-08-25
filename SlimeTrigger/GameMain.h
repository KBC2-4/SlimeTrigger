#pragma once
#include "AbstractScene.h"
#include "Player.h"
#include "Stage.h"
#include "Lemon.h"
#include "GrapeFruit.h"
#include "Tomato.h"
#include "Element.h"
#include "Result.h"
#include "Pause.h"
#include "Itemball.h"
class GAMEMAIN :
    public AbstractScene
{
private:
	//�w�i�摜
	int backGraundImage[3];
	int backGraundMusic[3];		//BGM
	int startTimeFont, timeFont;	//�t�H���g
	int cursorMoveSe, okSe, countSe, startSe, clearSe;	//SE
	int startTime;		//�J�n�܂ł̒x������
	int startEffectTimer;
	int startAddTime;
	int elapsedTime;	//�o�ߎ���
	const char* stageName;	//���X�^�[�g���̃X�e�[�W�I��p
	int nowGraph;		//���X�^�[�g���̍����݂̉��
	int clearInterval;	//�N���A���̉�ʐ��ڂ܂ł̎���

	//�v���C���[�̃��C�t�̉摜
	int hpImage;

	//�����i�[�̐��p�̕ϐ�
	int lemonerCount;

	//�Ƃ܂Ƃ�̐��p�̕ϐ�
	int tomatonCount;

	//�O���|���̐��p�̕ϐ�
	int gureponCount;

	//�A�C�e���̐��p�ϐ�
	int itemCount;
	int itemNum;
	int itemRand;

	bool reStart;	//�����t���O

	PLAYER* player;	//�v���C���[
	STAGE* stage;	//�X�e�[�W
	PAUSE* pause;	//�|�[�Y
	LEMON** lemoner;	//�����i�[
	GRAPEFRUIT** gurepon;	//�O���ۂ�
	TOMATO** tomaton;	//�Ƃ܃g��
	ITEMBALL** item;     //�A�C�e��
	ELEMENT* element;	//�X�e�[�W���v�f


	//PV����p�i�������摦���ɏ����j
	int inputMargin;
	int scrollSpeed;
	bool playerVisible;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GAMEMAIN(bool reStart = false, int halfwayTime = 0, const char* stageName = "StageSelect");
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GAMEMAIN();
	/// <summary>
	/// �`��ȊO�̍X�V�����s
	/// </summary>
	/// <returns>�V�[���̃A�h���X��Ԃ�</returns>
	AbstractScene* Update() override;
	/// <summary>
	/// �`��Ɋւ��邱�Ƃ�����
	/// </summary>
	void Draw() const override;
};

