#include "..\..\Header\Calculator.h"

#include "Export_Utility.h"


CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}


CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator()
{
	return S_OK;
}

_float CCalculator::Compute_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{

	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	_float	fRatioX = _float(pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float	fRatioZ = _float(pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;

	D3DXPLANE	Plane;

	// 오른쪽 상단 삼각형
	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + dwCntX + 1],
			&pTerrainVtxPos[dwIndex + 1]);
	}

	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}

	// ax + by + cz + d
	// by = -ax - cz - d
	// y = (-ax - cz - d) / b

	return (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;
}


_vec3 CCalculator::Picking_OnTerrain(HWND hWnd, CTerrainTex* pTerrainBufferCom, CTransform* pTerrainTransformCom, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);


	// 뷰포트 변환 -> 투영 변환
	D3DXVECTOR3	vMouse;
	vMouse.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMouse.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMouse.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix		matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	// matView = 뷰의 역 행렬

	_vec3		vRayPos, vRayDir; // 로컬 ~

	vRayPos = { 0.f, 0.f, 0.f }; // -1 ~ 1
	vRayDir = vMouse - vRayPos;  // 뷰스페이스

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);  // 0.0.0 * 뷰의 역행렬 = 월드
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬

	_matrix	matWorld = *(pTerrainTransformCom->Get_WorldMatrix());
	D3DXMatrixInverse(&matWorld, 0, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3* pTerratVtxPos = pTerrainBufferCom->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};

	_float	fU(0.f), fV(0.f), fDist(0.f);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwCntX + j;

			// 오른쪽 위

			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + dwCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerratVtxPos[dwVtxIdx[1]],
				&pTerratVtxPos[dwVtxIdx[0]],
				&pTerratVtxPos[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerratVtxPos[dwVtxIdx[1]].x + (pTerratVtxPos[dwVtxIdx[0]].x - pTerratVtxPos[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerratVtxPos[dwVtxIdx[1]].z + (pTerratVtxPos[dwVtxIdx[2]].z - pTerratVtxPos[dwVtxIdx[1]].z) * fV);
			}

			// V1 + U(V2 - V1) + V(V3 - V1)

			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerratVtxPos[dwVtxIdx[2]],
				&pTerratVtxPos[dwVtxIdx[1]],
				&pTerratVtxPos[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerratVtxPos[dwVtxIdx[2]].x + (pTerratVtxPos[dwVtxIdx[1]].x - pTerratVtxPos[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerratVtxPos[dwVtxIdx[2]].z + (pTerratVtxPos[dwVtxIdx[0]].z - pTerratVtxPos[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return _vec3();
}

_vec3 CCalculator::Picking_OnCube(HWND hWnd, CCubeTex* pTerrainBufferCom, CTransform* pCubeTransformCom, const vector<CUBE>* pVectorCube)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);


	// 뷰포트 변환 -> 투영 변환
	D3DXVECTOR3	vMouse;
	vMouse.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMouse.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMouse.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix		matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	// matView = 뷰의 역 행렬

	_vec3		vRayPos, vRayDir; // 로컬 ~

	vRayPos = { 0.f, 0.f, 0.f }; // -1 ~ 1
	vRayDir = vMouse - vRayPos;  // 뷰스페이스

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);  // 0.0.0 * 뷰의 역행렬 = 월드
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬

	_matrix	matWorld = *(pCubeTransformCom->Get_WorldMatrix());
	D3DXMatrixInverse(&matWorld, 0, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);


	return _vec3();
}



_vec3 CCalculator::MapCursor_From_Mouse(HWND hWnd, CWireTex* pWireBufferCom, CTransform* pWireTransformCom, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);


	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 변환 -> 투영 변환
	D3DXVECTOR3	vMouse;
	vMouse.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMouse.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMouse.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix		matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	// matView = 뷰의 역 행렬

	_vec3		vRayPos, vRayDir; // 로컬 ~

	vRayPos = { 0.f, 0.f, 0.f }; // -1 ~ 1
	vRayDir = vMouse - vRayPos;  // 뷰스페이스

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);  // 0.0.0 * 뷰의 역행렬 = 월드
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix matWorld = *(pWireTransformCom->Get_WorldMatrix());
	D3DXMatrixInverse(&matWorld, 0, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3* pTerratVtxPos = pWireBufferCom->Get_VtxPos();
	//지형 메쉬(Terrain or WireFrame)의 버텍스 좌표를 가져옴
	//BufferCom에 담겨 있는 정보.

	_ulong	dwVtxIdx[3]{};
	_float	fU(0.f), fV(0.f), fDist(0.f);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwCntX + j;

			// 오른쪽 위

			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + dwCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerratVtxPos[dwVtxIdx[1]],
				&pTerratVtxPos[dwVtxIdx[0]],
				&pTerratVtxPos[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{

				return _vec3(pTerratVtxPos[dwVtxIdx[1]].x + (pTerratVtxPos[dwVtxIdx[0]].x - pTerratVtxPos[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerratVtxPos[dwVtxIdx[1]].z + (pTerratVtxPos[dwVtxIdx[2]].z - pTerratVtxPos[dwVtxIdx[1]].z) * fV);
			}

			// V1 + U(V2 - V1) + V(V3 - V1)

			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerratVtxPos[dwVtxIdx[2]],
				&pTerratVtxPos[dwVtxIdx[1]],
				&pTerratVtxPos[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerratVtxPos[dwVtxIdx[2]].x + (pTerratVtxPos[dwVtxIdx[1]].x - pTerratVtxPos[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerratVtxPos[dwVtxIdx[2]].z + (pTerratVtxPos[dwVtxIdx[0]].z - pTerratVtxPos[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return _vec3(-20.f, -20.f, -20.f);
}


CCalculator* CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator* pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Calculator Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CCalculator::Clone()
{
	return new CCalculator(*this);
}

void CCalculator::Free()
{
	CComponent::Free();
}
