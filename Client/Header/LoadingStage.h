#pragma once

#include "Scene.h"

#include "Loading.h"
class CLoadingStage : public Engine::CScene
{
private:
	explicit CLoadingStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadingStage();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Layer_Environment(LAYERTAG _eLayerTag);
	HRESULT			Ready_Layer_GameLogic(LAYERTAG _eLayerTag) { return S_OK; }
	HRESULT			Ready_Layer_UI(LAYERTAG _eLayerTag) { return S_OK; }

private:
	CLoading*	m_pLoading;
public:
	static CLoadingStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free()override;
};

