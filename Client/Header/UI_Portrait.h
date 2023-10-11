#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CPortrait : public Engine::CGameObject
{
private:
	explicit			 CPortrait(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CPortrait(const CPortrait& rhs);
	virtual				~CPortrait();

public:
	virtual HRESULT						Ready_GameObject() override;
	virtual _int						Update_GameObject(const _float& fTimeDelta) override;
	virtual void						LateUpdate_GameObject() override;
	virtual void						Render_GameObject() override;


public:
	PORTRAITTAG							Get_PortraitTag() { return m_ePortrait; }
	void								Set_PortraitTag(PORTRAITTAG _ePortraitTag)
										{
											if (m_ePortrait != _ePortraitTag)
											{
												m_ePortrait = _ePortraitTag;
												SetPortrait();
											}
										}

private:
	void								SetPortrait();


private:
	HRESULT								Add_Component();

	//	enum class PORTRAITTAG {PORT_DES, PORT_DESNEON, PORT_HALEY, PORT_DEEPCOAT, PORT_BAUSS, PORT_TURNCOAT};

private:
	CUITex*								m_pBufferCom = nullptr;
	CTexture*							m_pTextureCom = nullptr;
	CTransform*							m_pTransformCom = nullptr;

	PORTRAITTAG							m_ePortrait;

private:
	_float								m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix								m_ViewMatrix, m_ProjMatrix;


public:
	static CPortrait*					Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void						Free();


};

