#include "Export_Utility.h"
#include "../../Header/CollisionMgr.h"

#define CMP(x, y) \
      (fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))
IMPLEMENT_SINGLETON(CollisionMgr)

CollisionMgr::CollisionMgr()
{
	Reset();
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::MONSTER);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::BOSS);
	//CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::ITEM);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::MONSTERBULLET);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::BOSSBULLET);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::BUILD_OBJ);
	CheckGroup(OBJECTTAG::MONSTER, OBJECTTAG::PLAYERBULLET);
	CheckGroup(OBJECTTAG::MONSTER, OBJECTTAG::BUILD_OBJ);
	CheckGroup(OBJECTTAG::BOSS, OBJECTTAG::BUILD_OBJ);
	CheckGroup(OBJECTTAG::BOSS, OBJECTTAG::PLAYERBULLET);
	CheckGroup(OBJECTTAG::RAY, OBJECTTAG::BUILD_OBJ);
	CheckGroup(OBJECTTAG::RAY, OBJECTTAG::MONSTER);
	CheckGroup(OBJECTTAG::PLAYER_LAZER, OBJECTTAG::MONSTER);
	CheckGroup(OBJECTTAG::PLAYER_LAZER, OBJECTTAG::BOSS);
	CheckGroup(OBJECTTAG::RAY_LASER, OBJECTTAG::MONSTER);
	CheckGroup(OBJECTTAG::RAY_LASER, OBJECTTAG::BOSS);
	

}

CollisionMgr::~CollisionMgr()
{
}

void CollisionMgr::LateUpdate_Collision()
{
	//static buildcude
	CheckCollisionStatic(OBJECTTAG::PLAYER);
	CheckCollisionStatic(OBJECTTAG::MONSTER);
	CheckCollisionStatic(OBJECTTAG::BOSS);
	CheckCollisionStatic(OBJECTTAG::MONSTERBULLET);
	CheckCollisionStatic(OBJECTTAG::PLAYERBULLET);
	//CheckCollisionStatic(OBJECTTAG::ITEM);
	CheckCollisionStatic(OBJECTTAG::RAY);
	CheckCollisionStatic(OBJECTTAG::RAY);
	CheckCollisionStatic(OBJECTTAG::RAY_LASER);
	//static OBj
	CheckCollisionStaticOBJ(OBJECTTAG::PLAYER);
	CheckCollisionStaticOBJ(OBJECTTAG::MONSTER);
	CheckCollisionStaticOBJ(OBJECTTAG::BOSS);
	CheckCollisionStaticOBJ(OBJECTTAG::MONSTERBULLET);
	CheckCollisionStaticOBJ(OBJECTTAG::PLAYERBULLET);
	CheckCollisionStaticOBJ(OBJECTTAG::RAY);
	CheckCollisionStaticOBJ(OBJECTTAG::PLAYER_LAZER);
	CheckCollisionStaticOBJ(OBJECTTAG::RAY_LASER);
	
	//Ray

	
	//dynamic Object
	for (UINT iRow = 0; iRow < (UINT)OBJECTTAG::OBJECT_END; ++iRow)
		for (UINT iCol = iRow; iCol < (UINT)OBJECTTAG::OBJECT_END; ++iCol)
			if (m_arrCheck[iRow] & (1 << iCol))
				CheckCollisionByType((OBJECTTAG)iRow, (OBJECTTAG)iCol);

}

void CollisionMgr::CheckGroup(OBJECTTAG _eLeft, OBJECTTAG _eRight)
{


	_uint iRow = (_uint)_eLeft;
	_uint iCol = (_uint)_eRight;

	if (iCol < iRow)
	{
		iRow = (_uint)_eRight;
		iCol = (_uint)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}
}

void CollisionMgr::Reset()
{
	//ZeroMemory(m_arrCheck, sizeof(OBJECTTAG)); 승용 수정
	ZeroMemory(m_arrCheck, sizeof(m_arrCheck));
}

bool CollisionMgr::IsCollision(CCollider* _pLeft, CCollider* _pRight)
{


	_vec3  vLeftPos = _pLeft->GetCenterPos();
	float* vLeftScale = _pLeft->GetAxisLen();

	_vec3	vRightPos = _pRight->GetCenterPos();
	float* vRightScale = _pRight->GetAxisLen();

	if ((abs(vRightPos.x - vLeftPos.x) <= (vLeftScale[0] + vRightScale[0])
		&& (abs(vRightPos.y - vLeftPos.y) <= (vLeftScale[1] + vRightScale[1])
			&& (abs(vRightPos.z - vLeftPos.z) <= (vLeftScale[2] + vRightScale[2])))))
	{
		return TRUE;
	}

    return FALSE;
}

