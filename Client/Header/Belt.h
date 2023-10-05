#pragma once

#include "Base.h"
#include "GameObject.h"

class CBeltState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CBelt : public Engine::CGameObject
{
private:
	explicit CBelt(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBelt(const CBelt& rhs);
	virtual ~CBelt();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

public:
	CRcTex* m_pBufferCom = nullptr; // �ؽ�ó�� �׸������� ���� ������Ʈ
	CTransform* m_pTransformCom = nullptr; // �̵� ������Ʈ
	CTexture* m_pTextureCom = nullptr; // �ؽ��� ������Ʈ
	CTexture* m_pTextureCom_2 = nullptr; // ����
	CTexture* m_pTextureCom_3 = nullptr; // ����

private:
	HRESULT Add_Component();
	void Mouse_Input(const _float& fTimeDelta);
	void Key_Input();

public:
	_float m_fBeltMoveRight = 0.f;
	_float m_fBeltMoveDown = 0.f;; //��ġ �̵�
	_float m_fHitTime = 0.f;;

	_vec3	m_vBeltScale = { 0.f,0.f,0.f };
	_vec3	m_vPlayerLook = { 0.f,0.f,0.f };

	CBeltState* BeltState = nullptr;	// �� ����

	_bool			m_bCharged = false;

	_bool			m_bHit = false;

	_bool			m_bFix = false;
	_bool			m_bCheck = false;

public:
	static CBelt* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

