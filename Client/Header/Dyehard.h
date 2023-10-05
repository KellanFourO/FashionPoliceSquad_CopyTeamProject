#pragma once

#include "Base.h"

#include "Gun.h"

class CShotGunState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CDyehard : public CGun
{
private:
	explicit		CDyehard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit		CDyehard(const CDyehard& rhs);
	virtual			~CDyehard();

	enum SHOTGUNSTATE { IDLE, WALK, JUMP, READY, SHOT, RELOAD, LAZER, ENTER, SHOTGUNSTATE_END};

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

public:
	CShotGunState* Get_State(_int _index) { return m_pStateArray[_index]; }

private:
	HRESULT			Add_Component();
	void			Mouse_Input();
	void			Key_Input();
	void			ReadyState();



public:
	CRcTex*			m_pBufferCom		= nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	//CTransform*		m_pTransformCom		= nullptr; // 이동 컴포넌트
	CTexture*		m_pTextureCom		= nullptr; // 텍스쳐 컴포넌트

	_int			m_iBulletColor		= 0;

	_bool			m_bLazer = false;

	vector<CGameObject*> BulletList;

	CShotGunState*	m_pStateArray[SHOTGUNSTATE_END];

public:
	void			Gun_Fire();

public:
	_vec3			m_vDir;
	_vec3			m_vStartDir;
	_bool			m_bLateInit = true;
	CTransform*		m_pPlayerTransformCom = nullptr;

public:
	void			Set_Player(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }

public:
	static CDyehard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free();
};

