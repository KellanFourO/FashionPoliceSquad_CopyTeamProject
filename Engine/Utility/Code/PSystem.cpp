#include "../Header/PSystem.h"


CPSystem::CPSystem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPSystem::CPSystem(const CPSystem& rhs)
	:CGameObject(rhs)
{
}

CPSystem::~CPSystem()
{
}

HRESULT CPSystem::Ready_GameObject(const _tchar* texFileName)
{
	TCHAR	szFileName[128] = L"";
	wsprintf(szFileName, texFileName);

	FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev,
		szFileName,
		(LPDIRECT3DTEXTURE9*)&m_tex),
		E_FAIL);

	//CVIBufferComp ���� �Ѱſ� ���� ���� ����� 
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(
		m_dVbSize * sizeof(Particle), // ������ ������ ũ��
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, // �����ϰ��� �ϴ� ���ؽ� ������ ����(0�� ��� ���� ����, D3DUSAGE_DYNAMIC)
		FVF,// ���ؽ� �Ӽ� �ɼ�
		D3DPOOL_DEFAULT,// �޸� Ǯ ��� - ���� ���� (���� ������ ��� MANAGED)
		&m_pVB,// ��� �� ��ü
		0),
		E_FAIL);

	// �� ������ ���� ��ƼŬ�� �����ؾ� �ϸ� �̴� �� ���ؽ� ������ �޸𸮿� �����ؾ� ���� �ǹ�.
	// ���� ���ؽ� ���۷��� ���� �ӵ��� ����� �����Ƿ� ���� ���ؽ� ���۸� �̿�

	return S_OK;
}

_int CPSystem::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_ParticlesList)
	{
		if (iter.isAlive)
		{
			iter.position += iter.velocity * fTimeDelta;

			iter.age += fTimeDelta;

			if (iter.age > iter.lifeTime)
				iter.isAlive = false;

			// ��� ������ ������� ���� Ȯ��
			if (m_pBoungingBox.m_vMax.x < iter.position.x || m_pBoungingBox.m_vMin.x > iter.position.x ||
				m_pBoungingBox.m_vMax.y < iter.position.y || m_pBoungingBox.m_vMin.y > iter.position.y ||
				m_pBoungingBox.m_vMax.z < iter.position.z || m_pBoungingBox.m_vMin.z > iter.position.z)
			{
				// ��Ȱ��
				ResetParticle(&iter);
			}
		}
	}

	return S_OK;
}

void CPSystem::LateUpdate_GameObject()
{
}

void CPSystem::Render_GameObject()
{
	if (!m_ParticlesList.empty())
	{
		preRender_Particle();

		m_pGraphicDev->SetTexture(0, m_tex);
		m_pGraphicDev->SetFVF(FVF);
		m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(Particle));

		if (m_dVbOffset >= m_dVbSize)
			m_dVbOffset = 0;

		Particle* v = 0;

		m_pVB->Lock(m_dVbOffset * sizeof(Particle), m_dVbBatchSize * sizeof(Particle), (void**)&v,
			m_dVbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		//��� ��ƼŬ�� ������ �� ������ 

// 		list<Attribute>::iterator iter;
// 		for (iter = m_ParticlesList.begin(); iter != m_ParticlesList.end(); iter++)
		for(auto& iter : m_ParticlesList)
		{
			if (iter.isAlive)
			{
				//�Ѵܰ��� ������ ��ƼŬ�� ���� ���ؽ� ���׸�Ʈ�� �����Ѵ�.
				v->_Position = iter.position;
				v->_Color = (D3DCOLOR)iter.color;

				v++;
				numParticlesInBatch++; //�ܰ� ī��Ʈ ����

				//���� �ܰ谡 ��� ä���� �ִ���?
				if (numParticlesInBatch == m_dVbBatchSize)
				{
					//���ؽ� ���۷� ����� ������ �ܰ��� ��ƼŬ�� �׸� 

					m_pVB->Unlock();

					m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_dVbOffset, m_dVbBatchSize);

					//�ܰ谡 �׷����� ���� ���� �ܰ踦 ��ƼŬ�� ä��.

					//���� �ܰ��� ó�� ���������� �̵�
					m_dVbOffset += m_dVbBatchSize;

					//���ؽ� ������ ��踦 �Ѵ¸޸𸮷� �������� �������� �ʴ´�.
					//��踦 ���� ��� ó������ ����! 
					if (m_dVbOffset >= m_dVbSize)
						m_dVbOffset = 0;

					m_pVB->Lock(m_dVbOffset * sizeof(Particle), m_dVbBatchSize * sizeof(Particle), (void**)&v,
						m_dVbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; //���� �ܰ踦 ���� ����

				}

			}
		}
		m_pVB->Unlock();
		//(numParticlesInBatch == m_dvbBatchSize) ������ �������� ���Ͽ� ������ �ܰ谡 ���������� �ʴ� ��찡 �߻��Ҽ� ����
		//�Ϻθ� ä���� �ܰ�� �ٷ� �̰����� ��������.
		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_dVbOffset, numParticlesInBatch);

		}
		//���� ���
		m_dVbOffset += m_dVbBatchSize;

		PostRender_Particle();
	}
}


