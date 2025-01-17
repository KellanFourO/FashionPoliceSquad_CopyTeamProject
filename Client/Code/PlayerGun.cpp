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
			//float fRandomRange = 0.5f;
			float fRandomRange = 0.005f;//((float)rand() / RAND_MAX)* D3DXToRadian(0.5f);
			//int iRandom = rand() % 2;
			//_vec3			vBulletDirMove;
			//CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
			//NULL_CHECK(pPlayerTransCom); // 플레이어 가져오기

			//pPlayerTransCom->Get_Info(INFO_LOOK, &m_vShotDir);
			//pPlayerTransCom->Get_Info(INFO_RIGHT, &vBulletDirMove);
			//D3DXVec3Normalize(&m_vShotDir, &m_vShotDir);
			for (int i = 0; i < 8; ++i)
			{
				CShotGunBullet* pBullet = CShotGunBullet::Create(m_pGraphicDev, m_vShotPos,m_vShotDir, m_iColorIndex,m_pColorTag);
				pBullet->Set_ObjectTag(OBJECTTAG::PLAYERBULLET);
				m_pColorTag = (COLORTAG)m_iColorIndex;
				Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::PLAYERBULLET, pBullet);
				m_vecShotGunBullet.push_back(pBullet);
			}


			if (!m_vecShotGunBullet.empty())
			{
				for (int i = 0; i < m_vecShotGunBullet.size(); ++i)
				{

					if(!m_vecShotGunBullet[i]->Get_Dead() && m_vecShotGunBullet[i]->Get_BulletType() == m_eBulletType)
					m_vecShotGunBullet[i]->Set_Color(m_iColorIndex);
					m_vecShotGunBullet[i]->Set_ColorTag(m_pColorTag);
				}

				for (int i = 0; i < m_vecShotGunBullet.size(); ++i)
				{
					if (rand() % 2 == 1) {
						fRandomRange *= -1;
					}
					m_vShotDir.x += (rand() % 8) * fRandomRange;
					if (rand() % 2 == 1) {
						fRandomRange *= -1;
					}
					m_vShotDir.y += (rand() % 8) * fRandomRange;
					if (rand() % 2 == 1) {
						fRandomRange *= -1;
					}
					m_vShotDir.z += (rand() % 8) * fRandomRange;
					//D3DXVec3Normalize(&vBulletDirMove, &vBulletDirMove);
					//m_vShotDir += _vec3(0.f, 0.f, 0.06f); //- vBulletDirMove / 40;
					
// 					if (iRandom)
// 					{
// 						m_vShotDir.x += rand() % 5 * fRandomRange;
// 
// 						m_vShotDir.y += rand() % 5 * fRandomRange;
// 
// 						m_vShotDir.z += rand() % 5 * fRandomRange;
// 					}
// 					else
// 					{
// 						m_vShotDir.x -= rand() % 5 * fRandomRange;
// 
// 						m_vShotDir.y -= rand() % 5 * fRandomRange;
// 
// 						m_vShotDir.z -= rand() % 5 * fRandomRange;
// 					}

					m_vecShotGunBullet.back()->Fire(m_vShotPos, m_vShotDir);
					m_vecShotGunBullet.pop_back();
				}


					--m_tGunInfo.m_iCurrentBullet;
					Add_BulletColor();

			}

			break;
		}

		case BULLETTYPE::SHOTGUN_RAZER:
		{

			break;
		}

		case BULLETTYPE::ASSERTRIFLE_BULLET:
		{

				CRifle_Bullet1* pBullet = CRifle_Bullet1::Create(m_pGraphicDev, m_vShotPos, m_iColorIndex);
				pBullet->Set_ObjectTag(OBJECTTAG::PLAYERBULLET);
				Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::PLAYERBULLET, pBullet);
				m_vecRifleBullet.push_back(pBullet);

				m_vecRifleBullet.back()->Fire(m_vShotPos, m_vShotDir);
				m_vecRifleBullet.pop_back();

				--m_tGunInfo.m_iCurrentBullet;

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
		m_vecShotGunBullet.clear();
			for (int i = 0; i < m_tGunInfo.m_iReloadBullet; ++i)
			{
				CShotGunBullet* pBullet = CShotGunBullet::Create(m_pGraphicDev, _vec3(0,0,0),m_vShotDir, _ColorIndex,m_pColorTag);
				pBullet->Set_ObjectTag(OBJECTTAG::PLAYERBULLET);
				Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::PLAYERBULLET,pBullet);
				m_vecShotGunBullet.push_back(pBullet);
			}

			break;
		}

	case Engine::BULLETTYPE::ASSERTRIFLE_BULLET:
		{
		m_vecRifleBullet.clear();
			for (int i = 0; i < m_tGunInfo.m_iReloadBullet; ++i)
			{
				CRifle_Bullet1* pBullet = CRifle_Bullet1::Create(m_pGraphicDev, _vec3(0, 0, 0),iRandomIndex);
				pBullet->Set_ObjectTag(OBJECTTAG::PLAYERBULLET);
				Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::PLAYERBULLET, pBullet);
				m_vecRifleBullet.push_back(pBullet);
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
	//TODO 총이 플레이어와 상호작용하여 항상 일정한 위치 유지
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

	if (m_eBulletType == BULLETTYPE::SHOTGUN_BULLET)
	{
		_vec3 vPos = { vPlayerPos + vPlayerLook * 2.5f + vGunMoveRight * m_fGunMoveRight * 9.4f + vGunMoveDown * m_fGunMoveDown * 10.f };

		m_pTransformCom->Set_Pos(vPos);
	}
	else if(m_eBulletType == BULLETTYPE::ASSERTRIFLE_BULLET)
	{
		m_pTransformCom->Set_Pos(vPlayerPos + vPlayerLook * 2.5f + vGunMoveRight * m_fGunMoveRight * 8.0f + vGunMoveDown * m_fGunMoveDown * 14.5f);
	}



	//TODO 발사 위치 구하기

	_vec3 vMyPos , vFirePos;
	m_pHostTransformCom->Get_Info(INFO_POS, &vFirePos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	m_pHostTransformCom->Get_Info(INFO_LOOK, &m_vShotDir);
	m_vShotPos = (vFirePos + vMyPos) / 2 + m_vShotDir + _vec3(0.f, -0.7f, 0.f);

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

