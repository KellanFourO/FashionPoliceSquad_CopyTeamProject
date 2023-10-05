#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Gun.h"

class CGunState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CRifle : public CGun
{
private:
	explicit CRifle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRifle(const CRifle& rhs);
	virtual ~CRifle();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	CRcTex* m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTransform* m_pTransformCom = nullptr; // 이동 컴포넌트
	CTexture* m_pTextureCom = nullptr; // 텍스쳐 컴포넌트


private:
	HRESULT Add_Component();
	void Mouse_Input();
	void Key_Input();

private:
	vector<CGameObject*> BulletList;

	_int				iBulletNum;

public:
	void Gun_Fire();


public:
	void Set_Player(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }


public:
	static CRifle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

