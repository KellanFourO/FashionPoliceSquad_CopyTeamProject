#pragma once

#include "Base.h"
#include "Bullet.h"
#include "Engine_Define.h"
BEGIN(Engine)

class CSYTex;
class CTexture;
class CTransform;
class CRigidBody;

END

class CShotGunBullet : public CBullet
{
private:
	explicit CShotGunBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShotGunBullet(const CShotGunBullet& rhs);
	virtual ~CShotGunBullet();

public:
	virtual	HRESULT		Ready_GameObject(_vec3 _StartPos, _int iColorIndex,COLORTAG pColorTag);
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

	void				Set_Color(_int iColor) { m_iColorIndex = iColor;}
	_int				Get_Color() {return m_iColorIndex; }
	void				Set_ColorTag(COLORTAG pColorTag) { m_pColorTag = pColorTag; }
	COLORTAG			Get_ColorTag() { return m_pColorTag; }
public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	HRESULT			Add_Component();

private:
	CSYTex*			m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTexture*		m_pTextureCom = nullptr; // 텍스쳐 컴포넌트
	CTexture*		m_pPaintBulletTrace = nullptr;//총알 탄흔 컴포넌트
private:
	_vec3			m_vDir, m_vPos;

	_float			m_fLiveTime;
	_bool			m_bShooting = false;

	COLORTAG		m_pColorTag;
	_int			m_iColorIndex;
	_vec3			m_vShotDir;
	_vec3			m_vStartPos;

	D3DXCOLOR		m_FilterColor; // 필터 색상
	_float			m_fBulletMoveRight, m_fBulletMoveDown, m_fBulletSpeed; // 랜덤으로 생성좌표를 이동시키기 위한 변수




public:
	static CShotGunBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 _StartPos,_vec3 _vShotDir, _int iColorIndex,COLORTAG pColorTag);

private:
	virtual void Free()	override;
};

