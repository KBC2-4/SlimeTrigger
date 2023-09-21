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
	: exitFlag(false), inputMargin(0), titleLogoAnimationY(100), titleAnimationFrame(0), bounceCount(0), velocity(-30.0)
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

	// �^�C�g�����S�A�j���[�V�����摜��ǂݍ���
	LoadDivGraph("Resource/Images/Scene/title/title_logo_animation.png", 8, 2, 4, 960, 668, titleLogoAnimationImage);

	// exit SE��ǂݍ���
	int se_random = GetRand(1);
	char dis_exit_se[30];
	sprintf_s(dis_exit_se, sizeof(dis_exit_se), "Resource/Sounds/SE/exit0%d.wav", se_random + 1);

	if ((exitSe = LoadSoundMem(dis_exit_se)) == -1)
	{
		throw dis_exit_se;
	}

	// �t�H���g��ǂݍ���
	titleFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 120, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 8);
	menuFont = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N-B", 80, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	guidFont = CreateFontToHandle("���C���I", 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	buttonGuidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	
	timer = 0;

	//�^�C�g���A�j���[�V����������N�����̂ݗL����
	if (animation_flg == false)
	{
		animationState = AnimationState::BOUNCING;
	}
	else
	{
		animationState = AnimationState::DONE;
		animation_flg = true;
	}

	// �^�C�}�[��������
	stateStartTime = std::chrono::steady_clock::now();
	option = new Option();
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
			if (animationState == AnimationState::DONE && !CheckSoundMem(backGraundMusic)) {
				PlaySoundMem(backGraundMusic, DX_PLAYTYPE_LOOP);
			}
			
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

		// ���j���[�I����ʂ֐���
		if ((PAD_INPUT::OnButton(Option::GetInputMode() ? XINPUT_BUTTON_B : XINPUT_BUTTON_A)))
		{
			if(animationState == AnimationState::DONE) {
				PlaySoundMem(okSe, DX_PLAYTYPE_BACK, TRUE);
				//ok_se����I����Ă����ʐ��ڂ���B
				while (CheckSoundMem(okSe)) {}
				StartJoypadVibration(DX_INPUT_PAD1,  OK_VIBRATION_POWER, OK_VIBRATION_TIME, -1);
				return new MenuSelectScene();
				

			}
			else
			{
				animationState = AnimationState::DONE;
				titleAnimationFrame = MAX_ANIMATION_FRAMES - 1;
			}
		}

		// �Q�[���I��
		if(PAD_INPUT::OnButton(XINPUT_BUTTON_BACK))
		{
			animationState = AnimationState::DONE;
			titleAnimationFrame = MAX_ANIMATION_FRAMES - 1;
			exitFlag = true;
			PlaySoundMem(exitSe, DX_PLAYTYPE_BACK, FALSE);
		}
		timer++;


		// �v���O�����I��
		if (exitFlag == true && !CheckSoundMem(exitSe)) { return nullptr; }
		
		auto now = std::chrono::steady_clock::now();
		double elapsedSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - stateStartTime).count() / 1000.0;
		
		switch (animationState) {
			// �A�j���[�V�����t���[�����X�V���A�ő�t���[���ɒB�����玟�̏�ԂɈڍs����
			case AnimationState::BOUNCING:
				{
					constexpr double GRAVITY = 0.8;  // �d�́i���������x�j
					constexpr int INITIAL_Y = 400;
					constexpr double TIME = 2.0;
						// Y���W���X�V
						titleLogoAnimationY += velocity * TIME + 0.5 * GRAVITY * std::pow(TIME, 2);

						// ���x���X�V�i�d�͂ɂ������j
						velocity += GRAVITY * TIME;

						// �n�ʂɐG�ꂽ�ꍇ�i���˕Ԃ�ꍇ�j
						if (titleLogoAnimationY >= INITIAL_Y && velocity > 0) {
							// Y���W��n�ʂɍ��킹��
							titleLogoAnimationY = INITIAL_Y;
							// �����x�Ƀ��Z�b�g
							velocity = -30.0;  

							// �o�E���X�J�E���g
							++bounceCount;
						}

					// �o�E���X��3��ȏォ��Y���W��100�ȉ��ɂȂ����ꍇ�A�X�e�[�g�J��
					if (bounceCount >= 3 && titleLogoAnimationY <= 100) {
						TransitState(AnimationState::PLAYING, now);
						bounceCount = 0;
					}
				}
			break;

		case AnimationState::PLAYING:
			if (titleAnimationFrame < MAX_ANIMATION_FRAMES - 1) {
				titleAnimationFrame = static_cast<int>(elapsedSeconds / 0.1) % MAX_ANIMATION_FRAMES;
			} else {
				TransitState(AnimationState::MOVING_UP, now);
			}
			break;

		case AnimationState::MOVING_UP:
			{
				constexpr double MOVE_UP_DURATION = 1.5;  // �ړ��ɂ����鎞��
				constexpr int INITIAL_Y = 100;  // �����ʒu
				constexpr int MOVE_DISTANCE = 300;  // �ړ�����

				if (elapsedSeconds <= MOVE_UP_DURATION) {
					titleLogoAnimationY = static_cast<int>(OutCubic(elapsedSeconds, INITIAL_Y, -MOVE_DISTANCE, MOVE_UP_DURATION));
				} else {
					TransitState(AnimationState::POP, now);
				}
			}
			break;

		case AnimationState::POP:
			{
				constexpr double POP_DURATION = 0.5;  // �A�j���[�V�����̎������ԁi�b�j
				constexpr double MAX_SCALE = 1.6;  // �ő�g�嗦
				constexpr double MIN_SCALE = 1.0;  // �ŏ��g�嗦

				if (elapsedSeconds <= POP_DURATION) {
					scaleFactor = OutQuad(elapsedSeconds, MIN_SCALE, MAX_SCALE - MIN_SCALE, POP_DURATION);
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
	if(animationState == AnimationState::DONE)
	{
		DrawRotaGraph(630,355, 1.065, 0, titleLogo,TRUE);
	} else
	{
		DrawRotaGraph(660, titleLogoAnimationY + 360, scaleFactor, 0, titleLogoAnimationImage[titleAnimationFrame], TRUE);	
	}
	
	if (animationState != AnimationState::DONE)
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
					guideElement({"SPACE"}, "�Ńv���C", GUIDE_SHAPE_TYPE::DYNAMIC_BOX, guidFont, 0xFFFFFF,
					             B_COLOR,
					             0xEBA05E, 0xFFFFFF, 200.0f, 30.0f, 20.0f, 20.0f, 2.5f),
				};
				DrawGuides(keyboardGuidesSpace, 450.0f, 480.0f, 5.0f, 60.0f);
			}
			else
			{
				const std::vector<guideElement> keyboardGuidesSpaceZ = {
					guideElement({"Z"}, "�Ńv���C", GUIDE_SHAPE_TYPE::FIXED_BOX,
					             guidFont, 0xFFFFFF, A_COLOR,
					             0xEBA05E, 0xFFFFFF, 200.0f, 60.0f, 60.0f, 20.0f, 2.5f),
				};

				DrawGuides(keyboardGuidesSpaceZ, 510.0f, 480.0f, 5.0f, 60.0f);
			}
		}
	}


	// �I���{�^���̑���ē�
	if (PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::XINPUT_GAMEPAD) ||
			PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::DIRECTINPUT_GAMEPAD))
	{
		const std::vector<guideElement> gamepadGuides = {
			guideElement({"BACK"}, "�Q�[���I��", GUIDE_SHAPE_TYPE::ROUNDED_BOX, buttonGuidFont, 0xFFFFFF, START_COLOR,
			 0xEBA05E, 0xFFFFFF,10.f,30.f,30.f, 0, 2.f),
		};
		DrawGuides(gamepadGuides, 545.0f, 668.0f, 5.0f, 60.0f);
	}
	else if (PAD_INPUT::GetInputMode() == static_cast<int>(PAD_INPUT::InputMode::KEYBOARD))
	{
			const std::vector<guideElement> keyboardGuidesSpace = {
				guideElement({"ESC"}, "�Q�[���I��", GUIDE_SHAPE_TYPE::DYNAMIC_BOX, buttonGuidFont, 0xFFFFFF,
							 START_COLOR,
							 0xEBA05E, 0xFFFFFF, 200.0f, 30.0f, 20.0f, 0.0f, 2.5f),
			};
			DrawGuides(keyboardGuidesSpace, 555.0f, 668.0f, 5.0f, 60.0f);
		}
}
