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

	//CVIBufferComp 에서 한거와 같은 버퍼 만들기 
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(
		m_dVbSize * sizeof(Particle), // 생성할 버퍼의 크기
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, // 생성하고자 하는 버텍스 버퍼의 종류(0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC)
		FVF,// 버텍스 속성 옵션
		D3DPOOL_DEFAULT,// 메모리 풀 방식 - 동적 생성 (정적 버퍼인 경우 MANAGED)
		&m_pVB,// 결과 컴 객체
		0),
		E_FAIL);

	// 매 프레임 마다 파티클을 갱신해야 하며 이는 곧 버텍스 버퍼의 메모리에 접근해야 함을 의미.
	// 정적 버텍스 버퍼로의 접근 속도는 상당히 느리므로 동적 버텍스 버퍼를 이용

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

			// 경계 범위를 벗어났는지 여부 확인
			if (m_pBoungingBox.m_vMax.x < iter.position.x || m_pBoungingBox.m_vMin.x > iter.position.x ||
				m_pBoungingBox.m_vMax.y < iter.position.y || m_pBoungingBox.m_vMin.y > iter.position.y ||
				m_pBoungingBox.m_vMax.z < iter.position.z || m_pBoungingBox.m_vMin.z > iter.position.z)
			{
				// 재활용
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

		//모든 파티클이 렌더링 될 떄까지 

// 		list<Attribute>::iterator iter;
// 		for (iter = m_ParticlesList.begin(); iter != m_ParticlesList.end(); iter++)
		for(auto& iter : m_ParticlesList)
		{
			if (iter.isAlive)
			{
				//한단계의 생존한 파티클을 다음 버텍스 세그먼트로 복사한다.
				v->_Position = iter.position;
				v->_Color = (D3DCOLOR)iter.color;

				v++;
				numParticlesInBatch++; //단계 카운트 증가

				//현재 단계가 모두 채워져 있는지?
				if (numParticlesInBatch == m_dVbBatchSize)
				{
					//버텍스 버퍼로 복사된 마지막 단계의 파티클을 그림 

					m_pVB->Unlock();

					m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_dVbOffset, m_dVbBatchSize);

					//단계가 그려지는 동안 다음 단계를 파티클로 채움.

					//다음 단계의 처음 오프셋으로 이동
					m_dVbOffset += m_dVbBatchSize;

					//버텍스 버퍼의 경계를 넘는메모리로 오프셋을 설정하지 않는다.
					//경계를 넘을 경우 처음부터 시작! 
					if (m_dVbOffset >= m_dVbSize)
						m_dVbOffset = 0;

					m_pVB->Lock(m_dVbOffset * sizeof(Particle), m_dVbBatchSize * sizeof(Particle), (void**)&v,
						m_dVbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; //다음 단계를 위한 리셋

				}

			}
		}
		m_pVB->Unlock();
		//(numParticlesInBatch == m_dvbBatchSize) 조건이 만족되지 못하여 마지막 단계가 렌더링되지 않는 경우가 발생할수 있음
		//일부만 채워진 단계는 바로 이곳에서 렌더링됨.
		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_dVbOffset, numParticlesInBatch);

		}
		//다음 블록
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
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 뒷면제거 
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// 포인트 스프라이트 렌더 상태 = 점 하나로 표현되는 스프라이트, 일반적으로 입자시스템에 이용 
	// 현재 지정된 전체 텍스처를 포인트 스프라이트의 텍스처 매핑에 이용할것임을 의미
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);

	// 포인트 크기를 뷰 스페이스 단위로 해석하도록 지정.
	// 포인트 스프라이트의 크기는 카메라와의 거리에 따라 적절하게 조정됨.
	// 즉, 카메라와 멀리 떨어진 파티클은 가까운 파티클에 비해작게 나타남.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);

	// 포인트 스프라이트의 크기를 지정.
	// 이 값은 D3DRS_POINTSCALEENABLE 상태 값에 따라서 뷰 스페이스 내의 크기나
	// 스크린 스페이스 내의 크기로 해석.
	// SetRenderState 호출이 float이 아닌 DWORD를 필요로 하기 때문.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_fSize));

	// 포인트 스프라이트의 지정할 수 있는 최소 크기를 지정 - 너무 작아지지않도록 제한 
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, DWORD(0.0f));

	// 거리에 따라 포인트 스프라이트의 크기가 변하는 방법을 제어
	// 여기에서 말하는 거리란 카메라와 포인트 스프라이트 간의 거리.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, DWORD(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, DWORD(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(m_fSize));

	//// 텍스처의 알파를 이용
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// 알파 블렌딩을 활성화하여 텍스처의 알파 채널이 텍스처의 픽셀 투명도를 결정하도록 했음.
	// 이를 통해 다양한 효과를 얻을 수 있으며,
	// 가장 대표적인 것이 사각형이 아닌 다른 파티클 형태를 구현 하는 것.
	// 예, "눈덩이와 비슷한" 둥근 파티클을 얻기 위해서는
	// 흰색의 원형과 검은색의 알파 채널을 갖는 흰색 텍스처를 이용하면 됨.
	// 이렇게 하면 사각형의 흰색 텍스처 전체가아닌 흰색 원 모양의 파티클을 만들 수 있다.
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
	//m_ParticleList를 검색하여 죽은 파티클을 리스트에서 제거 한다.! 
	list<Attribute>::iterator iter;

	iter = m_ParticlesList.begin();

	while (iter !=m_ParticlesList.end())
	{
		if (iter->isAlive == false)
		{
			//erase는 다음 반복자를 리턴하므로 반복자를 증가시킬필요 없으니까 건드리지말것
			iter = m_ParticlesList.erase(iter);
		}
		else
		{
			iter++; //다음 리스트
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
	// 시스템에 파티클을 추가.
// 리스트에 추가 하기전에 파티클을 초기화 하는데 resetPaticle 이용
	Attribute attribute;
	ResetParticle(&attribute); // 바운딩박스 최상단으로 파티클을 밀어넣음 
	m_ParticlesList.push_back(attribute);
}

void CPSystem::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_tex);
	RemoveDeadParticles(); // 리스트 erase
}
