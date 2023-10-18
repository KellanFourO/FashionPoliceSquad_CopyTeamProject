#include "stdafx.h"
#include "Belt_Rope.h"
#include "BeltState.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBelt_Rope::CBelt_Rope()
{


}

CBelt_Rope::~CBelt_Rope()
{
}
void CBelt_Rope::Initialize(CBelt* Belt)
{
    m_bAttack = false;
    m_pHost = Belt;

    m_pHost->Get_Target()->Get_Transform()->Get_Info(INFO_POS,&m_vTargetPos);
}

CBeltState* CBelt_Rope::Update(CBelt* Belt, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    _vec3 vPlayerPos, vDir, vPlayerLook;
    m_pHost->Get_HostTransform()->Get_Info(INFO_POS, &vPlayerPos);
    m_pHost->Get_HostTransform()->Get_Info(INFO_LOOK, &vPlayerLook);

    m_vPlayerVelocity = vPlayerPos - m_vPrevPlayerPos;

    vDir = m_vTargetPos - vPlayerPos; // 타겟을 바라보는 방향


    _float fLength;
    fLength = D3DXVec3Length(&vDir);

    D3DXVec3Normalize(&vDir, &vDir);
    D3DXVec3Normalize(&vPlayerLook,&vPlayerLook);
    // 방향은 구했고, 각도 구해
    //todo 플레이어의 중력은 RigidBody에서 알아서 해주고 있다.
    //todo 진자가 매달린 끈은 vTargetPos

    _float fAngle = D3DXVec3Dot(&vPlayerLook, &vDir);

    fAngle = acosf(fAngle);
    fAngle = D3DXToDegree(fAngle);

    _float fForce = 1.f * m_pHost->Get_Host()->Get_INFO()->fMoveSpeed;



     m_pHost->Get_Host()->Get_Transform()->Move_Pos(&vDir, fTimeDelta, fLength);

	if (Key_Down(DIK_SPACE))
	{
        _vec3 vForce;
        vForce = vDir * fLength;

        m_pHost->Get_Host()->Get_RigidBody()->Add_Force(vForce);
        return m_pHost->Get_State(0);
    }


//     _vec3 vRopeStartPos = vPlayerPos;
//     _float fRopeLength;
//
//     m_fRopeLength = fLength;
//     m_fRopeAngle = fAngle;
//
//     m_fTime += fTimeDelta;
//
//     vRopeStartPos.x = vPlayerPos.x + m_fRopeLength * sin(m_fRopeAngle + m_fAmplitude * sin(m_fFrequency * m_fTime));
//
//     m_vTargetPos = vRopeStartPos + _vec3(0.0f, -m_fRopeLength, 0.0f);
//
//
//     _float fTensionAccel = CalculateTensionAccel(m_vPlayerVelocity);
//     vPlayerPos = UpdatePlayerPos(vPlayerPos, m_vPlayerVelocity, fTensionAccel, fTimeDelta);
//
//     m_vPrevPlayerPos = vPlayerPos;
//
//     m_pHost->Get_HostTransform()->Set_Pos(vPlayerPos);
//     m_fTime = 0.f;
//
//
//
//
//     if(Key_Down(DIK_SPACE))
//     {
//         _vec3 vAccel = {fTensionAccel,fTensionAccel,fTensionAccel};
//         m_pHost->Get_Host()->Get_RigidBody()->Add_Force(vAccel);
//         return m_pHost->Get_State(0);
//
//     }

    //_float fScala = D3DXVec3Dot(&v)


    // 양의 방향일경우 거리만큼 가속도 ++
    // 음의 방향일경우 장력만큼 가속도 ++
    // 플레이어 속도 기준




    return nullptr;
}

void CBelt_Rope::Release(CBelt* Belt)
{
    m_pHost->Set_Target(nullptr);
    m_fTime = 0.f;
}

_float CBelt_Rope::CalculateTensionAccel(const _vec3 vPlayerVelocity)
{
    _float fTensionMagnitude = m_fRopeLength * 9.8f;

    _vec3 vPlayerDir = vPlayerVelocity;
    D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);

    _vec3 vTensionDir = m_vTargetPos - m_vPrevPlayerPos;
    D3DXVec3Normalize(&vTensionDir, &vTensionDir);

    _float fTensionAccel = fTensionMagnitude / 9.8f;

    if (D3DXVec3Dot(&vPlayerDir, &vTensionDir) < 0.0f)
    {
        fTensionAccel *= -1.0f;
    }

     return fTensionAccel;
}

_vec3 CBelt_Rope::UpdatePlayerPos(const _vec3& _vPlayerPos, const _vec3& _vPlayerVelocity, const _float fTensionAccel, const _float fTimeDelta)
{
	_vec3 vNewPos = _vPlayerPos;

	// 간단한 위치 업데이트 예시
    vNewPos.x += _vPlayerVelocity.x * fTimeDelta;
    vNewPos.y += (_vPlayerVelocity.y - fTensionAccel) * fTimeDelta;
    vNewPos.z += _vPlayerVelocity.z * fTimeDelta;

	return vNewPos;
}
