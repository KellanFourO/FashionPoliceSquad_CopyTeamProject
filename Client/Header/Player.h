#pragma once
#include "Base.h"
#include "GameObject.h"
#include "PlayerGun.h"
#include "PaintShotGun.h"
#include "TailorAssertRifle.h"
#include "SYRay.h"

class CPlayerState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCalculator;
class CRigidBody;
class CCollider;

END

class CPlayer : public Engine::CGameObject
{
	enum PLAYERSTATE { IDLE, LEFT, RIGHT, HIT, LEVELUP, HEAL, ARMOR, JUMP, PLAYERSTATE_END };

private:
	explicit		CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit		CPlayer(CPlayer& rhs);
	virtual			~CPlayer();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual void	ReadyState();
public:
	void			PlayerLookAtReset(_vec3 PlayerSettingPos);
public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);


public:
	CRigidBody*		Get_RigidBody() { return m_pRigidBody; }
	CTexture*		Get_Texture()	{ return m_pTextureCom;}
	CRcTex*			Get_Buffer()	{ return m_pBufferCom; }
	Player_Info*	Get_INFO() { return &INFO; }
	CPlayerState*	Get_State(_int _index) { return m_pStateArray[_index]; }
	_vec3			Get_Dir() { return m_vMoveDir; }
	CPlayerGun*		Get_Gun() { return m_pGun; }
	_float			Get_XMove() { return m_fXmove;}

	_bool			Get_SceneChange() { return m_bSceneChange;}
	_bool			Get_bRopeSkill() { return m_bRopeSkillOn;}

	void			Set_SceneChange(_bool _bSceneChange) { m_bSceneChange = _bSceneChange;}
	void			SetGun(CLayer* _pLayer = nullptr);
	void			ClearGunList() { m_vecPlayerGun.clear(); }
	void			PlayerRay();


	void			DashOn() { m_bDashSkillOn = true;}
	void			RopeOn() { m_bRopeSkillOn = true; }
	void			EncounterOff() { for(int i = 0; i < 3; ++i) { m_bMonsterEncounter[i] = true;} }

private:
	HRESULT			Add_Component();
	void			Mouse_Input(const _float& fTimeDelta);
	void			TestRopeAction(const _float& fTimeDelta);

	void			StateMachine(_float _fTimeDelta);

public:
	void			Key_Input(const _float& fTimeDelta);
	void			Healed(_float _fHP);		// 회복됐을때
	void			Armor_Get(_float _iArmor);	// 아머 획득
	void			Attacked(_float _fDamage);	// 아머/체력에 받은 데미지 계산
	void			HP_Release();					// 체력 감소(MaxHP를 넘었을때만)
	void			EXP_Up(_int _EXP);		// 경험치 증가
	void			Level_Check();					// 경험치 확인해서 기준 달성 되어있으면 레벨업
	//CGameObject*		Get_Inventory() { return m_Inventory; }

	_bool			Get_TriggerCheck() { return m_bTriggerCheck; } //유진 추가
	void			Set_TriggerCheck() { m_bTriggerCheck = !m_bTriggerCheck; }

	void			Set_TR_NUMBER(TRIGGER_NUMBER eName) { m_eTrName = eName; }
	void			Set_TR_STATE(TRIGGER_STATE eState) { m_eTrState = eState; }

	TRIGGER_NUMBER	Get_TR_NUMBER() { return m_eTrName; } //고유값
	TRIGGER_STATE	Get_TR_STATE() { return m_eTrState; } //전, 진행, 후


private:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CRigidBody*		m_pRigidBody = nullptr;
	Player_INFO		INFO;
	CSYRay*			m_pRay = nullptr;

	TRIGGER_NUMBER	m_eTrName = TRIGGER_NUMBER::TR_END;
	TRIGGER_STATE	m_eTrState = TRIGGER_STATE::TR_BEFORE;

	_bool			m_bSceneChange = false; // 플레이어 신 넘어갈때 제어용
	_bool			m_bJump = false;
	_bool			m_bFix;
	_bool			m_bCheck;
	_bool			m_bLateInit = true;
	_bool			m_bFire = true; // 총쏘는 시간 제어용
	_bool			m_bstair = false; // 계단
	_bool			m_bStep = true;
	_bool			m_bOnGround = true;
	_bool			m_bDash = false; // 대쉬!
	_bool			m_bTriggerCheck = false; //유진 추가 트리거충돌감지용

	_bool			m_bDashSkillOn = true;
	_bool			m_bRopeSkillOn = true;

	_float			m_fTime_HP_Release;
	_float			m_fHP_Reduction = 5.f; // HP 감소 속도
	_float			m_fStepTick = 0.f;
	_float			m_fJumpTick;		//
	_float			m_fJumpCount;		//
	_float			m_fSpeed_Vertical;	//점프 관련
	_float			m_fTall;			//키
	_float			m_fXmove; // 플레이어 바라보는 방향
	_float			m_fDashDelay; //대쉬 딜레이 연속으로 눌리는거 방지
	_float			m_fRayLength = 70.f;

	_vec3			m_vMoveDir;

	CPlayerGun*		m_pGun = nullptr; // 장비중인 총

	_bool			m_bDashCheck = false;
	_uint			m_bDashCount = 0;

	_uint			m_iTriggerTime = 0;

	vector<CPlayerGun*> m_vecPlayerGun;

private:			//TODO 테스트 변수
	_float			m_fTime_HP_Test = 0.f;
	_float			m_fTime_Level_Test = 0.f;
	_float			m_fTime_DEAD_Test = 0.f;

	_bool			m_Speed_Cheat_ON = false;

	_bool			m_bMonsterEncounter[3];
	_bool			m_bEncounterSearch = true;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CPlayerState* m_pStateArray[PLAYERSTATE_END];

private:
	virtual void	Free();

};


