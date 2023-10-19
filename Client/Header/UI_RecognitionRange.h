#pragma once
#include "GameObject.h"
#include "Frustum.h"

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;


class CRecognitionRange : public Engine::CGameObject
{
private:
	explicit CRecognitionRange(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRecognitionRange(const CRecognitionRange& rhs);
	virtual ~CRecognitionRange();

public:
	virtual void	Render_GameObject() override;
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;

public:

	//CTexture*							Get_Texture() { return m_pTextureCom; }
public:
	_bool					Get_bTargetPos() { return m_bTargetPos;}

	void					Set_Target(CGameObject* pTarget) { m_pTarget = pTarget;}
	void					Set_TargetPos(const _vec3& vPos) {m_vTargetPos = vPos; m_bTargetPos = true;	}
	void					Set_RecogTextureIndex(_int _iIndex) { m_iRecogTextureIndex = _iIndex;}

private:
	HRESULT					Add_Component();
	void					TextureSet();

	void					DestinationUI(const _float& fTimeDelta);
	void					DestinationSceneFunction();
	void					RecogUI(const _float& fTimeDelta);
	void					TargetUI(const _float& fTimeDelta);

	void					DestinationRender();
	void					RecogRender();
	void					TargetRender();

	void					DistanceRender();
	void					AngleRender();




private:
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pPlayerTransformCom = nullptr;
	CUITex*				m_pBufferCom = nullptr;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float				m_fRenderDistance = 150.f;

	_float				m_fRecogAge = 0.f;
	_float				m_fRecogLifeTime = 3.f;

	_float				m_fRecogHitTypeAge = 0.f;
	_float				m_fRecogHitTypeLifeTime = 3.f;
	_int				m_iRecogTextureIndex = 0.f;


	_float				m_fDestinationDistance;
	_vec2				m_vDestinationTextPos;

	_matrix				m_matProj, m_matView;


	CGameObject*		m_pTarget;
	CFrustum*			m_pFrustum = nullptr;

	_vec3				m_vTargetPos;


	UI_TYPE				m_eUIType;

	_bool				m_bDistanceRender = false; // 플레이어와 타겟의 거리로 렌더링되게출력
	_bool				m_bAngleRender = true; //todo 임시로 true 나중에 false로 바꾼후 작업해야함 플레이어와 타겟의 각도가 양의 방향인 경우에만?
	_bool				m_bTargetPos = false;

	_bool				m_bRecogDetect = false;
	_bool				m_bDestinationSwitch = true;




public:
	static CRecognitionRange* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget = nullptr, UI_TYPE eUIType = UI_TYPE::UI_TYPE_END);
								
private:
	virtual void Free() override;
};

END

