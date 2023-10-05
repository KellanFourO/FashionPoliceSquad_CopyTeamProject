#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;

END
class CObjectPoolMgr : public CBase
{
	DECLARE_SINGLETON(CObjectPoolMgr)
private:
	explicit CObjectPoolMgr();
	virtual ~CObjectPoolMgr();

public:
	void	Ready_Pool();

	//�߰��ϰ��� �ϴ� ������Ʈ create

	void	Delete_Object(CGameObject* _pGameObject);
public:
	virtual void Free() override;
private:
	class ObjectPool : public CBase
	{
	public:
		ObjectPool();
		~ObjectPool();

		void Ready_Pool();
		//queue���ٰ� ���� �ֱ�

		virtual void Free() override;
	private:
		static const _uint iMaxBullet = 1024;
		static const _uint iMaxEffect = 512;
		//queue�� �ҷ��̶� , ����Ʈ ���
	};
private:
	ObjectPool* m_pPool;
};


