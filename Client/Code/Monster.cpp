

#include "stdafx.h"
#include "Monster.h"
#include "BrifCase.h"
#include "Player.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "MonsterState.h"

// TODO - 승용 추가 : 몬스터 HP UI.
#include "UIMgr.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CMonster::CMonster(CMonster& rhs)
	: Engine::CGameObject(rhs)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
	Set_ObjectTag(OBJECTTAG::MONSTER);

	// TODO - 승용 추가
	//if (Set_HP() == E_FAIL)
	//{
	//	MSG_BOX("승용 몬스터 HP 에러");
	//}
	// TODO - 승용 추가 종료



	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{

	if (INFO.bHit == true) {
		m_fHitTime += fTimeDelta;
	}
	if (m_fHitTime >= 0.1f)
	{
		m_fHitTime = 0.f;
		INFO.bHit = false;
	}

	//TODO Ready 단계에서 얻어올 수 없어서 불변수 통제로 단 한번만 수행하는 함수
	Init_PlayerTransform();

	if (m_pPlayerTransform && m_bBillBoard)
	{
		BillBoard();
	}

	StateMachine(fTimeDelta);

	//m_pUI_HPFrame->Update_GameObject(fTimeDelta);
	//m_pUI_HPValue->Update_GameObject(fTimeDelta);
	m_pCollider->SetCenterPos(m_pTransformCom->m_vInfo[INFO_POS]);
	m_pRigidBody->Update_RigidBody(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CMonster::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	//m_pUI_HPFrame->LateUpdate_GameObject();
	//m_pUI_HPValue->LateUpdate_GameObject();
}

void CMonster::Render_GameObject()
{
}

void CMonster::ReadyState()
{
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE)
	{
		_vec3	vOtherPos = _pOther->GetCenterPos();
		_float* fOtherAxis = _pOther->GetAxisLen();

		_vec3	vThisPos = m_pCollider->GetCenterPos();
		_float* fThisAxis = m_pCollider->GetAxisLen();


		// OBJECTTAG에 따른 예외 처리 가능성
		_float fWidth = fabs(vOtherPos.x - vThisPos.x);
		_float fHeight = fabs(vOtherPos.y - vThisPos.y);
		_float fDepth = fabs(vOtherPos.z - vThisPos.z);

		_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
		_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
		_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

		_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

		if (fRadiusY == fMinAxis)
		{

			if (vOtherPos.y < vThisPos.y)
			{
				if (m_pRigidBody)
					m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));

				m_pTransformCom->Translate(_vec3(0.f, fRadiusY - 0.000001f, 0.f));

				//			if (!m_bDieEffect)
				//				m_bDieEffect = true;
			}
			else
			{
				m_pTransformCom->Translate(_vec3(0.f, -fRadiusY, 0.f));
				//m_bDieEffect = false;
			}

		}
		else if (fRadiusZ == fMinAxis)
		{
			if (vOtherPos.z < vThisPos.z)
				m_pTransformCom->Translate(_vec3(0.f, 0.f, fRadiusZ));
			else
				m_pTransformCom->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else if (fRadiusX == fMinAxis)
		{
			if (vOtherPos.x < vThisPos.x)
				m_pTransformCom->Translate(_vec3(fRadiusX, 0.f, 0.f));
			else
				m_pTransformCom->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
		// 충돌 밀어내기 후 이벤트 여기다가 구현 ㄱㄱ ! .

		if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
		{
			cout << "워리어 공격" << endl;
		}
	}
}

void CMonster::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE) {
		_vec3	vOtherPos = _pOther->GetCenterPos();
		_float* fOtherAxis = _pOther->GetAxisLen();

		_vec3	vThisPos = m_pCollider->GetCenterPos();
		_float* fThisAxis = m_pCollider->GetAxisLen();

		// OBJECTTAG에 따른 예외 처리 가능성
		_float fWidth = fabs(vOtherPos.x - vThisPos.x);
		_float fHeight = fabs(vOtherPos.y - vThisPos.y);
		_float fDepth = fabs(vOtherPos.z - vThisPos.z);

		_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
		_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
		_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

		_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

		if (fRadiusY == fMinAxis)
		{
			if (vOtherPos.y < vThisPos.y)
			{
				if (m_pRigidBody)
					m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));

				m_pTransformCom->Translate(_vec3(0.f, fRadiusY, 0.f));
				//m_bDieEffect = true;
			}
			else
			{
				m_pTransformCom->Translate(_vec3(0.f, -fRadiusY, 0.f));
				//m_bDieEffect = false;
			}
		}
		else if (fRadiusZ == fMinAxis)
		{
			if (vOtherPos.z < vThisPos.z)
				m_pTransformCom->Translate(_vec3(0.f, 0.f, fRadiusZ));
			else
				m_pTransformCom->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else if (fRadiusX == fMinAxis)
		{
			if (vOtherPos.x < vThisPos.x)
				m_pTransformCom->Translate(_vec3(fRadiusX, 0.f, 0.f));
			else
				m_pTransformCom->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
	}
}

