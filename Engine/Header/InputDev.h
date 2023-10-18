#pragma once
#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev : public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev(void);
	virtual ~CInputDev(void);

public:
	_byte	Get_DIKeyState(_ubyte byKeyID)
	{ return m_byKeyState[byKeyID]; }

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
	{ 	return m_tMouseState.rgbButtons[eMouse]; 	}

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

_bool		Key_Pressing(_ubyte _byKeyID);
_bool		Key_Down(_ubyte _byKeyID);
_bool		Key_Up(_ubyte _byKeyID);

_bool		Mouse_Pressing(MOUSEKEYSTATE eMouse);
_bool		Mouse_Down(MOUSEKEYSTATE eMouse);
_bool		Mouse_Up(MOUSEKEYSTATE eMouse);
public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);
	void	LateUpdate_InputDev();

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse	= nullptr;

private:
	_byte					m_byKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����
	_byte					m_byKeyData[256];

	DIMOUSESTATE			m_tMouseState;
	_bool					m_tMouseData[MOUSEKEYSTATE::DIM_END];

public:
	virtual void	Free(void);

};
END
#endif // InputDev_h__
