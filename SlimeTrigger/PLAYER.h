#pragma once
#include"PadInput.h"
#include "Element.h"
#include <vector>
#include "ThrowSlime.h"
#include "Stage.h"

class TOMATO;

#define MAX_LIFE				5		//�v���C���[�̍ő僉�C�t
#define SPEED					4.2f	//�v���C���[�̃X�s�[�h
#define DEVIATION				10000	//�X�e�B�b�N���͂̌���͈͂̔�
#define JUMP_VELOCITY			-6.2f	//�W�����v�X�s�[�h
#define HOOK_MAX_DISTANCE		280
#define ANIMATION_TYPE			7
#define THROW_INTERVAL			60		//������Ƃ��̃N�[���^�C��
#define HOOK_INTERVAL			20

#define PI 3.1415926535897932384626433832795
#define LENGTH      200                 // �R�̒���
#define CLENGTH     (LENGTH * 2 * PI)   // �R��L�΂��Ĉ���������ꍇ�ɏo����~�̉~���̒���
#define G           9.81                // �d�͉����x

#define MIN_SIZE_SCALE		0.8f	//�v���C���[�̍ŏ��T�C�Y(�{��)

//�ړ��X�e�[�g
enum class PLAYER_MOVE_STATE {
	IDLE = 0,	//��~���Ă���Ƃ�
	MOVE,		//�����Ă�Ƃ�
	JUMP,		//����
	FALL,		//����
	THROW,		//�����鎞
	GROW_HOOK,	//�t�b�N�ɐL�т�Ƃ�
	HOOK,		//�U��q���Ă���Ƃ�
	//DAMAGE,		//�_���[�W���󂯂���
	DEAD,		//���񂾂Ƃ�
};

//�A�j���[�V�����X�e�[�g
enum class PLAYER_ANIM_STATE {
	IDLE = 0,//�A�C�h���A�j���[�V����
	MOVE,	 //�ړ��A�j���[�V����
	THROW,	 //������A�j���[�V����
	HOOK,
	JUMP,	//�W�����v�A�j���[�V����
	FALL,	//�����A�j���[�V����
	LANDING,//���n�A�j���[�V����
};

class PLAYER
{

private:
	bool isVisible;
	float playerX, playerY;
	float oldPlayerX, oldPlayerY;
	int mapX, mapY;
	float jumpMoveX;
	bool isGround;		//�n�ʂɂ��Ă邩�ǂ���
	bool hitCeil;
	float playerSpeed;

	int life;
	int images[ANIMATION_TYPE][10];		//�A�j���[�V�����̉摜
	int moveType;			//�����E�̈ړ�(���]�p)
	float moveX;

	//Jump
	int jumpMode;			//��~�W�����v(1)���ړ��W�����v(2)��
	float jumpPower;
	float jumpVelocity;
	bool isJump;
	bool jumpRequest;
	bool isGravity;

	//hook
	bool isHookMove;
	bool isHookPendulumMove; //�t�b�N�܂ł̈ړ��I������
	float hookMoveX, hookMoveY;
	float hookAngle;
	float hookDistance;
	float hookMoveDistance;
	float hookY, hookX;
	int hookIndex;
	int idleNobiImage;
	std::vector<int> grabbedHookArray;
	
	double x;     // �R��L�΂��Ĉ���������ꍇ�ɏo����~�̐���̍��W�A�O�͕R�����̐^���ɐL�т��ʒu
	double speed; // x�̕ω����x
	double angle;
	int nX, nY;
	double b = 8.0;		//�f�t�H���g8
	double mass = 4.0;

	//Throw
	bool throwPreparation;
	int throwInterval;
	int throwBallImage;
	float throwRad;

	std::vector<ThrowSlime> throwSlime;
	std::vector<float>throwX;
	std::vector<float>throwY;
	int throwIndex = 0;

	//�_�ŗp
	bool isDamage;
	int alphaParameter;	//�p���[���[�^�[
	int alphaTime;		//�_�Ŏ���

	bool isHear;

	//�v���C���[�̃T�C�Y(�{��)
	float playerScale;

	//�T�E���h
	int damageSE;		//�_���[�W��H������Ƃ�
	int jumpSE;			//�W�����v�����Ƃ�
	int landingSE;		//���n�����Ƃ�
	int hookMoveSe;	//�t�b�N�ւ̈ړ���
	int hookPendulumSe;//�U��q�^����
	int healSe;			//�񕜂����Ƃ�
	int throwBallSe;	//������Ƃ�

	typedef struct Animation_old {
		//�摜��؂�ւ���^�C�~���O(�t���[��)
		const int switchFrame;
		
		//int animationPlayType [10] = {};
		//�A�j���[�V�����̍Đ��̎d��;
		//-1: �Œ�
		// 0: �ꖇ�ڂ���Đ�������t�Đ�����
		// 1: �ꖇ�ڂ���Đ�������ꖇ�ڂɖ߂�
		// 2: �Ō�܂ł��ꂽ��Ō�̉摜�ŌŒ�
		const int playType;

		//�A�j���[�V�����摜�̖���
		const int imageNum;

		//�A�j���[�V�����̗D��x
		const int priority;

		//�A�j���[�V�����̃t���[���̃J�E���g
		int frame = 0;

		//���̃A�j���[�V�����̓Y����
		int type = 0;

		//play_type��0�̃A�j���[�V�����̒i�K(0: �O��, 1: �㔼)
		int phase = 0;

		//���[�v�Đ����ǂ���(0: ���[�v�Đ�)
		int playMode;

		//�A�j���[�V�����̏I������
		bool endAnim;
	};
	Animation_old animation[ANIMATION_TYPE]{
		/*
		* �؂�ւ��^�C�~���O�i�t���[���j
		* �A�j���[�V�����̍Đ�����
		* �摜�̖���
		* �A�j���[�V�����؂�ւ��̗D��x
		*/

		{  3,  1,  9, 0 },	//�A�C�h��
		{  1,  0, 10, 0 },	//�ړ�
		{  2,  1,  7, 2 },	//������
		{  1, -1,  1, 0 },	//�t�b�N
		{ 20,  1,  4, 1 },	//�W�����v
		{ 20,  2,  4, 1 },	//����
		{  2,  1, 10, 1 },	//���n
	};

