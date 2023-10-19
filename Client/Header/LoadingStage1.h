#pragma once

#include "Scene.h"

#include "BackGround.h"
#include "Title.h"
#include "ProgressFrame.h"
#include "ProgressValue.h"

#include "Engine_Define.h"

#include "Loading.h"
#include "MapTool.h"
#include "BossStage.h"
#include "Stage.h"
#include "Stage2.h"
#include "LobbyStage.h"


class CLoadingStage1 : public Engine::CScene
{
private:
	explicit CLoadingStage1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadingStage1();

public:
	virtual HRESULT		Ready_Scene(SCENETAG eSceneTag);
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		LateUpdate_Scene();
	virtual void		Render_Scene();

private:
	HRESULT				Ready_Prototype();
	HRESULT				Ready_Layer_Environment(LAYERTAG eLayerTag);
	HRESULT				Ready_Layer_GameLogic(LAYERTAG eLayerTag) { return S_OK; }
	HRESULT				Ready_Layer_UI(LAYERTAG eLayerTag) { return S_OK; }

private:
	CLoading*					m_pLoading;
	CLoading::LOADINGID			m_pLoadingID;
	CScene*						m_pScene;
	_int						m_CheckPoint;
	CProgressValue*				m_pProgressBar = nullptr;

	_float						m_fTick = 0.f;


public:
	static CLoadingStage1* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);
private:
	virtual void		Free();
};


