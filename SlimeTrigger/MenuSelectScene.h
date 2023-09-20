#pragma once
#include "AbstractScene.h"
#include "Option.h"
#include <cmath>
#include <chrono>

constexpr int IMAGE_NUM = 4;

class MenuSelectScene :
    public AbstractScene
{
private:
    //�w�i�摜
    int backGraundImage;

    // ���摜
    int grassImage[2];

    // �A�Z�b�g
    int menuImages[IMAGE_NUM];


    //BGM
    int backGraundMusic;

    // SE
    int cursorMoveSe;
    int okSe;
    int exitSe;

    //�t�H���g
    int guidFont, buttonGuidFont;
    int menuFont;
    int MenuBigFont;

    //�K�C�h�\���^�C�}�[
    int guidTimer;

    Option* option;

    // �^�C�}�[
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

    enum class MENU
    {
        PLAY,
        RANKING,
        OPTION,
        EXIT
    };

    struct MenuItem
    {
        int x, y;
        const char* name;
        MENU menu;
    };

    // ���j���[���ڂ̒�`
    MenuItem menuItems[4] = {
        {350, 400, "������", MENU::PLAY},
        {830, 260, "�����L���O", MENU::RANKING},
        {690, 480, "�����Ă�", MENU::OPTION},
        {980, 480, "�����", MENU::EXIT}
    };

    // ���ݑI������Ă��郁�j���[����
    int selected;

    // �ł��߂����j���[�v�f
    int closestIndex;

    //����Ԋu����
    int inputMargin;

public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    MenuSelectScene();
    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~MenuSelectScene() override;
    /// <summary>
    /// �`��ȊO�̍X�V�����s
    /// </summary>
    /// <returns>�V�[���̃A�h���X��Ԃ�</returns>
    AbstractScene* Update() override;
    /// <summary>
    /// �`��Ɋւ��邱�Ƃ�����
    /// </summary>
    void Draw() const override;

    /**
     * \brief ���j���[���X�e�B�b�N�̊p�x�ɂ���đI������
     * \param closestIndex ���j���[�v�f
     * \param selected �I������Ă��郁�j���[�v�f
     * \param stickAngle �X�e�B�b�N�̊p�x
     */
    void UpdateMenuSelection(int& closestIndex, int& selected, double stickAngle) const;
};
