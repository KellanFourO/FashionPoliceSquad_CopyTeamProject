#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CBullet_GoldBar : public Engine::CGameObject
{
	enum BULLET_TEXTURE { VER, HOR };

protected:
	explicit CBullet_GoldBar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet_GoldBar(const CBullet_GoldBar& rhs);
	virtual ~CBullet_GoldBar();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;
public:
	HRESULT Add_Component();
	void	Shot(_vec3 _StartPos);

public:
	static CBullet_GoldBar* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vTargetDir);

private:
	CRcTex*			m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTexture*		m_pTextureCom = nullptr; // 텍스쳐 컴포넌트

	_vec3			m_vTargetDir;
	_bool			m_bShot = false;
	_float			m_fTranslateTick = 0;
	BULLET_TEXTURE  m_eTexture;

	_int			m_iTextureIndex = 0;

	_float			m_fLifeTime = 3.f;
	_float			m_fAge;
private:
	virtual void Free();
};

