#include "stdafx.h"
#include "KickBoardMonster_Chase.h"
#include "SYTex.h"
#include "Texture.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "BrifCase_2.h"


CKickBoardMonster_Chase::CKickBoardMonster_Chase()
{


}

CKickBoardMonster_Chase::~CKickBoardMonster_Chase()
{
}

void CKickBoardMonster_Chase::Initialize(CMonster* _Monster)
{
	m_pHost = _Monster;

	m_fMinFrame = 1.0f;
	m_fMaxFrame = 2.0f;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 2;
}

CMonsterState* CKickBoardMonster_Chase::Update(CMonster* Monster, const float& fDetltaTime)
{

	m_fTick += fDetltaTime;

	if (m_fTick >= 0.3f)
	{
		++m_fCurFrame;
		m_fTick = 0.f;
	}

	if (m_fCurFrame > m_fMaxFrame)
	{
		m_fCurFrame = m_fMinFrame;
	}

	//TODO 추격중 플레이어가 공격범위 안에 들어오면
	if (m_pHost->ChaseCatch())
	{
		return dynamic_cast<CKickBoardMonster*>(m_pHost)->Get_State(2);
	}
	//TODO 공격범위 안에 없으면 추격
	else
	{
		m_pHost->Chase_Target(fDetltaTime);
	}

	return nullptr;
}

void CKickBoardMonster_Chase::Release(CMonster* _Monster)
{

}

void CKickBoardMonster_Chase::LateUpdate(CMonster* _Monster)
{
}

void CKickBoardMonster_Chase::Render(CMonster* _Monster)
{

}
