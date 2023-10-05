#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Monster.h"
//#include "..\Header\MonsterState.h"

//TODO - �¿��߰�
#include "MyUI.h"

class CMonsterState;


BEGIN(Engine)

class CRcTex;
class CTransform;
class CCalculator;
class CTexture;
class CRigidBody;

class CCollider;

END



class CKickBoard : public CMonster
{

private:
	explicit			CKickBoard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			CKickBoard(CMonster& rhs);
	virtual				~CKickBoard();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;


public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:
	void				Ride_On(_vec3 _vDir, _float _fSpeed, _float _fTimeDelta);

public:
	LPDIRECT3DDEVICE9	Get_GraphicDev() { return m_pGraphicDev; }

	// TODO - �¿� �߰�
	//HRESULT				Set_HP();
	//void				Set_HPValue();

private:
	HRESULT				Add_Component();

private:
	//TODO - �¿� �߰�
	CMyUI* m_pUI_HPFrame = nullptr;
	CMyUI* m_pUI_HPValue = nullptr;

private:
	_float			m_fBehaviorTime = 0.f; // �ൿ �� �ð�

	_float			m_fCurFrame = 4.f;
	_float			m_fMinFrame = 4.f;
	_float			m_fMaxFrame = 6.f;
	_float			m_fAnimateTime = 0.f;   //�ִϸ��̼� ����
	_float			m_fAdditional_frame = 0.f;

	_float			m_fAttackTime = 0;

public:
	CGameObject* MonsterBullet = nullptr;
public:
	static CKickBoard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

