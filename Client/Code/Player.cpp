#include "stdafx.h"
#include "Player.h"

#include "Rifle.h"
#include "Dyehard.h"
#include "CrossHair.h"

#include "Player_IDLE.h"
#include "Player_Armor.h"
#include "Player_Heal.h"
#include "Player_Hit.h"
#include "Player_Left.h"
#include "Player_Right.h"
#include "Player_LevelUp.h"
#include "Player_Jump.h"

#include "Export_System.h"
#include "Export_Utility.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bJump(false),
	m_fJumpTick(10.f), m_fJumpCount(0.f), m_fSpeed_Vertical(0.f)
{
}

CPlayer::CPlayer(CPlayer& rhs)
	: Engine::CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{

}

HRESULT CPlayer::Ready_GameObject()
{
	Set_ObjectTag(OBJECTTAG::PLAYER);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	ReadyState();

	m_pTransformCom->Set_Host(this);
	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	m_pRigidBody->Set_Host(this);
	m_pRigidBody->Set_Transform(m_pTransformCom);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());


	m_fJumpTick = 10.f;
	m_fJumpCount = 0.f;

	m_bJump = false;

	m_pTransformCom->Set_Scale({ 1.5f,5.5f,1.5f });
	m_pTransformCom->Set_Pos(10.f,10.f,10.f);

	m_fTall = 1.f;

	INFO.fJumpHight = 0.75f;
	INFO.fMoveSpeed = 15.f; // 이동

	INFO.iLevel = 1;
	INFO.iEXP = 0;

	INFO.fHP = 80;
	INFO.fMaxHP = 100;
	INFO.fMaXHP_Additional = 40; // 체력

	INFO.fArmor = 0;
	INFO.fMaxArmor = 40;

	INFO.Player_GunType = PLAYER_GUNTYPE::SHOTGUN;
	INFO.PlayerState = m_pStateArray[IDLE];
	INFO.bGameOver = false;

	INFO.vPos = { 20.f,5.f,20.f };
	INFO.fStartDir = 0.f;//생각처럼 잘 안댐...


	//m_pTransformCom->Rotation(ROT_Y,D3DXToRadian(INFO.fStartDir));

	return S_OK;
}

Engine::_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	//m_pTransformCom->Get_Info(INFO_POS, &INFO.vPos);

	if (m_bLateInit)
	{
		SetGun();
	}


	StateMachine(fTimeDelta);

	if (false == m_bFix)
	{
		Mouse_Input(fTimeDelta);
	}

	m_pRigidBody->Update_RigidBody(fTimeDelta);

	//m_pGun->Update_GameObject(fTimeDelta);
	Key_Input(fTimeDelta);


	__super::Update_GameObject(fTimeDelta);
	return OBJ_NOEVENT;
}

void CPlayer::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayer::ReadyState()
{
	m_pStateArray[IDLE] =		new CPlayer_IDLE;
	m_pStateArray[LEFT] =		new CPlayer_Left;
	m_pStateArray[RIGHT] =		new CPlayer_Right;
	m_pStateArray[HIT] =		new CPlayer_Hit;
	m_pStateArray[LEVELUP] =	new CPlayer_LevelUp;
	m_pStateArray[HEAL] =		new CPlayer_Heal;
	m_pStateArray[ARMOR] =		new CPlayer_Armor;
	m_pStateArray[JUMP] =		new CPlayer_Jump;
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CPlayer::SetGun()
{

	CPlayerGun* pPlayerGun = nullptr;

	pPlayerGun = dynamic_cast<CPaintShotGun*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER_GUN).front());
	m_vecPlayerGun.push_back(pPlayerGun);

	pPlayerGun = dynamic_cast<CTailorAssertRifle*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER_GUN).back());
	m_vecPlayerGun.push_back(pPlayerGun);

	m_pGun = m_vecPlayerGun[0];

}

