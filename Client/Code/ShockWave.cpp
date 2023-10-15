#include "stdafx.h"
#include "ShockWave.h"
#include <random>


CShockWave::CShockWave(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
}

CShockWave::~CShockWave()
{
}

HRESULT CShockWave::Ready_GameObject(_vec3 vOriginPos, _int numParticles)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Set_Scale({ 1.f,1.f,1.f });
	//m_pTransformCom->Translate(_vec3(0.f, 10.f, 0.f));


	srand(_ulong(time(NULL)));
	m_pBoungingBox.m_vMin = _vec3(-5.0f,-5.0f,-5.0f);
	m_pBoungingBox.m_vMax = _vec3(5.0f, 5.0f, 5.0f);

	m_pTransformCom->Set_Pos(vOriginPos);
	//m_vOrigin = vOriginPos;			// 시스템 내에서 파티클이 시작되는 곳.
	m_fSize = 4.f;					// 시스템 내 모든 파티클의 크기
	m_dVbSize = 4096;					// 버텍스 버퍼가 보관할 수 있는 파티클의 수- 실제 파티클 시스템 내의 파티클 수와는 독립적.
	m_dVbOffset = 0;					// 버텍스 버퍼에서 복사를 시작할 파티클 내 다음 단계로의 오프셋(바이트가 아닌 파티클 단위)
	m_dVbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		AddParticle();

	_tchar* pPath = L"../Bin/Resource/Texture/Particles/atlas_big_popcon1.png";

	CPSystem::Ready_GameObject(pPath);

	return S_OK;
}

_int CShockWave::Update_GameObject(const _float& fTimeDelta)
{
	m_fTime += 1.f * fTimeDelta;
	srand(static_cast<unsigned>(time(0)));
	//CPSystem::Update_GameObject(fTimeDelta);
	for (auto& iter : m_ParticlesList)
	{
		if (iter.isAlive)
		{
			iter.position += iter.velocity * fTimeDelta;

			iter.age += fTimeDelta;

			if (iter.age > iter.lifeTime)
				iter.isAlive = false;

			// 경계 범위를 벗어났는지 여부 확인
			if (m_pBoungingBox.m_vMax.x   < iter.position.x || m_pBoungingBox.m_vMin.x > iter.position.x ||
				m_pBoungingBox.m_vMax.y   < iter.position.y || m_pBoungingBox.m_vMin.y > iter.position.y ||
				m_pBoungingBox.m_vMax.z   < iter.position.z || m_pBoungingBox.m_vMin.z > iter.position.z)
			{
				// 재활용

				//ResetParticle(&iter);
			}

		}
	}

	//billboard();
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CShockWave::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CShockWave::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CPSystem::Render_GameObject();
}

CShockWave* CShockWave::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles)
{
	CShockWave* pInstance = new CShockWave(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vOriginPos, numParticles)))
	{
		Safe_Release(pInstance);

		MSG_BOX("DustGrey Create Failed");
		return nullptr;
	}

	return pInstance;
}



void CShockWave::ResetParticle(Attribute* attribute)
{


	attribute->position.x =m_pTransformCom->m_vInfo[INFO_POS].x +m_pBoungingBox.m_vMin.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_pBoungingBox.m_vMax.x - m_pBoungingBox.m_vMin.x)));
	attribute->position.y =m_pTransformCom->m_vInfo[INFO_POS].y - 2.f +m_pBoungingBox.m_vMin.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_pBoungingBox.m_vMax.y - m_pBoungingBox.m_vMin.y)));
	attribute->position.z =m_pTransformCom->m_vInfo[INFO_POS].z +m_pBoungingBox.m_vMin.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_pBoungingBox.m_vMax.z - m_pBoungingBox.m_vMin.z)));

	//D3DXVECTOR3 min = _vec3(0.00001f, 0.00001f, 0.00001f);
	//D3DXVECTOR3 max = _vec3(1.f, 1.0f, 1.f);

	//GetRandomVector(&attribute->velocity, &min, &max);

	//구체를 만들기 위한 정규화
	//D3DXVec3Normalize(&attribute->velocity, &attribute->velocity);

	attribute->velocity = {0.f,3.f,0.f};

	attribute->color = D3DXCOLOR(158.f, 158.f, 158.f, 1.f);
	//attribute->color = D3DXCOLOR(GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f), 1.f);
	attribute->age = 0.f;
	attribute->lifeTime = 1.f;//1초뒤 파티클 사망
	//attribute->lifeTime = 1000.f;//1초뒤 파티클 사망

}


HRESULT CShockWave::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);
    return S_OK;
}

void CShockWave::billboard()
{

}

void CShockWave::Free()
{

}
