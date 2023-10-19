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

	m_iVer = 1;

}

CMonsterState* CDullSuitMonster_Attack::Update(CMonster* Monster, const float& fDetltaTime)
{

	switch (m_eAttack)
	{
	case CDullSuitMonster_Attack::READY1:
	{
		m_fTick += fDetltaTime;

		if (m_fTick > 0.5f)
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

		if (m_fTick >= 0.5f)
		{
			
			++m_fCurFrame;
			m_eAttack = THROW;
			m_fTick = 0.f;
		}
		break;
	}

	case CDullSuitMonster_Attack::THROW:
	{
		m_fTick += fDetltaTime;

		if (m_fTick >= 0.5f)
		{
			m_pHost->Set_Bullet(LoadBullet());
			static_cast<CBrifCase_2*>(m_pHost->Get_MonsterBullet())->Shot(m_pHost->Get_Info().vPos);

			++m_fCurFrame;
			m_eAttack = THROWEND;
			m_fTick = 0.f;
		}
		break;
	}

	case CDullSuitMonster_Attack::THROWEND:
	{
		m_fTick += fDetltaTime;

		if (m_fTick >= 0.5f)
		{
			//TODO ���� ���� �� �÷��̾ ���ݹ����� ������ �߰� ���·� ����
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
}

CBullet* CDullSuitMonster_Attack::LoadBullet()
{
	CBullet* pBullet = CBrifCase_2::Create(m_pHost->Get_GraphicDev(),m_pHost->Get_Transform(),m_pHost->Get_PlayerTransform());
	Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::MONSTERBULLET, pBullet);

	return pBullet;
}
