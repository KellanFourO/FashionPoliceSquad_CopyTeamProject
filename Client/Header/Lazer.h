#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRigidBody;

END

class CLazer : public Engine::CGameObject
{
private:
	explicit CLazer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLazer(const CLazer& rhs);
	virtual ~CLazer();

public:
	virtual	HRESULT	Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:
	HRESULT			Add_Component();
	void			Color_Select();
	void			Set_Gun(CGameObject* _pGun) { m_pGun = _pGun; };
	void			Shoot(_vec3 _StartPos, int iColorIndex);
private:
	CRcTex* m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTexture* m_pTextureCom2 = nullptr;
	//CTransform* m_pTransformCom = nullptr; // 이동 컴포넌트
	//CTransform* m_pTransformCom2 = nullptr; // 이동 컴포넌트2
	//CTransform* m_pTransformCom3 = nullptr; // 이동 컴포넌트3
	//CTransform* m_pTransformCom4 = nullptr; // 이동 컴포넌트3

	CTexture* m_pTextureCom = nullptr; // 텍스쳐 컴포넌트

public:
	

private:
	_vec3			m_vDir, m_vPos;

	_float			m_fLiveTime;
	_bool			m_bShooting = false;

	_tchar*			m_pColorTag;
	_int			m_iColorIndex;

	_int			m_SpinAngle2 = 0.f;
	_int			m_SpinAngle3 = 0.f;

	_vec3			m_vStartPos;

	_float			m_fAnimateTime,m_fFrame,m_fTestTime;
	_bool			CollideMob = false;

	_matrix			m_Mat_Axis;

private:
	CGameObject* m_pGun; // 발사 확인을 위한 타겟

public:
	static CLazer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free()	override;
};

