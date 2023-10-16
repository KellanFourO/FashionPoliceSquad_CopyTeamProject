#pragma once
#include "Scene.h"

#include "Terrain.h"
#include "WireFrame.h"
#include "MapCursor.h"
#include "Build_Cube.h"
#include "Build_Obj.h"
#include "Trigger.h"

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
	HRESULT				Ready_Layer_GameLogic(LAYERTAG eLayerTag);
	void				BUILD_NOP_MODE();

	HRESULT				Build_Map();
	HRESULT				Delete_Map();

	HRESULT				Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag);
	HRESULT				Load_Data_C(const TCHAR* pFilePath, OBJECTTAG eTag);
	HRESULT				Load_Data_T(const TCHAR* pFilePath, OBJECTTAG eTag);


	bool				CheckDuplicateCube(const _vec3& pPos, const _vec3& pSize);

public:
	vector<CUBE*>&		Get_VecCubeData() { return m_VecCubeData; }
	void				Set_VecCubeData(vector<CUBE*>* pVecCubeData) { m_VecCubeData = *pVecCubeData;	}

	vector<OBJData*>&	Get_VecOBJData() { return m_VecOBJData; }
	void				Set_VecOBJData(vector<OBJData*>* pVecOBjData) { m_VecOBJData = *pVecOBjData; }
 	
	vector<TRIGGER*>&	Get_VecTrigger() { return m_TriggerData; }
	void				Set_VecTrigger(vector<TRIGGER*>* pVecTRData) { m_TriggerData = *pVecTRData; }

	vector<OBJData*>&	Get_VecLight()  { return m_VecLight; }
	vector<C_POINT*>&	Get_VecCreatePoint() { return m_VecCreatePoint; }

	void				Cursor_Update();
	void				CubeSize_Update();

	//BuildOBJ내에서 텍스쳐넘버 구분용으로 씀
	vector<IDirect3DCubeTexture9*>&		Get_VecTempCube()  { return m_VecTempCube; } 
	vector<IDirect3DBaseTexture9*>&		Get_VecTempPlane() { return m_VecTempPlane; }

private:
	//BuildOBJ내에서 텍스쳐넘버 구분용으로 씀
	vector<IDirect3DCubeTexture9*> m_VecTempCube;
	vector<IDirect3DBaseTexture9*> m_VecTempPlane;

	//Create Point 용 벡터 컨테이너
	vector<C_POINT*>			   m_VecCreatePoint;
	// 조명 
	vector<OBJData*>			   m_VecLight;

	CLayer*				m_pLayer				= nullptr;
	CLayer*				m_pGLayer				= nullptr;
	CMapCursor*			m_pMapCursor			= nullptr;
		
	_vec3*				m_vCursor_Pos;
	_vec3				m_vCursor_Size;
	_float				m_vCursor_Height;
	CUBESIZE			m_fCubesize;

	bool				m_Build_time_Check		= false; //큐브
	_int				m_Build_time			= 0;

	bool				m_Build_time_Check2		= false; //OBJ
	_int				m_Build_time2			= 0;

	//객체 인덱스 : 한 개 한 개 마다의 번호(중복 없게)
	_uint				m_iOBJIndex				= 10000;   //오브젝트용 10000~18999
	_uint				m_iTriggerIndex			= 19000;  //트리거용 19000~19999
	_uint				m_iCubeIndex			= 20000;  //맵 큐브용 : 맵을 제일 큰 단위로 놓을 것 - 10000~

	//텍스쳐 인덱스 : 한 텍스쳐 마다의 번호(같은 텍스쳐라면 같을 수도 있음)
	_uint				m_iTextureNum			= 0;  //몇 번째 텍스쳐냐
	_uint				m_iTextureNum2			= 0;  //몇 번째 텍스쳐냐	

	//OBJ start index , 겹치지 않게 분류마다 더해주는 보정값임
	const int			cubeObjTextureStartIndex	= 1000;
	const int			planeObjTextureStartIndex	= 3000;

	bool				m_BUILD_NOP_MODE		= false;
	CHAR*				pTag					= nullptr; //Load 때 쓰는 녀석.

	vector<OBJData*>	m_VecOBJData;	//메인 OBJ 데이터(큐브타입+플레인타입) 벡터
	vector<CUBE*>		m_VecCubeData;	//빌딩용 큐브 한정 데이터 벡터
	vector<TRIGGER*>	m_TriggerData;  //Trigger 보관용 데이터 벡터

private:
	OBJData*			OBJTemp					= nullptr;  //오브젝트용
	OBJData*			OBJTemp2				= nullptr;  //오브젝트용
	OBJData*			OBJTemp3				= nullptr;  //오브젝트용
	CUBE*				CubeTemp2				= nullptr; //맵 큐브용
	C_POINT*			OBJ_C_POINT				= nullptr; //몬스터 리젠 위치용
	TRIGGER*			TriggerTemp				= nullptr; //트리거 설치용

public:
	static CMapTool*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();
};

