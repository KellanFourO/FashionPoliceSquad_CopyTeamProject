#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Monster.h"
#include "BrifCase.h"


class CMonsterState;


class CBigDaddyMonster : public CMonster
{
	enum BIGDADDYSTATE { IDLE, CHASE, ATTACK, DEAD, BIGDADDYSTATE_END };

private:
	explicit			CBigDaddyMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			CBigDaddyMonster(CMonster& rhs);
	virtual				~CBigDaddyMonster();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;
	virtual void		ReadyState() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:
	CMonsterState*				Get_State(_int _index) { return m_pStateArray[_index]; }

private:
	HRESULT				Add_Component();



private:
	CMonsterState*		  m_pStateArray[BIGDADDYSTATE_END];

public:
	static CBigDaddyMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

