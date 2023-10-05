#include "stdafx.h"
#include "FlyingCamera.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "CameraManager.h"


CFlyingCamera::CFlyingCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CFlyingCamera::CFlyingCamera(const CFlyingCamera& rhs)
	: Engine::CGameObject(rhs)
{
}

CFlyingCamera::~CFlyingCamera()
{
	Free();
}

HRESULT CFlyingCamera::Ready_GameObject(void)
{
	m_eObjectTag = OBJECTTAG::FLYCAMERA;
	m_eCameraMode = CAMERA_MODE::THIRD_PERSON;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CFlyingCamera::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	if (SCENETAG::MAPTOOL == Management()->Get_Scene()->Get_SceneTag())
	{
		if (false == m_isMouseFree)
		{
			Mouse_Fix();
		}
	}

	_int iExit = __super::Update_GameObject(fTimeDelta);

	return iExit;
}

void CFlyingCamera::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}


void CFlyingCamera::Change_Mode()
{
// 	if(m_eCameraMode)
// 		m_pTransformCom->Copy_RUL_AddPos(m_pTransformCom->m_pParent->m_vInfo);
// 	m_eCameraMode = (CAMERA_MODE)((m_eCameraMode + 1) % 2);
}

HRESULT CFlyingCamera::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::ProtoMgr()->GetInstance()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCamera = static_cast<CCameraCom*>(Engine::ProtoMgr()->GetInstance()->Clone_Proto(L"Proto_CameraCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::CAMERA, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CFlyingCamera::Key_Input(const _float& fTimeDelta)
{	
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		m_pTransformCom->Translate(m_fSpeed * fTimeDelta * m_pTransformCom->m_vInfo[INFO_LOOK]);

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		m_pTransformCom->Translate(m_fSpeed * fTimeDelta * -m_pTransformCom->m_vInfo[INFO_LOOK]);

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		m_pTransformCom->Translate(m_fSpeed * fTimeDelta * -m_pTransformCom->m_vInfo[INFO_RIGHT]);

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		m_pTransformCom->Translate(m_fSpeed * fTimeDelta * m_pTransformCom->m_vInfo[INFO_RIGHT]);

	if (Engine::Get_DIKeyState(DIK_Q) & 0x80)
		m_pTransformCom->Translate(m_fSpeed * fTimeDelta * m_pTransformCom->m_vInfo[INFO_UP]);

	if (Engine::Get_DIKeyState(DIK_E) & 0x80)
		m_pTransformCom->Translate(m_fSpeed * fTimeDelta * -m_pTransformCom->m_vInfo[INFO_UP]);

	_long		dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		//if (Engine::Get_DIMouseState(DIM_LB))
		m_pTransformCom->Rotate(ROT_Y, -D3DXToRadian(dwMouseMove) * fTimeDelta * 4.f);
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		//if (Engine::Get_DIMouseState(DIM_LB))
		m_pTransformCom->Rotate(ROT_X, -D3DXToRadian(dwMouseMove) * fTimeDelta * 4.f);
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		//if (Engine::Get_DIMouseState(DIM_RB))
		m_pTransformCom->Translate(dwMouseMove * fTimeDelta * -m_pTransformCom->m_vInfo[INFO_RIGHT] * 2.f);
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		//if (Engine::Get_DIMouseState(DIM_RB))
		m_pTransformCom->Translate(dwMouseMove * fTimeDelta * m_pTransformCom->m_vInfo[INFO_UP] * 2.f);
	}

	if (Engine::Get_DIKeyState(DIK_F1) && SCENETAG::MAPTOOL == Management()->Get_Scene()->Get_SceneTag())
	{
		Change_Mode();
	}

	if (Engine::Get_DIKeyState(DIK_TAB))
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		m_isMouseFree = !m_isMouseFree;
	}
	else
		m_bCheck = false;
}

void CFlyingCamera::Mouse_Fix()
{
	POINT		pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

CFlyingCamera* CFlyingCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFlyingCamera* pInstance = new CFlyingCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FlyingCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFlyingCamera::Free()
{
	__super::Free();
}
