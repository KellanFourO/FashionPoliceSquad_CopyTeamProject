#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CCrossTex;
class CRcTex;

class CTexture;
class CTransform;
class CCalculator;
class CCollider;

END

class CBuild_Obj : public CGameObject
{
private:
	explicit CBuild_Obj(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBuild_Obj(const CBuild_Obj& rhs);
	virtual ~CBuild_Obj();

public:
	virtual HRESULT Ready_GameObject(_uint pRotate, _vec3 pMouse_Pos, _vec3 Size);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

	void			Set_TextrureNum(_uint iNum) { m_TextureNumber = iNum; }
	void			Set_OBJ_TYPE(OBJ_TYPE eType) { m_eOBJ_Type = eType; }
	void			Set_Cursor_Pos(_vec3& pPos) { m_Cursor_Pos = pPos; }
	void			Set_OBJIndex(_uint pIndex) { m_iIndex = pIndex; }
	_uint			Get_OBJIndex() { return m_iIndex; }

protected:
	virtual HRESULT	Add_Component();

private:

	vector<IDirect3DCubeTexture9*> m_VecTempCube;
	vector<IDirect3DBaseTexture9*> m_VecTempPlane;
	
	CCubeTex*		m_pBufferCubeCom = nullptr;
	CRcTex*			m_pBufferRcCom = nullptr;

	CTransform*		m_pTransformCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;

	const int		cubeTextureStartIndex = 1000;
	const int		planeTextureStartIndex = 3000;

	_vec3			m_Cursor_Pos = {};
	_uint			m_TextureNumber = 0;

	OBJ_TYPE		m_eOBJ_Type = OBJ_TYPE::OBJ_TYPE_END;
	CUBESIZE		m_fCubeSize;

public:
	static  CBuild_Obj* Create(LPDIRECT3DDEVICE9 pGraphicDev, 
			_vec3 pMouse_Pos, _uint TextureNum, _vec3 Size, 
			_uint pRotate, _uint Index, OBJ_TYPE eType);

private:
	virtual void Free();

};

