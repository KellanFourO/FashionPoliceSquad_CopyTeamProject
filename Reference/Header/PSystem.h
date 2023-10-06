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
	HRESULT				Ready_GameObject(const _tchar* texFileName);	// ����Ʈ ��������Ʈ�� �����ϱ����� ���ؽ� ���۸� ����� �ؽ�ó�� ����µ� �ʱ�ȭ�۾� ó���ϴ°� - ��å�� bool init�� �ش�
	_int				Update_GameObject(const _float& fTimeDelta);
	void				LateUpdate_GameObject();
	virtual void		Render_GameObject()override;

protected:
	virtual void ResetParticle(Attribute* attribute) PURE; // ��ƼŬ���� ���µǴ� ����� �ٸ���.

public:
	virtual void AddParticle(); // �ý��ۿ� ��ƼŬ�� �߰�
	virtual void Free();
	virtual void RemoveDeadParticles(); // �Ӽ� ����Ʈ _particle�� �˻��Ͽ� ���� ��ƼŬ�� ����Ʈ���� ����.

private:
	virtual void ResetList(); //�ý��� ���� ��� ��ƼŬ �Ӽ��� ����
	virtual void preRender_Particle();	// �������� �ռ� �����ؾ� �� �ʱ� ���� ���¸� ����
	virtual void PostRender_Particle();  // Ư�� ��ƼŬ �ý����� �������� �� �ִ� ���� ���¸� �����ϴ� �� �̿�

protected:
	_bool isEmpty() { return m_ParticlesList.empty(); }   // ���� �ý��ۿ� ��ƼŬ�� ���� ��� true ����.
	_bool isDead(); // �ý��� ���� ��ƼŬ�� ��� ���� ��� true ����.
public:

	DWORD FtoDw(float f)
	{
		return *((DWORD*)&f);//float�� DWORD�� ��ȯ
	}

	float GetRandomFloat(float lowBound, float highBound)
	{
		if (lowBound >= highBound)//�߸��� �Է�
		{
			return lowBound;
		}
		//0,1 ������ ���� float�� ��´�.
		float f = (rand() % 10000) * 0.0001f;
		//lowBound , highBound ������ float�� ����
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
	_vec3						m_vOrigin;		// �ý����� ��õ. ��ƼŬ�� �����ϴ°�.
	BoundingBox					m_pBoungingBox;	//������.
	_float						m_fMitRate;		//�ý��ۿ� ���ο� ��ƼŬ�� �߰��Ǵ� ����. ���� �ʴ� ��ƼŬ���� ���
	_float						m_fSize;		//��ƼŬ ũ��
	LPDIRECT3DTEXTURE9			m_tex;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	list<Attribute>				m_ParticlesList;//�ý��� �� ��ƼŬ �Ӽ� ����Ʈ. ��ƼŬ�� ����� �����ϴµ� �̰� ���!
	_int						m_iMaxParticles;//�־��� �ð� ���� �ý����� ���� �� �ִ� �ִ� ��ƼŬ�� ��.��ƼŬ �������� �ӵ����� ����� �ӵ��� ������ ���ÿ� �����Ҽ� �ִ� ��ƼŬ�� ���� �����ϴ°�!

	DWORD						m_dVbSize; //���ؽ� ���۰� �����Ҽ� �ִ� ��ƼŬ�� �� , �̰��� ���� ��ƼŬ �ý��� ���� ��ƼŬ���ʹ� �ٸ�.
	DWORD						m_dVbOffset;
	DWORD						m_dVbBatchSize;

};

END