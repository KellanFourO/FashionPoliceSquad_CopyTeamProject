#include "stdafx.h"
#include "MapCursor.h"

#include "ImGuiManager.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMapCursor::CMapCursor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMapCursor::CMapCursor(const CMapCursor& rhs)
	: CGameObject(rhs)
{
}

CMapCursor::~CMapCursor()
{
}


HRESULT CMapCursor::Add_Component()
{
	CComponent* pComponent = nullptr;

// 	pComponent = m_pBufferCom = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"Proto_RcCol"));
// 	NULL_CHECK_RETURN(pComponent, E_FAIL);
// 	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::CALCULATOR, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CursorTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

HRESULT CMapCursor::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { VTXITV, VTXITV, VTXITV };

	return S_OK;
}

_int CMapCursor::Update_GameObject(const _float& fTimedelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	Key_Input(fTimedelta, MapCursor_At_Tool());

	if (true == CImGuiManager::GetInstance()->Get_NotNormal_Check())
	{
		m_fCubesize.fX = CImGuiManager::GetInstance()->Get_CubeSize().fX;
		m_fCubesize.fY = CImGuiManager::GetInstance()->Get_CubeSize().fY;
		m_fCubesize.fZ = CImGuiManager::GetInstance()->Get_CubeSize().fZ;
	}
	if (false == CImGuiManager::GetInstance()->Get_NotNormal_Check())
	{
		m_pTransformCom->m_vScale = { VTXITV, VTXITV, VTXITV };
	}
	__super::Update_GameObject(fTimedelta);
	return 0;
}

void CMapCursor::LateUpdate_GameObject()
{
	m_pTransformCom->m_vScale = { m_fCubesize.fX, m_fCubesize.fY, m_fCubesize.fZ };

	__super::LateUpdate_GameObject();
}

void CMapCursor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMapCursor::Key_Input(const _float& fTimeDelta, _vec3 pMouse_Pos)
{
	if (Engine::Get_DIMouseState(DIM_RB))
	{
		m_vCursor_Pos = pMouse_Pos;
	}
	return S_OK;
}

_vec3 CMapCursor::MapCursor_At_Tool()
{
 	_vec3		vPos;
 	m_pTransformCom->Get_Info(INFO_POS, &vPos);

 	CWireTex* pWireCom = dynamic_cast<CWireTex*>(Engine::Get_Component(ID_STATIC, LAYERTAG::ENVIRONMENT, OBJECTTAG::WIREFRAME, COMPONENTTAG::BUFFER));
 	NULL_CHECK_RETURN(pWireCom, _vec3());

 	CTransform* pWireTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_STATIC, LAYERTAG::ENVIRONMENT, OBJECTTAG::WIREFRAME, COMPONENTTAG::TRANSFORM));
 	NULL_CHECK_RETURN(pWireTransCom, _vec3());

 	vPos = m_pCalculatorCom->MapCursor_From_Mouse(g_hWnd, pWireCom, pWireTransCom);
 	vPos.y = CImGuiManager::GetInstance()->Get_CubeHeightLevel();

 	if (false == CImGuiManager::GetInstance()->Get_OBJModeCheck())
 	{
 		//뚝뚝 끊기는 효과 : 오브젝트 모드에선 비해당
 		int temp1 = (int)vPos.x / VTXWITV;
 		int temp2 = (int)vPos.y / VTXWITV;
 		int temp3 = (int)vPos.z / VTXWITV;
 		vPos.x = (float)temp1 * VTXWITV + (VTXWITV * 0.5f);
 		vPos.y = (float)temp2 * VTXWITV;
 		vPos.z = (float)temp3 * VTXWITV + (VTXWITV * 0.5f);
 	}


 	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	return vPos;
}

CMapCursor* CMapCursor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapCursor* pInstance = new CMapCursor (pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("MapCursor Create FAILED!");
		return nullptr;
	}

	return pInstance;
}

void CMapCursor::Free()
{
	__super::Free();
}
