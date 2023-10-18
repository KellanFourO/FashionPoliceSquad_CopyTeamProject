#include "stdafx.h"
#include "BossBigDaddy_Attack.h"
#include "SYTex.h"
#include "Texture.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "BrifCase.h"


CBossBigDaddy_Attack::CBossBigDaddy_Attack()
{


}

CBossBigDaddy_Attack::~CBossBigDaddy_Attack()
{
}

void CBossBigDaddy_Attack::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);
	m_eAttack = READY1;
	m_fMinFrame = 1.0f;
	m_fMaxFrame = 4.0f;
	m_fCurFrame = m_fMinFrame;
	m_iVer =1;
}

CMonsterState* CBossBigDaddy_Attack::Update(CMonster* Monster, const float& fDetltaTime)
{
	_vec3 Temp = { 0.f, 5.f,0.f };
	switch (m_eAttack)
	{
	case CBossBigDaddy_Attack::READY1:
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

	case CBossBigDaddy_Attack::READY2:
		{
		m_fTick += fDetltaTime;

		if (m_fTick >= 1.f)
		{
			
			++m_fCurFrame;
			m_eAttack = THROW;
			m_fTick = 0.f;
			m_pHost->Set_Bullet(LoadBullet());
			static_cast<CBrifCase*>(m_pHost->Get_MonsterBullet())->Shot(m_pHost->Get_Transform()->m_vInfo[INFO_POS]+Temp);

		}
		break;
		}

	case CBossBigDaddy_Attack::THROW:
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

	case CBossBigDaddy_Attack::THROWEND:
		{
			m_fTick += fDetltaTime;

			if (m_fTick >= 1.f)
			{
				//TODO 만약 던진 후 플레이어가 공격범위에 없을시 추격 상태로 변경
				m_eAttack = READY1;
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

void CBossBigDaddy_Attack::Release(CMonster* _Monster)
{

}

void CBossBigDaddy_Attack::LateUpdate(CMonster* _Monster)
{
}

void CBossBigDaddy_Attack::Render(CMonster* _Monster)
{

}

CBullet* CBossBigDaddy_Attack::LoadBullet()
{
		CBullet* pBullet = CBrifCase::Create(m_pHost->Get_GraphicDev(), m_pHost->Get_Transform(), m_pHost->Get_PlayerTransform());
		Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::MONSTERBULLET, pBullet);

		return pBullet;


}

