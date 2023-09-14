#pragma once
#include <vector>
#include <string>

enum class GUIDE_SHAPE_TYPE
{
	TEXT,			//�e�L�X�g
	DYNAMIC_CIRCLE,	//���I�ȉ~
	DYNAMIC_BOX,	//���I�Ȏl�p�`
	FIXED_CIRCLE,	//�Œ�T�C�Y�̉~
	FIXED_BOX,		//�Œ�T�C�Y�̎l�p�`
	ROUNDED_BOX,		//�ۂ݂�тт��l�p�`
	JOYSTICK,		//�W���C�X�e�B�b�N�`
};

// �K�C�h�v�f�\����
struct guideElement
{
	std::vector<std::string> buttons; // �{�^��
	std::string description; // ����
	GUIDE_SHAPE_TYPE shapeType;
	// 0: TEXT, 1: DYNAMIC_CIRCLE, 2: DYNAMIC_BOX, 3: FIXED_CIRCLE, 4: FIXED_BOX, 5: ROUNDED_BOX, 6: JOYSTICK
	int font; // �t�H���g�n���h��
	unsigned int buttonColor; // �{�^���̐F
	unsigned int buttonStringColor; // �{�^���̕����F
	unsigned int color; // �����̐F
	unsigned int edgeColor; // �����̗֊s���̐F
	float innerPadding; // �����̃{�^��������ꍇ�̃{�^���Ԋu
	float width; // �Œ�T�C�Y��Box�ACircle�p����
	float height; // �Œ�T�C�Y��Box�ACircle�p�c��
	float offsetStringY; // �������Y���W�̔�����
	float offsetButtonStringY; // �{�^��������Y���W�̔�����
	float radiusOffset; // �Œ�T�C�Y��Circle�p���a�̔�����
	float padding;		// �{�^���Ɛ����̊Ԃ̃p�f�B���O
	float lineWidth; // �֊s���̑���
	unsigned int lineColor; // �֊s���̐F
	
	guideElement(const std::vector<std::string>& buttons = {""}, const std::string& description = "",
	             GUIDE_SHAPE_TYPE shapeType = GUIDE_SHAPE_TYPE::TEXT, int font = NULL,
	             unsigned int buttonColor = 0xffffff,
	             unsigned int buttonStringColor = 0x000000,
	             unsigned int color = 0xffffff, unsigned int edgeColor = 0x000000,
	             float innerPadding = 10.f, float width = 30.f, float height = 30.f, float offsetStringY = 0.f,  float offsetButtonStringY = 0.f, float radiusOffset = -2.f,
	             float padding = 2.f, float lineWidth = 4.f,
	             unsigned int lineColor = 0xEFEAF6)
		: buttons(buttons), description(description), shapeType(shapeType), font(font), buttonColor(buttonColor),
		  buttonStringColor(buttonStringColor), color(color), edgeColor(edgeColor),
		  innerPadding(innerPadding), width(width), height(height), offsetStringY(offsetStringY), offsetButtonStringY(offsetButtonStringY),
		  radiusOffset(radiusOffset), padding(padding), lineWidth(lineWidth), lineColor(lineColor)
	{
	}
};

class AbstractScene
{
public:
	//�R���X�g���N�^
	AbstractScene();
	// CommonProcess�����s���Ȃ��悤�ɂ�������L��̋�R���X�g���N�^
	AbstractScene(bool notInitialized) {};
	//�f�X�g���N�^
	virtual ~AbstractScene() {};
	//�`��ȊO�̍X�V�����s
	virtual AbstractScene* Update() = 0;
	//�`��Ɋւ��邱�Ƃ�����
	virtual void Draw() const = 0;

	/// <summary>
	/// �S�V�[�����ʂ̃R���X�g���N�^�ōs������
	/// </summary>
	void CommonProcess();

	/// <summary>
	/// �����̕`�悷��X���W�����S�ɂȂ�X���W���擾����B
	/// </summary>
	/// <param name="string">�`�悷�镶����</param>
	/// <param name="fontHandle">�`��Ɏg�p����t�H���g�n���h��(�����͖���0�̏ꍇ�̓f�t�H���g�t�H���g�n���h�����g�p</param>
	/// <param name="margin">������X���W�ɊԊu���󂯂�l</param>
	int GetDrawCenterX(const char* string, int fontHandle = -1, int margin = 0)const;

	/// <summary>
	/// �K�C�h�\������
	/// </summary>
	/// <param name="guides">�K�C�h�v�f</param>
	/// <param name="x">�`��̊�ƂȂ�X���W</param>
	/// <param name="y">�`��̊�ƂȂ�Y���W</param>
	/// <param name="padding">�{�^���Ɛ����̊Ԃ̊Ԋu</param>
	/// <param name="elementSpacing">�e�v�f�̊Ԋu</param>
	void DrawGuides(const std::vector<guideElement>& guides, float x, float y, float padding, float elementSpacing) const;
	void DrawGuideElement(const guideElement& guide, float& currentX, float y) const;
};