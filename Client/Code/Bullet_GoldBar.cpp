#include "stdafx.h"
#include "Bullet_GoldBar.h"

#include "Export_System.h"
#include "Export_Utility.h"


CBullet_GoldBar::CBullet_GoldBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CBullet_GoldBar::CBullet_GoldBar(const CBullet_GoldBar& rhs)
	:CBullet(rhs)
{
}

CBullet_GoldBar::~CBullet_GoldBar()
{
}

HRESULT CBullet_GoldBar::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vScale = { 1.5f, 1.5f, 1.5f };
	_vec3 vPos = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).back()->m_pTransformCom->m_vInfo[INFO_POS];
	m_fDmg = 10.f;

	Set_ObjectTag(OBJECTTAG::BOSSBULLET);
// 	CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
//
// 	_vec3 vPlayerPos, vPlayerPos_Rel;
// 	vPlayerPos_Rel = vPlayerPos - vPos;
// 	D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
// 	_float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);
// 	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);

	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Set_Scale(vScale);


	m_pCollider->Set_Host(this);
	m_pTransformCom->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());


	m_pTextureCom->Ready_Texture(TEX_NORMAL, L"../Bin/Resource/Texture/Monster/gold-bar-projectile_1.png", 1);

	return S_OK;
}

Engine::_int CBullet_GoldBar::Update_GameObject(const _float& fTimeDelta)
{
		Engine::Add_RenderGroup(RENDER_ALPHA, this);
		__super::Update_GameObject(fTimeDelta);

		if(m_bDead)
		return OBJ_DEAD;
		//m_pTransformCom->Set_Rotate(ROT_Y, fTimeDelta + D3DX_PI);
		m_pTransformCom->Move_Pos(&m_vTargetDir, fTimeDelta, m_fSpeed);

		_vec3 vPlayerPos, vPlayerPos_Rel;
		m_pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
		vPlayerPos.y = 0.f;
		// 이동 코드


		vPlayerPos_Rel = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
		_float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);
		m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);

		switch (m_eTexture)
		{
		case CBullet_GoldBar::VER:
			m_fTranslateTick += fTimeDelta;

			if (m_fTranslateTick > 0.35f)
			{
				m_iTextureIndex = 1;
				m_eTexture = HOR;
				m_fTranslateTick = 0;
			}
			break;

		case CBullet_GoldBar::HOR:
			m_fTranslateTick += fTimeDelta;

			if(m_fTranslateTick > 0.35f)
			{
				m_iTextureIndex = 0;
				m_eTexture = VER;
				m_fTranslateTick = 0;
			}
			break;

		}




	return OBJ_NOEVENT;
}

void CBullet_GoldBar::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CBullet_GoldBar::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Render_Textrue(m_iTextureIndex);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CBullet_GoldBar::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Stage1Bullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CBullet_GoldBar::Shot(_vec3 _StartPos)
{
	m_pTransformCom->Set_Pos(_StartPos);
}



CBullet_GoldBar* CBullet_GoldBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vTargetDir, _float fSpeed)
{

	CBullet_GoldBar* pInstance = new CBullet_GoldBar(pGraphicDev);
	pInstance->m_vTargetDir = vTargetDir;
	pInstance->m_fSpeed = fSpeed;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Bullet_GoldBar Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CBullet_GoldBar::Free()
{
	SoundMgr()->StopSound(SOUND_BOSS2);
	__super::Free();
}
