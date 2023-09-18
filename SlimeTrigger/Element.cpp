#include "Element.h"
#include "Player.h"
#include "Option.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ELEMENT::ELEMENT(const char* stageName) : STAGE(stageName) {

	guidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	keyboardGuidFont = CreateFontToHandle("���C���I", 9, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	if ((doorCloseSe = LoadSoundMem("Resource/Sounds/SE/Stage/door_close.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/door_close.wav";
	}

	if ((pressTheButtonSe = LoadSoundMem("Resource/Sounds/SE/Stage/press_the_button.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/press_the_button.wav";
	}

	if ((switchSe = LoadSoundMem("Resource/Sounds/SE/Stage/switch.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/switch.wav";
	}

	if ((walkPuddleSe = LoadSoundMem("Resource/Sounds/SE/Stage/walk_puddle.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/walk_puddle.wav";
	}

	if ((manholeOpenedSe = LoadSoundMem("Resource/Sounds/SE/Stage/manhole_opened.wav")) == -1) {
		throw "Resource/Sounds/SE/Stage/manhole_opened.wav";
	}

	//�_���܂�̉摜�ǂݍ���
	LoadDivGraph("Resource/Images/Stage/acid.png",12,12,1,80,80,acidImage);
	//�A�A�j���[�V�����̏�����
	foamAnimation = 4;

	//�������̉摜�ǂݍ���
	LoadDivGraph("Resource/Images/Stage/MoveFloor.png", 3, 3, 1, 80, 80, moveFloorImage);
	//�A�A�j���[�V�����̏�����
	moveFloorAnimation = 0;

	ELEMENT_DATA data;
	for (int i = 0; i < mapData.size(); i++)
	{
		for (int j = 0; j < mapData.at(0).size(); j++)
		{
			int search_vector = 1;
			//�T�����W
			int lift_pos = i + 1;
			int button_num_1 = 0;
			int button_num_2 = 0;
			int door_num = 0;
			switch (mapData.at(i).at(j))
			{
				//�{�^����
			case 61:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2) + 25);
				data.y = static_cast<float>(i * MAP_CEllSIZE + MAP_CEllSIZE / 2);
				data.type = 1;
				data.flag = false;
				data.animTimer = 0;

				//�{�^���ƃh�A�̘A�g�ԍ����i�[
				while (button_num_1 < buttonInfo.size()) {
					if (buttonInfo.at(button_num_1).at(0) == i && buttonInfo.at(button_num_1).at(1) == j) {
						data.pairNum = buttonInfo.at(button_num_1).at(2);
						break;
					}
					button_num_1++;
				}

				button.push_back(data);
				break;

				//�{�^��(��)
			case 62:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.type = 2;
				data.flag = false;
				data.animTimer = 0;

				//�{�^���ƃh�A�̘A�g�ԍ����i�[
				while (button_num_2 < buttonInfo.size()) {
					if (buttonInfo.at(button_num_2).at(0) == i && buttonInfo.at(button_num_2).at(1) == j) {
						data.pairNum = buttonInfo.at(button_num_2).at(2);
						break;
					}
					button_num_2++;

				}

				button.push_back(data);
				break;


				//�{�^��(ones)��
			case 63:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2) + 25);
				data.type = 3;
				data.flag = false;
				data.animTimer = 0;
				button.push_back(data);
				break;

				//�܂��Ă���h�A�̉�
			case 66:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.type = 1;
				data.flag = false;
				data.animTimer = 0;

				//�{�^���ƃh�A�̘A�g�ԍ����i�[
				while (door_num < doorInfo.size()) {
					if (doorInfo.at(door_num).at(0) == i && doorInfo.at(door_num).at(1) == j) {
						data.pairNum = doorInfo.at(door_num).at(2);
						break;
					}
					door_num++;
				}

				door.push_back(data);
				break;

				//�}���z�[���̊W
			case 68:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.leftInitX = j;		//�z��f�[�^
				data.leftInitY = i;		//�z��f�[�^
				data.pairNum = 0;			//���Z�����t���O
				data.liftWaitTimer = 0;	//���Ԓn�_�̐��J�E���g�p
				data.type = 1;
				data.flag = false;
				data.animTimer = 0;
				data.liftWaitTimer = 0;	//�ÈŃG�t�F�N�g
				manhole.push_back(data);
				break;

				//�}���z�[��(����)
			case 69:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.leftInitX = j;		//�z��f�[�^
				data.leftInitY = i;		//�z��f�[�^
				data.pairNum = 0;			//���Z�����t���O
				data.type = 2;
				data.flag = false;
				data.animTimer = 0;
				manhole.push_back(data);
				break;

				//�}���z�[��(�o��)
			case 97:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.leftInitX = j;		//�z��f�[�^
				data.leftInitY = i;		//�z��f�[�^
				data.liftWaitTimer = 0;	//���Ԓn�_�̐��J�E���g�p
				data.type = 3;
				data.flag = false;
				data.animTimer = 0;
				manhole.push_back(data);
				break;

				//�_���J�̐����܂�(���[)
			case 74:
			case 75:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.type = 1;
				data.animTimer = 0;
				data.flag = false;
				acidrainPuddles.push_back(data);
				break;

				//�_���J�̐����܂�(����)
			case 76:
			case 77:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.type = 2;
				data.animTimer = 0;
				data.flag = false;
				data.image = 99;
				acidrainPuddles.push_back(data);
				break;

				//�_���J�̐����܂�(�E�[)
			case 78:
			case 79:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.type = 3;
				data.animTimer = 0;
				data.flag = false;
				acidrainPuddles.push_back(data);
				break;

				//�t�b�N
			case 72:
				data.x = static_cast<float>((j * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				data.y = static_cast<float>((i * MAP_CEllSIZE + MAP_CEllSIZE / 2));
				hook.push_back(data);
				break;

				//������(�c�ړ�)
			case 51:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.leftInitX = data.x;
				data.leftInitY = data.y;
				data.leftVectorX = 0;
				data.leftVectorY = 0;
				data.liftWaitTimer = 0;
				
				if (mapData.size() < lift_pos)
				{
					lift_pos = i - 1;
					search_vector = -1;
				}
				//�S�[���ʒu���i�[
				for (lift_pos; lift_pos >= 0 && lift_pos < mapData.size(); lift_pos += search_vector) {
					if (mapData.at(lift_pos).at(j) == 53) {
						data.leftGoalX = static_cast<float>((j * MAP_CEllSIZE));
						data.leftGoalY = static_cast<float>((lift_pos * MAP_CEllSIZE));
						break;
					}
					else if (mapData.at(lift_pos).at(j) > 0 && (mapData.at(i).at(lift_pos) < 21 || 32 < mapData.at(i).at(lift_pos) && mapData.at(i).at(lift_pos) != 71 && mapData.at(i).at(lift_pos) != 72 && mapData.at(i).at(lift_pos) < 91 && 93 < mapData.at(i).at(lift_pos)))
					{
						if (search_vector < 0) { break; }
						lift_pos = i - 1;
						search_vector = -1;
					}
					else if (mapData.size() <= lift_pos + 1)
					{
						lift_pos = i - 1;
						search_vector = -1;
					}
				}
				data.type = 1;
				data.flag = false;
				data.animTimer = 0;
				data.image = 0;
				lift.push_back(data);
				break;
				//������(���ړ�)
			case 52:
				data.x = static_cast<float>((j * MAP_CEllSIZE));
				data.y = static_cast<float>((i * MAP_CEllSIZE));
				data.leftInitX = data.x;
				data.leftInitY = data.y;
				data.leftVectorX = 0;
				data.leftVectorY = 0;
				data.liftWaitTimer = 0;
				//�S�[���ʒu���i�[
				for (int lift_pos = j + 1; lift_pos >= 0 && lift_pos < mapData.at(0).size(); lift_pos += search_vector) {
					if (mapData.at(i).at(lift_pos) == 54) {
						data.leftGoalX = static_cast<float>((lift_pos * MAP_CEllSIZE));
						data.leftGoalY = static_cast<float>((i * MAP_CEllSIZE));
						break;
					}//�i�H�����ɏ�Q��������΃S�[���T���ʒu���t�ɂ���
					if (0 < mapData.at(i).at(lift_pos) && (mapData.at(i).at(lift_pos) < 21 || 32 < mapData.at(i).at(lift_pos) && mapData.at(i).at(lift_pos) != 71 && mapData.at(i).at(lift_pos) != 72 && mapData.at(i).at(lift_pos) < 91 && 93 < mapData.at(i).at(lift_pos))) {
						if (search_vector < 0) { break; }
						lift_pos = j - 1;
						search_vector *= -1;
						continue;
					}
				}
				data.type = 2;
				data.flag = false;
				data.animTimer = 0;
				data.image = 0;
				lift.push_back(data);
				break;

			default:
				break;
			}


		}

	}

	if (0 < acidrainPuddles.size())
	{
		acidrainPuddles.at(0).image = 1;
	}

	playerMapX = 0;
	playerMapY = 0;
	keepPushing = false;

	playerState = 0;
	guidTimer = 0;
	acidrainPuddlesAniTimer = 0;

	hookFlag = false;
	isInUnder = false;

	//SE
	ChangeVolumeSoundMem(Option::GetSEVolume(), doorCloseSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), pressTheButtonSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), switchSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), walkPuddleSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), manholeOpenedSe);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ELEMENT::~ELEMENT() {
	DeleteFontToHandle(guidFont);
	DeleteSoundMem(doorCloseSe);
	DeleteSoundMem(pressTheButtonSe);
	DeleteSoundMem(switchSe);
	DeleteSoundMem(walkPuddleSe);
	DeleteSoundMem(manholeOpenedSe);

	//�t��1MB���������򂤂̂ł��Ȃ���
	/*hook.clear();
	hook.shrink_to_fit();*/

	underGroundEffects = 0;
}

/// <summary>
/// �`��
/// </summary>
void ELEMENT::Draw(STAGE* stage, PLAYER* player) {
	//DrawFormatString(100, 50, 0xffffff, "%2f %2f", scrollX, scrollY);
	//DrawFormatString(100,50,0xffffff,"mapData:%d",mapData[int(playerMapY) / MAP_CEllSIZE + 1][int(playerMapX) / MAP_CEllSIZE]);
	//static int animTimer = 0;
	//printfDx("%d", animTimer);
	//DrawFormatString(200, 100, 0xFFFFFF, "acidrainPuddles.x%f\acidrainPuddles.y%f", acidrainPuddles[1].x, acidrainPuddles[1].y);
	//DrawFormatString(200, 200, 0xFFFFFF, "x%f\nY%f", playerMapX, playerMapY);
	//�f�o�b�O�p



	//�t�b�N�̃K�C�h�\��
	for (int i = 0; i < hook.size(); i++)
	{
		if (hook[i].flag == true)
		{
			if (playerState != static_cast<int>(PLAYER_MOVE_STATE::HOOK))
			{
				DrawGuide(hook[i].x, hook[i].y, stage);
			}
		}
	}

	//�{�^��
	for (int i = 0; i < button.size(); i++) {

		if (button[i].type == 2) {

			if (button[i].flag == false) {
				DrawOvalAA(button[i].x + stage->GetScrollX(), button[i].y + stage->GetScrollY() + 30, 25, 10, 20, 0xbfcb4e, TRUE, 1.0f);
			}
			if (button[i].flag == true && button[i].animTimer < 30) {
				SetDrawArea(static_cast<int>(button[i].x + stage->GetScrollX()) - MAP_CEllSIZE / 2, static_cast<int>( button[i].y + stage->GetScrollY()) - MAP_CEllSIZE / 2, static_cast<int>( button[i].x + stage->GetScrollX()) + MAP_CEllSIZE / 2, static_cast<int>(button[i].y + stage->GetScrollY()) + MAP_CEllSIZE / 2);
				DrawOvalAA(button[i].x + stage->GetScrollX(), button[i].y + stage->GetScrollY() + 30 + button[i].animTimer, 25, 10, 20, 0xbfcb4e, TRUE, 1.0f);
				SetDrawAreaFull();
			}

			DrawGraphF(button[i].x + stage->GetScrollX() - MAP_CEllSIZE / 2, button[i].y + stage->GetScrollY() - MAP_CEllSIZE / 2, blockImage1[61], TRUE);
		}
	}

	//������
	for (int i = 0; i < lift.size(); i++) {
		//DrawFormatString(100+i*100, 400, 0xffffff, "%d", lift[i].leftVectorY);
		//DrawFormatString(100 + i * 100, 400, 0xffffff, "%f", lift[i].x);
		/*DrawFormatString(100+i*100, 400, 0xffffff, "%f", lift_goal_X[i].x);
		DrawBox(lift_goal_X[i].x + stage->GetScrollX(), lift_goal_X[i].y + stage->GetScrollY(), lift_goal_X[i].x + MAP_CEllSIZE * 2 + stage->GetScrollX(), lift_goal_X[i].y + MAP_CEllSIZE / 2 + stage->GetScrollY(),0xff0000,FALSE);*/
		//DrawExtendGraphF(lift[i].x + stage->GetScrollX(), lift[i].y - 31 + stage->GetScrollY(), lift[i].x + LIFT_SIZE + stage->GetScrollX(), lift[i].y + 70 + stage->GetScrollY(), blockImage1[51], TRUE);
		DrawExtendGraphF(lift[i].x + stage->GetScrollX(), lift[i].y - 50 + stage->GetScrollY(), lift[i].x + LIFT_SIZE + stage->GetScrollX(), lift[i].y + 70 + stage->GetScrollY(), moveFloorImage[lift[i].image], TRUE);
	}

	//�h�A
	for (int i = 0; i < door.size(); i++) {
		//if (button[i].type == 2 && button[i].flag == false)DrawOvalAA(button[i].x + scrollX, button[i].y + scrollY + 30, 25, 10, 20, 0xbfcb4e, TRUE, 1.0f);
		if (door[i].flag == true) {

		}
	}

	//�}���z�[��
	for (int i = 0; i < manhole.size(); i++) {

		if (manhole[i].type == 1) {
			if (manhole[i].flag == true)
			{
				if (manhole[i].animTimer < 20)
				{
					DrawModiGraphF(manhole[i].x + stage->GetScrollX(),
					               manhole[i].y + stage->GetScrollY() - manhole[i].animTimer * (288 / 20),
					               manhole[i].x + stage->GetScrollX() + MAP_CEllSIZE,
					               manhole[i].y + stage->GetScrollY() - manhole[i].animTimer * (288 / 20),
					               manhole[i].x + stage->GetScrollX() + MAP_CEllSIZE,
					               manhole[i].y + stage->GetScrollY() + MAP_CEllSIZE,
					               manhole[i].x + stage->GetScrollX(),
					               manhole[i].y + stage->GetScrollY() + MAP_CEllSIZE,
					               blockImage1[67], TRUE);
				}
				else
				{
					DrawGraphF(manhole[i].x + stage->GetScrollX(), manhole[i].y + stage->GetScrollY(), blockImage1[97],
					           TRUE);
				}
			}
			else
			{
				DrawGraphF(manhole[i].x + stage->GetScrollX(), manhole[i].y + stage->GetScrollY(), blockImage1[67],
				           TRUE);

				//�}���z�[���̃K�C�h�\��
				DrawGuide(manhole[i].x + MAP_CEllSIZE / 2, manhole[i].y + MAP_CEllSIZE - 20, stage);
			}
		}

		if (manhole[i].type == 3) {
			if (manhole[i].flag == true) {
				DrawGraphF(manhole[i].x + stage->GetScrollX() - MAP_CEllSIZE / 2, manhole[i].y + stage->GetScrollY() - MAP_CEllSIZE / 2, blockImage1[97], TRUE);
			}
			else {
				DrawGraphF(manhole[i].x + stage->GetScrollX() - MAP_CEllSIZE / 2, manhole[i].y + stage->GetScrollY() - MAP_CEllSIZE / 2, blockImage1[67], TRUE);
			}
			//�}���z�[���̃K�C�h�\��

			int x = static_cast<int>(manhole[i].leftInitX) * MAP_CEllSIZE + MAP_CEllSIZE / 2;
			int y = static_cast<int>(manhole[i].leftInitY) * MAP_CEllSIZE + MAP_CEllSIZE / 2;
			DrawGuide(x, y - 20, stage);
		}
	}


	//�_���J�̐����܂�
	for (int i = 0; i < acidrainPuddles.size(); i++) {

		/*if (stageName == "Stage02") { DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[7], TRUE); }
		else if (stageName == "Stage03") { DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[11], TRUE); }
		else { DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[3], TRUE); }*/

		int imageNum = acidrainPuddles.at(i).image;
		if (imageNum == 99)
		{
			imageNum = 0;
		}

		switch (acidrainPuddles[i].type)
		{
		//case 1:		//���[
		//	if (acidrainPuddlesAniTimer > 5) {
		//		DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[74], TRUE);
		//	}
		//	else {
		//		DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[73], TRUE);
		//	}
		//	break;

		case 2:		//����
			/*if (acidrainPuddlesAniTimer > 5) {
				DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[76], TRUE);
			}
			else {
				DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[75], TRUE);
			}*/

			DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), acidImage[imageNum], TRUE);
			DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), acidImage[foamAnimation], TRUE);
			DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY()+80.0f, acidImage[0], TRUE);
			//DrawBoxAA(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY() + 80.0f, acidrainPuddles[i].x + stage->GetScrollX()+80.0f, acidrainPuddles[i].y + stage->GetScrollY() + 160.0f, 0xc731e8, TRUE);
			//DrawGraphF(acidrain_puddles[i].x + stage->GetScrollX(), acidrain_puddles[i].y + stage->GetScrollY(), acid, TRUE);
			break;

		//case 3:		//�E�[
		//	if (acidrainPuddlesAniTimer > 5) {
		//		DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[78], TRUE);
		//	}
		//	else {
		//		DrawGraphF(acidrainPuddles[i].x + stage->GetScrollX(), acidrainPuddles[i].y + stage->GetScrollY(), blockImage1[77], TRUE);
		//	}
		//	break;
		default:
			break;
		}
	}


	//�n���G�t�F�N�g
	if (underGroundEffects != 0) {
		DrawCircleAA(player->GetPlayerX(), player->GetPlayerY(), 1200.0F - underGroundEffects * 3, 32, 0x000000, FALSE, 1200.0F);

	}

}

