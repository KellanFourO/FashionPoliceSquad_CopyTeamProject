#include "Snow.h"
#include "OctreeNode.h"
CSnow::CSnow(BoundingBox* boundingBox, int numParticles)
{
// 	m_pBoungingBox = boundingBox;
// 	m_fSize = 0.8f;
// 	m_dVbSize = 2048;
// 	m_dVbOffset = 0; 
// 	m_dVbBatchSize = 512;
// 
// 	for (int i = 0; i < numParticles;i++)
// 		addParticle();
}

void CSnow::resetParticle(Attribute* attribute)
{
// 	attribute->isAlive = true;
// 
// 	//눈송이의 위치 지정을 위해 임의의 x,z좌표를 얻는다.
// 	GetRandomVector(&attribute->position, &m_pBoungingBox->GetMin(), &m_pBoungingBox->GetMax());
// 
// 	//높이 (Y좌표)는 항상 경계 상자의 최상단이 된다.
// 	attribute->position.y = m_pBoungingBox->GetMax().y;
// 
// 	//눈송이는 아래쪽으로 떨어지며 약간 왼쪽을 향한다.
// 	//attribute->color = 
}

void CSnow::Update_Particle(_float fTimeDelta)
{
}
