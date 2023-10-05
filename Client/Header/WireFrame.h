#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CWireTex;
class CTexture;
class CTransform;

END

class CWireFrame : public Engine::CGameObject
{
private:
	explicit CWireFrame(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWireFrame(const CWireFrame& rhs);
	virtual ~CWireFrame();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	HRESULT			Add_Component();

private:
	CWireTex*		m_pBufferCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;

public:
	static CWireFrame* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

