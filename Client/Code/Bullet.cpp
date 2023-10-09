#include "Bullet.h"
#include "Player.h"
#include "Monster.h"
#include "Export_Utility.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CBullet::CBullet(const CBullet& rhs):CGameObject(rhs)
{
	m_pGraphicDev->AddRef();
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_GameObject()
{
	return S_OK;
}

_int CBullet::Update_GameObject(const _float& fTimeDelta)
{

	//if(m_eObjectTag != OBJECTTAG::PLAYERBULLET)
	Destroy(fTimeDelta);

	if(m_bDead)
	return OBJ_DEAD;

	if (m_bLateInit)
	{
		m_pPlayerTransformCom = dynamic_cast<CTransform*>(Management()->Get_Component(ID_DYNAMIC,LAYERTAG::GAMELOGIC,OBJECTTAG::PLAYER,COMPONENTTAG::TRANSFORM));
		m_bLateInit = false;
	}

	if (m_bShot)
	{
		m_pTransformCom->Move_Pos(&m_vShotDir,fTimeDelta,m_fSpeed);
	}


	m_pCollider->SetCenterPos(m_pTransformCom->m_vInfo[INFO_POS]);

	_int iExit = __super::Update_GameObject(fTimeDelta);
	return iExit;
}

void CBullet::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBullet::Render_GameObject()
{
}

void CBullet::Destroy(const _float& fTimeDelta)
{
	//TODO 오브젝트 풀링을 할 것이라면 여기서 해도되고 총알에서 해도되고 안할거면 여기서 삭제시킬거임
	m_fAge += fTimeDelta;

	if(m_fAge > m_fLifeTime)
	m_bDead = true;
}

void CBullet::OnCollisionEnter(CCollider* _pOther)
{

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		dynamic_cast<CPlayer*>(_pOther->Get_Host())->Attacked(m_fDmg);
		m_bDead = true;
		//TODO 플레이어 총알 오브젝트 풀링 할거면 여기서
	}
	// && m_eHitType == _pOther->Get_Host()->Get_HitType()f
	else if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
	{
		//TODO 몬스터 총알 오브젝트 풀링 할거면 여기서
		dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked(m_fDmg);
		m_bDead = true;
	}
	else
		return;
}

void CBullet::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE)
		return;

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		dynamic_cast<CPlayer*>(_pOther->Get_Host())->Attacked(m_fDmg);
		//TODO 플레이어 총알 오브젝트 풀링 할거면 여기서
	}
	// && m_eHitType == _pOther->Get_Host()->Get_HitType()f
	else if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
	{
		//TODO 몬스터 총알 오브젝트 풀링 할거면 여기서
		dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked(m_fDmg);
	}
	else
		return;
}

void CBullet::OnCollisionExit(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE)
		return;

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		dynamic_cast<CPlayer*>(_pOther->Get_Host())->Attacked(m_fDmg);
		//TODO 플레이어 총알 오브젝트 풀링 할거면 여기서
	}
	// && m_eHitType == _pOther->Get_Host()->Get_HitType()f
	else if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
	{
		//TODO 몬스터 총알 오브젝트 풀링 할거면 여기서
		dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked(m_fDmg);
	}
	else
		return;
}

void CBullet::Fire(_vec3 vShotPos, _vec3 vShotDir)
{
	m_vShotDir = vShotDir;
	m_vShotPos = vShotPos;
	m_pTransformCom->Set_Pos(m_vShotPos);
	m_bShot = true;

}


void CBullet::Free()
{
	__super::Free();
}
