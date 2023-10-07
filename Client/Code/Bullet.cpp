#include "Bullet.h"
#include "Player.h"
#include "Monster.h"
#include "Export_Utility.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CBullet::CBullet(const CBullet& rhs) : CGameObject(rhs)
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
	if (m_bLateInit)
	{
		m_pPlayerTransformCom = dynamic_cast<CTransform*>(Management()->Get_Component(ID_DYNAMIC,LAYERTAG::GAMELOGIC,OBJECTTAG::PLAYER,COMPONENTTAG::TRANSFORM));
		m_bLateInit = false;
	}

	if (m_bShot)
	{
		m_pTransformCom->Move_Pos(&m_vShotDir,fTimeDelta,m_fSpeed);
	}



	__super::Update_GameObject(fTimeDelta);
	return OBJ_NOEVENT;
}

void CBullet::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBullet::Destroy()
{
	//TODO ������Ʈ Ǯ���� �� ���̶�� ���⼭ �ص��ǰ� �Ѿ˿��� �ص��ǰ� ���ҰŸ� ���⼭ ������ų����
}

void CBullet::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		dynamic_cast<CPlayer*>(_pOther->Get_Host())->Attacked(m_fDmg);
		//TODO �÷��̾� �Ѿ� ������Ʈ Ǯ�� �ҰŸ� ���⼭
	}
	else if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER && m_eHitType == _pOther->Get_Host()->Get_HitType())
	{
		//TODO ���� �Ѿ� ������Ʈ Ǯ�� �ҰŸ� ���⼭
		dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked(m_fDmg);
	}
	else
		return;
}

void CBullet::OnCollisionStay(CCollider* _pOther)
{
}

void CBullet::OnCollisionExit(CCollider* _pOther)
{
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
