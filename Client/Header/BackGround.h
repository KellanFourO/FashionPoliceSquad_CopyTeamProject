#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CBackGround : public Engine::CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	HRESULT					Add_Component();

private:
	CRcTex* m_pBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CTexture* m_pBackTextureCom = nullptr;

private:
	_float		m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix		m_ViewMatrix, m_ProjMatrix;

public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();


};