void CPSystem::preRender_Particle()
{
	_matrix WorldMatrix;
	D3DXMatrixIdentity(&WorldMatrix);
	WorldMatrix._41 = m_vOrigin.x;
	WorldMatrix._42 = m_vOrigin.y;
	WorldMatrix._43 = m_vOrigin.z;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &WorldMatrix);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // �޸����� 
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ����Ʈ ��������Ʈ ���� ���� = �� �ϳ��� ǥ���Ǵ� ��������Ʈ, �Ϲ������� ���ڽý��ۿ� �̿� 
	// ���� ������ ��ü �ؽ�ó�� ����Ʈ ��������Ʈ�� �ؽ�ó ���ο� �̿��Ұ����� �ǹ�
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);

	// ����Ʈ ũ�⸦ �� �����̽� ������ �ؼ��ϵ��� ����.
	// ����Ʈ ��������Ʈ�� ũ��� ī�޶���� �Ÿ��� ���� �����ϰ� ������.
	// ��, ī�޶�� �ָ� ������ ��ƼŬ�� ����� ��ƼŬ�� �����۰� ��Ÿ��.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);

	// ����Ʈ ��������Ʈ�� ũ�⸦ ����.
	// �� ���� D3DRS_POINTSCALEENABLE ���� ���� ���� �� �����̽� ���� ũ�⳪
	// ��ũ�� �����̽� ���� ũ��� �ؼ�.
	// SetRenderState ȣ���� float�� �ƴ� DWORD�� �ʿ�� �ϱ� ����.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_fSize));

	// ����Ʈ ��������Ʈ�� ������ �� �ִ� �ּ� ũ�⸦ ���� - �ʹ� �۾������ʵ��� ���� 
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, DWORD(0.0f));

	// �Ÿ��� ���� ����Ʈ ��������Ʈ�� ũ�Ⱑ ���ϴ� ����� ����
	// ���⿡�� ���ϴ� �Ÿ��� ī�޶�� ����Ʈ ��������Ʈ ���� �Ÿ�.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, DWORD(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, DWORD(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(m_fSize));

	//// �ؽ�ó�� ���ĸ� �̿�
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// ���� ������ Ȱ��ȭ�Ͽ� �ؽ�ó�� ���� ä���� �ؽ�ó�� �ȼ� ������ �����ϵ��� ����.
	// �̸� ���� �پ��� ȿ���� ���� �� ������,
	// ���� ��ǥ���� ���� �簢���� �ƴ� �ٸ� ��ƼŬ ���¸� ���� �ϴ� ��.
	// ��, "�����̿� �����" �ձ� ��ƼŬ�� ��� ���ؼ���
	// ����� ������ �������� ���� ä���� ���� ��� �ؽ�ó�� �̿��ϸ� ��.
	// �̷��� �ϸ� �簢���� ��� �ؽ�ó ��ü���ƴ� ��� �� ����� ��ƼŬ�� ���� �� �ִ�.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CPSystem::PostRender_Particle()
{
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

bool CPSystem::isDead()
{
	for (const auto& attribute : m_ParticlesList)
	{
		if (attribute.isAlive)
			return false;
	}
	return true;
}

void CPSystem::RemoveDeadParticles()
{
	//m_ParticleList�� �˻��Ͽ� ���� ��ƼŬ�� ����Ʈ���� ���� �Ѵ�.! 
	list<Attribute>::iterator iter;

	iter = m_ParticlesList.begin();

	while (iter !=m_ParticlesList.end())
	{
		if (iter->isAlive == false)
		{
			//erase�� ���� �ݺ��ڸ� �����ϹǷ� �ݺ��ڸ� ������ų�ʿ� �����ϱ� �ǵ帮������
			iter = m_ParticlesList.erase(iter);
		}
		else
		{
			iter++; //���� ����Ʈ
		}
	}
}

void CPSystem::ResetList()
{
	for (auto& iter : m_ParticlesList)
	{
		ResetParticle(&iter);
	}
}

void CPSystem::AddParticle()
{
	// �ý��ۿ� ��ƼŬ�� �߰�.
// ����Ʈ�� �߰� �ϱ����� ��ƼŬ�� �ʱ�ȭ �ϴµ� resetPaticle �̿�
	Attribute attribute;
	ResetParticle(&attribute); // �ٿ���ڽ� �ֻ������ ��ƼŬ�� �о���� 
	m_ParticlesList.push_back(attribute);
}

void CPSystem::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_tex);
	RemoveDeadParticles(); // ����Ʈ erase
}
