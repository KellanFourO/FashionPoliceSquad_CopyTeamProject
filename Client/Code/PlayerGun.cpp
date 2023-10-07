#include "PlayerGun.h"
#include "Player.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "PaintShotGun.h"

#include "ShotGun_Bullet.h"
#include "Rifle_Bullet1.h"
#include "PlayerGunState.h"

CPlayerGun::CPlayerGun(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CPlayerGun::CPlayerGun(const CPlayerGun& rhs) : CGameObject(rhs)
{
	m_pGraphicDev->AddRef();
}

CPlayerGun::~CPlayerGun()
{
}

HRESULT CPlayerGun::Ready_GameObject()
{
	m_pHostTransformCom = m_pHost->m_pTransformCom;
	ZeroMemory(&m_tGunInfo, sizeof(Gun_INFO));
	return S_OK;
}

_int CPlayerGun::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	StateMachine(fTimeDelta);

	for (int i = 0; i < m_vecBullet.size(); ++i)
	{
		if(!m_vecBullet[i])
			m_vecBullet.pop_back();
	}

	__super::Update_GameObject(fTimeDelta);
	return OBJ_NOEVENT;
}

void CPlayerGun::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}


void CPlayerGun::Fire()
{
	switch (m_eBulletType)
	{
		case BULLETTYPE::SHOTGUN_BULLET:
		{
			if (!m_vecBullet.empty())
			{
				m_vecBullet.back()->Fire(m_vShotPos, m_vShotDir);
				--m_tGunInfo.m_iCurrentBullet;
			}

			break;
		}

		case BULLETTYPE::SHOTGUN_RAZER:
		{

			break;
		}

		case BULLETTYPE::ASSERTRIFLE_BULLET:
		{
				if (!m_vecBullet.empty())
				{
					m_vecBullet.back()->Fire(m_vShotPos, m_vShotDir);
					--m_tGunInfo.m_iCurrentBullet;
				}
			break;
		}

		case BULLETTYPE::ASSERTRIFLE_BOMB:
		{

			break;
		}

	}


}

void CPlayerGun::Reload(_int _ColorIndex,_int iRandomIndex)
{

	if(m_tGunInfo.m_iMaxBullet < 0 && m_tGunInfo.m_iCurrentBullet == m_tGunInfo.m_iReloadBullet)
		return;

	switch (m_eBulletType)
	{
	case Engine::BULLETTYPE::SHOTGUN_BULLET:
		{

			for (int i = 0; i < m_tGunInfo.m_iReloadBullet; ++i)
			{
				CBullet* pBullet = CShotGunBullet::Create(m_pGraphicDev, _vec3(0,0,0),_ColorIndex);
				Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::PLAYERBULLET,pBullet);
				m_vecBullet.push_back(pBullet);

			}
			break;
		}

	case Engine::BULLETTYPE::ASSERTRIFLE_BULLET:
		{
			for (int i = 0; i < m_tGunInfo.m_iReloadBullet; ++i)
			{
				CBullet* pBullet = CRifle_Bullet1::Create(m_pGraphicDev, _vec3(0, 0, 0),iRandomIndex);
				Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::PLAYERBULLET, pBullet);
				m_vecBullet.push_back(pBullet);
			}
			break;
		}

	case Engine::BULLETTYPE::ASSERTRIFLE_BOMB:
		{
			for (int i = 0; i < m_tGunInfo.m_iReloadBullet; ++i)
			{

			}
			break;
		}

	}

	m_tGunInfo.m_iCurrentBullet = m_tGunInfo.m_iReloadBullet;
	m_tGunInfo.m_iMaxBullet -= m_tGunInfo.m_iReloadBullet;
}

void CPlayerGun::Rebound()
{
}

void CPlayerGun::StateMachine(const _float& fTimeDelta)
{
	CPlayerGunState* State = m_pGunState->Update(this, fTimeDelta);
	if (State != nullptr) {
		m_pGunState->Release(this);
		m_pGunState = State;
		m_pGunState->Initialize(this);
	}
}

void CPlayerGun::HostMove(const _float& fTimeDelta)
{
	//TODO ���� �÷��̾�� ��ȣ�ۿ��Ͽ� �׻� ������ ��ġ ����
	_vec3 vPlayerPos, vPlayerLook, vPlayerUp;

	m_pHostTransformCom->Get_Info(INFO_UP, &vPlayerUp);
	m_pHostTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pHostTransformCom->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	_vec3 vGunMove;

	D3DXVec3Cross(&vGunMove, &vPlayerUp, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
	D3DXVec3Normalize(&vGunMove, &vGunMove);

	_vec3 vGunMoveRight = vGunMove / 20;
	_vec3 vGunMoveDown = -vPlayerUp / 20;

	_vec3 vPos = { vPlayerPos + vPlayerLook * 2.5f + vGunMoveRight * m_fGunMoveRight * 9.4f + vGunMoveDown * m_fGunMoveDown * 10.f };

	m_pTransformCom->Set_Pos(vPos);

	//TODO �߻� ��ġ ���ϱ�
	_vec3 vMyPos, vDir;
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	vDir = dynamic_cast<CPlayer*>(m_pHost)->Get_Dir();
	m_vShotPos = (vPlayerPos + vMyPos) / 2 + vDir + _vec3(0.f, -0.7f, 0.f);

	MouseInput();

}

void CPlayerGun::MouseInput()
{
	_long	dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
	}
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f));
	}
}


void CPlayerGun::Ready_State()
{
}

void CPlayerGun::Free()
{
	__super::Free();
}

