#include "stdafx.h"
#include "UI_RecognitionRange.h"

#include "Export_Utility.h"
#include "Export_System.h"
#include "Engine_Enum.h"
#include "Monster.h"


#include <DirectXMath.h>

CRecognitionRange::CRecognitionRange(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CRecognitionRange::CRecognitionRange(const CRecognitionRange& rhs)
	: CGameObject(rhs)
{
}

CRecognitionRange::~CRecognitionRange()
{
}

HRESULT Engine::CRecognitionRange::Ready_GameObject()
{
 	D3DXMatrixIdentity(&m_matView);
 	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f,2.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vTargetPos = {0.f,0.f,0.f};

	TextureSet();

	switch (m_eUIType)
	{
	case Engine::UI_TYPE::DESTINATION:
		m_pTransformCom->Set_Scale(_vec3(32.f,32.f,1.f));
		break;
	case Engine::UI_TYPE::TARGETPOS:
		m_pTransformCom->Set_Scale(_vec3(32.f, 32.f, 1.f));
		break;
	case Engine::UI_TYPE::RECOG:
		m_pTransformCom->Set_Scale(_vec3(32.f, 32.f, 1.f));
		break;
	}
	return S_OK;
}

Engine::_int Engine::CRecognitionRange::Update_GameObject(const _float& fTimeDelta)
{
	//	TODO - ������ ���°��� �����´ٰ� �����ѻ�Ȳ ( �߰�, �̹߰�, ���� )

	UI_Translate();

	Engine::Add_RenderGroup(RENDER_UI, this);

	_int iExit = __super::Update_GameObject(fTimeDelta);

	return 0;
}

void Engine::CRecognitionRange::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CRecognitionRange::Render_GameObject()
{
	switch (m_eUIType)
	{
		case Engine::UI_TYPE::DESTINATION:
		{
			if (m_bTargetPos)
			{
				destinationUI();
			}
			else
				MSG_BOX("�� ������UI Ÿ�ټ��� ���߾�.");
			break;
		}

		case Engine::UI_TYPE::TARGETPOS:
		{
			TargetUI();
			break;
		}

		case Engine::UI_TYPE::RECOG:
		{
			RecogUI();
			break;
		}
	}
}

HRESULT Engine::CRecognitionRange::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_RecogUITexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CRecognitionRange::UI_Translate()
{
}

void CRecognitionRange::destinationUI()
{
	_matrix matWorld, matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vWindowPos = m_vTargetPos;

	vWindowPos.y += 5.f;

	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matView); // ���� ��,���� ��ķ� ��
	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matProj);	// ���� ��,���� ��ķ� ��

	vWindowPos.x *= WINCX * 0.5f;
	vWindowPos.y *= WINCY * 0.5f;

	m_pTransformCom->Set_Pos(vWindowPos);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue(5); // ��ǥ���� UI �ε���
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRecognitionRange::RecogUI()
{
	_matrix matWorld, matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vWindowPos = m_vTargetPos;

	vWindowPos.y += 5.f;

	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matView); // ���� ��,���� ��ķ� ��
	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matProj);	// ���� ��,���� ��ķ� ��

	vWindowPos.x *= WINCX * 0.5f;
	vWindowPos.y *= WINCY * 0.5f;

	m_pTransformCom->Set_Pos(vWindowPos);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	if (static_cast<CMonster*>(m_pTarget)->Detect())
	{
		m_pTextureCom->Render_Textrue(0); // �߰�
	}
	else
	{
		m_pTextureCom->Render_Textrue(1); // �� �߰�
	}


	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRecognitionRange::TargetUI()
{
	_matrix matWorld, matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vWindowPos = m_pTarget->Get_Transform()->m_vInfo[INFO_POS];

	vWindowPos.y += 5.f;

	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matView); // ���� ��,���� ��ķ� ��
	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matProj);	// ���� ��,���� ��ķ� ��

	vWindowPos.x *= WINCX * 0.5f;
	vWindowPos.y *= WINCY * 0.5f;

	m_pTransformCom->Set_Pos(vWindowPos);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue(4); //todo index = 4 ���� ��ġ ǥ�� UI
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRecognitionRange::TextureSet()
{
																													//todo index = 0 Idle�����̰� �÷��̾ �߰��� ����
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/Sleep.png", 1);			//todo index = 1 Idle�����̰� �÷��̾ �߰����� ���� ����
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/Match.png", 1);			//todo index = 2 ��ƮŸ�԰� �Ѿ�Ÿ���� ��ġ
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/NoMatch.png", 1);		//todo index = 3 ��ƮŸ�԰� �Ѿ�Ÿ���� ����ġ
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/ObjectiveIcon1.png", 1); //todo index = 4 ���� ��ġ ǥ�� UI
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/Destination.png", 1);	//todo index = 5 ���� ��ġ ǥ�� UI
}


CRecognitionRange* CRecognitionRange::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget, UI_TYPE eUIType)
{
	CRecognitionRange* pInstance = new CRecognitionRange(pGraphicDev);

	pInstance->m_pTarget = pTarget;
	pInstance->m_eUIType = eUIType;

	pInstance->Set_Target(pTarget);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("RecogUI Create Failed");
		return nullptr;
	}



	return pInstance;
}

void Engine::CRecognitionRange::Free()
{
	__super::Free();
}
