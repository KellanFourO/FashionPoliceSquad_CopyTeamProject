#pragma once


#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "Base.h"
#include "Engine_Define.h"
//#include "MyUI.h"
#include "Export_Utility.h"
//#include "UIMgr.h"


class CImGuiManager : public CBase
{
	DECLARE_SINGLETON(CImGuiManager)
private:
	explicit	CImGuiManager();
	virtual		~CImGuiManager();

public:
	_bool		Get_Switch() { return m_bToolSwitch; };
	void		Set_Switch(_bool bSwitch) { m_bToolSwitch = bSwitch; }

public:
	HRESULT		SetUp_ImGui(LPDIRECT3DDEVICE9 pGraphicDev);
	_int		Update_ImGui(const _float& fTimeDelta);
	void		LateUpdate_ImGui(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Render_ImGui(LPDIRECT3DDEVICE9 pGraphicDev);

///////////////////// 유진 맵툴 ////////////////////////////
public:
	void		Set_CubeData(vector<CUBE*> vecCubeData)
				{	vectorCubeTemp = vecCubeData;	}
 	void		Set_CubeSize(_float& Width, _float& Height, _float& Depth)
 				{
					m_fCubesize.fX = Width;
					m_fCubesize.fY = Height;
					m_fCubesize.fZ = Depth;
 				}
	void		Save_MapData();
	void		Load_MapData();

	_uint		Get_CubeTexNum() { return m_iTextureNum; }
	float		Get_CubeHeightLevel() { return m_fCubeHeightLevel; }

	bool		Get_Load_Check() { return m_bLoad_Check; }
	void	 	Set_Load_Check() { m_bLoad_Check = !m_bLoad_Check; }

	bool		Get_DeleteMode_Check() { return m_bDelete_Mode_Check; }
	bool		Get_NotNormal_Check() { return m_bNotNormal_Check; }
	bool		Get_BuildModeCheck() { return m_bBuild_Mode_Check; }

	CUBESIZE    Get_CubeSize()	{ return m_fCubesize; }

private:
	vector<IDirect3DCubeTexture9*>		m_MainCubeTexture;

	_uint								m_iIndex;
	SORTCUBE*							m_defSortCube = nullptr;

	vector<SORTCUBE*>					m_pCubeForSort; //큐브 정렬용

	//vector<const char*>				m_FileName;
	vector<CUBE*>						vectorCubeTemp;

	ImTextureID							selected_texture0		 = nullptr;
	_uint								selected_texture_index0	 = 0;

	CTerrainTex*						m_pTerrainTex;

	CUBESIZE							m_fCubesize;

	_uint								m_iTextureNum			 = 1;
	float								m_fCubeHeightLevel		 = 0.f;
	bool								m_bLoad_Check			 = false;

	bool								m_bDelete_Mode_Check	 = false;
	bool								m_bNotNormal_Check		 = false;
	bool								m_bRotate_Check			 = false;

	bool								m_bBuild_Mode_Check		 = false;


	///////////////////// 유진 OBJ툴 ////////////////////////////////
public:
	void				LoadTexturesFromDirectory
						(const wchar_t* folderPath, vector<IDirect3DCubeTexture9*>& textureVector);

	void				LoadTexturesFromDirectory
						(const wchar_t* folderPath, vector<IDirect3DBaseTexture9*>& textureVector);
	bool				Get_OBJModeCheck() { return m_bOBJ_Mode_Check; }

	bool				Get_CubeType() { return m_bCubeType; }
	bool				Get_PlaneType() { return m_bPlaneType; }

	void				Save_ObjData();
	void				Save_CPointData();

	bool				Get_OBJLoad_Check() { return m_bOBJLoad_Check; }
	void	 			Set_OBJLoad_Check() { m_bOBJLoad_Check = !m_bOBJLoad_Check; }
	bool				Get_CPOINT_Load_Check() { return m_bCPOINT_Load_Check; }
	void	 			Set_CPOINT_Load_Check() { m_bCPOINT_Load_Check = !m_bCPOINT_Load_Check; }

	OBJ_TYPE			Get_OBJType() { return m_eOBJType; }
	void				Set_OBJType(OBJ_TYPE eType) { m_eOBJType = eType; }
	_uint				Get_OBJTexNum() { return m_iOBJTextureNum; }

	OBJ_ATTRIBUTE		Get_OBJATTRIBUTE() { return m_eOBJ_Attribute; }
	void				Set_OBJATTRIBUTE(OBJ_ATTRIBUTE eAttri) { m_eOBJ_Attribute = eAttri; }

	MonsterType			Get_MonsterType() { return m_eMonsterType; }

	_uint				Get_OBJ_RotateCountCW() { return m_Rotate_Count_CW; }
	void				Set_OBJ_RotateCountCW_Zero() { m_Rotate_Count_CW = 0; }

	vector<IDirect3DCubeTexture9*>&		Get_CubeTextureObjVector() { return m_pCubeTextureObj; }
	vector<IDirect3DBaseTexture9*>&		Get_PlaneTextureObjVector() { return m_pPlaneTextureObj; }

private:

	SORTTEX*							m_defSortTex = nullptr; //텍스쳐 정렬용
	vector<SORTTEX*>					m_pTexForSort;			//텍스쳐 정렬용

	vector<IDirect3DCubeTexture9*>		m_pCubeTextureObj;		//환경 OBJ 중 큐브
	vector<IDirect3DBaseTexture9*>		m_pPlaneTextureObj;		//환경 OBJ 중 Plane

	ImTextureID							selected_texture1       = nullptr;
	_uint								selected_texture_index1 = 0;
	_uint								m_iOBJTextureNum		= 1;

	const int							cubeTextureStartIndex   = 1000;
	const int							planeTextureStartIndex  = 3000;

	_uint								m_Rotate_Count_CW;		//시계방향

	bool								m_bOBJ_Mode_Check		= false;
	bool								m_bOBJLoad_Check		= false;
	bool								m_bCPOINT_Load_Check	= false;

	bool								m_bCubeType				= false;
	bool								m_bPlaneType			= false;
	OBJ_TYPE							m_eOBJType				= OBJ_TYPE::OBJ_TYPE_END;

	int									m_forObjAttribute		= 7; //0 ~ 7
	OBJ_ATTRIBUTE						m_eOBJ_Attribute		= OBJ_ATTRIBUTE::ATTRIBUTE_END;

	int									m_forMobType			= 4; // 0 ~ 4
	MonsterType							m_eMonsterType			= MonsterType::MOBTYPE_END;

	vector<OBJData*>					vectorOBJTemp;

	///////////////////// 유진 트리거 툴 ////////////////////////////
public:
	bool				Get_TriggerMode_Check() { return m_bTrigger_Mode_Check; }
	void				Set_TriggerMode_Check() { m_bTrigger_Mode_Check = !m_bTrigger_Mode_Check; }

	bool				Get_TriggerLoad_Check() { return m_bTriggerLoad_Check; }
	void				Set_TriggerLoad_Check() { m_bTriggerLoad_Check = !m_bTriggerLoad_Check; }

	vector<TRIGGER*>&	Get_VecTrigger() { return m_VecTrigger; }
	void				Trigger_Info_Clear() {
		m_eTR_NUM = TRIGGER_NUMBER::TR_END;
		m_eTR_CASE = TRIGGER_CASE::TR_CASE_END;
		m_eTR_TYPE = TRIGGER_TYPE::TR_TYPE_END;
		m_fCubesize.fX = 1.f;
		m_fCubesize.fY = 1.f;
		m_fCubesize.fZ = 1.f;
		m_SizeSet_Check = false;
	}

private:
	//콤보박스 선택용
	const char*				Trigger_Number[21] = { "TR0", "TR1", "TR2", "TR3", "TR4", "TR5", "TR6", "TR7", "TR8", "TR9", "TR10",
								"TR11", "TR12", "TR13", "TR14", "TR15", "TR16", "TR17", "TR18", "TR19", "TR20" };
	const char*				Trigger_Case[4] = { "ONCE", "TWICE", "REPEAT", "CASE_END"};
	const char*				Trigger_Type[6] = { "MOVING", "UI", "DAMAGE", "MONSTER", "NOTHING", "TYPE_END"};
	
	vector<TRIGGER*>		m_VecListbox;
	TRIGGER_NUMBER			m_eTR_NUM = TRIGGER_NUMBER::TR_END;
	TRIGGER_CASE			m_eTR_CASE = TRIGGER_CASE::TR_CASE_END;
	TRIGGER_TYPE			m_eTR_TYPE = TRIGGER_TYPE::TR_TYPE_END;

private:
	bool					m_SizeSet_Check = false;			
	bool					m_bTrigger_Mode_Check = false;
	bool					m_bTriggerLoad_Check = false;
	bool					m_bTR_Set_Ready_Check = false; //항목 세팅
	bool					m_bTriggerReady_Check = false; //리스트 넣고 찍기 직전

	TRIGGER*				m_TriggerTemp = nullptr;
	vector<TRIGGER*>		m_VecTrigger;



	///////////////////// 승용 UI툴 /////////////////////////////////
public:
	//CMyUI*				Get_ImguiUI() { return m_vecUIList[selected_UIIndex]; }
	//CMyUI*				Get_UI(const _tchar* szKey) { return FindUI(szKey); }
	//vector<CMyUI*>		Get_vecUI() { return m_vecUIList; };

public:
	void				Toggle_OnOff() { bUI_OnOff = !bUI_OnOff; }
	//HRESULT				ReadImgPath(const _tchar* folderPath, LPDIRECT3DDEVICE9 pGraphicDev);
	//HRESULT				SaveData(const _tchar* mapTag);

	//void				SaveUI(wstring wstrFileName, wstring wstrFilePath);
	//CMyUI*				FindUI(const _tchar* szKey);

private:
	_bool								m_bToolSwitch;
	_bool								m_bCameraMode;
	_bool								bUI_OnOff;

private:
	//map<const _tchar*, CMyUI*>			m_mapLoadUI1;
	//map<const _tchar*, CMyUI*>			m_mapChoiceUI;

	_tchar*								selected_UI_FileName;
	_uint								selected_UIIndex = 0;
	_bool								m_bClick = false;

	int									m_iUI_Type;
	ImGuiIO*							m_pIo;

	//vector<CMyUI*>						m_vecUIList;
	//vector<const _tchar*>				m_vecUIName;



	// 맵툴 예시, 현재 다른 툴 껍데기에도 사용중임, 변경시 전달 부탁.
	vector<IDirect3DBaseTexture9*>		m_pCubeTexture;
	ImTextureID							selected_texture = nullptr;
	_uint								selected_texture_index = 0;
	_uint								m_iPickingMode;

public:
	virtual void		Free();

};



