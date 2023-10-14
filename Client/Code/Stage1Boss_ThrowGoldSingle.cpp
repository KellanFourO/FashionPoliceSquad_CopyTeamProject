#include "stdafx.h"
#include "Stage1Boss_ThrowGoldSingle.h"
#include "SYTex.h"
#include "Texture.h"
#include "Transform.h"
#include "Export_Utility.h"

//#include "..\Header\MonsterState_IDLE_Walk.h"
//#include "..\Header\MonsterState_IDLE_Attack.h"

CStage1Boss_ThrowGoldSingle::CStage1Boss_ThrowGoldSingle()
{

}

CStage1Boss_ThrowGoldSingle::~CStage1Boss_ThrowGoldSingle()
{
}

void CStage1Boss_ThrowGoldSingle::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);

	m_fMinFrame = 5.f;
	m_fMaxFrame = 1.f;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 1;


	//TODO 페이즈에 따라 투척 속도 또는 개수 증가 할 예정
	switch (m_ePhase)
	{
	case Engine::BOSSPHASE::PHASE_1:
		m_iThrowCount = 6;
		m_iBulletCount = 2;
		m_fBulletSpeed = 120.f;
		m_fThrowSpeed = 0.25f;
		break;
	case Engine::BOSSPHASE::PHASE_2:
		m_iThrowCount = 8;
		m_iBulletCount = 4;
		m_fBulletSpeed = 140.f;
		m_fThrowSpeed = 0.15f;
		break;
	case Engine::BOSSPHASE::PHASE_3:
		m_iThrowCount = 8;
		m_iBulletCount = 6;
		m_fBulletSpeed = 170.f;
		m_fThrowSpeed = 0.10f;
		break;
	}
}

CMonsterState* CStage1Boss_ThrowGoldSingle::Update(CMonster* Monster, const float& fDetltaTime)
{

	Targeting();
	switch (m_eThrowstate)
	{

	case CStage1Boss_ThrowGoldSingle::THROWREADY:
		m_fTick += fDetltaTime;
		if (m_fTick > m_fThrowSpeed)
		{
			m_fCurFrame = m_fMaxFrame;
			m_iVer = 2;

			m_eThrowstate = THROW;
			m_fTick = 0;

		}
		break;

	case CStage1Boss_ThrowGoldSingle::THROW:
			m_fTick += fDetltaTime;
			if (m_fTick > m_fThrowSpeed * 0.5)
			{
				LoadBullet();
				m_iVer = 1;
				m_fCurFrame = m_fMinFrame;

				m_eThrowstate = THROWREADY;
				m_fTick = 0;
				++m_iCountTick;

				if (m_iCountTick >= m_iThrowCount)
				{

					m_iCountTick = 0;
					return static_cast<CStage1Boss*>(m_pHost)->Get_State(1);
				}
			}
		break;
	}
	return nullptr;
}

void CStage1Boss_ThrowGoldSingle::LateUpdate(CMonster* _Monster)
{
}

void CStage1Boss_ThrowGoldSingle::Release(CMonster* _Monster)
{
}

void CStage1Boss_ThrowGoldSingle::Render(CMonster* _Monster)
{
}

void CStage1Boss_ThrowGoldSingle::Targeting()
{

	_vec3 vPlayerPos = Management()->Get_Player()->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vHostPos = m_pHost->Get_TransformCom()->m_vInfo[INFO_POS];

	m_vTargetDir = vPlayerPos - vHostPos;

	D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir);

}

void CStage1Boss_ThrowGoldSingle::LoadBullet()
{
	_float fAngleIncrement = D3DXToRadian(10.0f);
	_float fAngle = D3DXToRadian(-15.0f);

	for (int i = 0; i < m_iBulletCount; ++i)
	{
		_float	bulletAngle = fAngle + m_iCountTick * fAngleIncrement + i * fAngleIncrement; // 시작 각도 계산

		_vec3	vBulletDir;
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY, bulletAngle);

		D3DXVec3TransformCoord(&vBulletDir, &m_vTargetDir, &matRotY);

		CGameObject* pBullet = CBullet_GoldBar::Create(m_pHost->Get_GraphicDev(), vBulletDir, m_fBulletSpeed);
		Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::BOSSBULLET, pBullet);
	}

	m_iCountTick++;
}

