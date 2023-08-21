#pragma once
#include"Enemy.h"
#include<math.h>
#include"DxLib.h"
#include"Player.h"
#include"Stage.h"
#define BULLET_SPEED 12

class ENEMY_BULLET
{
private:
    int myX, myY;                   //���̓G�̍��W
    int drawX;                       //�ǂɓ����������ɕ`��ꏊ�����炷���߂̕ϐ�
    double bulletX, bulletY, hypote;//�e�ׂ̍������W�ƎO�p�`�̎Ε�
    double playerX, playerY;        //�v���C���[�̍��W
    double disX, disY;              //�v���C���[�ƓG�̋���
    double bulletSx, bulletSy;      //�e��X�AY���ꂼ��̃X�s�[�h
    int mapX, mapY;                 //�e�̃}�b�v���X, Y�iint�^�j
    float mapdX, mapdY;             //�e�̃}�b�v���X,Y�idouble�j
    int oldMapX, oldMapY;             //�e�̃}�b�v��̌Â����W
    bool endFlg;                     //�e�̏�����A�j���[�V���������邩�ǂ���
    double radX;                     //�p�x�ύX�p�ϐ�
    float scrollX;                   //�X�N���[���̒l(x)
    float scrollY;                   //�X�N���[���̒l(y)
    bool deleteFlg;                  //�f���[�g���邽�߂̃t���O
    bool hitFlg;                     //�v���C���[�̓������{�[���ɓ����������ǂ����̃t���O
    bool rightSideHit;              //���̂̉E���ʂɓ����������ǂ����̃t���O
    bool leftSideHit;               //���̂̍����ʂɂ����������ǂ����̃t���O
    double hitRad;                   //�e�����̂ɓ����������ɂł�A�j���[�V�����̊p�x�̕ϐ�
    int bulletImages[4];             //�e�̃A�j���[�V�����p�̕ϐ�
    int bulletEndImages[20];        //�e���n�ʂɓ����������ɂł�A�j���[�V�����̕ϐ�
    double rad;                       //�p�x�̕ϐ�
    int imageIndex;                  //�摜�̔��ʗp�̕ϐ�
    int image;                        //�摜�̕ϐ�
    int animationTimer;              //�A�j���[�V�����̎���
    int animationType;               //�A�j���[�V�����̔���
    int splashSe;                    //�e����Ƃ���SE

    PLAYER* player;                   //�v���C���[�̃N���X�̕ϐ�
    STAGE* stage;                     //�X�e�[�W�̃N���X�̕ϐ�

    const int ANIMATION_TIMER = 5;    //�A�j���[�V�����̎���
    const int IMAGE_Y_SIZE = 20;      //�摜�̏c�̃T�C�Y

private:
    float GetDrawX() const;           //�X�N���[���̒l���v�Z����֐�
    float GetDrawY() const;           //�X�N���[���̒l���v�Z����֐�

public:
    ENEMY_BULLET();                   //�ŏ��̃R���X�g���N�^
    ENEMY_BULLET(PLAYER* player, STAGE* augStage, int x, int y, double dis,double,int); //�����A���̃R���X�g���N�^
    ~ENEMY_BULLET();               //�f�X�g���N�^

    void Update();                    //�e�̃A�b�v�f�[�g
    void Move();                      //�e�̓���
    void Hit();                       //�e�̓����蔻��
    /// <summary>
    /// �e��������A�j���[�V����
    /// </summary>
    /// <returns>�A�j���[�V�����̏I������</returns>
    bool EndAnimation();
    /// <summary>
    /// �ړ��A�j���[�V����
    /// </summary>
    void MoveAnimation();
    void Draw() const;                 //�e�̕`��
    bool GetDeleteFlg(){return deleteFlg;}//�f���[�g�t���O�̃Q�b�g
};

