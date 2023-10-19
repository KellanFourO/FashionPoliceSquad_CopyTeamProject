#include "stdafx.h"
#include "..\Header\MainApp.h"
#include "Logo.h"
#include "Stage.h"
#include "ImGuiManager.h"
#include "FontMgr.h"
#include "EventMgr.h"
#include "SoundMgr.h"

CMainApp::CMainApp()
	: m_pDeviceClass(nullptr), m_pGraphicDev(nullptr), m_pManagementClass(nullptr)
{
}


CMainApp::~CMainApp()
{

}

HRESULT CMainApp::Ready_MainApp()
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	//CImGuiManager::GetInstance()->SetUp_ImGui(m_pGraphicDev);//

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTEXF_POINT);

	return S_OK;
}

int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	m_fFPS = ceil(1 / fTimeDelta);
	Engine::Update_InputDev();
	m_pManagementClass->Update_Scene(fTimeDelta);

	if (GetAsyncKeyState(VK_F5) & 0x8000)
	{
		CImGuiManager::GetInstance()->Set_Switch(true);//
	}

	if (GetAsyncKeyState(VK_F6) & 0x8000)
	{

		CImGuiManager::GetInstance()->Set_Switch(false);//
	}

	//CImGuiManager::GetInstance()->Update_ImGui(fTimeDelta);//

	return 0;
}

void CMainApp::LateUpdate_MainApp()
{

	Engine::LateUpdate_Scene();
	Engine::LateUpdate_InputDev();
	//CImGuiManager::GetInstance()->LateUpdate_ImGui(m_pGraphicDev);//

}

void CMainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	Engine::Render_Font(L"UI_FONT", to_wstring(m_fFPS).c_str(), &_vec2(500, 20), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),20, false);
	Engine::Render_Scene(m_pGraphicDev);


	//CImGuiManager::GetInstance()->Render_ImGui(m_pGraphicDev);//


	Engine::Render_End();

}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement)
{
	Engine::CScene* pScene = nullptr;

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(&m_pDeviceClass, g_hWnd, MODE_WIN), E_FAIL);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
	//이걸 켜면 조명 START .. Stage에서 켰으므로 여기에선 손대지 말 것.


	// InputDev
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	// Font
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Jinji", L"궁서", 30, 30, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"UI_WEAPON_NAME", L"../Bin/Font/Montserrat-Bold.ttf", 12, 12, FW_BOLD), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"UI_FONT", L"../Bin/Font/Montserrat-Bold.ttf", 27, 27, FW_REGULAR), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"DIALOG_FONT", L"../Bin/Font/Montserrat-Bold.ttf", 11, 18, FW_REGULAR), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"MISSION_FONT", L"../Bin/Font/Montserrat-Bold.ttf", 11, 13, FW_BOLD), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"TEST_FONT", L"../Bin/Font/Montserrat-Bold.ttf", 11, 13, FW_BOLD), E_FAIL);

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;


	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("MainApp Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CMainApp::Free()
{

	CEventMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CImGuiManager::GetInstance()->DestroyInstance();

	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);

	Engine::Release_Utility();
	Engine::Release_System();

}