void CollisionMgr::CheckCollisionByType(OBJECTTAG _eObjectLeft, OBJECTTAG _eObjectRight)
{
	CScene* pScene = Management()->Get_Scene();

	const vector<CGameObject*>& vecLeft = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, _eObjectLeft);
	const vector<CGameObject*>& vecRight = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, _eObjectRight);

	map<ULONGLONG, bool>::iterator iter;

	//for (auto& iterL = vecLeft.begin(); iterL != vecLeft.end(); ++iterL)
	for (auto& iterL : vecLeft)
	{

		//for (auto& iterR = vecRight.begin(); iterR != vecRight.end(); ++iterR)
		for (auto& iterR : vecRight)
		{
			if (nullptr == iterR->Get_Collider() || iterL == iterR)
				continue;

			CCollider* pLeftCol = iterL->Get_Collider();
			static_cast<CCollider*>(iterL->Get_Component(ID_DYNAMIC,COMPONENTTAG::COLLIDER));
			CCollider* pRightCol = iterR->Get_Collider();
			static_cast<CCollider*>(iterR->Get_Component(ID_DYNAMIC,COMPONENTTAG::COLLIDER));

			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();

			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(pLeftCol, pRightCol))
			{	// 현재 충돌 중
				if (iter->second)
				{	// 이전에도 충돌
					if (iterL->IsDead() || iterR->IsDead())
					{	// 둘 중 하나 삭제 예정이면 충돌 해제

						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollisionStay(pRightCol);
						pRightCol->OnCollisionStay(pLeftCol);
					}
				}
				else
				{	// 이전에는 충돌 x	// 근데 둘 중 하나 삭제 예정이면 충돌하지 않은 것으로 취급
					if (pRightCol->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYERBULLET || pLeftCol->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYERBULLET)
						int i = 0;
					if (!iterL->IsDead() && !iterR->IsDead())
					{

						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
					else
					{
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
				}
				//return;
			}
			else
			{
			// 현재 충돌 x면
				if (iter->second)
				{	//이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
					//return;
				}
			}
		}
	}
}

void CollisionMgr::CheckCollisionStatic(OBJECTTAG _eObjectLeft)
{
	CScene* pScene = CManagement::GetInstance()->Get_Scene();

	const vector<CGameObject*>& vecLeft = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, _eObjectLeft);

	if (vecLeft.empty()) {
		return;
	}

	map<ULONGLONG, bool>::iterator iter;

	for (auto& iterL : vecLeft)
	{
		if (nullptr == iterL->Get_Collider())
			continue;


		_vec3 vHostPos = dynamic_cast<CTransform*>(iterL->Get_Component(ID_DYNAMIC, COMPONENTTAG::TRANSFORM))->m_vInfo[INFO_POS];
		COctreeNode* pParentNode = Octree()->GetParentNodeByPos(vHostPos, Engine::Octree()->GetOctreeRoot());

		if (!pParentNode) return;

		const vector<CGameObject*>& vecRight = pParentNode->GetObjectList();
		//const vector<CGameObject*>& vecRight = pScene->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::BUILD_CUBE); // 유진 추가

		for (auto& iterR : vecRight)
		{
			if (nullptr == iterR->Get_Collider() || iterL == iterR)
				continue;

			CCollider* pLeftCol = iterL->Get_Collider();
			CCollider* pRightCol = iterR->Get_Collider();

			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(pLeftCol, pRightCol))
			{	// 현재 충돌 중
				if (iter->second)
				{	// 이전에도 충돌
					if (iterL->IsDead() || iterR->IsDead())
					{	// 둘 중 하나 삭제 예정이면 충돌 해제
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollisionStay(pRightCol);
						pRightCol->OnCollisionStay(pLeftCol);
					}
				}
				else
				{	// 이전에는 충돌 x	// 근데 둘 중 하나 삭제 예정이면 충돌하지 않은 것으로 취급
					if (!iterL->IsDead() && !iterR->IsDead())
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
					else
					{
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
				}

				//return;
			}
			else
			{		// 현재 충돌 x면
				if (iter->second)
				{	//이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
					//return;
				}
			}
		}
	}
}

