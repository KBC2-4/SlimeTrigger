#pragma once
#include "AbstractScene.h"

class DRAW_RANKING :
	public AbstractScene
{
private:
	int waitTime;
	int bestTime[3];
	int titleFont,timeFont;
	int image;
	int okSe;
	int backGraundImage;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DRAW_RANKING();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DRAW_RANKING();
	/// <summary>
	/// �`��ȊO�̍X�V�����s
	/// </summary>
	/// <returns>�V�[���̃A�h���X��Ԃ�</returns>
	AbstractScene* Update() override;
	/// <summary>
	/// �`��Ɋւ��邱�Ƃ�����
	/// </summary>
	void Draw() const override;
};