void CMonster::OnCollisionExit(CCollider* _pOther)
{
}

void CMonster::Init_PlayerTransform()
{
	if (m_bLateInit)
	{
		m_pPlayerTransform = dynamic_cast<CTransform*>(Management()->Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
		m_bLateInit = false;
	}
}

void CMonster::BillBoard()
{
	_vec3 vPlayerPos, vPlayerPos_Rel;
	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	vPlayerPos.y = 0.f;
	// 이동 코드

	m_pTransformCom->Get_Info(INFO_POS, &INFO.vPos);
	vPlayerPos_Rel = vPlayerPos - INFO.vPos;
	D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
	_float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);
	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);
	//방향전환 코드 (플레이어 방향)
}

_bool CMonster::Detect()
{
	_vec3 vPlayerPos, vMonsterPos;

	m_pPlayerTransform->Get_Info(INFO_POS,&vPlayerPos);
	//m_pTransformCom->Get_Info(INFO_POS,&vMonsterPos);

	_float fDistance = D3DXVec3Length(&(vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS]));

	if (fDistance < m_fDectedRange)
	{
		return true;
	}

	return false;
}

_bool CMonster::ChaseCatch()
{
	_vec3 vPlayerPos, vMonsterPos;

	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	//m_pTransformCom->Get_Info(INFO_POS, &vMonsterPos);

	_float fDistance = D3DXVec3Length(&(vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS]));

	if (fDistance < m_fAttackRange)
	{
		return true;
	}

	return false;
}

void CMonster::StateMachine(const _float& fTimeDelta)
{
	CMonsterState* State = INFO.MonsterState->Update(this, fTimeDelta);
	if (State != nullptr) {
		INFO.MonsterState = State;
		INFO.MonsterState->Initialize(this);
	}
}

HRESULT CMonster::Set_HP()
{
	//m_pUI_HPFrame = CUIMgr::GetInstance()->Get_UI_Clone(L"HP_Monster.png");
	//m_pUI_HPValue = CUIMgr::GetInstance()->Get_UI_Clone(L"VALUE_Monster.png");
	//
	////m_pUI_Recognition = CRecognitionRange::Create(m_pGraphicDev, this);
	//
	//m_pUI_HPFrame->Set_Target(this);
	//m_pUI_HPValue->Set_Target(this);
	//
	//_float fRatio = 5.f;
	//_float fFrame = 5.1f;
	//m_pUI_HPFrame->Set_ScaleRatio(fFrame);
	//m_pUI_HPValue->Set_ScaleRatio(fRatio);


	int i = 0;

	//if (!m_pUI_HPValue || !m_pUI_HPFrame)
	//	return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);


	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_suit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

// 	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
// 	NULL_CHECK_RETURN(pComponent, E_FAIL);
// 	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::CALCULATOR, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CMonster::Set_TransPos()
{
	m_pTransformCom->Set_Pos(INFO.vPos);
}

void CMonster::Chase_Target(const _float& fTimeDelta)
{


	_vec3 vPlayerPos, vMyPos, vPlayerPos_Rel;

	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	D3DXVec3Normalize(&_vec3(vPlayerPos - vMyPos), &vPlayerPos_Rel);

	m_pTransformCom->Chase_Target(&vPlayerPos, fTimeDelta, m_fSpeed);
}

void CMonster::Set_Frame(_float _fVerDevide, _float _fHorDevide, _float _fCurFrame)
{
	m_fFrame = _fCurFrame;
	m_fVerDevide = _fVerDevide;
	m_fHorDevide = _fHorDevide;
}

_bool CMonster::Find_ColCube(_vec3 _MoveVector, _float _MoveSpeed, const _float& fTimeDelta)
{
	return false;
}

void CMonster::Change_State_IDLE()
{
}

void CMonster::Attacked(_float _fDamage)
{
	if (INFO.fHP != 0) {
		INFO.fHP -= _fDamage;
		INFO.bHit = true;
		if (INFO.fHP <= 0) {
			INFO.fHP = 0;
			INFO.bDead = true;
			Set_ObjectTag(OBJECTTAG::DEAD_MONSTER);
		}
	}
}

void CMonster::Attacked_Fail()
{
}

void CMonster::Set_Info(Mob_INFO _INFO)
{
	INFO = _INFO;
	Set_TransPos();
}

void CMonster::Set_Pos(_vec3 _vPos)
{
	INFO.vPos = _vPos;
	Set_TransPos();
}

void CMonster::Free()
{
	//Safe_Release(m_pUI_HPFrame);
	//Safe_Release(m_pUI_HPValue);


	__super::Free();
}
