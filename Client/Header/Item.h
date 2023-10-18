#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Inventory.h"

class CItemState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CItem : public Engine::CGameObject
{
private:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItem(const CItem& rhs);
	virtual ~CItem();

public:
	virtual	HRESULT	Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual void	LateUpdate_GameObject();
	virtual void	Render_GameObject();

private:
	HRESULT			Add_Component();

	void			Item_Enter();

	_bool			m_bDead;


private:
	CRcTex*		m_pBufferCom = nullptr; // �ؽ�ó�� �׸������� ���� ������Ʈ
	CTransform* m_pTransformCom = nullptr; // �̵� ������Ʈ
	CTexture*	m_pTextureCom = nullptr; // �ؽ��� ������Ʈ

	CTransform* m_pPlayerTranformCom = nullptr;

public:
	Item_Info		INFO;
	void			Set_INFO(Item_Info _INFO) { INFO = _INFO; }

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();
};

