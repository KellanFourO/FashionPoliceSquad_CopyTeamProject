#include "stdafx.h"
#include "LoadingStage1.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "BossStage.h"


CLoadingStage1::CLoadingStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev), m_pLoading(nullptr)
{
}

CLoadingStage1::~CLoadingStage1()
{
}

HRESULT CLoadingStage1::Ready_Scene()
{
	return E_NOTIMPL;
}

_int CLoadingStage1::Update_Scene(const _float& fTimeDelta)
{
	return _int();
}

void CLoadingStage1::LateUpdate_Scene()
{
}

void CLoadingStage1::Render_Scene()
{
}

HRESULT CLoadingStage1::Ready_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CLoadingStage1::Ready_Layer_Environment(LAYERTAG eLayerTag)
{
	return E_NOTIMPL;
}

CLoadingStage1* CLoadingStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CLoadingStage1::Free()
{
}
