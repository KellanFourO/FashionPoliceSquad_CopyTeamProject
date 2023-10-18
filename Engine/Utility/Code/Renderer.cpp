#include "Renderer.h"
#include "Frustum.h"
#include "../Client/Header/NewFPSCamera.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eType, CGameObject* pGameObject)
{
	if (RENDER_END <= eType || pGameObject == nullptr)
		return;

	m_RenderGroup[eType].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Render_Priority(pGraphicDev);
	if (Management()->Get_Scene()->Get_ReadyCube())
	{
		Render_Block(pGraphicDev);
	}
	Render_Alphatest(pGraphicDev);
	Render_NonAlpha(pGraphicDev);
	Render_Alpha(pGraphicDev);

	Render_UI(pGraphicDev);
	Clear_RenderGroup();
}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_GameObject();
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
/*	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);

	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_GameObject();
// 	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_RenderGroup[RENDER_ALPHA].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		});

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_GameObject();


	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{


	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_GameObject();


}

void CRenderer::Render_Block(LPDIRECT3DDEVICE9& pGraphicDev)
{
	if (Management()->Get_Scene()->Get_SceneTag() == SCENETAG::STAGE)
	{
// 		if (!Management()->Get_Layer(LAYERTAG::CAMERA)->Get_ObjectList(OBJECTTAG::FPSCAMERA).size()) return;
// 		CNewFPSCamera* pNEWFPSCamera = dynamic_cast<CNewFPSCamera*>(Engine::Management()->Get_Layer(LAYERTAG::CAMERA)->Get_ObjectList(OBJECTTAG::FPSCAMERA).back());
//
// 		//TODO - �¿��߰�
//
// 		_matrix matView = pNEWFPSCamera->Get_View();
// 		_matrix matProj = pNEWFPSCamera->Get_Proj();
//
// 		_matrix matViewProj = matView * matProj;
// 		CFrustum* m_pFrustum = new CFrustum(); // �����ڵ�
//
// 		m_pFrustum->MakeFrustum(&matViewProj);
//
// 		for (auto& iter : m_RenderGroup[RENDER_BLOCK])
// 		{
// 			if (m_pFrustum->IsIn(iter->Get_Transform()->m_vInfo[INFO_POS]))
// 			{
// 				iter->Render_GameObject();
// 			}
// 		}
// 		Safe_Release(m_pFrustum);
		//TODO - �¿��߰� ����
		//pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}


	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); //�Ʒ����̶� �����ٲ�
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 50); //50 �մ������ÿ� - ����
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		for (auto& iter : m_RenderGroup[RENDER_BLOCK])
		{
			iter->Render_GameObject();
		}
	//pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CRenderer::Render_Alphatest(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);

	for (auto& iter : m_RenderGroup[RENDER_ALPHATEST])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CRenderer::Free()
{
	Clear_RenderGroup();
}
