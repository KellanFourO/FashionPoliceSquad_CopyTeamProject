#include "stdafx.h"
#include "KickBoardMonster_Idle.h"
#include "SYTex.h"
#include "Texture.h"
#include "Export_Utility.h"

CKickBoardMonster_Idle::CKickBoardMonster_Idle()
{

}

CKickBoardMonster_Idle::~CKickBoardMonster_Idle()
{
}

void CKickBoardMonster_Idle::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);

	m_fMinFrame = 3.f;
	m_fMaxFrame = 3.f;
	m_fCurFrame = m_fMinFrame;

	m_iVer = 4;
	m_fSpeed = 25.5f;
}

CMonsterState* CKickBoardMonster_Idle::Update(CMonster* Monster, const float& fDetltaTime)
{

	if (m_pHost->Detect() || m_pHost->Get_Info().bHit)
	{
		m_iVer = 2;
		m_fCurFrame = 5;

		m_fTick += fDetltaTime;

		if (m_fTick > 1.25f)
		{
			return dynamic_cast<CKickBoardMonster*>(m_pHost)->Get_State(1);
		}

		//TODO 플레이어가 일정범위 안에 들어오거나 피격시 추격. Chase 상태로 변경
	}

	return nullptr;


}

void CKickBoardMonster_Idle::LateUpdate(CMonster* _Monster)
{
}

void CKickBoardMonster_Idle::Release(CMonster* _Monster)
{
}

void CKickBoardMonster_Idle::Render(CMonster* _Monster)
{

}
