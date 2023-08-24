#include "Player.h"
#include"DxLib.h"
#include "Element.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Option.h"

//�f�o�b�O������
//#define _DEBUG

/*�R���X�g���N�^*/
PLAYER::PLAYER(STAGE* stage) {
	this->stage = stage;
	playerX = 0.0f;
	playerY = 0.0f;
	mapX = 0;
	mapY = 0;
	life = MAX_LIFE;
	jumpPower = 0.0f;
	jumpVelocity = 0.0f;
	jumpMode = 0;
	jumpMoveX = 0;
	jumpRequest = false;
	isJump = false;
	isGravity = true;
	isHookMove = false;
	isDamage = false;
	isVisible = true;
	throwPreparation = false;
	throwInterval = 0.0f;
	playerState = PLAYER_MOVE_STATE::IDLE;
	grabbedHookArray.clear();
	// �����ʒu�͎��̐^�����獶������45�x�X�����ʒu
	x = CLENGTH / b;
	// �������x�͂O
	speed = 0;

	isHear = false;

	//�摜�̓ǂݍ���
	// �A�C�h�����
	animation[0].images.resize(9);
	if (LoadDivGraph("Resource/Images/Player/IdorSlime.png", 9, 9, 1, 80, 80, animation[0].images.data()) == -1) {
		throw "Resource/Images/Player/IdorSlime.png";
	}
	for (int i = 0; i < 9; i++) {
		animation[0].animation_indexes.push_back(i);
	}
	
	// �ړ�����Ƃ�
	animation[1].images.resize(11);
	if (LoadDivGraph("Resource/Images/Player/Slime.png", 11, 11, 1, 80, 80, animation[1].images.data()) == -1) {
		throw "Resource/Images/Player/Slime.png";
	}
	animation[1].animation_indexes.resize(20);
	for (int i = 0; i < 20; i++) {
		if (i < 11) {
			animation[1].animation_indexes[i] = i;
		}
		else {
			animation[1].animation_indexes[i] = 10 - i % 10;
		}
	}
	
	// ������Ƃ�
	animation[2].images.resize(7);
	if (LoadDivGraph("Resource/Images/Player/ThrowSlime.png", 7, 7, 1, 80, 80, animation[2].images.data()) == -1) {
		throw "Resource/Images/Player/ThrowSlime.png";
	}
	for (int i = 0; i < 7; i++) {
		animation[2].animation_indexes.push_back(i);
	}

	// �t�b�N�̉摜
	animation[3].images.resize(2);
	if ((animation[3].images[0] = LoadGraph("Resource/Images/Player/nobi.png")) == -1) {
		throw "Resource/Images/Player/nobi.png";
	}
	if ((animation[3].images[1] = LoadGraph("Resource/Images/Player/nobi2.png")) == -1) {
		throw "Resource/Images/Player/nobi2.png";
	}

	// �W�����v����Ƃ�
	animation[4].images.resize(4);
	if (LoadDivGraph("Resource/Images/Player/JumpSlime01.png", 4, 4, 1, 80, 80, animation[4].images.data()) == -1) {
		throw "Resource/Images/Player/JumpSlime01.png";
	}
	for (int i = 0; i < 4; i++) {
		animation[4].animation_indexes.push_back(i);
	}

	// ��������Ƃ�
	animation[5].images.resize(4);
	if (LoadDivGraph("Resource/Images/Player/JumpSlime02.png", 4, 4, 1, 80, 80, animation[5].images.data()) == -1) {
		throw "Resource/Images/Player/JumpSlime02.png";
	}
	for (int i = 0; i < 4; i++) {
		animation[5].animation_indexes.push_back(i);
	}

	// ���n����Ƃ�
	animation[6].images.resize(10);
	if (LoadDivGraph("Resource/Images/Player/LandSlime.png", 10, 10, 1, 80, 80, animation[6].images.data()) == -1) {
		throw "Resource/Images/Player/JumpSlime1.png";
	}
	for (int i = 0; i < 10; i++) {
		animation[6].animation_indexes.push_back(i);
	}

	if ((throw_ball_image = LoadGraph("Resource/Images/Player/Slime_Bullet.png")) == -1) {
		throw "Resource/Images/Player/Slime_Bullet.png";
	}
	if ((idleNobiImage = LoadGraph("Resource/Images/Player/FuckAnim2.png")) == -1) {
		throw "Resource/Images/Player/FuckAnim2.png";
	}

	//SE�̓ǂݍ���
	if ((damageSE = LoadSoundMem("Resource/Sounds/SE/Player/damage.wav")) == -1) {
		throw "Resource/Sounds/SE/Player/damage.wav";
	}
	if ((jumpSE = LoadSoundMem("Resource/Sounds/SE/Player/jump.wav")) == -1) {
		throw "Resource/Sounds/SE/Player/jump.wav";
	}
	if ((landingSE = LoadSoundMem("Resource/Sounds/SE/Player/landing.wav")) == -1) {
		throw "Resource/Sounds/SE/Player/jump2.wav";
	}
	if ((hookMoveSe = LoadSoundMem("Resource/Sounds/SE/Player/hook_move.wav")) == -1) {
		throw "Resource/Sounds/SE/Player/hook_move.wav";
	}
	if ((hookPendulumSe = LoadSoundMem("Resource/Sounds/SE/Player/hook.wav")) == -1) {
		throw "Resource/Sounds/SE/Player/hook_pendulum.wav";
	}
	if ((healSe = LoadSoundMem("Resource/Sounds/SE/Player/heal.wav")) == -1) {
		throw "Resource/Sounds/SE/Player/heal.wav";
	}
	if ((throwBallSe = LoadSoundMem("Resource/Sounds/SE/Player/throw_ball.wav")) == -1) {
		throw "Resource/Sounds/SE/Player/throw_ball.wav";
	}

	//SE�̃{�����[���ύX
	ChangeVolumeSoundMem(Option::GetSEVolume(), damageSE);
	ChangeVolumeSoundMem(Option::GetSEVolume(), jumpSE);
	ChangeVolumeSoundMem(Option::GetSEVolume(), landingSE);
	ChangeVolumeSoundMem(Option::GetSEVolume(), hookMoveSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), hookPendulumSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), healSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), throwBallSe);

	//�A�j���[�V�����̏�����
	animation_state = PLAYER_ANIM_STATE::IDLE;
	animation[2].is_loop = false;
	animation[6].is_loop = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PLAYER::~PLAYER() {
	DeleteGraph(throwBallImage);
	DeleteGraph(idleNobiImage);
	for (int i = 0; i < ANIMATION_TYPE; i++) {
		for (auto& img : animation[i].images) {
			DeleteGraph(img);
		}
	}

	DeleteSoundMem(damageSE);
	DeleteSoundMem(jumpSE);
	DeleteSoundMem(landingSE);
	DeleteSoundMem(hookMoveSe);
	DeleteSoundMem(hookPendulumSe);
	DeleteSoundMem(healSe);
	DeleteSoundMem(throwBallSe);
}

