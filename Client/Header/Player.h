#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Gun.h"
#include "Inventory.h"

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
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(CPlayer& rhs);
	virtual  ~CPlayer();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

// 	유진 추가
// 	CTransform*&	Get_TransformCom()	{ return m_pTransformCom; }
	//준호 추가
	CRigidBody*		Get_RigidBody()		{ return m_pRigidBody; }

private:
	HRESULT			Add_Component();
	void			Mouse_Input(const _float& fTimeDelta);

	void			Gun_Select(_int Gun_Number);

public:
	void			Healed		(_float _fHP);		// 회복됐을때
	void			Armor_Get	(_float _iArmor);	// 아머 획득
	void			Attacked	(_float _fDamage);	// 아머/체력에 받은 데미지 계산
	void			HP_Release();					// 체력 감소(MaxHP를 넘었을때만)
	void			EXP_Up		(_int _EXP);		// 경험치 증가
	void			Level_Check();					// 경험치 확인해서 기준 달성 되어있으면 레벨업


private:
	CRcTex* m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTexture* m_pTextureCom = nullptr; // 텍스쳐 컴포넌트
	//CCalculator* m_pCalculatorCom = nullptr; // 계산 컴포넌트

public:
	CRigidBody* m_pRigidBody = nullptr;// 강체 컴포넌트

public:
	float			m_fXmove;
	_vec3			m_vDir;
	_vec3			m_vMoveDir;
private:
	Player_INFO		INFO;



private:
	_bool			m_bJump;
	_bool			On_a_Ground;
	_bool			m_bLineCol;

	_float			m_fJumpTick;		//
	_float			m_fJumpCount;		//
	_float			m_fSpeed_Vertical;	//점프 관련
	_float			m_fTall;			//키

	_float			m_fTimeDelta;
	_float			m_fTime_HP_Release;




	_bool			m_bFix = false;		//
	_bool			m_bCheck = false;	//화면정지

	CGun*			m_RightHand = nullptr; //들고있는 총
	_int			m_iGunNumber=0;

	CGameObject*	m_Inventory = nullptr;

private:										//테스트용or치트용 코드
	_float			m_fTime_HP_Test = 0.f;
	_float			m_fTime_Level_Test = 0.f;
	_float			m_fTime_DEAD_Test = 0.f;

	_bool			m_Speed_Cheat_ON = false;

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void				Set_vPos();
	void				Set_Info(Player_Info _INFO);
	void				Set_Gun(CGameObject* _Gun) { m_RightHand = dynamic_cast<CGun*>(_Gun); }
	void				Set_Bullet(_float _CurBullet, _float _MaxBullet) { INFO.fCurBullet = _CurBullet; INFO.fMaxBullet = _MaxBullet; }
	Player_Info*		Get_INFO() { return &INFO; }

	//CGameObject*		Get_Inventory() { return m_Inventory; }

	void				Key_Input(const _float& fTimeDelta);

private:
	virtual void	Free();

};


