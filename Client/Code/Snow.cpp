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
// 	//�������� ��ġ ������ ���� ������ x,z��ǥ�� ��´�.
// 	GetRandomVector(&attribute->position, &m_pBoungingBox->GetMin(), &m_pBoungingBox->GetMax());
// 
// 	//���� (Y��ǥ)�� �׻� ��� ������ �ֻ���� �ȴ�.
// 	attribute->position.y = m_pBoungingBox->GetMax().y;
// 
// 	//�����̴� �Ʒ������� �������� �ణ ������ ���Ѵ�.
// 	//attribute->color = 
}

void CSnow::Update_Particle(_float fTimeDelta)
{
}
