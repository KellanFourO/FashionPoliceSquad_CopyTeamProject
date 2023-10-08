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
	HRESULT				Load_Obj(const TCHAR* pFilePath);
	bool				CheckDuplicateCube(const _vec3& pPos, const _vec3& pSize);

public:
	vector<CUBE*>&		Get_VecCubeData() { return m_VecCubeData; }
	void				Set_VecCubeData(vector<CUBE*>* pVecCubeData) { m_VecCubeData = *pVecCubeData;	}

	vector<OBJData*>&	Get_VecOBJData() { return m_VecOBJData; }
	void				Set_VecOBJData(vector<OBJData*>* pVecOBjData) { m_VecOBJData = *pVecOBjData; }

	void				Cursor_Update();
	void				CubeSize_Update();

	//BuildOBJ������ �ؽ��ĳѹ� ���п����� ��
	vector<IDirect3DCubeTexture9*>&		Get_VecTempCube()  { return m_VecTempCube; } 
	vector<IDirect3DBaseTexture9*>&		Get_VecTempPlane() { return m_VecTempPlane; }

private:
	//BuildOBJ������ �ؽ��ĳѹ� ���п����� ��
	vector<IDirect3DCubeTexture9*> m_VecTempCube;
	vector<IDirect3DBaseTexture9*> m_VecTempPlane;

	CLayer*				m_pLayer				= nullptr;
	CMapCursor*			m_pMapCursor			= nullptr;
		
	_vec3*				m_vCursor_Pos;
	_vec3				m_vCursor_Size;
	_float				m_vCursor_Height;
	CUBESIZE			m_fCubesize;

	bool				m_Build_time_Check		= false;
	_int				m_Build_time			= 0;

	//��ü �ε��� : �� �� �� �� ������ ��ȣ(�ߺ� ����)
	_uint				m_iOBJIndex				= 5000;   //������Ʈ�� 5000~9999
	_uint				m_iCubeIndex			= 10000;  //�� ť��� : ���� ���� ū ������ ���� �� - 10000~

	//�ؽ��� �ε��� : �� �ؽ��� ������ ��ȣ(���� �ؽ��Ķ�� ���� ���� ����)
	_uint				m_iTextureNum			= 0;  //�� ��° �ؽ��ĳ�
	_uint				m_iTextureNum2			= 0;  //�� ��° �ؽ��ĳ�	

	//OBJ start index , ��ġ�� �ʰ� �з����� �����ִ� ��������
	const int			cubeObjTextureStartIndex	= 1000;
	const int			planeObjTextureStartIndex	= 3000;

	bool				m_BUILD_NOP_MODE		= false;
	CHAR*				pTag					= nullptr; //Load �� ���� �༮.

	vector<OBJData*>	m_VecOBJData;	//���� OBJ ������(ť��Ÿ��+�÷���Ÿ��) ����
	vector<CUBE*>		m_VecCubeData;	//������ ť�� ���� ������ ����

public:
	OBJData*			OBJTemp					= nullptr;  //������Ʈ��
	CUBE*				CubeTemp2				= nullptr; //�� ť���

public:
	static CMapTool*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();
};

