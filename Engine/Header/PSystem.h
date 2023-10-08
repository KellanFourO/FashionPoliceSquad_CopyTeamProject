#pragma once

#include "Engine_Define.h"
#include "GameObject.h"
#include "OctreeNode.h"
BEGIN(Engine)

class BoundingBox;

class ENGINE_DLL CPSystem : public CGameObject
{
public:
	explicit	CPSystem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CPSystem(const CPSystem& rhs);
	virtual		~CPSystem();
public:
	HRESULT				Ready_GameObject(const _tchar* texFileName);	// 포인트 스프라이트를 저장하기위한 버텍스 버퍼를 만들고 텍스처를 만드는등 초기화작업 처리하는곳 - 용책의 bool init에 해당
	_int				Update_GameObject(const _float& fTimeDelta);
	void				LateUpdate_GameObject();
	virtual void		Render_GameObject()override;

protected:
	virtual void ResetParticle(Attribute* attribute) PURE; // 파티클마다 리셋되는 방식이 다르다.

public:
	virtual void AddParticle(); // 시스템에 파티클을 추가
	virtual void Free();
	virtual void RemoveDeadParticles(); // 속성 리스트 _particle을 검색하여 죽은 파티클을 리스트에서 제거.

private:
	virtual void ResetList(); //시스템 내의 모든 파티클 속성을 리셋
	virtual void preRender_Particle();	// 렌더링에 앞서 지정해야 할 초기 렌더 상태를 지정
	virtual void PostRender_Particle();  // 특정 파티클 시스템이 지정했을 수 있는 렌더 상태를 복구하는 데 이용

protected:
	_bool isEmpty() { return m_ParticlesList.empty(); }   // 현재 시스템에 파티클이 없는 경우 true 리턴.
	_bool isDead(); // 시스템 내의 파티클이 모두 죽은 경우 true 리턴.
public:

	DWORD FtoDw(float f)
	{
		return *((DWORD*)&f);//float을 DWORD로 변환
	}

	float GetRandomFloat(float lowBound, float highBound)
	{
		if (lowBound >= highBound)//잘못된 입력
		{
			return lowBound;
		}
		//0,1 범위의 임의 float을 얻는다.
		float f = (rand() % 10000) * 0.0001f;
		//lowBound , highBound 범위의 float을 리턴
		return (f * (highBound - lowBound)) + lowBound;
	}

	void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
	{
		out->x = GetRandomFloat(min->x, max->x);
		out->y = GetRandomFloat(min->y, max->y);
		out->z = GetRandomFloat(min->z, max->z);
	}
	
protected:
	//IDirect3DDevice9*			m_pGraphicDev;
	_vec3						m_vOrigin;		// 시스템의 원천. 파티클이 시작하는곳.
	BoundingBox					m_pBoungingBox;	//경계상자.
	_float						m_fMitRate;		//시스템에 새로운 파티클이 추가되는 비율. 보통 초당 파티클수로 기록
	_float						m_fSize;		//파티클 크기
	LPDIRECT3DTEXTURE9			m_tex;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	list<Attribute>				m_ParticlesList;//시스템 내 파티클 속성 리스트. 파티클을 만들고 제거하는데 이거 사용!
	_int						m_iMaxParticles;//주어진 시간 동안 시스템이 가질 수 있는 최대 파티클의 수.파티클 없어지는 속도보다 만드는 속도가 빠르면 동시에 존재할수 있는 파티클의 수를 제한하는것!

	DWORD						m_dVbSize; //버텍스 버퍼가 보관할수 있는 파티클의 수 , 이값은 실제 파티클 시스템 내의 파티클수와는 다름.
	DWORD						m_dVbOffset;
	DWORD						m_dVbBatchSize;

};

END