/// <summary>
/// �X�V
/// </summary>
void ELEMENT::Update(PLAYER* player, STAGE* stage) {

	ChangeVolumeSoundMem(Option::GetSEVolume(), doorCloseSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), pressTheButtonSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), switchSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), walkPuddleSe);
	ChangeVolumeSoundMem(Option::GetSEVolume(), manholeOpenedSe);

	playerState = static_cast<int>(player->GetPlayerMoveState());
	//�v���C���[�̃}�b�v�����W��ݒ�
	playerMapX = roundf(player->GetPlayerX() - stage->GetScrollX());
	playerMapY = floorf(player->GetPlayerY() - stage->GetScrollY());

	Button(player);
	Door(stage);
	Lift(player, stage);
	Manhole(player, stage);
	Acidrain_puddles(player);

	if (guidTimer < 100) { guidTimer++; }
	else { guidTimer = 0; }

	Hook_Distance(player, stage);	//�t�b�N�̃K�C�h�\���p�����v�Z
}

void ELEMENT::DrawGuide(const float elementX, const float elementY, STAGE* stage, const int offsetX, const int offsetY) const
{
	float x1 = elementX + offsetX + stage->GetScrollX();
	float y1 = elementY + offsetY + stage->GetScrollY();
	const int currentInputMode = PAD_INPUT::GetInputMode();

	if (currentInputMode == static_cast<int>(PAD_INPUT::InputMode::XINPUT_GAMEPAD) ||
		currentInputMode == static_cast<int>(PAD_INPUT::InputMode::DIRECTINPUT_GAMEPAD))
	{
		int color = (guidTimer < 50) ? 0xFFFFFF : 0xFFCB33;
		DrawCircleAA(x1, y1, 15, 20, color, 1);

		std::string buttonText = Option::GetInputMode() ? "B" : "A";
		int textColor = Option::GetInputMode() ? B_COLOR : A_COLOR;
		DrawStringToHandle(static_cast<int>(x1) - 7, static_cast<int>(y1) - 12, buttonText.c_str(), textColor, guidFont, 0xFFFFFF);
	}
	else if (currentInputMode == static_cast<int>(PAD_INPUT::InputMode::KEYBOARD))
	{
		float width = 20;
		float height = 0;

		if (!Option::GetInputMode())
		{
			x1 += 6;
			width = 10;
			height = 10;
		}

		int color = (guidTimer < 50) ? 0xFFFFFF : 0xFFCB33;
		DrawBoxAA(x1 - 20, y1 - 20, x1 + width, y1 + height, color, 1);

		if (Option::GetInputMode())
		{
			DrawStringToHandle(static_cast<int>(x1) - 20, static_cast<int>(y1) - 15, "SPACE", B_COLOR, keyboardGuidFont, 0xFFFFFF);
		}
		else
		{
			DrawStringToHandle(static_cast<int>(x1) - 12, static_cast<int>(y1) - 17, "Z", A_COLOR, guidFont, 0xFFFFFF);
		}
	}
}

