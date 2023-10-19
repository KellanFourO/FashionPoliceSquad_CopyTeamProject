
#include "stdafx.h"
#include "Mini_Enemy.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMini_Enemy::CMini_Enemy(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMini_Enemy::CMini_Enemy(const CMini_Enemy& rhs)
	: Engine::CGameObject(rhs)
{
}

CMini_Enemy::~CMini_Enemy()
{
}



HRESULT CMini_Enemy::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Mini_Enemy_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}


HRESULT CMini_Enemy::Ready_GameObject(_float pPos_X, int TexNum)
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EnemyState = EnemyState::Live;

	Set_Texture(TexNum);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;
	m_ENEMYSize = 35.f;

	vPos = { pPos_X, 440.f, 0.f };
	vScale = { m_ENEMYSize * fMultiply, (m_ENEMYSize + 10.f) * fMultiply, 1.f };

	//vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_ENEMYPos = vPos;
	m_ENEMYScale = vScale;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	// -1 ~ 1 -> 0 ~ 2

	return S_OK;
}

_int CMini_Enemy::Update_GameObject(const _float& fTimeDelta)
{
	if (m_EnemyState == EnemyState::Live) {

		Engine::Add_RenderGroup(RENDER_UI, this);
		int iExit = __super::Update_GameObject(fTimeDelta);
	}
	return 0;
}

void CMini_Enemy::LateUpdate_GameObject()
{
	if (m_EnemyState == EnemyState::Live) {

		__super::LateUpdate_GameObject();

		m_DeadCount++;
		if (m_DeadCount > 399)
		{
			m_EnemyState = EnemyState::Dead;
		}
	}
}

void CMini_Enemy::Render_GameObject()
{
	if (m_EnemyState == EnemyState::Live) {
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



CMini_Enemy* CMini_Enemy::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float pPos_X, int TexNum)
{
	CMini_Enemy* pInstance = new CMini_Enemy(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPos_X, TexNum)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MINIGame_Enemy Create Fail");
		return nullptr;
	}

	return pInstance;
}

void CMini_Enemy::Free()
{
	__super::Free();
}
