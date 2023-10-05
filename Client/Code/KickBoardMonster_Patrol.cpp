#include "stdafx.h"
#include "KickBoardMonster_Patrol.h"
#include "SYTex.h"
#include "Texture.h"
#include "Export_System.h"
#include "Export_Utility.h"



CKickBoardMonster_Patrol::CKickBoardMonster_Patrol()
{


}

CKickBoardMonster_Patrol::~CKickBoardMonster_Patrol()
{
}

void CKickBoardMonster_Patrol::Initialize(CMonster* _Monster)
{
	m_pHost = _Monster;
	m_pHost->Set_BillBoard(FALSE);
	//TODO 패트롤상태일때는 빌보드가 안되게끔 해야한다.
	m_fCurFrame = 2;

}

CMonsterState* CKickBoardMonster_Patrol::Update(CMonster* Monster, const float& fDetltaTime)
{

	ChangeDirection(fDetltaTime);

	m_fChangeTick += fDetltaTime;

	if (m_fChangeTick >= 5.f)
	{
		if (m_pHost->ChaseCatch())
		{
			m_pHost->Set_BillBoard(true);
			return dynamic_cast<CKickBoardMonster*>(m_pHost)->Get_State(3);
		}
		else
		{
			m_pHost->Set_BillBoard(true);
			return dynamic_cast<CKickBoardMonster*>(m_pHost)->Get_State(2);
		}
	}

	return nullptr;
}

void CKickBoardMonster_Patrol::Release(CMonster* _Monster)
{

}

void CKickBoardMonster_Patrol::LateUpdate(CMonster* _Monster)
{
}

void CKickBoardMonster_Patrol::Render(CMonster* _Monster)
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

void CKickBoardMonster_Patrol::ChangeDirection(const float& fDetltaTime)
{
	_vec3 vPos, vDir;

	m_pHost->m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pHost->m_pTransformCom->Get_Info(INFO_LOOK, &vDir);



	m_fTick += fDetltaTime;
	m_pHost->m_pTransformCom->Move_Pos(&vDir,fDetltaTime,m_pHost->Get_Speed());

	if (m_fTick >= 1.f)
	{
		m_pHost->m_pTransformCom->Set_Rotate(ROT_Y, m_fRotateAngle);
		m_fTick = 0.f;
	}


}
