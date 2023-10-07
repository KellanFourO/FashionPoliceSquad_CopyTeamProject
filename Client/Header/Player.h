#pragma once
#include "Base.h"
#include "GameObject.h"
#include "PlayerGun.h"
#include "PaintShotGun.h"
#include "TailorAssertRifle.h"

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


private:
	HRESULT			Add_Component();
	void			SetGun();
	void			Mouse_Input(const _float& fTimeDelta);

	void			StateMachine(_float _fTimeDelta);

public:
	void			Key_Input(const _float& fTimeDelta);
	void			Healed(_float _fHP);		// ȸ��������
	void			Armor_Get(_float _iArmor);	// �Ƹ� ȹ��
	void			Attacked(_float _fDamage);	// �Ƹ�/ü�¿� ���� ������ ���
	void			HP_Release();					// ü�� ����(MaxHP�� �Ѿ�������)
	void			EXP_Up(_int _EXP);		// ����ġ ����
	void			Level_Check();					// ����ġ Ȯ���ؼ� ���� �޼� �Ǿ������� ������
	//CGameObject*		Get_Inventory() { return m_Inventory; }


private:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CRigidBody*		m_pRigidBody = nullptr;

	Player_INFO		INFO;

	_bool			m_bJump = false;
	_bool			m_bFix;
	_bool			m_bCheck;
	_bool			m_bLateInit = true;
	_bool			m_bFire = true; // �ѽ�� �ð� �����

	_float			m_fTime_HP_Release;
	_float			m_fHP_Reduction = 5.f; // HP ���� �ӵ�
	_float			m_fJumpTick;		//
	_float			m_fJumpCount;		//
	_float			m_fSpeed_Vertical;	//���� ����
	_float			m_fTall;			//Ű
	_float			m_fXmove; // �÷��̾� �ٶ󺸴� ����

	_vec3			m_vMoveDir;


	CPlayerGun*		m_pGun = nullptr; // ������� ��


	vector<CPlayerGun*> m_vecPlayerGun;

private:			//TODO �׽�Ʈ ����
	_float			m_fTime_HP_Test = 0.f;
	_float			m_fTime_Level_Test = 0.f;
	_float			m_fTime_DEAD_Test = 0.f;
	_bool			m_Speed_Cheat_ON = false;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CPlayerState* m_pStateArray[PLAYERSTATE_END];

private:
	virtual void	Free();

};