	//�X�e�[�g�ϐ�
	PLAYER_MOVE_STATE playerState;
	PLAYER_ANIM_STATE animationState;
	STAGE *stage;

public:
	PLAYER(STAGE* stage);
	~PLAYER();

	void Move();
	void Draw(STAGE* stage) const;
	void HookMove(ELEMENT* element, STAGE* stage);
	void JumpMove();
	void Throw(STAGE* stage);
	void MoveAnimation();
	void Update(ELEMENT* element, STAGE* stage, TOMATO** tomaton, int tomatonCount, bool is_stay = false);
	void Hit(ELEMENT* element, STAGE* stage);
	void ChangeAnimation(PLAYER_ANIM_STATE anim, bool compelChange = false);

	//�ϐ��̃Q�b�g

	/// <summary>
	/// ���S�����Getter
	/// </summary>
	bool IsDeath() { return playerState == PLAYER_MOVE_STATE::DEAD; }
	
	/// <summary>
	/// �v���C���[�̃��C�t��Getter
	/// </summary>
	int GetLife() { return life; };

	/// <summary>
	/// �v���C���[�̒e�̐���Getter
	/// </summary>
	int GetThrowCnt() { return static_cast<int>(throwSlime.size()); }

	/// <summary>
	/// �v���C���[�̃}�b�v�z��Y��Getter
	/// </summary>
	/// <returns>int�^�F�z��̓Y����</returns>
	int GetMapY() { return mapY; }

	/// <summary>
	/// �v���C���[�̉�ʓ�X���W��Getter
	/// </summary>
	/// <returns>float�^�F�v���C���[�̉�ʓ�X���W</returns>	
	float GetPlayerX() { return playerX + stage->GetScrollX(); }

	/// <summary>
	/// �v���C���[�̉�ʓ�Y���W��Getter
	/// </summary>
	/// <returns>float�^�F�v���C���[�̉�ʓ�Y���W</returns>	
	float GetPlayerY() { return playerY + stage->GetScrollY(); }

	/// <summary>
	/// �v���C���[�̉�ʓ���X���W��Getter
	/// </summary>
	/// <returns>float�^�F�v���C���[�̉�ʓ���X���W</returns>	
	float GetOldPlayerX() { return oldPlayerX + stage->GetScrollX(); }

	/// <summary>
	/// �v���C���[�̉�ʓ���Y���W��Getter
	/// </summary>
	/// <returns>float�^�F�v���C���[�̉�ʓ���Y���W</returns>	
	float GetOldPlayerY() { return oldPlayerY + stage->GetScrollY(); }

	/// <summary>
	/// �v���C���[�̈ړ�������Getter
	/// </summary>
	/// <returns>float�^�F-1.0 or 1.0</returns>
	float GetMoveX() { return moveX; }

	/// <summary>
	/// �v���C���[�̃T�C�Y�̔{����Getter
	/// </summary>
	/// <returns>float�^�F�{��</returns>
	float GetPlayerScale() { return playerScale; }

	/// <summary>
	/// �v���C���[�̃W�����v�E�������x��Getter
	/// </summary>
	/// <returns>-10 �` 10</returns>
	float GetJumpVelocity() { return jumpVelocity; }

	/// <summary>
	/// �v���C���[�̈ړ����x��Getter
	/// </summary>
	/// <returns>float�^�F�v���C���[�̈ړ����x</returns>
	float GetPlayerSpeed() { return playerSpeed; }

	/// <summary>
	/// �v���C���[�̐U��q�̑��x��Getter
	/// </summary>
	/// <returns>float�^�F�v���C���[�̐U��q�̑��x</returns>
	float GetPlayerHookSpeed() { return static_cast<float>(speed); }

	/// <summary>
	/// �v���C���[�̓����Ԃ�Getter
	/// </summary>
	/// <returns>enum class�^</returns>
	PLAYER_MOVE_STATE GetPlayerMoveState() { return playerState; }

	/// <summary>
	/// �v���C���[�̒e�̃N���X��Getter
	/// </summary>
	ThrowSlime GetThrowSlime(int index) { return throwSlime[index]; }

	//�ϐ��̃Z�b�g

	/// <summary>
	/// �v���C���[�̃��C�t��Setter
	/// </summary>
	void SetLife(int _life);

	/// <summary>
	/// �v���C���[�̉�ʓ�X���W��Setter
	/// </summary>
	void SetPlayerX(float x) { playerX = x - stage->GetScrollX(); }

	/// <summary>
	/// �v���C���[�̃}�b�v��Y���W��Setter
	/// </summary>
	void SetPlayerY(float y) { playerY = y; }

	/// <summary>
	/// �v���C���[�̃}�b�v�����W��Setter
	/// </summary>
	void SetPlayer_Screen(POINT screen) { playerX = screen.y; playerY = screen.x; }

	/// <summary>
	/// �v���C���[�̏d�͂�Setter
	/// </summary>
	/// <param name="gravity">true:�L�� false:����</param>
	void SetGravity(bool gravity) { isGravity = gravity; }

	/// <summary>
	/// �v���C���[�̕\����\��
	/// </summary>
	/// <param name="visible">true:�L�� false:����</param>
	void SetVisible(bool visible) { isVisible = visible; }
};
