#pragma once
#include "BeltState.h"
class CBelt_Rope : public CBeltState
{
public:
	CBelt_Rope();
	virtual ~CBelt_Rope();

public:
	virtual void Initialize(CBelt* _Belt) override;
	virtual CBeltState* Update(CBelt* Belt, const float& fDetltaTime) override;
	virtual void Release(CBelt* _Belt) override;

private:

_float CalculateTensionAccel(const _vec3 vPlayerVelocity);
_vec3 UpdatePlayerPos(const _vec3& _vPlayerPos, const _vec3& _vPlayerVelocity, const _float fTensionAccel, const _float fTimeDelta);

	_bool m_bAttack = false;

	_vec3 m_vTargetPos;
	_vec3 m_vPrevPlayerPos;
	_vec3 m_vPlayerVelocity;

	_float m_fRopeLength = 0.f;
	_float m_fRopeAngle = 0.f;

	_float m_fAmplitude = 1.0f; //진폭
	_float m_fFrequency = 1.0f; //주파수
	_float m_fTime = 0.0f; // 경과시간

};
