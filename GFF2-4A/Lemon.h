#pragma once
#include"Enemy.h"

class LEMON :
    public ENEMY
{
private:

public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="player"></param>
    LEMON();
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move() override;
	/// <summary>
	/// �ʏ`�𔭎�
	/// </summary>
	void ShotFruitJuice() override;
	/// <summary>
	/// �����蔻��
	/// </summary>
	void Hit() override;
	/// <summary>
	/// �A�j���[�V����
	/// </summary>
	void Animation() override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw()const override;
};
