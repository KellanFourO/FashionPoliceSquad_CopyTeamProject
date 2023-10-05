#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class BoundingBox
{
public:
	_float		GetRadius() { return m_fRadius; }
	void		SetRadius(_float _fRadius) { m_fRadius = _fRadius; }

	D3DXVECTOR3		GetMin() { return m_vMin; }
	void			SetMin(D3DXVECTOR3 _vMin) { m_vMin = _vMin; }
	D3DXVECTOR3		GetMax() { return m_vMax; }
	void			SetMax(D3DXVECTOR3 _vMax) { m_vMax = _vMax; }

public:
	D3DXVECTOR3 m_vMin;
	D3DXVECTOR3 m_vMax;
private:
	_float  m_fRadius = 0.f;

};

class CGameObject;
class CCollider;

class ENGINE_DLL COctreeNode : public CBase
{
public:
	explicit COctreeNode();
	explicit COctreeNode(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COctreeNode(const CGameObject& rhs);
	virtual ~COctreeNode();

public:
	HRESULT					InitBoundingBoxVisible();

	void					AddchildNode(COctreeNode* pChild);
	void					AddObject(CGameObject* pObject) { m_vecObjects.push_back(pObject); }

	_bool					IsInNode(const _vec3 _vPos);
	_vec3&					GetPosition()						{ return m_vPosition; }
	_vec3*					GetCorner()							{ return m_vCorner; }
	COctreeNode* const		GetParent()							{ return m_pParent; }
	vector<COctreeNode*>&	GetChildren()						{ return m_vecChildren; }
	COctreeNode* const		GetChildNode(_uint _iChildNum)		{ return m_vecChildren[_iChildNum]; }

	vector<CGameObject*>&	GetObjectList()						{ return m_vecObjects; }
	BoundingBox*			GetBoundingBox()					{ return m_pBoundingBox; }

	void					SetPosition(_vec3 _vPos)			{ m_vPosition = _vPos; }
	void					SetCorners(_vec3* _vCorners)		{ ::CopyMemory(m_vCorner, _vCorners, 8 * sizeof(_vec3)); }
	void					SetParent(COctreeNode* const _pParent) { m_pParent = _pParent; }

	void					Render_OctreeNode(LPDIRECT3DDEVICE9 pGraphicDev, bool IsRoot = false);
	void					CheckCullNode(COctreeNode* Node);

	void                    CullNode(_int _iCulled)				{ m_iCulled = _iCulled; }
	_int                    IsCulled()							{ return m_iCulled; }

public:
	const static _uint      m_iChild_Node_Count = 8;

private:
	BoundingBox*			m_pBoundingBox = nullptr;
	const _float			m_fLooseFactor = 2.f;

	COctreeNode*			m_pParent = nullptr;

	vector<COctreeNode*>    m_vecChildren;
	vector<CGameObject*>    m_vecObjects;

	_vec3					m_vPosition;
	_vec3					m_vCorner[8];

	_int					m_iCulled;

	// Only Used in Debug mode
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
public:
	virtual	void	Free();
};

END