/// <summary>
/// 3��ނ̃{�^���̏���
/// </summary>
void ELEMENT::Button(PLAYER* player) {


	for (int i = 0; i < button.size(); i++) {
		if (button[i].flag == true)button[i].animTimer++;
		if (button[i].animTimer > 180 && keepPushing == false) {
			button[i].animTimer = 0;
			button[i].flag = false;
		}

		if (button[i].type == 1) {		//���{�^��
			int max_ball_num = player->GetThrowCnt();
			for (int ball_num = 0; ball_num < max_ball_num; ball_num++) {
				if ((player->GetThrowSlime(ball_num).GetThrowX() >= button[i].x - MAP_CEllSIZE / 2 + 33) && (player->GetThrowSlime(ball_num).GetThrowX() <= button[i].x + MAP_CEllSIZE / 2 - 30) && (player->GetThrowSlime(ball_num).GetThrowY() >= button[i].y - MAP_CEllSIZE / 2) && (player->GetThrowSlime(ball_num).GetThrowY() <= button[i].y + MAP_CEllSIZE / 2)) {
					//�f�o�b�O
					//printfDx("1�Ԃɓ����Ă��I");
					if (CheckSoundMem(pressTheButtonSe) == FALSE)PlaySoundMem(pressTheButtonSe, DX_PLAYTYPE_BACK, TRUE);
					/*if (i < door.size() && i >= 0) {
						int j = i + 1;
						door[i].flag = true;
					}*/
					int k = 0;
					while (k < door.size()) {
						if (button[i].pairNum == door[k].pairNum) {
							door[k].flag = true;
							break;
						}
						k++;
					}

				}
			}
		}
		if (button[i].type == 2) {	//�{�^��
			bool ball_flg = false;	//�{�[���̓����蔻��t���O
			for (int ball_num = 0; ball_num < player->GetThrowCnt(); ball_num++) {
				if ((player->GetThrowSlime(ball_num).GetThrowX() >= button[i].x - MAP_CEllSIZE / 2 + 10) && (player->GetThrowSlime(ball_num).GetThrowX() <= button[i].x + MAP_CEllSIZE / 2 - 10) && (player->GetThrowSlime(ball_num).GetThrowY() >= button[i].y - MAP_CEllSIZE / 2) && (player->GetThrowSlime(ball_num).GetThrowY() <= button[i].y + MAP_CEllSIZE / 2)) {
					ball_flg = true;
				}
				else { keepPushing = false; }
			}

			if ((ball_flg == true) || ((playerMapX >= button[i].x - MAP_CEllSIZE + 25) && (playerMapX <= button[i].x + MAP_CEllSIZE - 25) && (playerMapY >= button[i].y - MAP_CEllSIZE / 2) && (playerMapY <= button[i].y + MAP_CEllSIZE / 2))) {
				//�f�o�b�O
				//printfDx("2�Ԃɓ����Ă��I");
				// �����蔻��o�O��׈ꎞ��~
				//if(ball_flg == false)player->SetPlayerY(button[i].y - 6.0f);
				if (button[i].flag == false) {
					if (CheckSoundMem(switchSe) == FALSE)PlaySoundMem(switchSe, DX_PLAYTYPE_BACK, TRUE);
				}
				keepPushing = true; //���������Ă���
				button[i].flag = true;		//�{�^����������

				/*if (i < door.size() && i >= 0) {
					int j = i - 1;
					door[i].flag = true;
				}*/
				int l = 0;
				while (l < door.size()) {
					if (button[i].pairNum == door[l].pairNum) {
						door[l].flag = true;
						break;
					}
					l++;
				}
			}
			else { keepPushing = false; }
		}

		//������
		if (button[i].type == 3) {	//Onec�{�^��
			if ((playerMapX >= button[i].x - MAP_CEllSIZE / 2) && (playerMapX <= button[i].x + MAP_CEllSIZE / 2) && (playerMapY >= button[i].y - MAP_CEllSIZE / 2) && (playerMapY <= button[i].y + MAP_CEllSIZE / 2)) {
				//�f�o�b�O
				//printfDx("3�Ԃɓ����Ă��I");
			}
		}
	}
}

