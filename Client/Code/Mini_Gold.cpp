
#include "stdafx.h"
#include "Mini_Gold.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMini_Gold::CMini_Gold(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMini_Gold::CMini_Gold(const CMini_Gold& rhs)
	: Engine::CGameObject(rhs)
{
}

CMini_Gold::~CMini_Gold()
{
}



HRESULT CMini_Gold::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Mini_Gold_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CMini_Gold::Set_Move()
{
	m_GoldPos.y -= fSpeed;
	m_pTransformCom->Set_Pos(m_GoldPos);

	if (m_GoldPos.y < -170.f)
	{
		m_GoldState = GoldState::Dead;
	}
}

void CMini_Gold::Set_Animation()
{
	m_iTexTick++;

	if (m_iTexTick > m_iTexTickMAX)  //텍스쳐틱이 돌 때마다
	{
		m_iTextureIndex++; //텍스쳐인덱스(이미지) 변경
		m_iTexTick = 0;
	}

	if (m_iTextureIndex > m_iTextureIndexEnd) //이미지가 끝번호보다 크면
	{
		m_iTextureIndex = 0;
	}
}



HRESULT CMini_Gold::Ready_GameObject(_float pPos_X)
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;
	m_GoldSize = 15.f;

	m_GoldState = GoldState::Live;

	vPos = { pPos_X, 120.f, 0.f };
	vScale = { m_GoldSize * fMultiply, m_GoldSize * fMultiply, 1.f };

	//vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_GoldPos = vPos;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	return S_OK;
}

_int CMini_Gold::Update_GameObject(const _float& fTimeDelta)
{
	if (m_GoldState == GoldState::Live) {
		Engine::Add_RenderGroup(RENDER_UI, this);
		int iExit = __super::Update_GameObject(fTimeDelta);

		Set_Move();
		Set_Animation();

	}
	return 0;
}

void CMini_Gold::LateUpdate_GameObject()
{
	if (m_GoldState == GoldState::Live) {

		__super::LateUpdate_GameObject();
	}
}

void CMini_Gold::Render_GameObject()
{
	if (m_GoldState == GoldState::Live) {

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

		m_pTextureCom->Render_Textrue(m_iTextureIndex);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}



CMini_Gold* CMini_Gold::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float pPos_X)
{
	CMini_Gold* pInstance = new CMini_Gold(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPos_X)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MINIGame_Gold Create Fail");
		return nullptr;
	}

	return pInstance;
}

void CMini_Gold::Free()
{
	__super::Free();
}
