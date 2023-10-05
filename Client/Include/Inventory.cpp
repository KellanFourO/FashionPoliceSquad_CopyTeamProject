#include "stdafx.h"
#include "Inventory.h"
#include "Player.h"

#include "Export_Utility.h"
#include "Export_System.h"
CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CInventory::CInventory(CInventory& rhs)
	: Engine::CGameObject(rhs)
{
}
CInventory::~CInventory()
{
	Free();
}

HRESULT CInventory::Ready_GameObject()
{
	for (_int i = 0; i < (_int)ItemID::ITEM_ID_END; i++) {
		m_ItemBox pItemBox;
		pItemBox.m_ItemID = (ItemID)i;
		pItemBox.iItmePcs = 0;
		Inventory.push_back(pItemBox);
	}
	return S_OK;
}

_int CInventory::Update_GameObject(const _float& fTimeDelta)
{

	return OBJ_NOEVENT;
}

void CInventory::LateUpdate_GameObject()
{
}

void CInventory::Item_Get(ItemID _ItemID)
{
	auto iter = Inventory.begin();
	for (int i = 0; i != (_int)_ItemID; ++i) {
		++iter;
	}
	iter->iItmePcs += 1; 
}

void CInventory::Use_Item(ItemID _Item_ID)
{
	CPlayer* Player = dynamic_cast<CPlayer*>(m_pPlayer);
	switch (_Item_ID) {

	}
}

void CInventory::Key_Input()
{
	if (Engine::Get_DIKeyState(DIK_I)) {
		Use_Item(ItemID::COCKTAIL);
	}
	if (Engine::Get_DIKeyState(DIK_O)) {
		Use_Item(ItemID::NECKTIE);
	}
	if (Engine::Get_DIKeyState(DIK_P)) {
		
	}
}

CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory* pInstance = new CInventory(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();
}
