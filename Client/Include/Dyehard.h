#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CDyehard : public Engine::CGameObject
{
private:
	explicit CDyehard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDyehard(const CDyehard& rhs);
	virtual ~CDyehard();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

	CRcTex* m_pBufferCom = nullptr; // �ؽ�ó�� �׸������� ���� ������Ʈ
	CTransform* m_pTransformCom = nullptr; // �̵� ������Ʈ
	CTexture* m_pTextureCom = nullptr; // �ؽ��� ������Ʈ

	HRESULT Add_Component();

public:
	static CDyehard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

