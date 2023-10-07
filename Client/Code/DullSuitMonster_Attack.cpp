#include "stdafx.h"
#include "DullSuitMonster_Attack.h"
#include "SYTex.h"
#include "Texture.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "BrifCase_2.h"


CDullSuitMonster_Attack::CDullSuitMonster_Attack()
{


}

CDullSuitMonster_Attack::~CDullSuitMonster_Attack()
{
}

void CDullSuitMonster_Attack::Initialize(CMonster* _Monster)
{
	m_pHost = _Monster;
	m_fMinFrame = 1.0f;
	m_fMaxFrame = 4.0f;
	m_fCurFrame = m_fMinFrame;

}

CMonsterState* CDullSuitMonster_Attack::Update(CMonster* Monster, const float& fDetltaTime)
{

	switch (m_eAttack)
	{
	case CDullSuitMonster_Attack::READY1:
	{
		m_fTick += fDetltaTime;

		if (m_fTick > 1.f)
		{
			++m_fCurFrame;
			m_eAttack = READY2;
			m_fTick = 0.f;
		}
		break;
	}

	case CDullSuitMonster_Attack::READY2:
	{
		m_fTick += fDetltaTime;

		if (m_fTick >= 1.f)
		{
			if (m_pHost->Get_MonsterBullet() == nullptr) {
				m_pHost->Set_Bullet(CBrifCase_2::Create(m_pHost->Get_GraphicDev(), m_pHost->m_pTransformCom, m_pHost->Get_PlayerTransform()));
				CManagement* pManagement;
				CLayer* pStageLayer = pManagement->GetInstance()->Get_Layer(LAYERTAG::GAMELOGIC);
				pStageLayer->Add_GameObject(OBJECTTAG::MONSTERBULLET, m_pHost->Get_MonsterBullet());
				dynamic_cast<CBrifCase_2*>(m_pHost->Get_MonsterBullet())->Shot(m_pHost->Get_Info().vPos);
			}
			else {
				dynamic_cast<CBrifCase_2*>(m_pHost->Get_MonsterBullet())->Shot(m_pHost->Get_Info().vPos);
			}

			++m_fCurFrame;
			m_eAttack = THROW;
			m_fTick = 0.f;
		}
		break;
	}

	case CDullSuitMonster_Attack::THROW:
	{
		m_fTick += fDetltaTime;

		if (m_fTick >= 1.f)
		{
			++m_fCurFrame;
			m_eAttack = THROWEND;
			m_fTick = 0.f;
		}
		break;
	}

	case CDullSuitMonster_Attack::THROWEND:
	{
		m_fTick += fDetltaTime;

		if (m_fTick >= 1.f)
		{
			//TODO 만약 던진 후 플레이어가 공격범위에 없을시 추격 상태로 변경
			if (!m_pHost->ChaseCatch())
				return dynamic_cast<CDullSuitMonster*>(m_pHost)->Get_State(1);
			else
			{
				m_eAttack = READY1;

			}
			++m_fCurFrame;
			m_fTick = 0.f;
		}
		break;
	}
	}

	if (m_fCurFrame > m_fMaxFrame)
	{
		m_fCurFrame = m_fMinFrame;
	}

	return nullptr;
}

void CDullSuitMonster_Attack::Release(CMonster* _Monster)
{

}

void CDullSuitMonster_Attack::LateUpdate(CMonster* _Monster)
{
}

void CDullSuitMonster_Attack::Render(CMonster* _Monster)
{
	if (m_pHost->Get_Info().bHit)
	{
		m_pHost->Get_TextureCom()->Render_Textrue(1);
	}
	else
	{
		m_pHost->Get_TextureCom()->Render_Textrue(0);
	}

	m_pHost->Get_BufferCom()->Render_Buffer(m_fCurFrame, 1);
}
