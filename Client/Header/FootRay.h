#pragma once
#include "GameObject.h"


class CFootRay :public CGameObject
{
private:
	explicit CFootRay(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFootRay(const CFootRay& rhs);
	virtual ~CFootRay();

public:
	virtual HRESULT						Ready_GameObject()override;
	virtual _int						Update_GameObject(const _float& fTimeDelta)override;
	virtual void						LateUpdate_GameObject()override;
	virtual void						Render_GameObject()override;
public:
	virtual void						OnCollisionEnter(CCollider* _pOther);
	virtual void						OnCollisionStay(CCollider* _pOther);
	virtual void						OnCollisionExit(CCollider* _pOther);
public:
	void								Set_Host(CGameObject* _pHost) { m_pHost = _pHost; }
	//void								Set_Player(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
public:
	CGameObject* m_pHost = nullptr;
	CGameObject* m_pColTarget = nullptr;
	//CGameObject* m_pPlayer = nullptr;
public:
	static CFootRay* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	HRESULT								Add_Component();
private:
	virtual void Free();
};

