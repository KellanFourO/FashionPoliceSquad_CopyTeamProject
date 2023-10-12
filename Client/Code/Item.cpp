#include "stdafx.h"
#include "..\Header\Item.h"

#include "Player.h"

#include "Export_System.h"
#include "Export_Utility.h"


CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev) :CGameObject(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CItem::CItem(const CItem& rhs) : CGameObject(rhs)
{
	m_pGraphicDev->AddRef();
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_GameObject()
{
	Set_ObjectTag(OBJECTTAG::ITEM);
	Add_Component();
	INFO.Item_ID = ItemID::COCKTAIL;
	INFO.vPos = _vec3(10.f, 0.5f, 10.f);
	m_bDead = false;

	return S_OK;
}

_int CItem::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bDead) {
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		m_pTransformCom->Set_Pos(INFO.vPos);
		__super::Update_GameObject(fTimeDelta);
		CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
		NULL_CHECK_RETURN(pPlayerTransCom, -1); // 플레이어 가져오기
		_vec3 vPlayerPos, vMyPos, vPlayerPos_Rel;
		pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
		m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
		vPlayerPos_Rel = vPlayerPos - vMyPos;
		_float Distatnce = D3DXVec3Length(&vPlayerPos_Rel);

		if (abs(Distatnce) < 10.f) {
			Item_Enter();
			m_bDead = true;
		}
		D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
		_float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);

		m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI); // 자전
		m_pTransformCom->Set_Scale(_vec3(3.f, 3.f, 3.f));



	}
	return OBJ_NOEVENT;
}

void CItem::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CItem::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pBufferCom->Get_Frame(8, 1, (_float)INFO.Item_ID);
	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CItem::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Item"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return E_NOTIMPL;
}

void CItem::Item_Enter()
{
	CManagement* Manager;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Manager->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	//pPlayer->Item_Get(INFO.Item_ID);

	switch(INFO.Item_ID) {
	case ItemID(0):
		pPlayer->Healed(10);
		break;
	case ItemID(1):
		pPlayer->Healed(20);
		break;
	case ItemID(2):
		pPlayer->Armor_Get(10);
		break;
	case ItemID(3):
		pPlayer->Armor_Get(20);
		break;
	case ItemID(4):
		pPlayer->Healed(3);
		break;
	case ItemID(5):

		break;

	}

}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItem* pInstance = new CItem(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CItem::Free()
{
	__super::Free();
}