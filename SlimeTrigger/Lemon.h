#pragma once
#include"Enemy.h"
#include"EnemyBullet.h"
#include"Stage.h"

class LEMON :
    public ENEMY
{
private:
	bool hitFlg;		//�_���[�W�󂯂����ǂ���
	bool deleteFlg;	//�G���폜���邩�ǂ���
	int shootCount;		//�e�������o
	
	int nowImage;		//�g�p���Ă���摜

	int damageSe;		//�_���[�W���󂯂�����SE
	int pressSe;		//�v���X�A�j���[�V��������SE
	int splashSe;		//�e����Ƃ���SE

	int spawnMapX;	//�X�|�[���n�_(x)
	int spawnMapY;	//�X�|�[���n�_(y)

	ENEMY_BULLET* bullet;

public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    LEMON();

	/// �R���X�g���N�^
	/// </summary>
	/// <param name="player">�v���C���[�̃A�h���X</param>
	/// <param name="stage">�X�e�[�W�̃A�h���X</param>
	/// <param name="spawnY">�}�b�v��̃X�|�[���n�_(y)</param>
	/// <param name="spawnX">�}�b�v��̃X�|�[���n�_(x)</param>
	LEMON(PLAYER* player, STAGE* stage, int spawnY, int spawnX);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~LEMON();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move() override;
	/// <summary>
	/// �����蔻��
	/// </summary>
	void Hit() override;
	/// <summary>
	/// ���̌`�ɖ߂�A�j���[�V����
	/// </summary>
	/// <returns>�A�j���[�V�����̏I������</returns>
	bool ReturnAnimation();
	/// <summary>
	/// �e�̔��ˎ��̃A�j���[�V����
	/// </summary>
	/// <returns>�A�j���[�V�����̏I������</returns>
	bool PressAnimation();
	/// <summary>
	/// �����A�j���[�V����
	/// </summary>
	void FallAnimation();
	/// <summary>
	/// ���S�A�j���[�V����
	/// </summary>
	/// <returns>�A�j���[�V�����̏I������</returns>
	bool DethAnimation();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw()const override;

	bool GetDeleteFlag() { return deleteFlg; }
	int GetMapX() { return mapX; }
	int GetMapY() { return mapY; }
};

