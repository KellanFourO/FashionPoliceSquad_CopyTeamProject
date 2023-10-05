#include "stdafx.h"
#include "BossStage.h"

CBossStage::CBossStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Ready_Scene()
{
	return E_NOTIMPL;
}

_int CBossStage::Update_Scene(const _float& fTimeDelta)
{
	return _int();
}

void CBossStage::LateUpdate_Scene()
{
}

void CBossStage::Render_Scene()
{
}

CBossStage* CBossStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CBossStage::Free()
{
}
