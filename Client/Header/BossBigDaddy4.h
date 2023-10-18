#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Monster.h"
#include "BrifCase.h"


class CMonsterState;


class CBossBigDaddy4 : public CMonster
{
	enum BIGDADDYSTATE { IDLE, ATTACK, DEAD, BIGDADDYSTATE_END };

private:
	explicit			CBossBigDaddy4(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			CBossBigDaddy4(CMonster& rhs);
	virtual				~CBossBigDaddy4();

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
	_int						GetIndex() { return m_iIndex; }
private:
	HRESULT				Add_Component();



private:
	CMonsterState*		  m_pStateArray[BIGDADDYSTATE_END];
	_int				  m_iIndex = 0;

public:
	static CBossBigDaddy4* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);

private:
	virtual void Free();
};

