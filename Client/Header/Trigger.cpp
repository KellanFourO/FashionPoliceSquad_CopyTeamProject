#include "stdafx.h"
#include "Trigger.h"

#include "ImGuiManager.h"

#include "Export_System.h"
#include "Export_Utility.h"


CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CTrigger::CTrigger(const CTrigger& rhs)
    : CGameObject(rhs)
{
}

CTrigger::~CTrigger()
{
}

HRESULT CTrigger::Ready_GameObject(_vec3 pMouse_Pos, _vec3 Size)
{
    return E_NOTIMPL;
}

_int CTrigger::Update_GameObject(const _float& fTimeDelta)
{
    return _int();
}

void CTrigger::LateUpdate_GameObject()
{
}

void CTrigger::Render_GameObject()
{
}

HRESULT CTrigger::Add_Component()
{
    return E_NOTIMPL;
}

CTrigger* CTrigger::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pMouse_Pos, _vec3 Size, TRIGGER_CASE eCase, TRIGGER_TYPE eType, TRIGGER_NUMBER eTrName)
{
    return nullptr;
}

void CTrigger::Free()
{
}
