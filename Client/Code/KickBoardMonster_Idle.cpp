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

	m_fSpeed = 25.5f;
}

CMonsterState* CKickBoardMonster_Idle::Update(CMonster* Monster, const float& fDetltaTime)
{
	if (m_pHost->Detect() || m_pHost->Get_Info().bHit)
	{
		//TODO 플레이어가 일정범위 안에 들어오거나 피격시 추격. Chase 상태로 변경
		return dynamic_cast<CKickBoardMonster*>(m_pHost)->Get_State(1);
	}

	return nullptr;


// 	m_fBehaviorTime += fDetltaTime;
// 	m_fAnimateTime += fDetltaTime;
//
//
//
// 	//m_fSpeed += 0.1f;
//
// 	if (m_fBehaviorTime <= 2.0f) {
// 		dynamic_cast<CKickBoardMonster*>(Monster)->Ride_On(m_vDir, m_fSpeed, fDetltaTime);
// 	}
// 	else if (m_fBehaviorTime <= 4.0f) {
// 		dynamic_cast<CKickBoardMonster*>(Monster)->Ride_On(_vec3(1.f, 0.f, 0.f), m_fSpeed, fDetltaTime);
// 	}
// 	else if (m_fBehaviorTime <= 6.0f) {
// 		dynamic_cast<CKickBoardMonster*>(Monster)->Ride_On(-m_vDir, m_fSpeed, fDetltaTime);
// 	}
// 	else if (m_fBehaviorTime <= 8.0f) {
// 		dynamic_cast<CKickBoardMonster*>(Monster)->Ride_On(_vec3(-1.f, 0.f, 0.f), m_fSpeed, fDetltaTime);
// 	}
// 	else if (m_fBehaviorTime <= 10.0f) {
// 		m_fBehaviorTime = 0.f;
// 	}

}

void CKickBoardMonster_Idle::LateUpdate(CMonster* _Monster)
{
}

void CKickBoardMonster_Idle::Release(CMonster* _Monster)
{
}

void CKickBoardMonster_Idle::Render(CMonster* _Monster)
{
	if (m_pHost->Get_Info().bHit)
	{
		m_pHost->Get_TextureCom()->Render_Textrue(1);
	}
	else
	{
		m_pHost->Get_TextureCom()->Render_Textrue(0);
	}

	m_pHost->Get_BufferCom()->Render_Buffer(m_fCurFrame, 4);
}
