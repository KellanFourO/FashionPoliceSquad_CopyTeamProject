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



class CKickBoardMonster : public CMonster
{
	enum KICKBOARDSTATE { IDLE, CHASE, ATTACK, PATROL, DEAD, KICKBOARDSTATE_END};

private:
	explicit			CKickBoardMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			CKickBoardMonster(CMonster& rhs);
	virtual				~CKickBoardMonster();

public:
	virtual HRESULT		Ready_GameObject(_vec3 pPoint);
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

public:
	void				Ride_On(_vec3 _vDir, _float _fSpeed, _float _fTimeDelta);

public:
	LPDIRECT3DDEVICE9	Get_GraphicDev() { return m_pGraphicDev; }

private:
	HRESULT				Add_Component();

private:
	_vec3			m_StartingPoint;
	CMonsterState*	m_pStateArray[KICKBOARDSTATE_END];

private:
	_float			m_fBehaviorTime = 0.f; // 행동 줄 시간

	_float			m_fCurFrame = 4.f;
	_float			m_fMinFrame = 4.f;
	_float			m_fMaxFrame = 6.f;
	_float			m_fAnimateTime = 0.f;   //애니메이션 관련
	_float			m_fAdditional_frame = 0.f;

	_float			m_fAttackTime = 0;


public:
	static CKickBoardMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pPoint);

private:
	virtual void Free();
};

