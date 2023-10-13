#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CRcTex;

class CTexture;
class CTransform;
class CCalculator;
class CCollider;

END

//만들어지는 OBJ 객체
//설정하는 사항에 대해서는 struct -> tagOBJData 참고
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

	void			Render_Texture();

	void			Init_PlayerTransform();
	void			Set_BillBoard(_bool _BillCheck) { m_bBillBoard = _BillCheck; }
	void			BillBoard();
	void			BillBoard_X();

	void			Set_TextrureNum(_uint iNum) { m_TextureNumber = iNum; }
	void			Set_OBJ_TYPE(OBJ_TYPE eType) { m_eOBJ_Type = eType; }
	void			Set_OBJ_ATTRIBUTE(OBJ_ATTRIBUTE eAttri) { m_eAttribute = eAttri; }
	void			Set_Cursor_Pos(_vec3& pPos) { m_Cursor_Pos = pPos; }
	void			Set_OBJIndex(_uint pIndex) { m_iIndex = pIndex; }
	_uint			Get_OBJIndex() { return m_iIndex; }

	int				Get_OBJ_HP() { return m_iHp; }
	OBJ_INTERACTION Get_OBJ_Interaction() { return m_eInteraction; }
	
protected:
	virtual HRESULT	Add_Component();

private:
	HRESULT			SetUp_Meterial();

private:

	vector<IDirect3DCubeTexture9*>			m_VecTempCube;
	vector<IDirect3DBaseTexture9*>			m_VecTempPlane;

	SCENETAG			m_NowScene			= SCENETAG::SCENETAG_END;

	CCubeTex*			m_pBufferCubeCom	= nullptr;
	CRcTex*				m_pBufferRcCom		= nullptr;

	CTransform*			m_pTransformCom		= nullptr;
	CTexture*			m_pTextureCom		= nullptr;
	CCalculator*		m_pCalculatorCom	= nullptr;
	CTransform*			m_pPlayerTransform	= nullptr;

	const int			cubeTextureStartIndex  = 1000;
	const int			planeTextureStartIndex = 3000;

	_bool				m_bLateInit			= true;
	_bool				m_bBillBoard		= true;

	_vec3				m_myObjPos;
	_vec3				m_Cursor_Pos		= {};
	_uint				m_TextureNumber		= 0;

	OBJ_TYPE			m_eOBJ_Type			= OBJ_TYPE::OBJ_TYPE_END;
	OBJ_ATTRIBUTE		m_eAttribute		= OBJ_ATTRIBUTE::ATTRIBUTE_END;

	//속성이 상호작용일 때 사용
	OBJ_INTERACTION		m_eInteraction;

	//속성이 파괴일 때 사용
	int					m_iHp;

	CUBESIZE			m_fCubeSize;

public:
	static  CBuild_Obj* Create(LPDIRECT3DDEVICE9 pGraphicDev, 
			_vec3 pMouse_Pos, _uint TextureNum, _vec3 Size, 
			_uint pRotate, _uint Index, OBJ_TYPE eType, OBJ_ATTRIBUTE eAttri);

private:
	virtual void Free();

};

