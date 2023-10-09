#include "stdafx.h"
#include "DustGrey.h"


CDustGrey::CDustGrey(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
}

CDustGrey::~CDustGrey()
{
}

HRESULT CDustGrey::Ready_GameObject(_vec3 vOriginPos, _int numParticles)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Set_Scale({ 1.f,1.f,1.f });
	//m_pTransformCom->Translate(_vec3(0.f, 10.f, 0.f));

	srand(_ulong(time(NULL)));
	m_pBoungingBox.m_vMin = _vec3(-5.0f,-5.0f,-5.0f);
	m_pBoungingBox.m_vMax = _vec3(5.0f, 5.0f, 5.0f);

	m_vOrigin = vOriginPos;			// �ý��� ������ ��ƼŬ�� ���۵Ǵ� ��.
	m_fSize = 1.f;					// �ý��� �� ��� ��ƼŬ�� ũ��
	m_dVbSize = 4096;					// ���ؽ� ���۰� ������ �� �ִ� ��ƼŬ�� ��- ���� ��ƼŬ �ý��� ���� ��ƼŬ ���ʹ� ������.
	m_dVbOffset = 0;					// ���ؽ� ���ۿ��� ���縦 ������ ��ƼŬ �� ���� �ܰ���� ������(����Ʈ�� �ƴ� ��ƼŬ ����)
	m_dVbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		AddParticle();

	_tchar* pPath = L"../Bin/Resource/Texture/Particles/atlas_big_popcon1.png";

	CPSystem::Ready_GameObject(pPath);

	return S_OK;
}

_int CDustGrey::Update_GameObject(const _float& fTimeDelta)
{
	m_fTime += 1.f * fTimeDelta;
	CPSystem::Update_GameObject(fTimeDelta);
// 	for (auto& iter : m_ParticlesList)
// 	{
// 		if (iter.isAlive)
// 		{
// 			iter.position += iter.velocity * fTimeDelta;
// 
// 			iter.age += fTimeDelta;
// 
// 			if (iter.age > iter.lifeTime)
// 				iter.isAlive = false;
// 
// 			if (m_pBoungingBox.m_vMax.x+ m_vOrigin.x < iter.position.x || m_pBoungingBox.m_vMin.x + m_vOrigin.x> iter.position.x ||
// 				m_pBoungingBox.m_vMax.y+ m_vOrigin.y < iter.position.y || m_pBoungingBox.m_vMin.y + m_vOrigin.y> iter.position.y ||
// 				m_pBoungingBox.m_vMax.z+ m_vOrigin.z < iter.position.z || m_pBoungingBox.m_vMin.z + m_vOrigin.z> iter.position.z)
// 			{
// 				// ��Ȱ��
// 				ResetParticle(&iter);
// 			}
// 			
// 		}
// 	}

	billboard();
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CDustGrey::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CDustGrey::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CPSystem::Render_GameObject();
}

CDustGrey* CDustGrey::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles)
{
	CDustGrey* pInstance = new CDustGrey(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vOriginPos, numParticles)))
	{
		Safe_Release(pInstance);

		MSG_BOX("DustGrey Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDustGrey::ResetParticle(Attribute* attribute)
{
	attribute->isAlive = true;
	attribute->position = m_vOrigin;

	D3DXVECTOR3 min = _vec3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = _vec3(1.0f, 1.0f, 1.0f);

	GetRandomVector(&attribute->velocity, &min, &max);

	//��ü�� ����� ���� ����ȭ
	D3DXVec3Normalize(&attribute->velocity, &attribute->velocity);

	attribute->velocity *= 2.f;

	attribute->color = D3DXCOLOR(158.f, 158.f, 158.f, 1.f);
	//attribute->color = D3DXCOLOR(GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f), 1.f);
	attribute->age = 0.f;
	//attribute->lifeTime = 0.8f;//1�ʵ� ��ƼŬ ���
	attribute->lifeTime = 1000.f;//1�ʵ� ��ƼŬ ���

}


HRESULT CDustGrey::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);
    return S_OK;
}

void CDustGrey::billboard()
{

}

void CDustGrey::Free()
{

}
