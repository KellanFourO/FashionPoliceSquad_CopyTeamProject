#pragma once
#include "MonsterState.h"
#include "KickBoard.h"
class CKickBoardState : public CMonsterState
{
public:
	CKickBoardState() {};
	virtual ~CKickBoardState() {};

public:
	virtual void Initialize(CMonster* _Monster) {};
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) { return nullptr; };
	virtual void Release(CMonster* _Monster) {};

	_vec3		m_vDir = { 0.f,0.f,1.f };

	_vec3		 Turn_Dir(_float _YAngle) {
		_vec3	vResult;
		_matrix YMatrix;
		D3DXMatrixRotationY(&YMatrix ,D3DXToRadian(_YAngle));
		D3DXVec3TransformNormal(&vResult, &vResult, &YMatrix);
		return vResult;
		};

	_float		Get_PlayerAngle(CMonster* Monster) {
		CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
		_vec3	vPlayerPos, vRel;
		pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
		vRel = vPlayerPos - Monster->Get_Info().vPos; //플레이어와 몬스터의 각도 구함
		D3DXVec3Normalize(&vRel, &vRel);
		_float fPlayerAngle = atan2f(vRel.x, vRel.z);
		return D3DXToDegree(fPlayerAngle);
	}
};