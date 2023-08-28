#pragma once
#include"DxLib.h"

#define BUTTONS 16

enum class PAD_STATE
{
	NOT = 0, //押されていない
	ON,		//押された瞬間
	DOWN	//押されている間
};

class PAD_INPUT
{
private:
	static int nowKey;
	static int oldKey;
	static PAD_STATE state;
	static XINPUT_STATE Input;
	static DINPUT_JOYSTATE dInput;

public:
	PAD_INPUT(); //コンストラクタ
	void UpdateKey();	//パッド入力の更新
	/**
	* @brief	DirectInputからXinputへの変換器
	* @param input DINPUT_JOYSTATE dInput構造体
	*/
	void InputConverter(const DINPUT_JOYSTATE& dInput);
	static int GetPadThumbLX(){ return Input.ThumbLX; }	 //左スティックの横軸値
	static int GetPadThumbLY() { return Input.ThumbLY; }	//左スティックの縦軸値
	static int GetPadThumbRX() { return Input.ThumbRX; }	//右スティックの横軸値
	static int GetPadThumbRY() { return Input.ThumbRY; }	//右スティックの縦軸値
	static int GetNowKey() { return nowKey; }
	static PAD_STATE GetPadState() { return state; }
};

