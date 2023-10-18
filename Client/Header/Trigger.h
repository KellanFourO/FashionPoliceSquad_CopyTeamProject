

#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTransform;
class CCalculator;
class CCollider;

END


class CTrigger : public CGameObject
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

	void			Set_TR_CASE(TRIGGER_CASE eCase) { m_eTRCase = eCase; }
	void			Set_TR_TYPE(TRIGGER_TYPE eType) { m_eTRType = eType; }
	void			Set_TR_NUMBER(TRIGGER_NUMBER eName) { m_eTrName = eName; }
	void			Set_TR_STATE(TRIGGER_STATE eState) { m_eTrState = eState; }
	void			Set_IndexNumber(_uint iIndex) { m_iIndex = iIndex; }

	TRIGGER_CASE	Get_TR_CASE() { return m_eTRCase; } //한번두번
	TRIGGER_TYPE	Get_TR_TYPE() { return m_eTRType; } //무빙 UI 등
	TRIGGER_NUMBER	Get_TR_NUMBER() { return m_eTrName; } //고유값   <중요>
	TRIGGER_STATE	Get_TR_STATE() { return m_eTrState; } //전, 진행, 후   <중요>

protected:
	virtual HRESULT	Add_Component();


private:
	SCENETAG			m_NowScene = SCENETAG::SCENETAG_END;

	CCubeTex*			m_pBufferCubeCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;

	_uint				m_iIndex;
	TRIGGER_CASE		m_eTRCase = TRIGGER_CASE::TR_CASE_END;
	TRIGGER_TYPE		m_eTRType = TRIGGER_TYPE::TR_TYPE_END;
	TRIGGER_NUMBER		m_eTrName = TRIGGER_NUMBER::TR_END;
	TRIGGER_STATE		m_eTrState = TRIGGER_STATE::TR_BEFORE;

public:
	static  CTrigger* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pMouse_Pos, _uint iIndex,
		_vec3 Size, TRIGGER_CASE eCase, TRIGGER_TYPE eType, TRIGGER_NUMBER eTrName);

private:
	virtual void Free();

};

