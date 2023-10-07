#pragma once
#include "GameObject.h"
#include "MyUI.h"
#include "PlayerGun.h"

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;


class CMyUI : public Engine::CGameObject
{

private:
	explicit CMyUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMyUI(const CMyUI& rhs);
	virtual ~CMyUI();

public:
	virtual void	Render_GameObject() override;
	virtual HRESULT Ready_GameObject(IDirect3DBaseTexture9* pTexture);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;

public:
	void SetData(_vec3& vSize, _vec3& vPos)
	{
		m_pInfo->vSize = vSize;
		m_pInfo->vPos = vPos;
	}

	UIDATA*					Get_Info() { return m_pInfo; }
	CTexture*				Get_Texture() { return m_pTextureCom; }

	CMyUI*					Get_Clone()
	{
		CMyUI* pClone = new CMyUI(*this);
		pClone->m_bClone = true;

		if (FAILED(pClone->Ready_GameObject(pClone->Get_Info()->pTexture)))
		{
			MSG_BOX("MyUI 클론 실패");
			Safe_Release(pClone);
		}
		return pClone;
	}

public:

	void					Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	void					Set_Pos(const _vec3& vPos) { m_fX = vPos.x; m_fY = vPos.y; };
	void					Set_Pos2(const _vec3& vPos) { m_pInfo->vPos = vPos; }
	void					Set_Clone(_bool bClone) { m_bClone = bClone; }
	void					Set_CamView(_matrix camView) { m_matCamView = camView; }
	void					Set_CamProj(_matrix camProj) { m_matCamProj = camProj; }

	void					Set_ScaleRatio(_float _fRatio)
	{
		m_pInfo->vSize.x = m_pInfo->vSize.x * _fRatio;
		m_pInfo->vSize.y = m_pInfo->vSize.y * _fRatio;
	}

private:
	HRESULT					Add_Component();

	void					HP_Function(const _float& fTimeDelta);
	void					Sprite_Function(const _float& fTimeDelta);
	void					Font_Function(const _float& fTimeDelta);
	void					Bullet_Function();

	void					NonTargetUI_Function();
	void					TargetUI_Function();


private:
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CUITex*				m_pBufferCom = nullptr;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix				m_matProj, m_matView, m_matCamView, m_matCamProj;
	_bool				m_bClone = false;
	_bool				m_bLateInit = true;

	UI_TYPE				m_eType;
	UIDATA*				m_pInfo;

	CGameObject*		m_pTarget;

	_int				iCount = 0;
	_int				iTempState = 0;

	//TODO - 승용 Player
	Player_Info* 		m_pPlayerInfo;
	CPlayerGun*			m_pPlayerGun;
	CPlayer*			m_pPlayer;


public:
	static CMyUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, IDirect3DBaseTexture9* pTexture, UI_TYPE eType);

private:
	virtual void Free() override;
};

END

