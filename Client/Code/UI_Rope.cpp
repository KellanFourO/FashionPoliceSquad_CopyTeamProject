#include "stdafx.h"
#include "UI_Rope.h"
#include "Belt.h"
#include "Export_Utility.h"
#include "Export_System.h"

CRopeUI::CRopeUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(UIDATA));
}

CRopeUI::CRopeUI(const CRopeUI& rhs)
	: Engine::CGameObject(rhs)
{
}

CRopeUI::~CRopeUI()
{
}

HRESULT Engine::CRopeUI::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vPos = { 1.f, 1.f, 0.1f };
	m_vScale = { 5.f, 5.f, 5.f };

	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(m_vPos);

	m_pPlayer = Management()->Get_Player();
	m_pBelt = Management()->Get_Belt();

	return S_OK;
}

Engine::_int Engine::CRopeUI::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bLateInit)
	{
		m_pCamera = dynamic_cast<CNewFPSCamera*>(Management()->Get_ObjectList(LAYERTAG::CAMERA,OBJECTTAG::FPSCAMERA).back());
		m_bLateInit = false;
	}

	if (m_pBelt->Get_Target())
	{
		Engine::Add_RenderGroup(RENDER_UI, this);
		_int iExit = __super::Update_GameObject(fTimeDelta);

		_vec3 vUItoPlayerDir;

		vUItoPlayerDir = m_pPlayer->Get_Transform()->m_vInfo[INFO_POS] - m_pTransformCom->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&vUItoPlayerDir, &vUItoPlayerDir);
		_float fAngle = atan2f(vUItoPlayerDir.x, vUItoPlayerDir.z);
		m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);

		m_pTransformCom->m_vScale.x = m_vScale.x;
		m_pTransformCom->m_vScale.y = m_vScale.y;
		m_pTransformCom->m_vScale.z = m_vScale.z;

		m_pTransformCom->m_vInfo[INFO_POS].x = m_pBelt->Get_Target()->Get_Transform()->m_vInfo[INFO_POS].x;
		m_pTransformCom->m_vInfo[INFO_POS].y = m_pBelt->Get_Target()->Get_Transform()->m_vInfo[INFO_POS].y - 10.f;
		m_pTransformCom->m_vInfo[INFO_POS].z = m_pBelt->Get_Target()->Get_Transform()->m_vInfo[INFO_POS].z;
	}





	return 0;
}

void Engine::CRopeUI::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CRopeUI::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pCamera->Get_View());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pCamera->Get_Proj());

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

}

HRESULT Engine::CRopeUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_RopeUITexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CRopeUI* CRopeUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRopeUI* pInstance = new CRopeUI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("RopeUI Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CRopeUI::Free()
{
	__super::Free();
}
