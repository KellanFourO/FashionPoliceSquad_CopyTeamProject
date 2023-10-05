#include "CameraCom.h"
#include "Transform.h"

CCameraCom::CCameraCom()
{
}

CCameraCom::CCameraCom(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CCameraCom::CCameraCom(const CCameraCom& rhs)
	: CComponent(rhs)
{
}

CCameraCom::~CCameraCom()
{
}

HRESULT CCameraCom::Ready_Camera()
{
	m_vecAt = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

_int CCameraCom::Update_Component(const _float& fTimeDelta)
{
	return _int();
}

void CCameraCom::LateUpdate_Component()
{
}

void CCameraCom::Set_ViewSpcae()
{
	m_vecAt = (m_pTransformCom->m_vInfo[INFO_POS] + m_pTransformCom->m_vInfo[INFO_LOOK]);

	D3DXMatrixLookAtLH(&m_matView,
		&m_pTransformCom->m_vInfo[INFO_POS],
		&m_vecAt,
		&m_pTransformCom->m_vInfo[INFO_UP]);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCameraCom::Set_Projection(const CAMERA_TYPE eMode)
{
	switch (eMode)
	{
	case CAMERA_TYPE::MOVE_CAMERA:
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.f, WINCX / (_float)WINCY, 0.1f, 1000.0f);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		break;
	case CAMERA_TYPE::NOTMOVE_CAMERA:	// custom
		D3DXMatrixOrthoLH(&m_matProj, WINCY, WINCY, 1.0f, 1000.0f);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		break;
	}
}

CCameraCom* CCameraCom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraCom* pInstance = new CCameraCom(pGraphicDev);
	// TODO : 매개변수 추가
	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CCameraCom::Clone(void)
{
	return new CCameraCom(*this);
}

void CCameraCom::Free()
{
	CComponent::Free();
}
