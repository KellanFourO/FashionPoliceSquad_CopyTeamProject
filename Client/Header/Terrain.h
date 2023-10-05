#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;

END

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	HRESULT					Add_Component();

private:
	CTerrainTex* m_pBufferCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();


};

