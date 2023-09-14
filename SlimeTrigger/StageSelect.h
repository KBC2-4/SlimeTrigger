#pragma once
#include "AbstractScene.h"
#include "Player.h"
#include "Stage.h"
#include "Lemon.h"
#include "GrapeFruit.h"
#include "Tomato.h"

class STAGE_SELECT :
	public AbstractScene
{
private:
	//�w�i�摜
	int backGraundImage[5];
	//BGM
	int backGraundMusic;

	int okSe;
	//�t�H���g
	int guidFont, buttonGuidFont, stageNameFont, moveToTitleFont;
	int guidTimer;		//�K�C�h�\���^�C�}�[
	bool effectDelta;
	int effectTimer[2];	//[0:�o���G�t�F�N�g, 1:�X�e�[�W�|�[�^���G�t�F�N�g]

	//�`���[�g���A���p
	int joysAniTimer;
	bool joyStickDelta;

	PLAYER* player;	//�v���C���[
	STAGE* stage;	//�X�e�[�W
	ELEMENT* element;	//�X�e�[�W���v�f
	LEMON** lemoner;	//�����i�[
	GRAPEFRUIT** gurepon;	//�O���ۂ�
	TOMATO** tomaton;	//�Ƃ܃g��

	//�����i�[���p�̕ϐ�
	int lemonerCount;
	//�Ƃ܂Ƃ�̐��p�̕ϐ�
	int tomatonCount;

	//�O���|���̐��p�̕ϐ�
	int gureponCount;

	float playerMapX, playerMapY;	//�v���C���[�}�b�v�����W�v�Z�p
	POINT stageReturn;		//�^�C�g���֖߂���W
	POINT stageMove[4];	//�X�e�[�W�|�[�^�����W

	//�O��̃X�e�[�W�ԍ� 0 == Title
	short lastStageNum;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	STAGE_SELECT(short lastStageNum);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~STAGE_SELECT();
	/// <summary>
	/// �`��ȊO�̍X�V�����s
	/// </summary>
	/// <returns>�V�[���̃A�h���X��Ԃ�</returns>
	AbstractScene* Update() override;
	/// <summary>
	/// �`��Ɋւ��邱�Ƃ�����
	/// </summary>
	void Draw() const override;

	/// <summary>
	/// �X�e�[�W�ɓ��鎞��Update
	/// </summary>
	void StageIn(void);
	/// <summary>
	/// �X�e�[�W�ɓ��鎞�̃A�j���[�V����
	/// </summary>
	void StageIn_Animation(void);
	/// <summary>
	/// �X�e�[�W�|�[�^���K�C�h�\��
	/// </summary>
	/// <param name="stageName">�\������������</param>
	/// <param name="x">�`��ʒuX</param>
	/// <param name="y">�`��ʒuY</param>
	/// <param name="stageNameFont">�\�����镶���̃t�H���g</param>
	/// <param name="text_color">�\�����镶���F</param>
	/// <param name="textback_color">�\�����镶���̗֊s�F (-1�̏ꍇ��0xFFFFFF)</param>
	/// <param name="text_margin_x">�\�����镶��X�ʒu���덷�C�� (�f�t�H���g��0)</param>
	/// <param name="text_margin_y">�\�����镶��Y�ʒu���덷�C�� (�f�t�H���g��0)</param>
	/// <param name="backcolor">�`�悷��ȉ~�̐F (-1�̏ꍇ��0xFFFFFF)</param>
	/// <param name="second_title">2�Ԗڂɕ\�����镶�� (""�󔒂Ŗ�����)</param>
	/// <param name="secont_margin_x">2�Ԗڂɕ\�����镶��X�ʒu���덷�C�� (�f�t�H���g��0)</param>
	/// <param name="secont_margin_y">2�Ԗڂɕ\�����镶��Y�ʒu���덷�C�� (�f�t�H���g��25 : 0�ɂ����stage_name�Əd�Ȃ�)</param>
	void DrawStageGuid(const char* stageName, const float x, const float y, const int stageNameFont, const int text_color, const int textback_color = -1,
		const int text_margin_x = 0, const int text_margin_y = 0, const int backcolor = -1, const char* second_title = "", const int secont_margin_x = 0, const int secont_margin_y = 25) const;
};

