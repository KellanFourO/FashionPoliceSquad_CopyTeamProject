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
		ItemID	m_ItemID;	// ������ ID
		_int	iItmePcs;	//������ ����
	};

	vector<m_ItemBox> Inventory; // Enum�� �ִ� ������ �ѹ��� �� ����

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

