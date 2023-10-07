#pragma once
#include "MonsterState.h"
#include "KickBoardMonster.h"
class CKickBoardMonsterState : public CMonsterState
{
public:
	CKickBoardMonsterState() {};
	virtual ~CKickBoardMonsterState() {};

public:
	virtual void Initialize(CMonster* _Monster) {};
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) { return nullptr; };
	virtual void LateUpdate(CMonster* _Monster) {};
	virtual void Release(CMonster* _Monster) {};
	virtual void Render(CMonster* _Monster) {};

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
		vRel = vPlayerPos - Monster->Get_Info().vPos; //�÷��̾�� ������ ���� ����
		D3DXVec3Normalize(&vRel, &vRel);
		_float fPlayerAngle = atan2f(vRel.x, vRel.z);
		return D3DXToDegree(fPlayerAngle);
	}
};