/// <summary>
/// �h�A�̏���
/// </summary>
void ELEMENT::Door(STAGE* stage) {
	for (int i = 0; i < door.size(); i++) {
		if (door[i].flag == true) {
			int x = floor(door[i].x / MAP_CEllSIZE);
			int y = floor(door[i].y / MAP_CEllSIZE);
			stage->SetMapData(y, x, 64);
			stage->SetMapData(y - 1, x, 65);
		}

		if (door[i].flag==true&&(playerMapX >= door[i].x + MAP_CEllSIZE) && (playerMapX <= door[i].x + MAP_CEllSIZE + 10) && (playerMapY >= door[i].y - MAP_CEllSIZE *1.5) && (playerMapY <= door[i].y + MAP_CEllSIZE / 2)) {
			int x = floor(door[i].x / MAP_CEllSIZE);
			int y = floor(door[i].y / MAP_CEllSIZE);
			stage->SetMapData(y, x, 66);
			stage->SetMapData(y - 1, x, 67);
			if (CheckSoundMem(doorCloseSe) == FALSE)PlaySoundMem(doorCloseSe, DX_PLAYTYPE_BACK, TRUE);
			door[i].flag = false;
		}
	}
}

/// <summary>
/// �������̏���
/// </summary>
void ELEMENT::Lift(PLAYER* player, STAGE* stage) {

	//�v���C���[���W�擾
	float pX = player->GetPlayerX() - stage->GetScrollX();
	float pY = player->GetPlayerY() - stage->GetScrollY();

	for (int i = 0; i < lift.size(); i++) {

		if (lift[i].flag == false)
		{
			if (lift[i].x - MAP_CEllSIZE / 2 <= pX &&
				lift[i].y - MAP_CEllSIZE / 2 <= pY && pY <= lift[i].y + MAP_CEllSIZE)
			{
				lift[i].flag = true;
			}
		}
		else
		{
			//�ړ�����

			//������(�c)�̓���
			if (lift[i].type == 1) {
				if (lift[i].y < lift[i].leftGoalY) { lift[i].leftVectorY = 1; }
				else if (lift[i].y > lift[i].leftGoalY) { lift[i].leftVectorY = -1; }
				else { lift[i].leftVectorY = 0; }
				if (lift[i].y != lift[i].leftGoalY) 
				{
					lift[i].y += lift[i].leftVectorY * 4;
				}
				else 
				{
					lift[i].liftWaitTimer++;
					if (lift[i].liftWaitTimer == 90) {
						float work = lift[i].leftGoalY;
						lift[i].leftGoalY = lift[i].leftInitY;
						lift[i].leftInitY = work;
						lift[i].liftWaitTimer = 0;
					}
				}
			}
			//������(��)�̓���
			else if (lift[i].type == 2) 
			{
				if (lift[i].x < lift[i].leftGoalX) { lift[i].leftVectorX = 1; }
				else if (lift[i].x > lift[i].leftGoalX) { lift[i].leftVectorX = -1; }
				else { lift[i].leftVectorX = 0; }

				if (lift[i].liftWaitTimer != 60)
				{
					lift[i].liftWaitTimer++;
				}
				else
				{
					if (lift[i].x != lift[i].leftGoalX)
					{
						lift[i].x += lift[i].leftVectorX * 4;
					}
					else
					{
						lift[i].liftWaitTimer = 0;
						float work = lift[i].leftGoalX;
						lift[i].leftGoalX = lift[i].leftInitX;
						lift[i].leftInitX = work;
					}
				}
			}
		}
		//�A�j���[�V��������
		if (lift[i].animTimer < 8)
		{
			lift[i].animTimer++;
		}
		else
		{
			if (lift[i].image < 2)
			{
				lift[i].image++;
			}
			else
			{
				lift[i].image = 0;
			}

			lift[i].animTimer = 0;
		}
	}
}

