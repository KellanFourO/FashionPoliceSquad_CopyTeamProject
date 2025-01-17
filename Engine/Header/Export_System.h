#pragma once
#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "InputDev.h"

BEGIN(Engine)

// GraphicDev
inline LPDIRECT3DDEVICE9		Get_GraphicDev();
inline HRESULT		Ready_GraphicDev(CGraphicDev** ppGraphicClass, HWND hWnd, WINMODE eMode, const _uint& iSizeX = WINCX, const _uint& iSizeY = WINCY);
inline void		Render_Begin(D3DXCOLOR Color);
inline void		Render_End();

// TimerMgr
inline _float			Get_TimeDelta(const _tchar* pTimerTag);
inline void				Set_TimeDelta(const _tchar* pTimerTag);
inline HRESULT			Ready_Timer(const _tchar* pTimerTag);

// FrameMgr
inline _bool			IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
inline HRESULT			Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

// FontMgr
inline HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,
	const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight);

inline void		Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color, _int iMaxWidth, _bool bAnimation);

// InputDev
inline _byte			Get_DIKeyState(_ubyte byKeyID);
inline _byte			Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _long			Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

inline _bool			Key_Pressing(_ubyte _byKeyID);
inline _bool			Key_Down(_ubyte _byKeyID);
inline _bool			Key_Up(_ubyte _byKeyID);

inline _bool			Mouse_Pressing(MOUSEKEYSTATE eMouse);
inline _bool			Mouse_Down(MOUSEKEYSTATE eMouse);
inline _bool			Mouse_Up(MOUSEKEYSTATE eMouse);

inline HRESULT			Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline void				Update_InputDev(void);
inline void				LateUpdate_InputDev();

inline void				Release_System();


#include "Export_System.inl"

END