void CPlayer::Key_Input(const _float& fTimeDelta)
{

	_vec3	vDir, vRight;
	m_fJumpTick += fTimeDelta;

	m_fTime_Level_Test += fTimeDelta;
	m_fTime_DEAD_Test += fTimeDelta;

	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);

	D3DXVec3Normalize(&m_vDir,&m_vDir);
	_vec3 vUp = { 0.f,1.f,0.f };

	_float fMoveSpeed = INFO.fMoveSpeed;
	_bool bMove = false;

	vDir.y = 0.f; //y값만 0이 되어야 하기 때문에 재연이가 해놓은거임 점프 때문에


	D3DXVec3Cross(&vRight, &vDir, &vUp);
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vRight, &vRight);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		m_vDir += vDir;
		bMove = true;
	}
	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		m_vDir -= vDir;
		bMove = true;
	}
	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		m_vDir += vRight;
		bMove = true;
	}
	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		m_vDir -= vRight;
		bMove = true;
	}
	if (bMove)
	{
		if (m_Speed_Cheat_ON) {
			fMoveSpeed *= 4.f;			//속도업
		}

		m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, fMoveSpeed);
		bMove = false;
	}

	if (Engine::Get_DIKeyState(DIK_1) & 0x80)
	{
		m_pGun = m_vecPlayerGun[0];
		INFO.Player_GunType = PLAYER_GUNTYPE::SHOTGUN;
	}
	else if (Engine::Get_DIKeyState(DIK_2) & 0x80)
	{
		m_pGun = m_vecPlayerGun[1];
		INFO.Player_GunType = PLAYER_GUNTYPE::ASSERTRIFLE;
	}

	if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80 && !m_bJump && m_fJumpTick >= 1.5f)
	{

	}

	if (Engine::Get_DIKeyState(DIK_LCONTROL) & 0x80)
	{

	}

	if ((Engine::Get_DIKeyState(DIK_L) & 0x80) && m_fTime_Level_Test >= 0.3f)
	{
		EXP_Up(5);
		m_fTime_Level_Test = 0.f;
	}

	if ((Engine::Get_DIKeyState(DIK_0) & 0x80) && m_fTime_DEAD_Test >= 0.3f)
	{
		m_Speed_Cheat_ON = !m_Speed_Cheat_ON;
		m_fTime_DEAD_Test = 0.f;
	}

}

void CPlayer::Mouse_Input(const _float& fTimeDelta)
{
	_long	dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
	}
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f));
	}

}

void CPlayer::Gun_Select(_int Gun_Number)
{
	Engine::GunID SelectGun = GunID::GUN_ID_END;
	switch (Gun_Number)
	{
	case 1:
		SelectGun = GunID::DYEHARD;

		break;
	case 2:
		SelectGun = GunID::RIFLE;
		break;
	default:
		break;
	}

	if (!Management()->Get_ObjectList(LAYERTAG::UI, OBJECTTAG::CROSSHAIR).empty())
	{
		dynamic_cast<CCrossHair*>(Management()->Get_ObjectList(LAYERTAG::UI, OBJECTTAG::CROSSHAIR).back())->Set_CrossHair(Gun_Number - 1);

	}

}

void CPlayer::Attacked(_float _fDamage)
{

	_float fDamage = _fDamage;
	if (INFO.fArmor > fDamage) {
		INFO.fArmor -= fDamage;
	}
	else {
		INFO.fArmor = 0;
		fDamage -= INFO.fArmor;
	}
	// 아머 계산

	if (INFO.fHP - fDamage > 0) {
		INFO.fHP -= fDamage;
	}
	else {
		INFO.fHP = 0;
		INFO.bGameOver = true;
	}

	// 체력 계산
	INFO.PlayerState = m_pStateArray[HIT];
	INFO.PlayerState->Initialize(this);


	m_pTransformCom->Get_Info(INFO_LOOK, &m_vDir);
	m_vDir.y = 0.f;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pRigidBody->Set_Force(_vec3(0.f, 10.f, 0.f));
	m_pRigidBody->Add_Force(-m_vDir * _fDamage);

}

