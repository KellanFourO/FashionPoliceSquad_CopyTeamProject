#pragma once

#include "Base.h"
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CollisionMgr : public CBase
{
	DECLARE_SINGLETON(CollisionMgr)

private:
	explicit CollisionMgr();
	virtual ~CollisionMgr();

public:
	void LateUpdate_Collision();

public:
	map<ULONGLONG, bool>& GetMapColInfo() { return m_mapColInfo; }
	void  SetMapColInfo(ULONGLONG ID, bool bCollision) { m_mapColInfo[ID] = bCollision; }
	//��ͳ��� �浹�Ѱ�����
	void  CheckGroup(OBJECTTAG _eLeft, OBJECTTAG _eRight);
	//�ʱ�ȭ
	void  Reset();
	bool CollisionRayToCube(CCollider* pCRay, CCollider* pCBox, _vec3 vRayDest);

private:
	map<ULONGLONG, bool> m_mapColInfo;
	_uint	m_arrCheck[(_uint)OBJECTTAG::OBJECT_END];

	bool IsCollision(CCollider* _pLeft, CCollider* _pRight);
	void CheckCollisionByType(OBJECTTAG _eObjectLeft, OBJECTTAG _eObjectRight);
	//���� ��ü�� �浹
	void CheckCollisionStatic(OBJECTTAG _eObjectLeft);
	void CheckCollisionStaticOBJ(OBJECTTAG _eObjectLeft);
private:
	_vec3 m_vRayDir;
	_vec3 m_fDist;
	_vec3 m_DestOriginLen;
private:
	virtual void Free();
};

END