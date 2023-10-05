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

	//추가하고자 하는 오브젝트 create

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
		//queue에다가 집어 넣기

		virtual void Free() override;
	private:
		static const _uint iMaxBullet = 1024;
		static const _uint iMaxEffect = 512;
		//queue로 불렛이랑 , 이펙트 담기
	};
private:
	ObjectPool* m_pPool;
};


