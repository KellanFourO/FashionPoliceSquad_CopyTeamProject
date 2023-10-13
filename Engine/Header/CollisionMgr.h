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
	//어떤것끼리 충돌한것인지
	void  CheckGroup(OBJECTTAG _eLeft, OBJECTTAG _eRight);
	//초기화
	void  Reset();

private:
	map<ULONGLONG, bool> m_mapColInfo;
	_uint	m_arrCheck[(_uint)OBJECTTAG::OBJECT_END];

	bool IsCollision(CCollider* _pLeft, CCollider* _pRight);
	void CheckCollisionByType(OBJECTTAG _eObjectLeft, OBJECTTAG _eObjectRight);
	//정적 물체와 충돌
	void CheckCollisionStatic(OBJECTTAG _eObjectLeft);
	void CheckCollisionStaticOBJ(OBJECTTAG _eObjectLeft);

private:
	virtual void Free();
};

END