#pragma once
#include "Monster.h"
#include "Engine_Define.h"


class CMonsterState;
BEGIN(Engine)

class CSYTex;
class CTransform;
class CCalculator;
class CTexture;
class CRigidBody;

class CCollider;

END

class CStage1Boss : public CMonster
{
	enum STAGE1_BOSSSTATE { IDLE, JUMP, THROWSINGLE, THROWMULTI, SPAWN, BRIFSHIELD, DEAD, STAGE1_BOSSSTATE_END };
private:
	explicit			CStage1Boss(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			CStage1Boss(CMonster& rhs);
	virtual				~CStage1Boss();

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
	_int				Get_OriginIndex() { return m_iOriginIndex;}

	void				Set_OriginIndex(_int _index) { m_iOriginIndex = _index;}

private:
	HRESULT				Add_Component();
	void				PhaseChange();

private:
	CMonsterState*			m_pStateArray[STAGE1_BOSSSTATE_END];
	_int					m_iOriginIndex = 0;
	_bool					m_bDead2 = false; // 죽었을 때, 이미지 변경을 위한 불변수

public:
	static CStage1Boss*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

