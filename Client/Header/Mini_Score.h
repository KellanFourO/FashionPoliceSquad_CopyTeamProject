#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CMini_Score :   public CGameObject
{
private:
	explicit			 CMini_Score(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMini_Score(const CMini_Score& rhs);
	virtual				~CMini_Score();

public:
	virtual HRESULT		Ready_GameObject(float pPos_X, int Count);
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

private:
	HRESULT				Add_Component();

public:
	void				Set_TexNum(float m_Tex);

private:
	CUITex* m_pBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

	_int					m_iTextureIndex = 12;
	_int					m_STDCount = 0; //�ڸ��� ���� 0 -������ 1-10���ڸ� 2 - 1���ڸ�
	_int					m_iCountNumber = 0; //���� �ڸ����� ��� ��

	_vec3					m_PlayerPos;
	_vec3					m_PlayerScale;
	_float					fSpeed = 1.f;

private:
	_matrix					m_ViewMatrix, m_ProjMatrix;

public:
	static CMini_Score* Create(LPDIRECT3DDEVICE9 pGraphicDev, float pPos_X, int Count);
private:
	virtual void		 Free();


};

