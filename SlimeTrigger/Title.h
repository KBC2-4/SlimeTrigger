#pragma once
#include "AbstractScene.h"
#include "Stage.h"
#include "GameOver.h"
#include "PadInput.h"
#include "Option.h"

enum class MENU
{
	GAME_SELECT = 0,
	RANKING,
	OPTION,
	END
};
class Title : public AbstractScene
{
private:

	int backgraundImage;
	int backgroundMusic;
	int cursormoveSe, okSe,exit_se;
	int titleFont, menuFont, guidFont;
	int selectmenu;
	int inputMargin;
	int timer;
	bool exit_flg;

	int title_anitimer[2];

	Option* option;

public:

	//�R���X�g���N�^
	Title();
	//�f�X�g���N�^
	~Title();
	//�`��ȊO�̍X�V�����s
	AbstractScene* Update() override;
	//�`��Ɋւ��邱�Ƃ�����
	void Draw() const override;
};