#include "stdafx.h"
#include "Stage1Boss_ThrowGoldMulti.h"
#include "SYTex.h"
#include "Texture.h"
#include "Export_Utility.h"

CStage1Boss_ThrowGoldMulti::CStage1Boss_ThrowGoldMulti()
{

}

CStage1Boss_ThrowGoldMulti::~CStage1Boss_ThrowGoldMulti()
{
}

void CStage1Boss_ThrowGoldMulti::Initialize(CMonster* _Monster)
{
	m_pHost = dynamic_cast<CStage1Boss*>(_Monster);

	//TODO 페이즈에 따라 투척 속도 또는 개수 증가 할 예정
	switch (m_ePhase)
	{
	case Engine::BOSSPHASE::PHASE_1:
		m_iThrowCount = 6;
		m_iBulletCount = 6;
		m_fThrowSpeed = 8.f;
		break;
	case Engine::BOSSPHASE::PHASE_2:
		break;
	case Engine::BOSSPHASE::PHASE_3:
		break;
	}
}

CMonsterState* CStage1Boss_ThrowGoldMulti::Update(CMonster* Monster, const float& fDetltaTime)
{

	switch (m_eThrowstate)
	{

	case CStage1Boss_ThrowGoldMulti::THROW:
		m_fTick += fDetltaTime;

		if (m_fTick > 0.35f)
		{
			m_bThrow = true;
			LoadBullet();
			m_eThrowstate = THROWREADY;
			m_fTick = 0;
			++m_iCountTick;

			if (m_iCountTick >= m_iThrowCount)
				return m_pHost->Get_State(1);
		}
		break;

	case CStage1Boss_ThrowGoldMulti::THROWREADY:
		m_fTick += fDetltaTime;
		if (m_fTick > 0.35f)
		{
			m_bThrow = false;
			Targeting();
			m_eThrowstate = THROW;
			m_fTick = 0;
		}

		break;
	}


}

void CStage1Boss_ThrowGoldMulti::LateUpdate(CMonster* _Monster)
{
}

void CStage1Boss_ThrowGoldMulti::Release(CMonster* _Monster)
{
}

void CStage1Boss_ThrowGoldMulti::Render(CMonster* _Monster)
{
		m_pHost->Get_TextureCom()->Render_Textrue(0);
		m_pHost->Get_BufferCom()->Render_Buffer(m_iMultiThrowStart, 2);
}

void CStage1Boss_ThrowGoldMulti::Targeting()
{
	CTransform* pHostTransform = m_pHost->Get_TransformCom();

	_vec3 vPlayerPos = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).back()->m_pTransformCom->m_vInfo[INFO_POS];
	_vec3 vHostPos = pHostTransform->m_vInfo[INFO_POS];

	m_vTargetDir = vPlayerPos - vHostPos;

	D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir);

}

void CStage1Boss_ThrowGoldMulti::LoadBullet()
{
	for (int i = 0; i < m_iBulletCount; ++i)
	{
		m_vTargetDir.x = m_vTargetDir.x + 0.5 * i;

		CBullet_GoldBar* pBullet = CBullet_GoldBar::Create(m_pHost->Get_GraphicDev(), m_vTargetDir);
		Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::BOSSBULLET, pBullet);
	}
}
