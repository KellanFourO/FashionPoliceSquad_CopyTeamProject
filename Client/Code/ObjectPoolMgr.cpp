#include "../Header/ObjectPoolMgr.h"
#include "Export_Utility.h"
#include "Export_System.h"

IMPLEMENT_SINGLETON(CObjectPoolMgr)

CObjectPoolMgr::CObjectPoolMgr()
	: m_pPool(nullptr)
{
}

CObjectPoolMgr::~CObjectPoolMgr()
{
	Free();
}

void CObjectPoolMgr::Ready_Pool()
{
	if (!m_pPool)
		m_pPool = new ObjectPool;
}

void CObjectPoolMgr::Delete_Object(CGameObject* _pGameObject)
{
	switch (_pGameObject->Get_ObjectTag())
	{
	case OBJECTTAG::PLAYERBULLET:
		_pGameObject->Set_Dead(true);
		//static_cast<CMonster*>(_pGameObject)->Init_Stat();
		//m_pPool->GetMonsterPool(static_cast<CMonster*>(_pGameObject)->Get_MonsterTag()).push(static_cast<CMonster*>(_pGameObject));
		//EventManager()->DeleteObject(_pGameObject);
		break;

	case OBJECTTAG::EFFECT:
		_pGameObject->Set_Dead(true);
		//static_cast<CTempEffect*>(_pGameObject)->Init_Data(); // CTempEffect에 virtual PURE로 순수가상함수 하나 만들어서
		//														// 자식 클래스에 오버라이딩하시면 됩니다
		//m_pPool->GetEffectPool(static_cast<CTempEffect*>(_pGameObject)->Get_EffectTag()).push(static_cast<CTempEffect*>(_pGameObject));
		break;
	}
}

void CObjectPoolMgr::Free()
{
	m_pPool->~ObjectPool();
	Safe_Delete<ObjectPool*>(m_pPool);
}

CObjectPoolMgr::ObjectPool::ObjectPool()
{
	Ready_Pool();
}

CObjectPoolMgr::ObjectPool::~ObjectPool()
{
	Free();
}

void CObjectPoolMgr::ObjectPool::Ready_Pool()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	//템플릿안쓸거면 swich문으로 불렛 태그랑 , 이펙트 태그 다 돌아야 함 ! 
}

void CObjectPoolMgr::ObjectPool::Free()
{
}
