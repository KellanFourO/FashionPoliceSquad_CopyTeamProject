#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;


class CRecognitionRange : public Engine::CGameObject
{
	enum TextureTag { SEARCH, NOSEARCH, MATCH, NOMATCH, STUN, STUNRING, POS, TEXTURETAG_END };

private:
	explicit CRecognitionRange(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRecognitionRange(const CRecognitionRange& rhs);
	virtual ~CRecognitionRange();

public:
	virtual void	Render_GameObject() override;
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;

public:

	//CTexture*							Get_Texture() { return m_pTextureCom; }

public:

	void					Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	void					Set_Pos(const _vec3& vPos) { m_fX = vPos.x; m_fY = vPos.y; };

private:
	HRESULT					Add_Component();

	void					UI_Translate();

	void					Sprite_Function(const _float& fTimeDelta);
	void					Font_Function(const _float& fTimeDelta);

	void					NonTargetUI_Function();
	void					TargetUI_Function();


private:
	CTexture*			m_pFrontTextureCom = nullptr;
	CTexture*			m_pBackTextureCom  = nullptr;



	IDirect3DBaseTexture9* m_pTextureArray[TEXTURETAG_END];

	CTransform*			m_pTransformCom = nullptr;
	CUITex*				m_pBufferCom = nullptr;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix				m_matProj, m_matView, m_matCamView, m_matCamProj;

	CGameObject*		m_pTarget;



public:
	static CRecognitionRange* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget);

private:
	virtual void Free() override;
};

END

