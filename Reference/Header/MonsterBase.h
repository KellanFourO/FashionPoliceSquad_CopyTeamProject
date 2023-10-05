#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRigidBody;
class CCollider;


class ENGINE_DLL CMonsterBase : public CGameObject
{
protected:

	explicit CMonsterBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterBase(const CMonsterBase& rhs);
	virtual ~CMonsterBase();

public:
	HRESULT		Ready_GameObject();
	_int		Update_GameObject(const _float& fTimeDelta);


public:
	virtual	void	OnCollisionEnter(CCollider* _pOther);
	virtual	void 	OnCollisionStay(CCollider* _pOther);
	virtual void    OnCollisionExit(CCollider* _pOther);

public:
	CRigidBody* Get_RigidBody() { return m_pRigidBody; }

protected:
	CRigidBody* m_pRigidBody = nullptr;

	//MONSTERTAG	m_eMonsterTag; ���Ŀ� ���� ������ ������ ���� �±� �����
protected:
	virtual void Free();
};

END