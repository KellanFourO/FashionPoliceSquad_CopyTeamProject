#pragma once

#include "Base.h"
#include "GameObject.h"
#include "WireFrame.h"
#include "Build_Cube.h"


BEGIN(Engine)

class CRcCol;
class CCubeTex;
class CTransform;
class CCalculator;
class CTexture;

END

class CMapCursor : public CGameObject
{
private:
	explicit CMapCursor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMapCursor(const CMapCursor& rhs);
	virtual ~CMapCursor();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimedelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	HRESULT			Add_Component();
	HRESULT			Update_ImGui();

private:
	HRESULT	Key_Input(const _float& fTimeDelta, _vec3 pMouse_Pos);
	_vec3	MapCursor_At_Tool();

public:
	_vec3*	Get_MapCursor_Pos() { return &m_vCursor_Pos; }

	CUBESIZE    Get_MapCursorSize() { return m_fCubesize; }
	CUBEROT		Get_MapCursorRot() { return m_fCubeRot; }

private:
	//CRcCol*					m_pBufferCom = nullptr;
	CCubeTex*					m_pBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CCalculator*				m_pCalculatorCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

	CUBESIZE					m_fCubesize;
	CUBEROT						m_fCubeRot;

public:
	static CMapCursor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	_vec3	m_vCursor_Pos;


private:
	virtual void Free();


};


