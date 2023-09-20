#include "Title.h"
#include "GameMain.h"
#include "StageSelect.h"
#include "DrawRanking.h"
#include "DxLib.h"
#include "Option.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "Guide.h"
#include "MenuSelectScene.h"


//�^�C�g���A�j���[�V����������N�����̂ݗL�������邽�߂̃t���O
static bool animation_flg = false;


Title::Title()
	: titleLogoAnimationY(100), titleAnimationFrame(0)
{
	if ((backGraundImage = LoadGraph("Resource/Images/Stage/TitleBackImage.png")) == -1)
	{
		throw "Resource/Images/Stage/TitleBackImage.png";
	}

	if ((titleLogo = LoadGraph("Resource/Images/Scene/title/title_logo.png")) == -1)
	{
		throw "Resource/Images/Scene/title/title_logo.png";
	}

	if ((backGraundMusic = LoadSoundMem("Resource/Sounds/BGM/title.wav")) == -1)
	{
		throw "Resource/Sounds/BGM/title.wav";
	}

	if ((cursorMoveSe = LoadSoundMem("Resource/Sounds/SE/cursor_move.wav")) == -1)
	{
		throw "Resource/Sounds/SE/cursor_move.wav";
	}

	if ((okSe = LoadSoundMem("Resource/Sounds/SE/ok.wav")) == -1)
	{
		throw "Resource/Sounds/SE/ok.wav";
	}

	LoadDivGraph("Resource/Images/Scene/title/title_logo_animation.png", 8, 2, 4, 960, 668, titleLogoAnimationImage);

	int se_random = GetRand(1);
	char dis_exit_se[30];
	sprintf_s(dis_exit_se, sizeof(dis_exit_se), "Resource/Sounds/SE/exit0%d.wav", se_random + 1);

	if ((exitSe = LoadSoundMem(dis_exit_se)) == -1)
	{
		throw dis_exit_se;
	}

	titleFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 120, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 8);
	menuFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 80, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	guidFont = CreateFontToHandle("���C���I", 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	
	inputMargin = 0;
	timer = 0;
	exitFlag = false;

	titleAniTimer[0] = 0;

	//�^�C�g���A�j���[�V����������N�����̂ݗL����
	if (animation_flg == false)
	{
		titleAniTimer[1] = 180;
		animation_flg = true;
	}
	else { titleAniTimer[1] = 0; }

	// �^�C�}�[��������
	stateStartTime = std::chrono::steady_clock::now();
	option = new Option();

	animationState = AnimationState::PLAYING;
	scaleFactor = 1.0;

	//BGM
	ChangeVolumeSoundMem(Option::GetBGMVolume(), backGraundMusic);

	//SE
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.6, cursorMoveSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.2, okSe);
	ChangeVolumeSoundMem(Option::GetSEVolume() * 1.3, exitSe);
}

Title::~Title()
{
	delete option;

	DeleteGraph(backGraundImage);
	DeleteGraph(titleLogo);
	for (int i = 0; i < MAX_ANIMATION_FRAMES; ++i)
	{
		DeleteGraph(titleLogoAnimationImage[i]);
	}
	StopSoundMem(backGraundMusic);
	DeleteSoundMem(backGraundMusic);
	DeleteSoundMem(cursorMoveSe);
	DeleteSoundMem(okSe);
	DeleteSoundMem(exitSe);
	DeleteFontToHandle(titleFont);
	DeleteFontToHandle(menuFont);
	DeleteFontToHandle(guidFont);
}