/// <summary>
/// �������̓����蔻��
/// </summary>
bool ELEMENT::HitLift(PLAYER* player) {
	for (int i = 0; i < lift.size(); i++) {
		if (playerMapX + player->GetPlayerScale() * 25 >= lift[i].x && playerMapX - player->GetPlayerScale() * 25 <= lift[i].x + LIFT_SIZE
			&& playerMapY >= lift[i].y - MAP_CEllSIZE && playerMapY <= lift[i].y + 10 && player->GetPlayerMoveState() != PLAYER_MOVE_STATE::JUMP
			/*&& (mapData[int(playerMapY) / MAP_CEllSIZE + 1][int(playerMapX) / MAP_CEllSIZE] == 0 || mapData[int(playerMapY) / MAP_CEllSIZE + 1][int(playerMapX) / MAP_CEllSIZE] >= 51)*/) {

			player->SetPlayerY(lift[i].y - MAP_CEllSIZE / 2);

			player->SetPlayerY(lift[i].y - MAP_CEllSIZE / 2 + lift[i].leftVectorY * 4);
			player->SetPlayerX(player->GetPlayerX() + lift[i].leftVectorX * 4);
			return true;
		}
	}
	return false;
}


/// <summary>
/// �}���z�[���̏���
/// </summary>
void ELEMENT::Manhole(PLAYER* player, STAGE* stage) {
	for (int i = 0; i < manhole.size(); i++) {
		if (manhole[i].flag == true && manhole[i].animTimer < 20)manhole[i].animTimer++;
		if (manhole[i].animTimer > 20) {
			//manhole[i].animTimer = 0;
			//manhole[i].flag = false;
		}
		if (manhole[i].type == 1) {

			int mapdata_down = stage->GetMapData(manhole[i].leftInitY + 1, manhole[i].leftInitX);

			//���Ԓn�_�𔲂��鉺�̍��W���擾����
			//���Ԓn�_�̎��Ƀ��[�v
			while (mapdata_down == 69 && manhole[i].pairNum == 0)
			{
				manhole[i].leftInitY++;
				mapdata_down = stage->GetMapData(manhole[i].leftInitY, manhole[i].leftInitY);
			}

			//���Z�����t���O
			manhole[i].pairNum = 1;

			if ((playerMapX >= manhole[i].x) && (playerMapX <= manhole[i].x + MAP_CEllSIZE) && (playerMapY <= manhole[i].y + MAP_CEllSIZE) && (playerMapY >= manhole[i].y)) {
				if (PAD_INPUT::OnPressed(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) {
					stage->SetTemporary_Hit(999);
					manhole[i].flag = true;
				}

				if (manhole[i].flag == false) {
					stage->SetTemporary_Hit(69);
				}

				if (manhole[i].animTimer == 19 && !CheckSoundMem(manholeOpenedSe))PlaySoundMem(manholeOpenedSe, DX_PLAYTYPE_BACK, TRUE);

				if (manhole[i].animTimer >= 20) {
					int x = floor(manhole[i].x / MAP_CEllSIZE);
					int y = floor(manhole[i].y / MAP_CEllSIZE);
					stage->SetMapData(y, x, 98);
				}
			}

			else if ((manhole[i].flag == true) && (playerMapX >= manhole[i].x - MAP_CEllSIZE * 4) && (playerMapX <= manhole[i].x + MAP_CEllSIZE * 5) && (playerMapY > manhole[i].y + MAP_CEllSIZE)) {
				if (underGroundEffects < 100) {
					underGroundEffects += 2;
				}

				const int speed = manhole[i].liftWaitTimer * 2;

				if ((manhole[i].flag == true) && player->GetPlayerY() + -stage->GetScrollY() < manhole[i].y) {
					player->SetGravity(false);
					stage->SetScrollY(stage->GetScrollY() - speed);
					player->SetPlayerY((player->GetPlayerY() - stage->GetScrollY()) + speed);
					//printfDx("�x�����Ă��܂�\n");
				}
			}

		}

		//���Ԓn�_
		if (manhole[i].type == 2) {
			if ((playerMapX >= manhole[i].x) && (playerMapX <= manhole[i].x + MAP_CEllSIZE) && (playerMapY <= manhole[i].y + MAP_CEllSIZE) && (playerMapY >= manhole[i].y)) {
				//�v���C���[�̗������x��x������
				//player->SetPlayerY(player->GetPlayerY() - 3.0f);
				//player->SetGravity(true);
				player->SetVisible(false);
			}
			else {
				//player->SetGravity(true);
			}


			int mapdata_down = stage->GetMapData(manhole[i].leftInitY, manhole[i].leftInitX);

			//���Ԓn�_�𔲂��鉺�̍��W���擾����
			//���Ԓn�_�̎��Ƀ��[�v
			while (mapdata_down == 69 && manhole[i].pairNum == 0)
			{
				manhole[i].leftInitY++;
				mapdata_down = stage->GetMapData(manhole[i].leftInitY, manhole[i].leftInitY);
			}

			//���Z�����t���O
			manhole[i].pairNum = 1;

			int manhole_down_y;
			manhole_down_y = manhole[i].leftInitY * MAP_CEllSIZE + MAP_CEllSIZE;

			//���Ԓn�_�̈�ԉ��ɍs������
			if ((playerMapX >= manhole[i].x) && (playerMapX <= manhole[i].x + MAP_CEllSIZE) && (playerMapY <= manhole_down_y + MAP_CEllSIZE) && (playerMapY >= manhole_down_y)) {
				player->SetVisible(true);
				player->SetGravity(true);
				//�w�i��`�悵�Ȃ�
				if (isInUnder == false)
				{
					isInUnder = true;
				}
			}
		}

		//�o��
		if (manhole[i].type == 3) {

			int mapdata = stage->GetMapData(manhole[i].leftInitY + 1, manhole[i].leftInitX);

			//���Ԓn�_�̎��Ƀ��[�v
			while (mapdata == 69)
			{
				manhole[i].leftInitY++;
				mapdata = stage->GetMapData(manhole[i].leftInitY, manhole[i].leftInitX);

				//���Ԃ̐����J�E���g
				manhole[i].liftWaitTimer++;
			}

			//�}���z�[���̏o���̍��[����E�[�܂ł�x���W���}���z�[���̏o���������ɂ���ꍇ
			if ((playerMapX >= manhole[i].x - MAP_CEllSIZE / 2) && (playerMapX <= manhole[i].x + MAP_CEllSIZE / 2) && (playerMapY > manhole[i].y)) {
				if (PAD_INPUT::OnPressed(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)) { manhole[i].flag = true; }


				//B�{�^����������flg��true�ɂȂ�����
				if (manhole[i].flag == true) {
					//player->SetGravity(false);
					//player->SetVisible(true);
					const int speed = manhole[i].liftWaitTimer * 4;

					if (player->GetPlayerY() + -stage->GetScrollY() > manhole[i].y) {
						stage->SetScrollY(stage->GetScrollY() + speed);
						player->SetPlayerY((player->GetPlayerY() - stage->GetScrollY()) - speed);
					}

					//�w�i��`�悷��
					if (isInUnder == true)
					{
						isInUnder = false;
					}

					if (underGroundEffects > 0) {
						underGroundEffects -= manhole[i].liftWaitTimer + 2;
					}
					//SetGravity��L���������Getter�̒l���ς��Ȃ�
					//printfDx("%f\n", player->GetPlayerY());
					if ((player->GetPlayerY() + -stage->GetScrollY()) < manhole[i].y) {
						if (!CheckSoundMem(manholeOpenedSe)) { PlaySoundMem(manholeOpenedSe, DX_PLAYTYPE_BACK, TRUE); }
						//player->SetVisible(false);
						//�ꎞ�I�ȓ����蔻�������B
						stage->SetTemporary_Hit(69);
						//player->SetGravity(true);
						player->SetVisible(true);
						manhole[i].flag = false;
						if (underGroundEffects != 0)
						{
							underGroundEffects = 0;
						}
					}
				}
			}
		}
	}
}

/// <summary>
/// �_���J�̐����܂�̏���
/// </summary>
void ELEMENT::Acidrain_puddles(PLAYER* player) {
	if (acidrainPuddlesAniTimer < 5) { acidrainPuddlesAniTimer++; }
	else
	{
		acidrainPuddlesAniTimer = 0;

		if (foamAnimation < 10)
		{
			foamAnimation++;
		}
		else
		{
			foamAnimation = 5;
		}
		
	}
	int acidSize = acidrainPuddles.size();
	for (int i = 0; i < acidSize; i++) {
		if (acidrainPuddles[i].image < 5)
		{
			if (acidrainPuddles[i].animTimer < 6)
			{
				acidrainPuddles[i].animTimer++;
			}
			else
			{
				acidrainPuddles.at(i).animTimer = 0;
				if (i < acidSize - 1)
				{
					if (acidrainPuddles.at(i + 1).image == 99)
					{
						acidrainPuddles.at(i + 1).image = 1;
					}
				}

				if (acidrainPuddles[i].image < 4)
				{
					acidrainPuddles[i].image++;

					/*else if (acidrain_puddles[i - 1].image < acidrain_puddles[i].image)
					{
						acidrain_puddles[i].image++;
					}*/
				}
				else
				{
					acidrainPuddles[i].image = 1;
				}

			}
		}

		/*if (acidrainPuddles[i].flag == false)acidrainPuddles[0].animTimer++;
		if (acidrainPuddles[0].animTimer > 120) {
			acidrainPuddles[0].animTimer = 0;
			acidrainPuddles[i].flag = true;
		}*/
		//�_���܂�
		if ((playerMapX >= acidrainPuddles[i].x) && (playerMapX <= acidrainPuddles[i].x + MAP_CEllSIZE))
		{
			if ((playerMapY >= acidrainPuddles[i].y - MAP_CEllSIZE / 2) && (playerMapY <= acidrainPuddles[i].y))
			{
				if (CheckSoundMem(walkPuddleSe) == FALSE && acidrainPuddles[0].animTimer % 90 == 0)PlaySoundMem(walkPuddleSe, DX_PLAYTYPE_BACK, TRUE);
			}
			//�_���܂�ɂ������v���C���[�����C�t���O�ɂ���
			if (acidrainPuddles[i].y + 40.0f <= playerMapY && playerMapY <= acidrainPuddles[i].y + 160.0f)
			{
				player->SetLife(0);
			}
		}
	}
}

/// <summary>
/// �t�b�N�̃K�C�h�\���p�����v�Z
/// </summary>
void ELEMENT::Hook_Distance(PLAYER* player, STAGE* stage) {
	float min_distance = HOOK_MAX_DISTANCE + 80;
	//�t�b�N�̃K�C�h�\���p
	//�t�b�N�܂ł̋����v�Z
	for (int i = 0; i < hook.size(); i++) {
		float diff_x = hook[i].x - (player->GetPlayerX() - stage->GetScrollX());
		float diff_y = hook[i].y - (player->GetPlayerY() - stage->GetScrollY());
		float distance = sqrtf(diff_x * diff_x + diff_y * diff_y);
		//�������ŒZ�������߂��Ƃ�
		if (distance <= min_distance) { min_distance = distance; hook[i].flag = true; }
		else { hook[i].flag = false; }
	}
}