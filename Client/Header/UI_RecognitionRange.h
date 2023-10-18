#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;


class CRecognitionRange : public Engine::CGameObject
{
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
	_bool					Get_bTargetPos() { return m_bTargetPos;}

	void					Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; m_bTargetPos = true; }
	void					Set_TargetPos(const _vec3& vPos) { m_vTargetPos = vPos;}


private:
	HRESULT					Add_Component();

	void					UI_Translate();

	void					destinationUI();
	void					RecogUI();
	void					TargetUI();
	void					TextureSet();


private:
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CUITex*				m_pBufferCom = nullptr;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix				m_matProj, m_matView, m_matCamView, m_matCamProj;

	CGameObject*		m_pTarget;

	_vec3				m_vTargetPos;
	UI_TYPE				m_eUIType;

	_bool				m_bTargetPos = false;

public:
	static CRecognitionRange* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget = nullptr, UI_TYPE eUIType = UI_TYPE::UI_TYPE_END);

private:
	virtual void Free() override;
};

END

