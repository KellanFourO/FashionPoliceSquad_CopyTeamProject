#include "Export_System.h"
LPDIRECT3DDEVICE9		Get_GraphicDev()
{
	return CGraphicDev::GetInstance()->Get_GraphicDev();
}
HRESULT		Ready_GraphicDev(CGraphicDev** ppGraphicClass, HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(ppGraphicClass, hWnd, eMode, iSizeX, iSizeY);
}
void		Render_Begin(D3DXCOLOR Color)
{
	return CGraphicDev::GetInstance()->Render_Begin(Color);
}
void		Render_End()
{
	CGraphicDev::GetInstance()->Render_End();
}

_float			Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
void				Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}
HRESULT			Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

// FrameMgr
_bool			IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
HRESULT			Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,
	const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

void		Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color, _int maxWidth, _bool bAnimation)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color,maxWidth, bAnimation);
}

// InputDev
_byte	Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
}
_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
}
_bool Key_Pressing(_ubyte _byKeyID)
{
	return CInputDev::GetInstance()->Key_Pressing(_byKeyID);
}
_bool Key_Down(_ubyte _byKeyID)
{
	return CInputDev::GetInstance()->Key_Down(_byKeyID);
}
_bool Key_Up(_ubyte _byKeyID)
{
	return CInputDev::GetInstance()->Key_Up(_byKeyID);
}

_bool		Mouse_Pressing(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Mouse_Pressing(eMouse);
}

_bool		Mouse_Down(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Mouse_Down(eMouse);
}

_bool		Mouse_Up(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Mouse_Up(eMouse);
}

HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}
void		Update_InputDev(void)
{
	CInputDev::GetInstance()->Update_InputDev();
}

void		LateUpdate_InputDev()
{
	CInputDev::GetInstance()->LateUpdate_InputDev();
}

void	Release_System()
{
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}