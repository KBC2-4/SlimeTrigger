#pragma once
#include "AbstractScene.h"
#include "Title.h"

enum class GAMEOVER_MENU
{


	NewGame = 0,		//Restart
	ReSelect			//GameSelect
};

class GameOver : public AbstractScene
{
private:
	int title_image,backgraundImage;
	int selectmenu;
	int menu_font;
	int guid_font;
	int backgroundMusic;
	int bad_se[4], okSe, cursormoveSe;
	int inputMargin;
	int timer,se_randnum;
	const char* stageName;	//���X�^�[�g���̃X�e�[�W�I��p
	
public:
	//�R���X�g���N�^
	GameOver(const char* stageName);
	//�f�X�g���N�^
	~GameOver();
	//�`��ȊO�̏���
	AbstractScene* Update() override;
	//�`�揈��
	void Draw() const override;
};