void CollisionMgr::CheckCollisionStaticOBJ(OBJECTTAG _eObjectLeft)
{
	CScene* pScene = CManagement::GetInstance()->Get_Scene();

	const vector<CGameObject*>& vecLeft = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, _eObjectLeft);

	if (vecLeft.empty()) {
		return;
	}

	map<ULONGLONG, bool>::iterator iter;

	for (auto& iterL : vecLeft)
	{
		if (nullptr == iterL->Get_Collider())
			continue;


		_vec3 vHostPos = dynamic_cast<CTransform*>(iterL->Get_Component(ID_DYNAMIC, COMPONENTTAG::TRANSFORM))->m_vInfo[INFO_POS];
		//COctreeNode* pParentNode = Octree()->GetParentNodeByPos(vHostPos, Engine::Octree()->GetOctreeRoot());

		//if (!pParentNode) return;

		//const vector<CGameObject*>& vecRight = pParentNode->GetObjectList();
		const vector<CGameObject*>& vecRight = pScene->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::BUILD_OBJ); // 유진 추가

		for (auto& iterR : vecRight)
		{
			if (nullptr == iterR->Get_Collider() || iterL == iterR)
				continue;

			CCollider* pLeftCol = iterL->Get_Collider();
			CCollider* pRightCol = iterR->Get_Collider();

			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(pLeftCol, pRightCol))
			{	// 현재 충돌 중
				if (iter->second)
				{	// 이전에도 충돌
					if (iterL->IsDead() || iterR->IsDead())
					{	// 둘 중 하나 삭제 예정이면 충돌 해제
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollisionStay(pRightCol);
						pRightCol->OnCollisionStay(pLeftCol);
					}
				}
				else
				{	// 이전에는 충돌 x	// 근데 둘 중 하나 삭제 예정이면 충돌하지 않은 것으로 취급
					if (!iterL->IsDead() && !iterR->IsDead())
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
					else
					{
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
				}

				//return;
			}
			else
			{		// 현재 충돌 x면
				if (iter->second)
				{	//이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
					//return;
				}
			}
		}
	}
}

void CollisionMgr::Free()
{

}
bool CollisionMgr::CollisionRayToCube(CCollider* pCRay, CCollider* pCBox, _vec3 vRayDest)
{

	//pcRay가 출발 vRayDest가 도착
	_vec3 vRayOrigin = pCRay->GetCenterPos();

	m_vRayDir = vRayDest - vRayOrigin;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	_vec3 vRayToCenter = pCBox->GetCenterPos() - vRayOrigin;

	_vec3 vColliderDir[3] = { pCBox->GetAxisDir()[0], pCBox->GetAxisDir()[1], pCBox->GetAxisDir()[2] };

	_vec3 vAxisDotToRD = { D3DXVec3Dot(&(vColliderDir[0]), &m_vRayDir),
					   D3DXVec3Dot(&(vColliderDir[1]), &m_vRayDir),
					   D3DXVec3Dot(&(vColliderDir[2]), &m_vRayDir) };

	_vec3 vAxisDotToRTC = { D3DXVec3Dot(&(vColliderDir[0]), &vRayToCenter),
					   D3DXVec3Dot(&(vColliderDir[1]), &vRayToCenter),
					   D3DXVec3Dot(&(vColliderDir[2]), &vRayToCenter) };

	float Temp[6] = {};

	// 각 축에 대한 반복문
	for (int i = 0; i < 3; ++i)
	{
		// 광선과 슬랩이 평행한 경우
		if (CMP(vAxisDotToRD[i], 0))
		{
			// 광선과 슬랩이 평행한 경우
			if (-vAxisDotToRTC[i] - pCBox->GetAxisLen()[i] > 0 || -vAxisDotToRTC.x + pCBox->GetAxisLen()[i] < 0)
			{
				return false; // 슬랩 밖에 있는 경우 조기 종료
			}
			vAxisDotToRD[i] = 0.00001f; // 0으로 나누는 것을 방지하기 위한 작은 값
		}

		// 축에 대한 최소 및 최대값 계산
		Temp[i * 2 + 0] = (vAxisDotToRTC[i] + pCBox->GetAxisLen()[i]) / vAxisDotToRD[i]; // tmin[x, y, z]
		Temp[i * 2 + 1] = (vAxisDotToRTC[i] - pCBox->GetAxisLen()[i]) / vAxisDotToRD[i]; // tmax[x, y, z]
	}

	// 광선이 AABB와 교차하는지 여부를 확인하기 위해 'tmin' 및 'tmax' 값 계산
	float tmin = fmaxf(fmaxf(fminf(Temp[0], Temp[1]), fminf(Temp[2], Temp[3])), fminf(Temp[4], Temp[5]));
	float tmax = fminf(fminf(fmaxf(Temp[0], Temp[1]), fmaxf(Temp[2], Temp[3])), fmaxf(Temp[4], Temp[5]));


	// 'tmax'가 0보다 작으면 광선은 AABB와 교차하지만 AABB 전체가 광선의 원점 뒤에 있음
	if (tmax < 0) {
		return false;
	}

	// 'tmin'이 'tmax'보다 크면 광선은 AABB와 교차하지 않음
	if (tmin > tmax) {
		return false;
	}

	// 'tmin'이 0보다 작으면 'tmax'가 더 가까운 교차 지점을 나타냄
	float t_result = tmin;
	if (tmin < 0.0f) {
		t_result = tmax;
	}
	//이것도 바꿔야 됨 
	m_fDist = (m_vRayDir * t_result);
	m_DestOriginLen = vRayDest - vRayOrigin;
	_float fDist = D3DXVec3Length(&(m_fDist));
	if (fDist < D3DXVec3Length(&(m_DestOriginLen)))
		return true;
	else
		return false;
}