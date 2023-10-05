#pragma once


#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "Base.h"
#include "Engine_Define.h"
#include "MyUI.h"
#include "Export_Utility.h"
//#include "UIMgr.h"


class CImGuiManager : public CBase
{
	DECLARE_SINGLETON(CImGuiManager)
private:
	explicit	CImGuiManager();
	virtual		~CImGuiManager();

public:
	void		Key_Input(const _float& fTimeDelta);

public:
	_bool		Get_Switch() { return m_bToolSwitch; };
	void		Set_Switch(_bool bSwitch) { m_bToolSwitch = bSwitch; }

public:
	HRESULT		SetUp_ImGui(LPDIRECT3DDEVICE9 pGraphicDev);
	_int		Update_ImGui(const _float& fTimeDelta);
	void		LateUpdate_ImGui(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Render_ImGui(LPDIRECT3DDEVICE9 pGraphicDev);

///////////////////// ���� ���� ////////////////////////////
public:
	typedef struct	SortCube {  //ť�� �ؽ��� ���� ���� ����ü
		_uint					iIndex;			//�ε���
		char*					stFileName;
		_uint					iNameNumber;	//���ϸ� ���� ����
		IDirect3DCubeTexture9* tTexture;
	}SORTCUBE;

	void		Set_CubeData(vector<CUBE*> vecCubeData)
				{	vectorCubeTemp = vecCubeData;	}
 	void		Set_CubeSize(_float& Width, _float& Height, _float& Depth)
 				{
					m_fCubesize.m_fX = Width;
					m_fCubesize.m_fY = Height;
					m_fCubesize.m_fZ = Depth;
 				}
	void		Save_MapData();
	void		Load_MapData();

	_uint		Get_CubeTexNum() { return m_iTextureNum; }
	float		Get_CubeHeightLevel() { return m_fCubeHeightLevel; }

	bool		Get_Load_Check() { return m_bLoad_Check; }
	void	 	Set_Load_Check() { m_bLoad_Check = !m_bLoad_Check; }

	bool		Get_ALLDelete_Check() { return m_bALLDelete_Check; }
	void		Set_ALLDelete_Check() { m_bALLDelete_Check = !m_bALLDelete_Check; }

	bool		Get_DeleteMode_Check() { return m_bDelete_Mode_Check; }
	bool		Get_NotNormal_Check() { return m_bNotNormal_Check; }
	bool		Get_BuildModeCheck() { return m_bBuild_Mode_Check; }

	CUBESIZE    Get_CubeSize()	{ return m_fCubesize; }

private:
	vector<IDirect3DCubeTexture9*>		m_MainCubeTexture;

	_uint								m_iIndex;
	SORTCUBE*							m_defSortCube = nullptr;

	vector<SORTCUBE*>					m_pCubeForSort; //ť�� ���Ŀ�
	
	vector<const char*>					m_FileName;
	vector<CUBE*>						vectorCubeTemp;

	ImTextureID							selected_texture0		 = nullptr;
	_uint								selected_texture_index0	 = 0;

	CTerrainTex*						m_pTerrainTex;

	CUBESIZE							m_fCubesize;

	_uint								m_iTextureNum			 = 1;
	float								m_fCubeHeightLevel		 = 0.f;
	bool								m_bLoad_Check			 = false;

	bool								m_bALLDelete_Check		 = false;
	bool								m_bDelete_Mode_Check	 = false;
	bool								m_bNotNormal_Check		 = false;
	bool								m_bRotate_Check			 = false;

	bool								m_bBuild_Mode_Check		 = false;
	 

	///////////////////// ���� OBJ�� ////////////////////////////////
public:
	void				LoadTexturesFromDirectory
						(const wchar_t* folderPath, vector<IDirect3DCubeTexture9*>& textureVector);
	void				LoadTexturesFromDirectory
						(const wchar_t* folderPath, vector<IDirect3DBaseTexture9*>& textureVector);
	bool				Get_OBJModeCheck() { return m_bOBJ_Mode_Check; }
	
	void				Save_ObjData();
	void				Load_ObjData();

	OBJ_TYPE			Get_OBJType() { return m_eOBJType; }
	void				Set_OBJType(OBJ_TYPE eType) { m_eOBJType = eType; }
	_uint				Get_OBJTexNum() { return m_iOBJTextureNum; }

	vector<IDirect3DCubeTexture9*>&		Get_CubeTextureObjVector() { return m_pCubeTextureObj; }
	vector<IDirect3DBaseTexture9*>&		Get_CrossTextureObjVector() { return m_pCrossTexture0; }
	vector<IDirect3DBaseTexture9*>&		Get_PlaneTextureObjVector() { return m_pPlaneTexture0; }

private:

	vector<IDirect3DCubeTexture9*>		m_pCubeTextureObj; //ȯ�� OBJ �� ť��
	vector<IDirect3DBaseTexture9*>		m_pCrossTexture0;  //ȯ�� OBJ �� Cross
	vector<IDirect3DBaseTexture9*>		m_pPlaneTexture0;  //ȯ�� OBJ �� Plane

	ImTextureID							selected_texture1       = nullptr;
	_uint								selected_texture_index1 = 0;
	_uint								m_iOBJTextureNum		= 1;

	const int							cubeTextureStartIndex   = 1000;
	const int							crossTextureStartIndex  = 2000;
	const int							planeTextureStartIndex  = 3000;

	bool								m_bMainAngleRot			= false;
	bool								m_bRightAngleRot		= false;
	
	bool								m_bOBJ_Mode_Check		= false;

	bool								m_bCubeType				= false;
	bool								m_bCrossType			= false;
	bool								m_bPlaneType			= false;
	OBJ_TYPE							m_eOBJType				= OBJ_TYPE::OBJ_TYPE_END;

	vector<OBJData*>					vectorOBJPlaneTemp;
	//vector<OBJData*>					vectorOBJCubeTemp;
	//vector<OBJData*>					vectorOBJCrossTemp;

	///////////////////// �¿� UI�� /////////////////////////////////
public:
	CMyUI*				Get_ImguiUI() { return m_vecUIList[selected_UIIndex]; }
	CMyUI*				Get_UI(const _tchar* szKey) { return FindUI(szKey); }
	vector<CMyUI*>		Get_vecUI() { return m_vecUIList; };

public:
	void				Toggle_OnOff() { bUI_OnOff = !bUI_OnOff; }
	HRESULT				ReadImgPath(const _tchar* folderPath, LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT				SaveData(const _tchar* mapTag);

	void				SaveUI(wstring wstrFileName, wstring wstrFilePath);
	CMyUI*				FindUI(const _tchar* szKey);

private:
	_bool								m_bToolSwitch;
	_bool								m_bCameraMode;
	_bool								bUI_OnOff;

private:
	map<const _tchar*, CMyUI*>			m_mapLoadUI1;
	map<const _tchar*, CMyUI*>			m_mapChoiceUI;

	_tchar*								selected_UI_FileName;
	_uint								selected_UIIndex = 0;
	_bool								m_bClick = false;

	int									m_iUI_Type;
	ImGuiIO*							m_pIo;

	vector<CMyUI*>						m_vecUIList;
	vector<const _tchar*>				m_vecUIName;



	// ���� ����, ���� �ٸ� �� �����⿡�� �������, ����� ���� ��Ź.
	vector<IDirect3DBaseTexture9*>		m_pCubeTexture;
	ImTextureID							selected_texture = nullptr;
	_uint								selected_texture_index = 0;
	_uint								m_iPickingMode;

public:
	virtual void		Free();

};



