#include "JoyPad.h"
#include <vector>
#include <dinput.h>
template<typename T>
void SarfeRelease(T*& p) {
	if (p)
	{
		p->Release();
		p = NULL;
	}
}
//検出したジョイパッドの情報を格納するための構造体
struct SearchJoyPadSet
{
	//DirectInput
	LPDIRECTINPUT8 dinput;
	//各コントローラーごとのデバイス
	std::vector <LPDIRECTINPUTDEVICE8> joyPadDev;

	SearchJoyPadSet()
	{
		dinput = NULL;
	}
};

////検出した入力オブジェクト1個ごとに呼ばれるこーぶバック
//BOOL CALLBACK EnumObjectCallBack(LPCDIDEVICEOBJECTINSTANCE pInstance, void* pvRef) {
//
//	return DIENUM_CONTINUE;
//}
//検出したジョイパッドごとに呼ばれるこーぶバック
BOOL CALLBACK EnumjoyPadCallBack(LPCDIDEVICEINSTANCE Ipddi,void* pvRef)
{
	//結果
	HRESULT ret;

	SearchJoyPadSet* param = reinterpret_cast<SearchJoyPadSet*>(pvRef);

	LPDIRECTINPUTDEVICE8 dev;

		//コントローラーごとにデバイスを生成
	ret = param->dinput->CreateDevice(
		Ipddi->guidInstance,
		&dev,
		NULL
	);



		//失敗したら
		if (ret != DI_OK)
		{
			goto NEXT;
		}

		//デーダ形式を設定(拡張機能つきジョイスティック)
		dev->SetDataFormat(&c_dfDIJoystick2);


	//配列に追加
		param->joyPadDev.push_back(dev);

NEXT:
	return DIENUM_CONTINUE;
}


JoyPad::JoyPad()
{

}


JoyPad::~JoyPad()
{
	//配列にあるデバイスを開放
	std::vector<JoyPadSet>::iterator it;
	for (it = m_joyPadSet.begin(); it != m_joyPadSet.end(); it++)
	{
		SarfeRelease(it->inputDevice);
	}
}

bool JoyPad::Initialize(HWND window)
{
	//結果
	HRESULT ret;
	//DirectInputの作成
	ret = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, NULL);

	//受け渡し用パラメータ
	SearchJoyPadSet param;
	param.dinput = m_pDInput;
	//利用可能なデバイスを列挙する（コントローラーを探す）
	ret = m_pDInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL, NULL, &param, DIEDFL_ATTACHEDONLY);

	// 失敗したら
	if (ret != DI_OK)
		return false;


	//デバイス配列を設定
	std::vector<LPDIRECTINPUTDEVICE8>::iterator it;
	for (it = param.joyPadDev.begin(); it != param.joyPadDev.end(); it++ )
	{
		LPDIRECTINPUTDEVICE8  dev = *it;
		ret = dev->SetCooperativeLevel(window, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

		//失敗したら
		if (ret != DI_OK)
			continue;

		ret = dev->Acquire();

		//失敗したら
		if (ret != DI_OK)
			continue;
	}

	//デバイス1個分の情報
	JoyPadSet initPad;
	initPad.inputDevice = dev;
	//配列に追加する
	m_joyPadSet.push_back(initPad);
	//次を検索
	return true;

}

void JoyPad::Update()
{
	std::vector<JoyPadSet>::iterator it;
	HRESULT ret;

	for (it = m_joyPadSet.begin(); it != m_joyPadSet.end; it++)
	{
		//情報を更新する
		it->inputDevice->poll();
		//失敗したら
		if (ret != DI_OK)
		{
			//アクセス権を要求
			ret = it->inputDevice->Acquire();
			while (ret == DIERR_INPUTLOST)
			{
				//アクセス権を要求
				ret = it->inputDevice->Acquire();
			}
	}
		//前フレームの情報をバックアップ
		it->joypadOld = it->joypad;
		//入力情報を取得
		ret = it->inputDevice->GetDeviceState(sizeof(DIJOYSTATE2),&it->joypad);

		//失敗したら
		if (ret != DI_OK)
			continue;

		//if ((BYTE)(it->joypad.rgbButtons[0] & 0x80 == 0))
		//{
		//	//ボタンが押されてる
		//}

	}
}
