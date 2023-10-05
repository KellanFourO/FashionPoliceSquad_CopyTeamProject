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

	CRcTex* m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTransform* m_pTransformCom = nullptr; // 이동 컴포넌트
	CTexture* m_pTextureCom = nullptr; // 텍스쳐 컴포넌트

	HRESULT Add_Component();

public:
	static CDyehard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

