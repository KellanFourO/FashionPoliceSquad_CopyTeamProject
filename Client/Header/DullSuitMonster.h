#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Monster.h"

class CMonsterState;

BEGIN(Engine)

class CSYTex;
class CTransform;
class CCalculator;
class CTexture;
class CRigidBody;

class CCollider;

END

class CDullSuitMonster : public CMonster
{
	enum DULLSUITSTATE { IDLE, CHASE, ATTACK, DEAD, DULLSUITSTATE_END };
private:
	explicit			CDullSuitMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			CDullSuitMonster(CMonster& rhs);
	virtual				~CDullSuitMonster();

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
	CMonsterState*		Get_State(_int _index) { return m_pStateArray[_index]; }

private:
	HRESULT				Add_Component();

private:
	CMonsterState*		m_pStateArray[DULLSUITSTATE_END];

public:
	static CDullSuitMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