/// <summary>
/// �v���C���[�̍X�V
/// </summary>
void PLAYER::Update(ELEMENT* element, STAGE* stage, TOMATO** tomaton, int tomatonCount, bool is_stay) {

	ChangeVolumeSoundMem(Option::GetSEVolume(), damageSE);
	ChangeVolumeSoundMem(Option::GetSEVolume(), jumpSE);
	ChangeVolumeSoundMem(Option::GetSEVolume(), landingSE);
	ChangeVolumeSoundMem(Option::GetSEVolume(), hookMoveSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 0.7, hookPendulumSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), healSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), throwBallSe);

	//�ړ�����
	Move();

	if (is_stay) {
		playerX = oldPlayerX;
	}
	else {
		//�t�b�N����
		HookMove(element, stage);

		//�����鏈��
		Throw(stage);
	}

	//�W�����v�Ɨ�������
	JumpMove();

	//�A�j���[�V�����̍Đ�
	MoveAnimation();

	//�u���b�N�Ƃ̓����蔻��
	Hit(element, stage);

	//������������
	for (int i = 0; i < throwSlime.size(); i++) {
		if (throwSlime[i].checkdel() == true) {
			throwSlime[i].Finalize();
			throwSlime.erase(throwSlime.begin() + i);
		}
	}

	//���̍X�V
	int throw_cnt = throwSlime.size();
	for (int i = 0; i < throw_cnt; i++) {
		throwSlime[i].Update(stage, element, tomaton, tomatonCount);
	}

	//������
	if (playerY + stage->GetScrollY() > 720 && playerState != PLAYER_MOVE_STATE::HOOK || life <= 0) {
		playerState = PLAYER_MOVE_STATE::DEAD;
		return;
	}

	//��ʒ[�̔���
	if (playerX <= 40 * playerScale) {
		playerX = 40 * playerScale;
	}
	if (playerX + stage->GetScrollX() >= 1280 - MAP_CEllSIZE / 2 * playerScale) {
		playerX = 1280 - MAP_CEllSIZE / 2 * playerScale - stage->GetScrollX();
	}

	//�_�ŏ���
	if (isDamage) {
		if (alphaTime > 0) {
			if (alphaTime % 20 < 10) {
				alphaParameter -= 25;
			}
			else {
				alphaParameter += 25;
			}
			alphaTime--;
		}
		else {
			isDamage = false;
		}
	}

	//�v���C���[�̃T�C�Y�̍X�V
	playerScale = static_cast<float>(life - 1) / static_cast<float>(MAX_LIFE) + MIN_SIZE_SCALE;
}

