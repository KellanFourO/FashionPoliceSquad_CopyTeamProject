#pragma once

#include "Base.h"
#include "GameObject.h"
#include "MapCursor.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CTransform;
class CCalculator;
class CCollider;
class CRigidBody;
END

class CBuild_Cube : public Engine::CGameObject
{
private:
	explicit CBuild_Cube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBuild_Cube(const CBuild_Cube& rhs);
	virtual ~CBuild_Cube();

public:
	virtual HRESULT		Ready_GameObject(_vec3 pMouse_Pos, _vec3 Size);
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

	void				Set_Cursor_Pos(_vec3& pPos) { m_Cursor_Pos = pPos; }
	void				Set_CubeIndex(_uint pIndex) { m_iIndex = pIndex; }
	_uint				Get_CubeIndex() { return m_iIndex; }

private:
	HRESULT				SetUp_Meterial();
	HRESULT				Add_Component();

private:
	CCubeTex*		m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	_vec3			m_Cursor_Pos = {};
	_uint			m_TextureNumber = 0;

	CUBESIZE		m_fCubeSize = { VTXITV, VTXITV, VTXITV };

public:
	static  CBuild_Cube* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pMouse_Pos, _uint TextureNum, _vec3 CubeSize, _uint CubeIndex);

private:
	virtual void Free();


};
