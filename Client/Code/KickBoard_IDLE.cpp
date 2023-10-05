#include "stdafx.h"
#include "..\Header\KickBoard_IDLE.h"


#include "Export_Utility.h"

//#include "..\Header\MonsterState_IDLE_Walk.h"
//#include "..\Header\MonsterState_IDLE_Attack.h"

CKickBoard_IDLE::CKickBoard_IDLE()
{

}

CKickBoard_IDLE::~CKickBoard_IDLE()
{
}

void CKickBoard_IDLE::Initialize(CMonster* _Monster)
{
	m_fMinFrame = 4.0f;
	m_fMaxFrame = 6.0f;
	m_fCurFrame = m_fMinFrame;
	m_fSpeed = 25.5f;
}

CMonsterState* CKickBoard_IDLE::Update(CMonster* Monster, const float& fDetltaTime)
{

	m_fBehaviorTime += fDetltaTime;
	m_fAnimateTime += fDetltaTime;



	//m_fSpeed += 0.1f;

	if (m_fBehaviorTime <= 2.0f) {
		dynamic_cast<CKickBoard*>(Monster)->Ride_On(m_vDir, m_fSpeed, fDetltaTime);
	}
	else if (m_fBehaviorTime <= 4.0f) {
		dynamic_cast<CKickBoard*>(Monster)->Ride_On(_vec3(1.f, 0.f, 0.f), m_fSpeed, fDetltaTime);
	}
	else if (m_fBehaviorTime <= 6.0f) {
		dynamic_cast<CKickBoard*>(Monster)->Ride_On(-m_vDir, m_fSpeed, fDetltaTime);
	}
	else if (m_fBehaviorTime <= 8.0f) {
		dynamic_cast<CKickBoard*>(Monster)->Ride_On(_vec3(-1.f, 0.f, 0.f), m_fSpeed, fDetltaTime);
	}
	else if (m_fBehaviorTime <= 10.0f) {
		m_fBehaviorTime = 0.f;
	}



	//if (m_fBehaviorTime >= 5.f) {
	//	return new CBrifMonsterBigState_Aggro;
	//}









	//Monster->m_pTransformCom->Move_Pos(&vDir, fDetltaTime, 1.5f);
	//Monster->Chase_Target(fDetltaTime, 1.5f); // 이동 코드

	return nullptr;
}

void CKickBoard_IDLE::LateUpdate(CMonster* _Monster)
{
}

void CKickBoard_IDLE::Release(CMonster* _Monster)
{
}

void CKickBoard_IDLE::Render(CMonster* _Monster)
{
}