void CPlayer::Healed(_float _iHP)
{
	if (INFO.fHP + _iHP <= INFO.fMaxHP + INFO.fMaXHP_Additional) {

		INFO.fHP += _iHP;
	}
	else {
		INFO.fHP = INFO.fMaxHP + INFO.fMaXHP_Additional;
	}
	INFO.PlayerState = m_pStateArray[HEAL];
	INFO.PlayerState->Initialize(this);
}

void CPlayer::Armor_Get(_float _fArmor)
{
	if (INFO.fArmor < INFO.fMaxArmor) {
		INFO.fArmor += _fArmor;
	}
	if (INFO.fArmor > INFO.fMaxArmor) {
		INFO.fArmor = INFO.fMaxArmor;
	}
	INFO.PlayerState = m_pStateArray[ARMOR];
	INFO.PlayerState->Initialize(this);
}

void CPlayer::HP_Release()
{
	if (INFO.fHP > INFO.fMaxHP) {
		if (m_fHP_Reduction >= 1.f) {//체력 감소 속도
			INFO.fHP -= 1;
			m_fHP_Reduction = 0.f;
		}
	}
}

void CPlayer::EXP_Up(_int _EXP)
{
	INFO.iEXP += _EXP;
	Level_Check();
}

void CPlayer::Level_Check()
{
	if (INFO.iEXP >= INFO.iLevel * 5 + 20) {
		INFO.iEXP = 0;		//경험치 초기화
		INFO.iLevel += 1;	//레벨업
		INFO.PlayerState = m_pStateArray[LEVELUP];
		INFO.PlayerState->Initialize(this);
	}
}

void CPlayer::StateMachine(_float _fTimeDelta)
{
	CPlayerState* State = INFO.PlayerState->Update(this, _fTimeDelta);
	if (State != nullptr) {
		INFO.PlayerState = State;
		INFO.PlayerState->Initialize(this);
	} // 상태 패턴
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		int i = 0;
	}

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE)
	{
		_vec3	vOtherPos = _pOther->GetCenterPos();
		_float* fOtherAxis = _pOther->GetAxisLen();

		_vec3	vThisPos = m_pCollider->GetCenterPos();
		_float* fThisAxis = m_pCollider->GetAxisLen();

		//OBJECTTAG에 따른 예외 처리 가능성
		_float fWidth = fabs(vOtherPos.x - vThisPos.x);
		_float fHeight = fabs(vOtherPos.y - vThisPos.y);
		_float fDepth = fabs(vOtherPos.z - vThisPos.z);

		_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
		_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
		_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

		_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

		if (fRadiusY == fMinAxis)
		{
			if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
				return;
			if (vOtherPos.y < vThisPos.y)
			{
				//m_IsJump = false;
				//m_pRigidBody->UseGravity(false);
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));
				m_pTransformCom->Translate(_vec3(0.f, fRadiusY, 0.f));
			}
			else
			{
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));
				m_pTransformCom->Translate(_vec3(0.f, -fRadiusY, 0.f));
			}
		}
		else if (fRadiusZ == fMinAxis)
		{
			if (vOtherPos.z < vThisPos.z)
				m_pTransformCom->Translate(_vec3(0.f, 0.f, fRadiusZ));
			else
				m_pTransformCom->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else //(fRadiusX == fMinAxis)
		{
			if (vOtherPos.x < vThisPos.x)
				m_pTransformCom->Translate(_vec3(fRadiusX, 0.f, 0.f));
			else
				m_pTransformCom->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
		m_bJump = false;
	}

}

void CPlayer::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		int i = 0;
	}

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
			if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
				return;
			if (vOtherPos.y < vThisPos.y)
			{
				//m_IsJump = false;
				//m_pRigidBody->UseGravity(false);
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));
				m_pTransformCom->Translate(_vec3(0.f, fRadiusY, 0.f));
			}
			else
				m_pTransformCom->Translate(_vec3(0.f, -fRadiusY, 0.f));
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

void CPlayer::OnCollisionExit(CCollider* _pOther)
{
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free()
{
	for(auto iter : m_pStateArray)
		Safe_Delete(iter);

	__super::Free();

}
