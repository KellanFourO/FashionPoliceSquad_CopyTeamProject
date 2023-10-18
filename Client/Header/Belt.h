#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Build_Obj.h"

class CBeltState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;


END

class CBelt : public Engine::CGameObject
{
	//todo			  0		1		2		3			4		5
	enum BELTSTATE { IDLE, READY, ATTACK, CHARGEATTACK, CHARGE, ROPE, BELTSTATE_END};

private:
	explicit CBelt(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBelt(const CBelt& rhs);
	virtual ~CBelt();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

public:
	CBeltState* Get_State(_int _index) { return m_pStateArray[_index]; }
	CPlayer*	Get_Host() { return m_pPlayer;}
	CTransform* Get_HostTransform() { return m_pPlayerTransform;}
	_vec3		Get_StartPos() { return m_vStartPos;}
	_vec3		Get_EndPos() { return m_vEndPos;}
	CBuild_Obj* Get_TargetObj() { return m_pTargetObj;}
	CGameObject* Get_Target() { return m_pTarget;}
	_float		Get_Range() { return m_fRange;}


	void		Set_Target(CGameObject* _pObj) { m_pTarget = _pObj;}
	void		Set_TargetObj(CBuild_Obj* _pObj) { m_pTargetObj = _pObj;}

private:
	HRESULT Add_Component();
	void Mouse_Input(const _float& fTimeDelta);
	void Key_Input();
	void Rope();
	void ReadyState();
	void StateMachine(const _float& fTimeDelta);

public:
	CRcTex* m_pBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pPlayerTransform = nullptr;
	CTransform* m_pBeltObjTransform = nullptr;


	_float m_fBeltMoveRight = 0.f;
	_float m_fBeltMoveDown = 0.f; //위치 이동
	_float m_fHitTime = 0.f;
	_float m_fRange = 70.0f;
	_float m_fDmg = 10.f;



	_vec3	m_vBeltScale = { 0.f,0.f,0.f };
	_vec3	m_vPlayerLook = { 0.f,0.f,0.f };
	_vec3	m_vStartPos = { 0.f,0.f,0.f};
	_vec3	m_vEndPos = { 0.f,0.f,0.f};

	CBeltState* BeltState = nullptr;	// 총 상태
	CBeltState* m_pStateArray[BELTSTATE_END];

	_bool			m_bCharged = false;
	_bool			m_bLateInit = true;
	_bool			m_bHit = false;

	_bool			m_bFix = false;
	_bool			m_bCheck = false;

	CPlayer*		m_pPlayer = nullptr;
	CBuild_Obj*		m_pTargetObj = nullptr;
	CGameObject*	m_pTarget = nullptr;


public:
	static CBelt* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

