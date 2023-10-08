#pragma once

#include "Base.h"
#include "Bullet.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRigidBody;

END

class CRifle_Bullet1 : public CBullet
{
private:
	explicit CRifle_Bullet1(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRifle_Bullet1(const CRifle_Bullet1& rhs);
	virtual ~CRifle_Bullet1();

public:
	virtual	HRESULT		Ready_GameObject(_vec3 _StartPos, _int iColorIndex);
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;


public:
	HRESULT				Add_Component();

private:
	CRcTex*				m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTexture*			m_pTextureCom = nullptr; // 텍스쳐 컴포넌트

private:
	_vec3			m_vDir, m_vPos;

	_float			m_fLiveTime;
	_vec3			m_vStartPos;

	D3DXCOLOR		m_FilterColor; // 필터 색상
	_float			m_fBulletMoveRight, m_fBulletMoveDown, m_fBulletSpeed; // 랜덤으로 생성좌표를 이동시키기 위한 변수




public:
	static CRifle_Bullet1* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _StartPos, _int iColorIndex);

private:
	virtual void Free()	override;
};

