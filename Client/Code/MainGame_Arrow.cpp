#include "stdafx.h"
#include <random>

#include "MainGame_Arrow.h"
#include "EventMgr.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMainGame_Arrow::CMainGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMainGame_Arrow::CMainGame_Arrow(const CMainGame_Arrow& rhs)
	: Engine::CGameObject(rhs)
{
}

CMainGame_Arrow::~CMainGame_Arrow()
{
}



HRESULT CMainGame_Arrow::Ready_GameObject()
{
//////////////////랜덤값 구하기/////////////////////////
	// 시드 값으로 사용할 난수 엔진 생성
	std::random_device rd;

	// 시드 값을 사용하여 유사 난수 엔진 생성
	std::mt19937 gen(rd());

	// 정수 분포 (예: 0 이상 4 미만의 난수)
	std::uniform_int_distribution<int> distribution(0, 4);

	// 10개의 랜덤 숫자 생성
// 	for (int i = 0; i < 10; ++i) {
// 		int random_number = distribution(gen);
// 		std::cout << "Random Number " << i + 1 << ": " << random_number << std::endl;
// 	}
	//////////////////////////////////////


	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pStateIcon = CMini_StateIcon::Create(m_pGraphicDev);

	for (int i = 0; i != m_ArrowCount; ++i)
	{
		int random_number = distribution(gen);
		int iTemp = m_ArrowCount - (m_ArrowCount - i);
		CMini_Arrow* pArrow = CMini_Arrow::Create(m_pGraphicDev, iTemp, random_number);
		m_pVecArrow.push_back(pArrow);
	}

	//m_pTimeBar = CMini_TimeBar::Create(m_pGraphicDev);
	//m_pCursor = CMini_Cursor::Create(m_pGraphicDev);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;	

	vPos = { 400.f, 300.f, 0.f };
	vScale = { 250.f * fMultiply, 250.f * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);
	return S_OK;
}

void CMainGame_Arrow::Render_GameObject()
{
	if (!m_ClearCheck) {

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

		m_pTextureCom->Render_Textrue();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);



		m_pStateIcon->Render_GameObject();

		for (auto& iter : m_pVecArrow)
		{
			iter->Render_GameObject();
		}
	}
}

_int CMainGame_Arrow::Update_GameObject(const _float& fTimeDelta)
{
	m_ClearCheck = CEventMgr::GetInstance()->Get_MiniGameClearCheck(0);
		
	if (!m_ClearCheck) {

		Engine::Add_RenderGroup(RENDER_UI, this);
		KeyInput();

		__super::Update_GameObject(fTimeDelta);


		m_pStateIcon->Update_GameObject(fTimeDelta);

		for (auto& iter : m_pVecArrow)
		{
			iter->Update_GameObject(fTimeDelta);
		}
	}

	return _int();
}

void CMainGame_Arrow::LateUpdate_GameObject()
{
	if (!m_ClearCheck) {
		CGameObject::LateUpdate_GameObject();

		m_pStateIcon->LateUpdate_GameObject();

		for (auto& iter : m_pVecArrow)
		{
			iter->LateUpdate_GameObject();
		}
	}
}

HRESULT CMainGame_Arrow::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MainGame_Arrow_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}



void CMainGame_Arrow::KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		CEventMgr::GetInstance()->OffMiniGame_Arrow(SCENETAG::LOBBY);
	}







}





CMainGame_Arrow* CMainGame_Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainGame_Arrow* pInstance = new CMainGame_Arrow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MyDialog Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CMainGame_Arrow::Free()
{
	__super::Free();
}
