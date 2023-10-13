#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;

class CTexture;
class CTransform;
class CCalculator;
class CCollider;

END


class CTrigger :  public CGameObject
{
private:
	explicit CTrigger(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrigger(const CTrigger& rhs);
	virtual	 ~CTrigger();

public:
	virtual HRESULT Ready_GameObject(_vec3 pMouse_Pos, _vec3 Size);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component();

public:
	static  CTrigger* Create(LPDIRECT3DDEVICE9 pGraphicDev,	_vec3 pMouse_Pos, 
		_vec3 Size, TRIGGER_CASE eCase, TRIGGER_TYPE eType, TRIGGER_NUMBER eTrName);

private:
	virtual void Free();

};

