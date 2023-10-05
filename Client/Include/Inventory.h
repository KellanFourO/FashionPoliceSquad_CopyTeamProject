#pragma once
#include "GameObject.h"
class CPlayer;
class CInventory : public CGameObject
{
private:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory(CInventory& rhs);
	virtual  ~CInventory();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() {};

	void	Item_Get(ItemID _ItemID);
	void    Use_Item(ItemID _Item_ID);

	struct m_ItemBox {
		ItemID	m_ItemID;	// 아이템 ID
		_int	iItmePcs;	//아이템 갯수
	};

	vector<m_ItemBox> Inventory; // Enum에 있는 아이템 한번에 다 받음

private:
	CPlayer* m_pPlayer;

public:
	void			Set_Player(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
	CPlayer*		Get_Player()				  { return m_pPlayer; }

private:
	void			Key_Input();

public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free();
};

