#pragma once
#include"Enemy.h"
#include"EnemyBullet.h"
#include"Stage.h"
#include<math.h>

#define GURAFRU_W 80
#define GURAFRU_H 80


class GRAPEFRUIT :
	public ENEMY
{
private:
	bool flag[3];                   //�e�����݂��Ă��邩�ǂ����̃t���O
	int shootCount;                 //�e�������o
	int animationTimer;            //�A�j���[�V�����̎���
	int animationType;             //�A�j���[�V�����̎��
	int spawnMapX;                //�}�b�v���X
	int spawnMapY;                //�}�b�v���Y
	int targetX;                   //�O���|���̃^�[�Q�b�g���W
	int bulletCount;               //�e�����^�C�~���O
	bool deleteFlg;                //������t���O
	int faceImage[2];				//��̉摜
	int fruitImage[3];             //�ʎ��̉摜
	double fruitImageSize;		//�ʎ��̉摜�̑傫��
	int damageSe;					//�_���[�W���󂯂�����SE
	int pressSe;					//�v���X�A�j���[�V��������SE
	int splashSe;					//�e����Ƃ���SE

	ENEMY_BULLET* bullet[3];       //�e�̃N���X
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GRAPEFRUIT();
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="player">�v���C���[�̃A�h���X</param>
	GRAPEFRUIT(PLAYER* player,STAGE* stage,int,int);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GRAPEFRUIT();
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


	//�Q�b�g�֐�
	bool GetDeleteFlg() { return deleteFlg; }
	int GetSpawnMapY() { return spawnMapY; }
	int GetSpawnMapX() { return spawnMapX; }
	int GetX();
};

