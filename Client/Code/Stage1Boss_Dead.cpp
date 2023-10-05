#include "stdafx.h"
#include "Stage1Boss_Dead.h"
#include "Texture.h"

CStage1Boss_Dead::CStage1Boss_Dead()
\
{

}

CStage1Boss_Dead::~CStage1Boss_Dead()
{
}

void CStage1Boss_Dead::Initialize(CMonster* _Monster)
{
	m_pHost = dynamic_cast<CStage1Boss*>(_Monster);

}

CMonsterState* CStage1Boss_Dead::Update(CMonster* Monster, const float& fDetltaTime)
{

	return nullptr;
}

void CStage1Boss_Dead::LateUpdate(CMonster* _Monster)
{
}

void CStage1Boss_Dead::Release(CMonster* _Monster)
{
}

void CStage1Boss_Dead::Render(CMonster* _Monster)
{
	m_pHost->Get_TextureCom()->Render_Textrue(1);
}
