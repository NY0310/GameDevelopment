/***************************************************************************
*|
*|	概要　　JoyPadを扱うクラス
*|　作成者　GS2 16 中田湧介
*|　作成日　2017/05/16
*|___________________________________________________________________________
****************************************************************************/

#pragma once
#include <dinput.h>
#include<vector>

//Directionのバージョンを指定
#define DIRECTINPUT_VERSION 0x0800


class JoyPad
{
public:
	JoyPad();
	~JoyPad();
	//初期化
	bool Initialize(HWND window);
	//更新処理
	void Update();
private:
	//ジョイパッド一個分の情報
	struct JoyPadSet
	{
		JoyPadSet() {
			inputDevice = NULL;
		}
		//デバイス
		LPDIRECTINPUT8 inputDevice;
		//入力情報
		DIJOYSTATE2 joypad;
		//前回の入力情報
		DIJOYSTATE2 joypadOld;
	};
	//DirectInputインスタンス
	LPDIRECTINPUT8  m_pDInput;


	//ジョイパッド配列
	std::vector<JoyPadSet> m_joyPadSet;
};

