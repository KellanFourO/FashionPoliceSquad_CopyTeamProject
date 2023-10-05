#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;

class CObjectPoolMgr : public CBase
{
	DECLARE_SINGLETON(CObjectPoolMgr)
private:
	explicit CObjectPoolMgr();
	virtual ~CObjectPoolMgr();

public:
	void	Ready_Pool();

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


	};
};

END
