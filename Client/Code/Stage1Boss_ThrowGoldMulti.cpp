#include "stdafx.h"
#include "Stage1Boss_ThrowGoldMulti.h"
#include "SYTex.h"
#include "Texture.h"
#include "Export_Utility.h"
#include "Stage1Boss_BrifBigShield.h"

CStage1Boss_ThrowGoldMulti::CStage1Boss_ThrowGoldMulti()
: m_fThrowSpeed(0.f),m_iBulletCount(0),m_iThrowCount(0),m_iCountTick(0),m_bThrow(false)
{

}

CStage1Boss_ThrowGoldMulti::~CStage1Boss_ThrowGoldMulti()
{
}

void CStage1Boss_ThrowGoldMulti::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);

	m_fMinFrame = 2.f;
	m_fMaxFrame = 3.f;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 2;

	//TODO 페이즈에 따라 투척 속도 또는 개수 증가 할 예정
	switch (m_ePhase)
	{
	case Engine::BOSSPHASE::PHASE_1:
		m_iThrowCount = 6;
		m_iBulletCount = 6;
		m_fBulletSpeed = 100.f;
		m_fThrowSpeed = 0.35f;
		break;
	case Engine::BOSSPHASE::PHASE_2:
		m_iThrowCount = 8;
		m_iBulletCount = 8;
		m_fBulletSpeed = 120.f;
		m_fThrowSpeed = 0.25f;
		break;
	case Engine::BOSSPHASE::PHASE_3:
		m_iThrowCount = 10;
		m_iBulletCount = 10;
		m_fBulletSpeed = 150.f;
		m_fThrowSpeed = 0.15f;
		break;
	}

}

CMonsterState* CStage1Boss_ThrowGoldMulti::Update(CMonster* Monster, const float& fDetltaTime)
{
	Targeting();
	switch (m_eThrowstate)
	{

	case CStage1Boss_ThrowGoldMulti::THROWREADY:
		m_fTick += fDetltaTime;
		if (m_fTick > m_fThrowSpeed)
		{
			m_fCurFrame = m_fMaxFrame;
			m_iVer = 2;

			m_eThrowstate = THROW;
			m_fTick = 0;
		}
		break;

	case CStage1Boss_ThrowGoldMulti::THROW:
		m_fTick += fDetltaTime;
		if (m_fTick > m_fThrowSpeed * 0.5f)
		{
			LoadBullet();
			m_fCurFrame = m_fMinFrame;

			m_eThrowstate = THROWREADY;
			m_fTick = 0;
			++m_iCountTick;
			if (m_ePhase == BOSSPHASE::PHASE_2 && !CStage1Boss_BrifBigShield::m_bAllDead)
			{
				return dynamic_cast<CStage1Boss*>(m_pHost)->Get_State(5);
			}
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

void CStage1Boss_ThrowGoldMulti::LateUpdate(CMonster* _Monster)
{
}

void CStage1Boss_ThrowGoldMulti::Release(CMonster* _Monster)
{

}

void CStage1Boss_ThrowGoldMulti::Render(CMonster* _Monster)
{
}

void CStage1Boss_ThrowGoldMulti::Targeting()
{

	_vec3 vPlayerPos = Management()->Get_Player()->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vHostPos = m_pHost->Get_TransformCom()->m_vInfo[INFO_POS];

	m_vTargetDir = vPlayerPos - vHostPos;

	D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir);

}

void CStage1Boss_ThrowGoldMulti::LoadBullet()
{

	_float fAngleIncrement = D3DXToRadian(10.0f);
	_float fAngle = -D3DXToRadian(30.f);


	for (int i = 0; i < m_iBulletCount; ++i)
	{
		_vec3 vBulletDir;
		_matrix matRotY;
		D3DXMatrixIdentity(&matRotY);

		D3DXMatrixRotationY(&matRotY, fAngle + i * fAngleIncrement);

		D3DXVec3TransformCoord(&vBulletDir, &m_vTargetDir, &matRotY);

		CGameObject* pBullet = CBullet_GoldBar::Create(m_pHost->Get_GraphicDev(),vBulletDir, m_fBulletSpeed);
		Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::BOSSBULLET, pBullet);
		SoundMgr()->PlaySoundW(L"BossBullet2.wav",SOUND_BOSS2,0.5f);
	}
}