/// <summary>
/// �v���C���[�̕\��
/// </summary>
void PLAYER::Draw(STAGE* stage)const {
	//�\���t���O
	if (!isVisible) return;

	if (isDamage) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaParameter);
	}

	//�v���C���[�̕\��
	if (playerState != PLAYER_MOVE_STATE::HOOK && !isHookMove) {
		//�`�悷��摜�̃Z�b�g
		int image_type = static_cast<int>(animation_state);
		int now_image = animation[image_type].images[animation[image_type].animation_indexes[animation[image_type].current_index]];

		DrawRotaGraphF(playerX + stage->GetScrollX(), (playerY - 20 + stage->GetScrollY()) + (1.6 - playerScale) * 40, playerScale, 0.0, now_image, TRUE, moveType);
	}
	//�t�b�N��
	else {
		//�U��q��
		if (playerState == PLAYER_MOVE_STATE::HOOK) {
			float diff_x = nX;
			float diff_y = nY;
			float distance = sqrt(diff_y * diff_y + diff_x * diff_x);
			float angle = atan2(diff_y, diff_x) + DX_PI_F;
			if (moveType == 0) {
				DrawRotaGraph3F(hookX + nX + stage->GetScrollX(), hookY + nY + stage->GetScrollY(), 80, 80,
					(distance) / MAP_CEllSIZE / 2, 0.6f, (double)angle,
					animation[3].images[1], TRUE, move_type);
			}
			else {
				DrawRotaGraph3F(hookX + nX + stage->GetScrollX(), hookY + nY + stage->GetScrollY(), 80, 80,
					(distance) / MAP_CEllSIZE / 2, 0.6f, (double)angle,
					animation[3].images[0], TRUE, move_type);
			}
		}
		//�L�т鎞
		else {
			DrawRotaGraph3F(playerX + stage->GetScrollX() + 20, (playerY + stage->GetScrollY()) + (1.6 - playerScale) * 40, 80, 40,
				(hookMoveDistance / (MAP_CEllSIZE * 3.0)) * playerScale, 1 * playerScale, (double)hookAngle + M_PI,
				idleNobiImage, TRUE, moveType);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�{�[���̕`��
	int throw_cnt = throwSlime.size();
	for (int i = 0; i < throw_cnt; i++) {
		throwSlime[i].Draw(stage);
	}

	//�{�[���̋O���̕`��
	for (int i = 0; i < throwX.size(); i += 5) {
		DrawGraphF(throwX[i], throwY[i], throwBallImage, TRUE);
	}
}

/// <summary>
/// �v���C���[�̈ړ�
/// </summary>
void PLAYER::Move()
{
	//�ړ��O�̍��W
	oldPlayerX = playerX;
	oldPlayerY = playerY;

	//�X�s�[�h�̃Z�b�g
	playerSpeed = SPEED + (MAX_LIFE - life) * 0.4f;

	//�t�b�N���͈ړ������Ȃ�
	if (isHookMove || playerState == PLAYER_MOVE_STATE::HOOK) return;

	//�X�e�B�b�N���͂̎擾
	int input_lx = PAD_INPUT::GetPadThumbLX();

	if (PAD_INPUT::GetNowKey() == XINPUT_BUTTON_DPAD_LEFT)  {
		input_lx = -20000;
	}
	if (PAD_INPUT::GetNowKey() == XINPUT_BUTTON_DPAD_RIGHT) {
		input_lx = 20000;
	}

	//�ړ�����
	/*if (input_lx > 0) {
		move_x = 1.0f;
	}*/
	move_x = input_lx > 0 ? 1.0f : -1.0f;	//�ړ������̃Z�b�g

	//�ړ�����Ƃ�
	if (fabs(input_lx) > DEVIATION)
	{
		//�W�����v���̂Ƃ�
		if (player_state == PLAYER_MOVE_STATE::JUMP || player_state == PLAYER_MOVE_STATE::FALL)
		{
			if (jump_move_x == 0) jump_move_x = move_x;	// �W�����v�̕���
			move_type = (jump_move_x > 0) ? 0 : 1;		// �摜�̌����̐ݒ�

			// ��~�W�����v�E���Ε����ւ̈ړ���
			if (jump_mode == 1 || jump_move_x != move_x)
			{
				player_speed /= 2.0f;
			}

			player_x += jump_move_x * player_speed;
		}
		else
		{
			move_type = (move_x > 0) ? 0 : 1;				//�ړ������̃Z�b�g(0: �E, 1: ��)
			player_x += move_x * player_speed;
			jump_move_x = move_x;
			player_state = PLAYER_MOVE_STATE::MOVE;	//�X�e�[�g��Move�ɐ؂�ւ�
			ChangeAnimation(PLAYER_ANIM_STATE::MOVE); //�A�j���[�V�����̐؂�ւ�
		}
	}
	//�ړ����ĂȂ���
	else
	{
		moveX = 0;
		//�ړ��A�j���[�V�������㔼�ֈڍs
		int move = static_cast<int>(PLAYER_ANIM_STATE::MOVE);
		if (animation_state == PLAYER_ANIM_STATE::MOVE) {
			int current_index = animation[move].current_index;
			if (current_index == 0) {
				ChangeAnimation(PLAYER_ANIM_STATE::IDLE);
			}
			else if (current_index < 10) {
				animation[move].current_index =
					(animation[move].animation_indexes.size() - current_index) - 1;
			}
		}
		else {
			ChangeAnimation(PLAYER_ANIM_STATE::IDLE);
		}
		//�W�����v������Ȃ���������X�e�[�g��؂�ւ���
		if (playerState != PLAYER_MOVE_STATE::JUMP && playerState != PLAYER_MOVE_STATE::FALL) {
			jumpMoveX = 0;
			playerState = PLAYER_MOVE_STATE::IDLE;	//�X�e�[�g��Idle�ɐ؂�ւ�
		}
	}

	if (playerX < oldPlayerX) {
		moveX = -1.0f;
	}
	else if (playerX > oldPlayerX) {
		moveX = 1.0f;
	}
}

/// <summary>
/// �t�b�N�̈ړ�����
/// </summary>
void PLAYER::HookMove(ELEMENT* element, STAGE* stage) {
	//�߂��Ƀt�b�N�����邩�ǂ���
	bool is_hook = false;

	//�X�e�B�b�N���͂̎擾
	int input_lx = PAD_INPUT::GetPadThumbLX();

	//B�{�^���������Ƃ�
	if (PAD_INPUT::GetNowKey() == (Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) {
		if (playerState != PLAYER_MOVE_STATE::HOOK) {
			//�t�b�N�܂ł̋���
			float min_distance = HOOK_MAX_DISTANCE;
			//�t�b�N�̈ʒu
			std::vector<ELEMENT::ELEMENT_DATA> hook_pos = element->GetHook();
			for (int i = 0; i < hook_pos.size(); i++) {
				//�O�ɒ͂񂾃t�b�N�͒��n����܂Œ͂܂Ȃ�
				if (std::find(grabbedHookArray.begin(), grabbedHookArray.end(), i) != grabbedHookArray.end()) continue;
				ELEMENT::ELEMENT_DATA pos = hook_pos[i];
				//�����v�Z
				float diff_x = pos.x - (playerX);
				float diff_y = pos.y - playerY;
				float distance = sqrtf(diff_x * diff_x + diff_y * diff_y);
				//�������ŒZ�������߂��Ƃ�
				if (distance <= min_distance) {
					//�t�b�N�̊p�x
					float angle = atan2f(diff_y, diff_x);
					//�ړ��̌v�Z
					hookMoveX = cosf(angle) * playerSpeed * 3;
					hookMoveY = sinf(angle) * playerSpeed * 3;
					//�v���C���[�̌��݂̈ʒu
					float x = playerX;
					float y = playerY;
					//�t�b�N�܂ł̈ړ��o�H�ɏ�Q�����Ȃ���
					while (!stage->HitMapDat(y / MAP_CEllSIZE, x / MAP_CEllSIZE)) {
						if (stage->GetMapData(y / MAP_CEllSIZE, x / MAP_CEllSIZE) == 72) {
							break;
						}
						x += hookMoveX;
						y += hookMoveY;
					}
					//�z��ɕϊ�
					int hook_map_x = x / MAP_CEllSIZE;
					int hook_map_y = y / MAP_CEllSIZE;
					//��Q��������ꍇ�͈ړ������Ȃ�
					if (stage->GetMapData(hook_map_y, hook_map_x) != 72) {		//�t�b�N�̔z��ԍ�������
						continue;
					}
					//�ŒZ�����̍X�V
					min_distance = distance;
					hookIndex = i;
					//�t�b�N�̍��W�̍X�V
					hookX = pos.x;
					hookY = pos.y;
					//�t�b�N���������������true
					is_hook = true;
				}
			}
		}
		else {
			is_hook = true;
		}
		//�t�b�N������������
		if (is_hook) {
			//�X�e�[�g�̕ύX
			playerState = PLAYER_MOVE_STATE::GROW_HOOK;
			ChangeAnimation(PLAYER_ANIM_STATE::IDLE, false);
			//�ړ����̎�
			if (!isHookPendulumMove) {
				//�t�b�N�܂ł̋����̌v�Z
				float y = hookY - playerY;
				float x = hookX - (playerX);
				hookDistance = sqrt(x * x + y * y);
				//�p�x�̌v�Z
				hookAngle = atan2f(y, x) + 180.0f * (DX_PI_F / 180.0f);
				//�t�b�N�ړ����ĂȂ���
				if (!isHookMove) {
					//�ړ������̌v�Z
					hookMoveX = cosf(hookAngle - 90.0f * (DX_PI_F / 180.0f)) * SPEED * 3;
					hookMoveY = sinf(hookAngle - 90.0f * (DX_PI_F / 180.0f)) * SPEED * 3;
					//�����I�ȓz
					jumpMoveX = hookMoveX > 0 ? 1 : -1;
					jumpMode == 2;
					//PlaySoundMem(hookMoveSe, DX_PLAYTYPE_BACK);
				}
				else {
					hookMoveDistance += hookDistance / 10.0f;
				}
				//�t�b�N�ɂ��ĂȂ���
				if (hookDistance > 40) {
					playerX += hookMoveX;
					playerY += hookMoveY;
				}
				//�t�b�N�ɂ�����ړ������̏I��
				else {
					//StopSoundMem(hookMoveSe);
					PlaySoundMem(hookPendulumSe, DX_PLAYTYPE_LOOP);
					isHookPendulumMove = true;
					//�U��q�̊J�n�p�x�̐ݒ�
					double angle = (double)hookAngle * (180.0 / M_PI) - 90.0;
					if (angle > 90 && angle < 180) angle = 90.0;
					else if (angle < -90 || angle > 180) angle = -90.0;
					this->x = (CLENGTH / 360.0) * angle;
					//�X�s�[�h�̏�����
					speed = 0;
				}
				//�t�b�N�܂ł̈ړ�����
				isHookMove = true;
			}
			//�ړ����I�������
			else {
				//�t�b�N�܂ł̈ړ�����
				isHookMove = false;
				//�X�e�[�g�̕ύX
				playerState = PLAYER_MOVE_STATE::HOOK;
				//�t�b�N�̍��W�Ƀv���C���[���ړ�
				playerX = hookX + nX;
				playerY = hookY + nY;
				// ���x�����Z
				speed += -mass * (G / 60) * sin(x / LENGTH);
				x += speed;
				// �������_�Ƃ��ĂԂ牺�����Ă��镨�̍��W���v�Z
				angle = x / LENGTH + PI / 2.0;
				nX = cos(angle) * LENGTH;
				nY = sin(angle) * LENGTH;
				if (speed >= 0) {
					if (input_lx < -15000) {
						speed += 0.05 * 2.0f;
					}
					if (input_lx > 15000) {
						speed -= 0.09 * 2.0f;
					}
				}
				else if (speed < 0) {
					if (input_lx < -15000) {
						speed += 0.09 * 2.0f;
					}
					if (input_lx > 15000) {
						speed -= 0.05 * 2.0f;
					}
				}
				if (hookY + nY < hookY) {
					nY = 0;
					speed = 0.0;
				}
				if (input_lx < 15000 && input_lx >-15000) {	//�����Ă���Ԃ͊p�x�������A�X�s�[�h��x�����Ă���
					if (speed > 0)speed -= 0.05;
					if (speed < 0)speed += 0.05;
				}
				StartJoypadVibration(DX_INPUT_PAD1, static_cast<int>(abs(speed) / 18.0 * 500.0), 1, -1);
			}
		}
	}
	//�t�b�N��������Ȃ�������
	if (!is_hook) {
		//������		
		isHookPendulumMove = false;
		hookMoveDistance = 0.0f;
		if (playerState == PLAYER_MOVE_STATE::HOOK || isHookMove) {
			//�t�b�N��̃W�����v�����̏C��
			StopSoundMem(hookPendulumSe);
			grabbedHookArray.push_back(hookIndex);

			if (input_lx < -DEVIATION) {
				jumpMoveX = -1;
			}
			else if (input_lx > DEVIATION) {
				jumpMoveX = 1;
			}
			else {
				if (nX < 0) {
					jumpMoveX = -1;
				}
				else {
					jumpMoveX = 1;
				}
			}
			if (playerState == PLAYER_MOVE_STATE::HOOK) {
				playerX = hookX + nX;
				playerY = hookY + nY;
				playerY += 1;
				jumpMode = 2;
				jumpPower = fabs(static_cast<float>(nX) / LENGTH);
				if (fabsf(nX) > LENGTH / 2) {
					jumpRequest = true;
				}
			}
			playerState = PLAYER_MOVE_STATE::FALL;
		}
		hookIndex = -1;
		isHookMove = false;
	}
}

/// <summary>
/// �v���C���[�̃W�����v����
/// </summary>
void PLAYER::JumpMove() {
#ifdef _DEBUG
	if (PAD_INPUT::GetNowKey() == XINPUT_BUTTON_Y || CheckHitKey(KEY_INPUT_SPACE))return;		//�f�o�b�O�p
#endif
	//A�{�^�����������Ƃ�
	if (PAD_INPUT::GetNowKey() == (Option::GetInputMode() ? XINPUT_BUTTON_A : XINPUT_BUTTON_B) || jumpRequest) {
		//�W�����v������Ȃ��Ƃ�
		if ((playerState != PLAYER_MOVE_STATE::JUMP && playerState != PLAYER_MOVE_STATE::FALL && playerState != PLAYER_MOVE_STATE::HOOK && isGround
			|| jumpRequest) && isGravity) {
			jumpRequest = false;
			isJump = true;			//�W�����v���Ɉڍs
			jumpVelocity = JUMP_VELOCITY * jumpPower;
			//���ړ����ĂȂ���
			if (playerState == PLAYER_MOVE_STATE::IDLE) {
				jumpMode = 1;
			}
			//���ړ����Ă�Ƃ�
			else if (playerState == PLAYER_MOVE_STATE::MOVE) {
				jumpMode = 2;
			}
			playerState = PLAYER_MOVE_STATE::JUMP;
			ChangeAnimation(PLAYER_ANIM_STATE::JUMP);
			PlaySoundMem(jumpSE, DX_PLAYTYPE_BACK);
		}
	}
	//�W�����v��
	if (isJump) {
		jumpVelocity += 0.2f;
		playerY += jumpVelocity;

		ChangeAnimation(PLAYER_ANIM_STATE::JUMP);

		if (jumpVelocity >= 0.0f || hitCeil || playerState == PLAYER_MOVE_STATE::HOOK || isHookMove) {
			isJump = false;
			hitCeil = false;
			jumpVelocity = 0;
		}
	}
	//������
	else {
		//�n�ʂ���Ȃ����͗���
		if (!isGround && isGravity) {
			jumpVelocity += 0.2f;
			playerY += jumpVelocity;
			playerState = PLAYER_MOVE_STATE::FALL;
			ChangeAnimation(PLAYER_ANIM_STATE::FALL);
		}
		//�n�ʂɂ�����
		else {
			if ((playerState == PLAYER_MOVE_STATE::FALL || playerState == PLAYER_MOVE_STATE::JUMP) && !isHookMove) {
				jumpPower = 1.0;
				float new_y = (float)(mapY - 1) * MAP_CEllSIZE + MAP_CEllSIZE / 2;
				if (fabsf(new_y - playerY) <= jumpVelocity) {
					playerY = new_y;
				}
				jumpVelocity = 0;
				grabbedHookArray.clear();
				playerState = PLAYER_MOVE_STATE::IDLE;
				ChangeAnimation(PLAYER_ANIM_STATE::LANDING);
				PlaySoundMem(landingSE, DX_PLAYTYPE_BACK);
			}
			if (playerState == PLAYER_MOVE_STATE::HOOK || isHookMove) {
				jumpVelocity = 0;
			}
		}
		if (animation_state == PLAYER_ANIM_STATE::LANDING) {
			if (animation[static_cast<int>(animation_state)].end_animation) {
				ChangeAnimation(PLAYER_ANIM_STATE::IDLE);
			}
		}
	}

}

/// <summary>
/// �����鏈��
/// </summary>
/// <param name="stage">STAGE�N���X�̃|�C���^</param>
void PLAYER::Throw(STAGE* stage) {
	//�O���̌v�Z
	throwIndex = 0;
	throwX.clear();
	throwY.clear();

	//�N�[���^�C��
	if (--throwInterval > 0) return;

	int input_ry = PAD_INPUT::GetPadThumbRY();
	int input_rx = PAD_INPUT::GetPadThumbRX();
	if ((abs(input_rx) <= DEVIATION && abs(input_ry) <= DEVIATION) || input_ry < DEVIATION) {
		return;
	}
	//�p�x�擾
	throwRad = atan2(input_ry, input_rx);
	float angle = throwRad * 180.0f / M_PI;
	//�p�x�̐���
	if (moveType == 0) {
		if (angle > 90) throwRad = 90 * M_PI / 180.0f;
		else if (angle < 30) throwRad = 30 * M_PI / 180.0f;
	}
	else {
		if (angle > 150) throwRad = 150 * M_PI / 180.0f;
		else if (angle < 90) throwRad = 90 * M_PI / 180.0f;
	}

	//�O���̌v�Z
	float ve = 110;

	float vx0 = ve * (float)cos(throwRad);
	float vy0 = ve * (float)sin(throwRad);

	float g = 9.8;
	float dt = 0.15f;

	float x0 = playerX + stage->GetScrollX();
	float y0 = playerY + stage->GetScrollY();

	float vx = vx0;
	float vy = vy0;

	float maxY = stage->GetMapSize().y * MAP_CEllSIZE;

	for (float t = 0.0; y0 <= maxY; t = t + dt) {
		x0 = x0 + vx * dt;
		y0 = y0 - vy * dt;
		vy = vy - g * dt;
		if (vy < 0) {
			g += 0.2f;
		}
		throwX.push_back(x0);
		throwY.push_back(y0);
	}
	for (int i = 0; i < 3; i++) {
		throwX.erase(throwX.begin());
	}
	for (int i = 0; i < 3; i++) {
		throwY.erase(throwY.begin());
	}

	//������Ƃ�
	if (PAD_INPUT::GetNowKey() == XINPUT_BUTTON_RIGHT_SHOULDER && PAD_INPUT::GetPadState() == PAD_STATE::ON) {
		//�����鏈��
		throwInterval = THROW_INTERVAL;
		throwSlime.push_back(ThrowSlime(playerX, playerY, throwRad, stage));
		PlaySoundMem(throwBallSe, DX_PLAYTYPE_BACK);
		ChangeAnimation(PLAYER_ANIM_STATE::THROW, true);
	}
}

/// <summary>
/// �����蔻��
/// </summary>
void PLAYER::Hit(ELEMENT* element, STAGE* stage) {
	//�}�b�v�`�b�v�̍��W�̃Z�b�g
	mapX = (int)roundf(playerX / MAP_CEllSIZE);
	mapY = (int)floorf((playerY + MAP_CEllSIZE / 2) / MAP_CEllSIZE);
	float player_left = playerX - 30 * playerScale;
	float player_right = playerX + 30 * playerScale;
	float old_player_left = oldPlayerX - 30 * playerScale;
	float old_player_right = oldPlayerX + 30 * playerScale;
	float player_top = (playerY - (playerScale - 0.6f) * MAP_CEllSIZE / 2);
	float player_bottom = (playerY + MAP_CEllSIZE / 2);

	//�V��̔���
	bool hit_ceil_center = stage->HitMapDat((int)(player_top / MAP_CEllSIZE), (int)(oldPlayerX / MAP_CEllSIZE));
	bool hit_ceil_left = stage->HitMapDat((int)(player_top / MAP_CEllSIZE), (int)((old_player_left + playerSpeed) / MAP_CEllSIZE));
	bool hit_ceil_right = stage->HitMapDat((int)(player_top / MAP_CEllSIZE), (int)((old_player_right - playerSpeed) / MAP_CEllSIZE));
	hitCeil = hit_ceil_center || hit_ceil_left || hit_ceil_right;

	//�n�ʂ̔���
	isGround = !isGravity;
	if (playerState == PLAYER_MOVE_STATE::HOOK || isHookMove) {
		isGround = true;
		return;
	}
	if (stage->HitMapDat((int)(player_bottom / MAP_CEllSIZE), (int)(playerX / MAP_CEllSIZE)) &&
		!stage->HitMapDat((int)(player_top / MAP_CEllSIZE), (int)(playerX / MAP_CEllSIZE)) &&
		!stage->HitMapDat((int)(playerY / MAP_CEllSIZE), (int)(playerX / MAP_CEllSIZE))) {
		isGround = true;
	}
	float diff_y = fabsf(playerY - oldPlayerY);
	if (fmodf(playerY, MAP_CEllSIZE / 2) <= diff_y) {
		if (stage->HitMapDat((int)(player_bottom / MAP_CEllSIZE), (int)(player_left / MAP_CEllSIZE)) &&
			!stage->HitMapDat((int)(player_top / MAP_CEllSIZE), (int)(player_left / MAP_CEllSIZE)) &&
			!stage->HitMapDat((int)(playerY / MAP_CEllSIZE), (int)(player_left / MAP_CEllSIZE))) {
			isGround = true;
		}
		if (stage->HitMapDat((int)(player_bottom / MAP_CEllSIZE), (int)(player_right / MAP_CEllSIZE)) &&
			!stage->HitMapDat((int)(player_top / MAP_CEllSIZE), (int)(player_right / MAP_CEllSIZE)) &&
			!stage->HitMapDat((int)(playerY / MAP_CEllSIZE), (int)(player_right / MAP_CEllSIZE))) {
			isGround = true;
		}
	}

	//�}���z�[���̔���
	int block_type_center = stage->GetMapData((int)(playerY / MAP_CEllSIZE), (int)(playerX / MAP_CEllSIZE));
	int block_type_top = stage->GetMapData((int)(player_top / MAP_CEllSIZE), (int)(playerX / MAP_CEllSIZE));
	int block_type_bottom = stage->GetMapData((int)(player_bottom / MAP_CEllSIZE), (int)(playerX / MAP_CEllSIZE));
	if (block_type_center == 98 || block_type_top == 98 || block_type_bottom == 98) {
		float diff = fabsf((float)((int)(playerX / MAP_CEllSIZE) * MAP_CEllSIZE) - player_left);
		if (diff < playerSpeed) {
			isGround = false;
		}
	}

	//�������̓����蔻��
	if (!hitCeil) {
		std::vector<ELEMENT::ELEMENT_DATA> lift = element->GetLift();
		int hit_lift_num = -1;
		for (int i = 0; i < lift.size(); i++) {
			if (player_right >= lift[i].x && player_left <= lift[i].x + LIFT_SIZE
				&& playerY >= lift[i].y - 45  && playerY <= lift[i].y && playerState != PLAYER_MOVE_STATE::JUMP) {

				if (((lift[i].type == 1 && !(lift[i].leftVectorY > 0 && isGround)) || (lift[i].type == 2 && !isGround)) &&
					(hit_lift_num == -1 || lift[hit_lift_num].y > lift[i].y)) {
					hit_lift_num = i;
				}
			}
		}
		if (hit_lift_num >= 0) {
			playerY = (lift[hit_lift_num].y - MAP_CEllSIZE / 2 + lift[hit_lift_num].leftVectorY * 4);
			playerX += lift[hit_lift_num].leftVectorX * 4;
			isGround = true;
		}
	}

	//�ǂ̔���
	int screen_left = static_cast<int>(-stage->GetScrollX() / MAP_CEllSIZE);
	for (int i = 0; i < stage->GetMapSize().x; i++) {
		for (int j = screen_left; j < screen_left + 20; j++) {
			if (j >= stage->GetMapSize().y) break;
			if (!stage->HitMapDat(i, j) && stage->GetMapData(i, j) != -1) continue;

			float block_left = j * MAP_CEllSIZE;
			float block_right = block_left + MAP_CEllSIZE;
			float block_top = i * MAP_CEllSIZE;
			float block_bottom = block_top + MAP_CEllSIZE;

			if (player_right > block_left && player_left < block_right) {
				if (player_bottom > block_top && player_top < block_bottom) {
					int block_type = stage->GetMapData(i, j);
					int y = static_cast<int>(player_top / MAP_CEllSIZE);
					//������
					if (block_type == -1) {
						playerState = PLAYER_MOVE_STATE::DEAD;
						return;
					}
					if (hitCeil && !isGround && y == i) continue;
					//�h�A�̔���
					if ((block_type == 66 || block_type == 67) && moveX > 0) {
						if (fabsf(player_left - block_right) < playerSpeed) {
							return;
						}
					}
					playerX = oldPlayerX;
					if (isHear) {
						playerX -= moveX * playerSpeed * 2.0f;
					}
					break;
				}
			}
		}
	}

	isHear = false;
}

/// <summary>
/// �A�j���[�V�����̐؂�ւ�
/// </summary>
/// <param name="anim">Enum�N���X �A�j���[�V����</param>
/// <param name="compelChange">�����I�ɐ؂�ւ��邩�ǂ���</param>
void PLAYER::ChangeAnimation(PLAYER_ANIM_STATE anim, bool compelChange) {
	int now_anim_type = static_cast<int>(animationState);	//���̃A�j���[�V����
	int next_anim_type = static_cast<int>(anim);			//�؂�ւ���A�j���[�V����
	
	if (animation_state != anim && player_state != PLAYER_MOVE_STATE::HOOK && !is_hook_move || compelChange) {
		if (animation[now_anim_type].priority <= animation[next_anim_type].priority || animation[now_anim_type].end_animation || compelChange) {
			animation_state = anim;
			int anim_type = static_cast<int>(anim);
			animation[anim_type].frame_count = 0;
			animation[anim_type].current_index = 0;
			animation[anim_type].end_animation = false;
		}
	}
}

/// <summary>
/// �A�j���[�V�����̐؂�ւ�
/// </summary>
void PLAYER::MoveAnimation() {
	//�摜�̐؂�ւ��^�C�~���O�̂Ƃ�
	int type = static_cast<int>(animation_state);
	if (++animation[type].frame_count >= animation[type].switch_frame) {
		animation[type].frame_count = 0;
		if (++animation[type].current_index >= animation[type].animation_indexes.size()) {
			animation[type].current_index = 0;
			animation[type].end_animation = true;
		}
	}
}

void PLAYER::SetLife(int _life)
{
	//�_���[�W����
	if (life > _life && isDamage) return;
	//�_���[�W
	if (life > _life) {
		//playerState = PLAYER_MOVE_STATE::DAMAGE;
		alphaTime = 120;
		isDamage = true;
		StartJoypadVibration(DX_INPUT_PAD1, 360, 320, -1);
		PlaySoundMem(damageSE, DX_PLAYTYPE_BACK);
	}
	//��
	else {
		isHear = true;
		PlaySoundMem(healSe, DX_PLAYTYPE_BACK);
	}
	life = _life;
}