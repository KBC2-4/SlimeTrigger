#include "DxLib.h"
#include "Lemon.h"
#include "Option.h"

LEMON::LEMON()
{
	//�摜�̎擾
	image = nullptr;
	damageSe = 0;

	shootCount = 0;
	hitFlg = false;
	deleteFlg = false;
	x = 0;
	spawnMapX = 0;
	spawnMapY = 0;
	nowImage = 0;

	State = ENEMY_STATE::IDOL;
	bullet = nullptr;
}


LEMON::LEMON(PLAYER* player, STAGE* stage, int spawn_y, int spawn_x)
{
	//�X�|�[���ꏊ�̐ݒ�
	spawnMapX = spawn_x;
	spawnMapY = spawn_y;
	x = (spawnMapX * MAP_CEllSIZE + MAP_CEllSIZE / 2);
	y = spawnMapY * MAP_CEllSIZE;
	hitFlg = false;
	deleteFlg = false;

	//�摜�̎擾
	image = new int[15];
	if (LoadDivGraph("Resource/Images/Enemy/lemon.png", 9, 9, 1, 80, 80, image) == -1)
	{
		throw "Resource/Images/Enemy/lemon.png";
	}
	if (LoadDivGraph("Resource/Images/Enemy/lemon_break.png", 6, 6, 1, 80, 80, image + 9) == -1)
	{
		throw "Resource/Images/Enemy/lemon_break.png";
	}
	//SE�̎擾
	if ((damageSe = LoadSoundMem("Resource/Sounds/SE/Enemy/damage.wav")) == -1) {
		throw "Resource/Sounds/SE/Enemy/damage.wav";
	}
	if ((pressSe = LoadSoundMem("Resource/Sounds/SE/Enemy/press.wav")) == -1) {
		throw "Resource/Sounds/SE/Enemy/press.wav";
	}
	if ((splashSe = LoadSoundMem("Resource/Sounds/SE/Enemy/splash.wav")) == -1) {
		throw "Resource/Sounds/SE/Enemy/splash.wav";
	}
	shootCount = 0;

	bullet = nullptr;
	this->player = player;
	this->stage = stage;


	nowImage = image[3];
	State = ENEMY_STATE::IDOL;
	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.2), damageSe);
	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 0.9), pressSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), splashSe);
}


LEMON::~LEMON()
{

	for (int i = 0; i < 15; i++)
	{
		DeleteGraph(image[i]);
	}


	delete[] image;
	if (bullet != nullptr)
	{
		delete bullet;
	}

	DeleteSoundMem(damageSe);
	DeleteSoundMem(pressSe);
	DeleteSoundMem(splashSe);
}

void LEMON::Update()
{

	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 1.2), damageSe);
	ChangeVolumeSoundMem(static_cast<int>(Option::GetSEVolume() * 0.9), pressSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), splashSe);

	if (animationTimer < 80) {
		++animationTimer;
	}

	switch (State)
	{
	case ENEMY_STATE::IDOL:
		break;
	case ENEMY_STATE::MOVE:
		ChangeAngle();
		if (player->GetMapY() > mapY)
		{
			if (++shootCount % 60 == 0)
			{
				animationTimer = 0;
				State = ENEMY_STATE::PRESS;

			}
		}
		break;
	case ENEMY_STATE::RETURN:
		ChangeAngle();
		if (ReturnAnimation())
		{
			animationTimer = 0;
			animationType = 0;
			State = ENEMY_STATE::MOVE;
		}
		break;
	case ENEMY_STATE::PRESS:
		ChangeAngle();
		if (PressAnimation())
		{
			bullet = new ENEMY_BULLET(player, stage, x, y, 0, rad, 1);
			animationTimer = 0;
			animationType = 0;
			State = ENEMY_STATE::RETURN;
			PlaySoundMem(pressSe, DX_PLAYTYPE_BACK);
		}
		break;
	case ENEMY_STATE::FALL:
		Move();
		FallAnimation();
		break;
	case ENEMY_STATE::DETH:
		if (DethAnimation())
		{
			deleteFlg = true;
		}
		break;
	default:
		break;
	}


	//�}�b�v��̍��W�̐ݒ�
	mapX = x / MAP_CEllSIZE;
	mapY = (y - IMAGE_SIZE / 2) / MAP_CEllSIZE;

	//�e�����݂��Ă���Ƃ��ɒe�̏������s��
	if (bullet != nullptr)
	{
		bullet->Update();

		//�e����ʊO�ɏo�邩�A�����ɓ���������e���폜
		if (bullet->GetDeleteFlg())
		{
			delete bullet;
			bullet = nullptr;
		}
	}

	Hit();

	if (((x + stage->GetScrollX() < -IMAGE_SIZE) || (x + stage->GetScrollX() > 1280 + IMAGE_SIZE) || (y + stage->GetScrollY() < 0)
		|| (y + stage->GetScrollY() > 720)) && (State != ENEMY_STATE::FALL && State != ENEMY_STATE::DETH))		//��ʊO�ɏo��ƃA�C�h����Ԃɂ���
	{
		State = ENEMY_STATE::IDOL;	//�X�e�[�g���A�C�h����Ԃ�
		//�A�C�h����Ԃ̉摜�ɕύX
		nowImage = image[3];
	}
	else if (State == ENEMY_STATE::IDOL)	//��ʓ��ɂ��āA�A�C�h����Ԃ̂Ƃ��G�̕����������悤�ɂ���
	{
		// �A�j���[�V�������Ԃ����Z�b�g���A�X�e�[�g�����[�u��
		animationTimer = 0;
		State = ENEMY_STATE::MOVE;
	}
	else {}
}


