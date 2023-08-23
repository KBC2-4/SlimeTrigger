#pragma once
#include <Windows.h>
#include <vector>

#define MAP_CEllSIZE 80

class PLAYER;
class ELEMENT;

class STAGE
{
private:
	
	int stageImage[5];			//�X�e�[�W�w�i�摜
	int halfWayPointSe;		//���Ԓn�_SE
	float mapX, mapY;			//�}�b�v�`����W
	POINT clearBox;		//�N���A�ɂȂ�{�b�N�X��͈͂̍��W
	int aniTimer;
	POINT halWayPointBox;		//���Ԓn�_���W
	POINT spawnPoint;			//�X�|�[���n�_���W

	bool clearFlag;			//�N���A����t���O
	int clearCount;		//�X�e�[�W�N���A���̃J�E���g�^�C�}�[
	bool halfWayPoint;		//���Ԓn�_�t���O
	int halfWayTimer;		//���Ԓn�_�p�^�C�}�[
	float playerXOld, playerYOld;		//���v���C���[���W
	float playerLongOld;	//���t���[���O�̃v���C���[���W
	int countTimer;		//�J�E���g�p�^�C�}�[
	float playerVectorX;					//�v���C���[�̈ړ�����x
	float playerVectorY;					//�v���C���[�̈ړ�����y
	float scrollSpeedX;					//�X�N���[���ړ���(x)
	float scrollSpeedY;					//�X�N���[���ړ���(y)

protected:
	int blockImage1[110];		//�X�e�[�W�u���b�N�摜
	std::vector<std::vector<int>> mapData;
	std::vector<std::vector<int>> doorInfo;
	std::vector<std::vector<int>> buttonInfo;
	float scrollX, scrollY;	//�}�b�v�X�N���[����
	int temporaryHit;		//�ꎞ�I�ȓ����蔻��
	const char* stageName;		//�X�e�[�W��
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	STAGE(const char* stageName = "StageSelect", bool restert = false);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~STAGE();
	/// <summary>
	/// �X�e�[�W�̍X�V
	/// </summary>
	void Update(PLAYER* player, ELEMENT* element);
	/// <summary>
	/// �X�e�[�W�̕`��
	/// </summary>
	virtual void Draw(ELEMENT* element) const;
	/// <summary>
	/// �}�b�v�摜�ԍ�Geter
	/// </summary>
	/// <returns>int�^�F�摜�����ԍ�</returns>	
	int GetMapImage(int num) { return blockImage1[num]; }
	/// <summary>
	/// �}�b�v�f�[�^�̓ǂݍ���
	/// </summary>
	/// <param name="stageName">�X�e�[�W�̃C���X�^���X</param>
	void LoadMapData(const char* stageName);
	/// <summary>
	/// �}�b�v�T�C�Y��Geter
	/// </summary>
	/// <returns>�\���̌^�F�}�b�v�T�C�Y</returns>	
	POINT GetMapSize(void) { POINT mapsize{ mapData.size(),mapData.at(0).size(), };  return mapsize; }
	/// <summary>
	/// �}�b�v�z���Getter
	/// </summary>
	/// <param name="y">�}�b�v�f�[�^�̉��v�f��</param>
	/// <param name="x">�}�b�v�f�[�^�̏c�v�f��</param>
	/// <returns>int�^�F�}�b�v�z��ԍ�</returns>	
	int GetMapData(int y, int x);
	/// <summary>
	/// �}�b�v�z��̒l��Setter
	/// </summary>
	/// <param name="y">�}�b�v�f�[�^�̉��v�f��</param>
	/// <param name="x">�}�b�v�f�[�^�̏c�v�f��</param>
	/// <param name="mapchip_num">�}�b�v�z��ԍ�</param>
	void SetMapData(int y, int x, int mapchip_num) { mapData[y][x] = mapchip_num; }


	/// <summary>
	/// �J�������[�N
	/// </summary>
	/// 	/// <param name="player">�v���C���[�̃C���X�^���X</param>
	/// <param name="element">�G�������g�̃C���X�^���X</param>
	void CameraWork(PLAYER* player, ELEMENT* element);
	/// <summary>
	/// �X�N���[��X��Setter
	/// </summary>
	void SetScrollX(float scrollX) { if (scrollX < 0) { this->scrollX = scrollX; } }
	/// <summary>
	/// �X�N���[��Y��Setter
	/// </summary>
	void SetScrollY(float scrollY) { if (scrollY < 0) { this->scrollY = scrollY; } }
	/// <summary>
	/// ��ʃX�N���[�����WX��Setter
	/// </summary>
	bool SetScrollPos(int moveX);
	/// <summary>
	/// ��ʃX�N���[�����WX��Getter
	/// </summary>
	float GetScrollX() { return scrollX; };
	/// <summary>
	/// ��ʃX�N���[�����WY��Getter
	/// </summary>
	float GetScrollY() { return scrollY; }


	/// <summary>
	/// �}�b�v�̓����蔻��
	/// </summary>
	/// <param name="y">�}�b�v�f�[�^�̉��v�f��</param>
	/// <param name="x">�}�b�v�f�[�^�̏c�v�f��</param>
	bool HitMapDat(int y, int x);
	/// <summary>
	/// �X���C���̂�����̓����蔻��
	/// </summary>
 	/// <param name="y">�}�b�v�f�[�^�̉��v�f��</param>
	/// <param name="x">�}�b�v�f�[�^�̏c�v�f��</param>
	bool HitThrowSlime(int y, int x);

	/// <summary>
	/// �X�|�[���n�_���W��Getter
	/// </summary>
	/// <returns>�\���̌^�F�X�|�[���n�_���W</returns>	
	POINT GetSpawnPoint(void) { return spawnPoint; }
	/// <summary>
	/// �X�e�[�W�N���A���菈��
	/// </summary>
	void StageClear(PLAYER* player);
	/// <summary>
	/// �N���A�������̃t���O��Getter
	/// </summary>
	bool GetClearFlg(void) { return clearFlag; };
	/// <summary>
	/// ���Ԓn�_���菈��
	/// </summary>
	void HalfwayPoint(PLAYER* player);
	/// <summary>
	/// ���Ԓn�_���W��Getter
	/// </summary>
	/// <returns>�\���̌^�F���Ԓn�_���W</returns>	
	POINT GetHalfwayPoint(void) {return POINT(halWayPointBox); }
	/// <summary>
	/// ���Ԓn�_�ʉ߂���������t���O��Getter
	/// </summary>
	bool GetHalfwayPointFlg(void) { return halfWayPoint; }
	/// <summary>
	/// �ꎞ�I�ȓ����蔻���Setter
	/// </summary>
	/// <param name="mapdata">�}�b�v�z��ԍ�</param>
	void SetTemporary_Hit(int mapdata) { temporaryHit = mapdata; }
};

