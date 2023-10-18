#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Monster.h"
#include "BrifCase.h"
#include "Stage1Boss.h"

class CMonsterState;


class CBossBigDaddy : public CMonster
{
	enum BIGDADDYSTATE { IDLE, ATTACK, DEAD, BIGDADDYSTATE_END };

private:
	explicit			CBossBigDaddy(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			CBossBigDaddy(CMonster& rhs);
	virtual				~CBossBigDaddy();

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
	_int						Get_Index() { return m_iIndex; }


private:
	HRESULT				Add_Component();
	_int				m_iIndex = 0;

private:
	CMonsterState*		  m_pStateArray[BIGDADDYSTATE_END];

public:
	static CBossBigDaddy* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);

private:
	virtual void Free();
};