void LEMON::Move()
{
	y += 10;
}

void LEMON::Hit()
{
	ThrowSlime throwSlime;

	float bx1, by1, bx2, by2;
	float gx1, gy1, gx2, gy2;
	//�v���C���[���������̂̈ꕔ�Ƃ̓����蔻��
	if ((State != ENEMY_STATE::FALL) && (State != ENEMY_STATE::DETH))
	{
		for (int i = 0; i < player->GetThrowCnt(); i++)
		{
			throwSlime = player->GetThrowSlime(i);
			//�X���C���̃{�[���̓����蔻��
			bx1 = throwSlime.GetThrowX();
			by1 = throwSlime.GetThrowY();
			bx2 = throwSlime.GetThrowX() + BALL_W;
			by2 = throwSlime.GetThrowY() - BALL_H;
			//�����i�[�̓����蔻��
			gx1 = x - IMAGE_SIZE / 2.5;
			gy1 = y - IMAGE_SIZE / 2;
			gx2 = gx1 + IMAGE_SIZE / 1.25;
			gy2 = gy1 + IMAGE_SIZE;
			if (((bx2 >= gx1 && bx2 <= gx2) || (bx1 <= gx2 && bx1 >= gx1)) && ((by1 >= gy2 && by1 <= gy1) || (by2 >= gy1 && by2 <= gy2)))
			{
				rad = 90 * (PI / 180);
				State = ENEMY_STATE::FALL;
				PlaySoundMem(damageSe, DX_PLAYTYPE_BACK);
			}
		}
	}

	//�n�ʂ�u���b�N�Ƃ̓����蔻��
	if (State == ENEMY_STATE::FALL)
	{
		if (stage->HitMapDat(mapY + 1, mapX))
		{
			State = ENEMY_STATE::DETH;
			animationTimer = 0;
			animationType = 0;
			PlaySoundMem(splashSe, DX_PLAYTYPE_BACK);
		}
	}
}
bool LEMON::PressAnimation()
{

	bool ret = false;
	if (animationTimer < 40) //40�t���[���ԃA�j���[�V����������
	{
		if (animationTimer % (ANIMATION_TIME * 2) == 0)
		{
			if (animationType < 2)
			{
				nowImage = image[(animationType++ % 7) + 4];
			}
			else
			{
				nowImage = image[0];
			}

		}
	}
	else //�A�j���[�V�����̏I��
	{
		ret = true;
	}
	return ret;
}

bool LEMON::ReturnAnimation()
{
	bool ret = false;
	if (animationTimer < 50) //50�t���[���ԃA�j���[�V����������
	{
		if (animationTimer % (ANIMATION_TIME * 2) == 0)
		{
			nowImage = image[(animationType++ % 7)];
		}
	}
	else //�A�j���[�V�����̏I��
	{
		ret = true;
	}
	return ret;
}

void LEMON::FallAnimation()
{
	if (animationTimer % ANIMATION_TIME == 0)
	{
		nowImage = image[(++animationType % 2) + 7];
	}
}

bool LEMON::DethAnimation()
{
	bool ret = false;
	if (animationTimer < 30) //30�t���[���ԃA�j���[�V����������
	{
		//�A�j���[�V����
		if (animationTimer % ANIMATION_TIME == 0)
		{
			nowImage = image[(++animationType % 6) + 9];
		}
	}
	else //�A�j���[�V�����̏I��
	{
		ret = true;
	}
	return ret;
}

void LEMON::Draw() const
{
	float gx1, gy1, gx2, gy2;

	if (bullet != nullptr)		//�e�����݂���Ƃ��e��`�悷��
	{
		bullet->Draw();
	}
	DrawRotaGraphF(x + stage->GetScrollX(), y + stage->GetScrollY(), 1, rad + (-90 * (PI / 180)), nowImage, TRUE);
}