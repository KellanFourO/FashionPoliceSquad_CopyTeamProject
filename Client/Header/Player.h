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

// 	���� �߰�
// 	CTransform*&	Get_TransformCom()	{ return m_pTransformCom; }
	//��ȣ �߰�
	CRigidBody*		Get_RigidBody()		{ return m_pRigidBody; }

private:
	HRESULT			Add_Component();
	void			Mouse_Input(const _float& fTimeDelta);

	void			Gun_Select(_int Gun_Number);

public:
	void			Healed		(_float _fHP);		// ȸ��������
	void			Armor_Get	(_float _iArmor);	// �Ƹ� ȹ��
	void			Attacked	(_float _fDamage);	// �Ƹ�/ü�¿� ���� ������ ���
	void			HP_Release();					// ü�� ����(MaxHP�� �Ѿ�������)
	void			EXP_Up		(_int _EXP);		// ����ġ ����
	void			Level_Check();					// ����ġ Ȯ���ؼ� ���� �޼� �Ǿ������� ������


private:
	CRcTex* m_pBufferCom = nullptr; // �ؽ�ó�� �׸������� ���� ������Ʈ
	CTexture* m_pTextureCom = nullptr; // �ؽ��� ������Ʈ
	//CCalculator* m_pCalculatorCom = nullptr; // ��� ������Ʈ

public:
	CRigidBody* m_pRigidBody = nullptr;// ��ü ������Ʈ

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
	_float			m_fSpeed_Vertical;	//���� ����
	_float			m_fTall;			//Ű

	_float			m_fTimeDelta;
	_float			m_fTime_HP_Release;




	_bool			m_bFix = false;		//
	_bool			m_bCheck = false;	//ȭ������

	CGun*			m_RightHand = nullptr; //����ִ� ��
	_int			m_iGunNumber=0;

	CGameObject*	m_Inventory = nullptr;

private:										//�׽�Ʈ��orġƮ�� �ڵ�
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