AbstractScene* Title::Update()
{
	{
		if (inputMargin < 20) {
			inputMargin++;
		}
		else {

			//�^�C�g���A�j���[�V�������I����Ă���BGM���Đ�����
			if (titleAniTimer[1] == 0 && !CheckSoundMem(backGraundMusic)) {
				PlaySoundMem(backGraundMusic, DX_PLAYTYPE_LOOP);
			}


			if (titleAniTimer[1] <= 0) {
				if ((PAD_INPUT::GetPadThumbLY() > 20000) || PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_UP))
				{
					inputMargin = 0;
					PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
					StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
				}

				if ((PAD_INPUT::GetPadThumbLY() < -20000) || PAD_INPUT::OnPressed(XINPUT_BUTTON_DPAD_DOWN))
				{
					inputMargin = 0;
					PlaySoundMem(cursorMoveSe, DX_PLAYTYPE_BACK, TRUE);
					StartJoypadVibration(DX_INPUT_PAD1, 100, 160, -1);
				}
			}
		}

		if ((PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)))
		{

			if (titleAniTimer[1] <= 0) {
				PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
				//ok_se����I����Ă����ʐ��ڂ���B
				while (CheckSoundMem(okSe)) {}
				StartJoypadVibration(DX_INPUT_PAD1,  OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
				return new MenuSelectScene();
				

			}
			else { titleAniTimer[1] = 0; titleAnimationFrame = MAX_ANIMATION_FRAMES - 1;}
		}
		timer++;


		// �v���O�����I��
		if (exitFlag == true && !CheckSoundMem(exitSe)) { return nullptr; }

		//���v�t���[��
		if (titleAniTimer[1] > 0) { titleAniTimer[1]--; }

		// ���݂̌o�ߎ���
		const auto currentTime = std::chrono::high_resolution_clock::now();
		
		auto now = std::chrono::steady_clock::now();
		double elapsedSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - stateStartTime).count() / 1000.0;

		switch (animationState) {
			// �A�j���[�V�����t���[�����X�V���A�ő�t���[���ɒB�����玟�̏�ԂɈڍs���� 
		case AnimationState::PLAYING:
			if (titleAnimationFrame < MAX_ANIMATION_FRAMES - 1) {
				titleAnimationFrame = static_cast<int>(elapsedSeconds / 0.1) % MAX_ANIMATION_FRAMES;
			} else {
				TransitState(AnimationState::MOVING_UP, now);
			}
			break;

		case AnimationState::MOVING_UP:
			{
				constexpr double MOVE_UP_DURATION = 2.0;
				if (elapsedSeconds <= MOVE_UP_DURATION) {
					titleLogoAnimationY = 100 - static_cast<int>(300 * elapsedSeconds / MOVE_UP_DURATION);
				} else {
					TransitState(AnimationState::SHRINKING, now);
				}
			}
			break;

		case AnimationState::SHRINKING:
			{
				constexpr double SHRINK_DURATION = 0.8;
				
				if (elapsedSeconds <= SHRINK_DURATION) {
					constexpr double SHRINK_FACTOR = 0.7;
						scaleFactor = 1.0 - (SHRINK_FACTOR * elapsedSeconds / SHRINK_DURATION);
					} else {
					TransitState(AnimationState::GROWING, now);
				}
			}
			break;

		case AnimationState::GROWING:
			{
				constexpr double GROW_DURATION = 1.0;
				constexpr double GROW_FACTOR = 1.0;

				if (elapsedSeconds <= GROW_DURATION) {
					scaleFactor = GROW_FACTOR + (1.0 - GROW_FACTOR) * (elapsedSeconds / GROW_DURATION);
				} else {
					TransitState(AnimationState::DONE, now);
				}
			}
			break;

		case AnimationState::DONE:
			break;
		}
		
	}

	return this;
}

void Title::Draw() const
{
	DrawGraph(0, 0, backGraundImage, false);

	// �^�C�g�����S��`��
	DrawRotaGraph(660, titleLogoAnimationY + 360, scaleFactor, 0, titleLogoAnimationImage[titleAnimationFrame], TRUE);

	if (titleAniTimer[1] > 0) { return; }
	if (titleAnimationFrame < MAX_ANIMATION_FRAMES - 1)
	{
		return;
	}


	//����ē�
	if (timer % 120 < 60)
	{
		if (PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::XINPUT_GAMEPAD) ||
			PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::DIRECTINPUT_GAMEPAD))
		{
			const std::vector<guideElement> gamepadGuides = {
				guideElement({Option::GetInputMode() ? "B" : "A"}, "�Ō���", GUIDE_SHAPE_TYPE::DYNAMIC_CIRCLE,
				             guidFont, 0xFFFFFF, Option::GetInputMode() ? B_COLOR : A_COLOR,
				             0xEBA05E, 0xFFFFFF, 10, 200.0f, 30.0f, 20.0f, 5.0f),
			};
			DrawGuides(gamepadGuides, 505.0f, 480.0f, 5.0f, 60.0f);
		}
		else if (PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::KEYBOARD))
		{
			if (Option::GetInputMode())
			{
				const std::vector<guideElement> keyboardGuidesSpace = {
					guideElement({"SPACE"}, "�Ō���", GUIDE_SHAPE_TYPE::DYNAMIC_BOX, guidFont, 0xFFFFFF,
					             B_COLOR,
					             0xEBA05E, 0xFFFFFF, 200.0f, 30.0f, 20.0f, 20.0f, 2.5f),
				};
				DrawGuides(keyboardGuidesSpace, 450.0f, 480.0f, 5.0f, 60.0f);
			}
			else
			{
				const std::vector<guideElement> keyboardGuidesSpaceZ = {
					guideElement({"Z"}, "�Ō���", GUIDE_SHAPE_TYPE::FIXED_BOX,
					             guidFont, 0xFFFFFF, A_COLOR,
					             0xEBA05E, 0xFFFFFF, 200.0f, 60.0f, 60.0f, 20.0f, 2.5f),
				};

				DrawGuides(keyboardGuidesSpaceZ, 510.0f, 480.0f, 5.0f, 60.0f);
			}
		}
	}
}
