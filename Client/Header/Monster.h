#pragma once
#include "Base.h"
#include "GameObject.h"
//#include "..\Header\MonsterState.h"

//TODO - 승용추가

#include "UI_RecognitionRange.h"


class CMonsterState;
class CBossState;

BEGIN(Engine)
class CSYTex;
class CRcTex;
class CTransform;
class CCalculator;
class CTexture;
class CRigidBody;

class CCollider;

END



class CMonster : public Engine::CGameObject
{

protected:
	explicit			CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			CMonster(CMonster& rhs);
	virtual				~CMonster();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;
	virtual void		ReadyState();

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:
	LPDIRECT3DDEVICE9	Get_GraphicDev() { return m_pGraphicDev; }
	CRigidBody*			Get_RigidBodyCom() { return m_pRigidBody; }
	CSYTex*				Get_BufferCom() { return m_pBufferCom; }
	CTexture*			Get_TextureCom() { return m_pTextureCom; }
	CTransform*			Get_TransformCom() { return m_pTransformCom; }
	CTransform*			Get_PlayerTransform() { return m_pPlayerTransform;}
	Mob_INFO			Get_Info() { return INFO; }
	CGameObject*		Get_MonsterBullet() { return m_pMonsterBullet;}
	_float				Get_Speed() { return m_fSpeed; }
	_float				Get_Length() { return D3DXVec3Length(&(m_pPlayerTransform->m_vInfo[INFO_POS] - m_pTransformCom->m_vInfo[INFO_POS]));}
	_bool				Get_Start() { return m_bStart; }
	_int				Get_MonsterIndex() { return m_iMonsterIndex; }


	void				Set_MonsterIndex(_int _iIndex) { m_iMonsterIndex = _iIndex; }
	void				Set_RecogTextureIndex(_int _iIndex) { m_pUI_Recognition->Set_RecogTextureIndex(_iIndex);}
	void				Set_Start(_bool _bStart) { m_bStart = _bStart; }
	void				Set_Info(Mob_INFO _INFO);
	HRESULT				Set_UI();
	void				Set_Pos(_vec3 _vPos);
	void				Set_TransPos();
	void				Set_MonsterPos(_vec3 vPos) {}
	void				Set_Frame(_float _fVerDevide, _float _fHorDevide, _float _fCurFrame);
	void				Set_Bullet(CGameObject* _pBullet) { m_pMonsterBullet = _pBullet;};
	void				Set_BillBoard(_bool _Billboard) { m_bBillBoard = _Billboard;}

	void				Attacked(_float _fDamage);
	void				Attacked_Fail();

	//TODO 승용추가
	void				Init_PlayerTransform();
	void				BillBoard();
	_bool				Detect();
	_bool				ChaseCatch();
	void				Chase_Target(const _float& fTimeDelta); // 몬스터 이동 코드
	void				StateMachine(const _float& fTimeDelta);


	void				Change_State_IDLE();

	_bool				Find_ColCube(_vec3 _MoveVector, _float _MoveSpeed, const _float& fTimeDelta);

protected:
	HRESULT				Add_Component();

protected:

	//CRcTex*					m_pBufferCom = nullptr;
	_bool					m_bStart = false;
	CSYTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr; // 텍스쳐 컴포넌트
	CCalculator*			m_pCalculatorCom = nullptr; // 계산 컴포넌트
	CRigidBody*				m_pRigidBody = nullptr; // 강체 컴포넌트

	Mob_INFO				INFO;

	_float					m_fFrame = 0;
	_float					m_fVerDevide = 0.f;
	_float					m_fHorDevide = 0.f;
	_int					m_iTextureIndex = 0;
	_int					m_iMonsterIndex = 0;

	_float					m_fAnimateTime = 0;
	_float					m_fHitTime = 0;
	_float					m_fAttackTime = 0;
	_float					m_fDeadTick = 0;



	vector<CGameObject*>	m_AttackVector;

	//TODO - 승용 추가
	_bool					m_bLateInit = true;
	_bool					m_bBillBoard = true;
	//CMyUI*					m_pUI_HPFrame = nullptr;


	CRecognitionRange*		m_pUI_Recognition = nullptr;
	CGameObject*			m_pMonsterBullet = nullptr;
	CTransform*				m_pPlayerTransform = nullptr;

	_float					m_fDectedRange = 15.f; // 탐색범위
	_float					m_fAttackRange = 1.f; // 공격범위
	_float					m_fSpeed = 10.f;


protected:
	virtual void Free();
};

