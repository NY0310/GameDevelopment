/***************************************************************************
*|
*|	�T�v�@�@JoyPad�������N���X
*|�@�쐬�ҁ@GS2 16 ���c�N��
*|�@�쐬���@2017/05/16
*|___________________________________________________________________________
****************************************************************************/

#pragma once
#include <dinput.h>
#include<vector>

//Direction�̃o�[�W�������w��
#define DIRECTINPUT_VERSION 0x0800


class JoyPad
{
public:
	JoyPad();
	~JoyPad();
	//������
	bool Initialize(HWND window);
	//�X�V����
	void Update();
private:
	//�W���C�p�b�h����̏��
	struct JoyPadSet
	{
		JoyPadSet() {
			inputDevice = NULL;
		}
		//�f�o�C�X
		LPDIRECTINPUT8 inputDevice;
		//���͏��
		DIJOYSTATE2 joypad;
		//�O��̓��͏��
		DIJOYSTATE2 joypadOld;
	};
	//DirectInput�C���X�^���X
	LPDIRECTINPUT8  m_pDInput;


	//�W���C�p�b�h�z��
	std::vector<JoyPadSet> m_joyPadSet;
};

