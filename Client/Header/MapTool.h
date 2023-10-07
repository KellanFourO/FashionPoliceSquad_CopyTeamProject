#pragma once
#include "Scene.h"

#include "Terrain.h"
#include "WireFrame.h"
#include "MapCursor.h"
#include "Build_Cube.h"
#include "Build_Obj.h"

#include "FlyingCamera.h"
#include "DynamicCamera.h"


class CMapTool : public CScene
{
private:
	explicit CMapTool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapTool();

public:
	virtual HRESULT		Ready_Scene();
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual	void		LateUpdate_Scene();
	virtual void		Render_Scene();

private:
	HRESULT				Ready_LightInfo();

	HRESULT				Ready_Layer_Environment(LAYERTAG eLayerTag);
	void				BUILD_NOP_MODE();

	HRESULT				Build_Map();
	HRESULT				Delete_Map();

	HRESULT				Load_Cube(const TCHAR* pFilePath);
	bool				CheckDuplicateCube(const _vec3& pPos, const _vec3& pSize);

public:
	vector<CUBE*>&		Get_VecCubeData() { return m_VecCubeData; }
	void				Set_VecCubeData(vector<CUBE*>* pVecCubeData) { m_VecCubeData = *pVecCubeData;	}

	vector<OBJData*>&	Get_VecOBJData() { return m_VecOBJData; }
	void				Set_VecOBJData(vector<OBJData*>* pVecOBjData) { m_VecOBJData = *pVecOBjData; }

	void				Cursor_Update();
	void				CubeSize_Update();

	vector<IDirect3DCubeTexture9*>&		Get_VecTempCube()  { return m_VecTempCube; }
	vector<IDirect3DBaseTexture9*>&		Get_VecTempPlane() { return m_VecTempPlane; }

private:
	vector<IDirect3DCubeTexture9*> m_VecTempCube;
	vector<IDirect3DBaseTexture9*> m_VecTempPlane;

	CLayer*				m_pLayer				= nullptr;
	CMapCursor*			m_pMapCursor			= nullptr;
	_uint				m_iTextureNum			= 0;  //몇 번째 텍스쳐냐
		
	_vec3*				m_vCursor_Pos;
	_vec3				m_vCursor_Size;
	_float				m_vCursor_Height;
	CUBESIZE			m_fCubesize;

	bool				m_Build_time_Check		= false;
	_int				m_Build_time			= 0;

	//객체 인덱스 : 한 개 한 개 마다의 번호(중복 없게)
	_uint				m_iOBJIndex				= 5000;   //오브젝트용
	_uint				m_iCubeIndex			= 10000;  //맵 큐브용 
														  //(맵을 제일 큰 단위로 놓을 것.)
	//텍스쳐 인덱스 : 한 텍스쳐 마다의 번호(같은 텍스쳐라면 같을 수도 있음)
	const int			cubeObjTextureStartIndex	= 1000;
	const int			planeObjTextureStartIndex	= 3000;

	bool				m_BUILD_NOP_MODE		= false;
	CHAR*				pTag					= nullptr; //Load 때 쓰는 녀석.

	vector<OBJData*>	m_VecOBJData;
	vector<CUBE*>		m_VecCubeData;

public:
	OBJData*			OBJTemp					= nullptr;  //오브젝트용
	CUBE*				CubeTemp2				= nullptr; //맵 큐브용

public:
	static CMapTool*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();
};

