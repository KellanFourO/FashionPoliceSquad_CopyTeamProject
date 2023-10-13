#include "stdafx.h"

#include "ImGuiManager.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "MapTool.h"
#include "Stage.h"

#include "commdlg.h"
#include "shlwapi.h"



IMPLEMENT_SINGLETON(CImGuiManager)



CImGuiManager::CImGuiManager()
    :m_bToolSwitch(true)
{
}

CImGuiManager::~CImGuiManager()
{
    //Free();
}

HRESULT CImGuiManager::SetUp_ImGui(LPDIRECT3DDEVICE9 pGraphicDev)
{

    //Engine::Management()->GetInstance()->Get_Scene()

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_pIo = &ImGui::GetIO(); (void)m_pIo;
    m_pIo->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    m_pIo->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    m_pIo->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //m_pIo->ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    //m_pIo->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();
    m_pIo->Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, m_pIo->Fonts->GetGlyphRangesKorean());
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(pGraphicDev);

    //승용 시작
    //Engine::Ready_Proto(L"Proto_EffectTexture", CTexture::Create(Engine::Get_GraphicDev(), TEX_NORMAL, L"../Bin/Resource/Texture/UI/Explosion%d.png", 90));
    //ReadImgPath(L"..\\Bin\\Resource\\Texture\\UI", pGraphicDev);

	//맵툴(환경)에서 사용할 파일 읽어오기 - 유진
	LoadTexturesFromDirectory(L"../Bin/Resource/Texture/Cube", m_MainCubeTexture);
	LoadTexturesFromDirectory(L"../Bin/Resource/Texture/Obj/CubeType", m_pCubeTextureObj);
	LoadTexturesFromDirectory(L"../Bin/Resource/Texture/Obj/PlaneType", m_pPlaneTextureObj);

    return S_OK;
}


_int CImGuiManager::Update_ImGui(const _float& fTimeDelta)
{


	return 0;
}

void CImGuiManager::LateUpdate_ImGui(LPDIRECT3DDEVICE9 pGraphicDev)
{
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 800), ImGuiCond_FirstUseEver);

    if (m_bToolSwitch)
    {
        ImGui::Begin("Tool", &m_bToolSwitch);

#pragma region 맵툴
        if (ImGui::TreeNode(u8"맵 툴 / OBJ 툴 / 트리거 툴"))
        {
            if (ImGui::TreeNode(u8"건물"))
            {
                ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
                if (ImGui::BeginTabBar(u8"건물 및 바닥", tab_bar_flags))
                {
                    if (ImGui::BeginTabItem(u8"건물 및 바닥"))
                    {

                        ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
                        ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
                        ImVec2 uv1 = ImVec2(1.0f, 1.0f);                          // UV coordinates for (32,32) in our texture
                        ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
                        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint

						ImGui::Checkbox(u8"Build 모드", &m_bBuild_Mode_Check);
						if (m_bBuild_Mode_Check)
						{
							m_bOBJ_Mode_Check = false;
							m_bTrigger_Mode_Check = false;

							ImGui::Image(selected_texture0, ImVec2(96.0f, 96.0f), uv0, uv1, tint_col, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
							ImGui::NewLine();

							for (int i = 0; i < 1; i++) //가로줄(행) 갯수
							{
								for (int j = 0; j < 25; j++) //세로줄(열) 갯수
								{
									_int iIndex = 25 * i + j;
									if (iIndex >= m_MainCubeTexture.size())
										break;

									ImGui::PushID(iIndex);

									if (ImGui::ImageButton("", m_MainCubeTexture[iIndex], size))
									{
										selected_texture0 = m_MainCubeTexture[iIndex];
										selected_texture_index0 = iIndex;
										m_iTextureNum = iIndex;
										Set_OBJType(OBJ_TYPE::BUILDING_TYPE);
									}
									ImGui::PopID();
									ImGui::SameLine();

									if ((j != 0) && (j % 5 == 4))
									{ ImGui::NewLine(); }
								}
								ImGui::NewLine();

							}

							if (ImGui::Button(" +5 ")) { m_fCubeHeightLevel += 5; }
							ImGui::SameLine();
							if (ImGui::Button(" -5 ")) { m_fCubeHeightLevel -= 5; }
							ImGui::SameLine();
							if (ImGui::Button(" + ")) { m_fCubeHeightLevel += 1; }
							ImGui::SameLine();
							if (ImGui::Button(" - ")) { m_fCubeHeightLevel -= 1; }

							ImGui::SameLine();
							char TextNow[MAX_PATH];
							sprintf_s(TextNow, u8"큐브 높이: %.f", m_fCubeHeightLevel);
							ImGui::Text(TextNow);

							ImGui::SameLine();
							if (ImGui::Button(u8"높이 초기화"))
							{	m_fCubeHeightLevel = 0;	}


							ImGui::Checkbox("Delete Mode", &m_bDelete_Mode_Check);

							ImGui::NewLine();

							float step = 2.0f; // 단위 설정

							ImGui::Checkbox("unnormalized Cube", &m_bNotNormal_Check);
							if (m_bNotNormal_Check == false)
							{
								m_fCubesize = { 1.f, 1.f, 1.f };
							}
							else if (m_bNotNormal_Check == true) {
								ImGui::SetNextItemWidth(70.0f);
								if (ImGui::InputFloat("sizeX", &m_fCubesize.fX))
									m_fCubesize.fX = (roundf(m_fCubesize.fX / step) * step) - 1.f;
								ImGui::SameLine();

								ImGui::SetNextItemWidth(70.0f);
								if (ImGui::InputFloat("SizeY", &m_fCubesize.fY))
									m_fCubesize.fY = (roundf(m_fCubesize.fY / step) * step) - 1.f;
								ImGui::SameLine();

								ImGui::SetNextItemWidth(70.0f);
								if (ImGui::InputFloat("SizeZ", &m_fCubesize.fZ))
									m_fCubesize.fZ = (roundf(m_fCubesize.fZ / step) * step) - 1.f;
							}

							ImGui::NewLine();

							if (ImGui::Button("Map Save"))
							{
								Save_MapData();
							}
							ImGui::SameLine();

							if (ImGui::Button("Map Load"))
							{
								m_bLoad_Check = true;
							}

						}
                        ImGui::EndTabItem();
                    }

                    ImGui::EndTabBar();
                }

                ImGui::Separator();
                ImGui::TreePop();
            }

#pragma endregion


#pragma region OBJ툴

		if (ImGui::TreeNode(u8"환경"))
		{
		    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar(u8"환경", tab_bar_flags))
			{
				if (ImGui::BeginTabItem(u8"입체 오브젝트"))
				{
					ImVec2 size1 = ImVec2(40.0f, 40.0f);                      // Size of the image we want to make visible
					ImVec2 uv2 = ImVec2(0.0f, 0.0f);                          // UV coordinates for lower-left
					ImVec2 uv3 = ImVec2(1.0f, 1.0f);                          // UV coordinates for (32,32) in our texture
					ImVec4 bg_col1 = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);          // Black background
					ImVec4 tint_col1 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);        // No tint

					ImGui::Checkbox(u8"OBJ 모드", &m_bOBJ_Mode_Check);

					if (m_bOBJ_Mode_Check)
					{
						m_bBuild_Mode_Check = false;
						m_bTrigger_Mode_Check = false;

						float step = 0.5f; // 단위 설정

						ImGui::Image(selected_texture1, ImVec2(96.0f, 96.0f), uv2, uv3, tint_col1, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
						ImGui::NewLine();


						if (ImGui::Button("OBJ Save"))
						{
							Save_ObjData();
						}
						ImGui::SameLine();

						if (ImGui::Button("OBJ Load"))
						{
							m_bOBJLoad_Check = true;
						}

						ImGui::NewLine();

						if (ImGui::Button("C_Point Save"))
						{
							Save_CPointData();
						}
						ImGui::SameLine();

						if (ImGui::Button("C_Point Load"))
						{
							m_bCPOINT_Load_Check = true;
						}

						ImGui::NewLine();

						if (ImGui::Button(" +5 ")) { m_fCubeHeightLevel += 5; }
						ImGui::SameLine();
						if (ImGui::Button(" -5 ")) { m_fCubeHeightLevel -= 5; }
						ImGui::SameLine();
						if (ImGui::Button(" + ")) { m_fCubeHeightLevel += 1; }
						ImGui::SameLine();
						if (ImGui::Button(" - ")) { m_fCubeHeightLevel -= 1; }

						ImGui::SameLine();
						char TextNow[MAX_PATH];
						sprintf_s(TextNow, u8"높이: %.f   ", m_fCubeHeightLevel);
						ImGui::Text(TextNow);
						ImGui::SameLine();
						if (ImGui::Button(u8"높이 초기화"))
						{
							m_fCubeHeightLevel = 0;
						}

						ImGui::NewLine();
						ImGui::Checkbox("Delete Mode", &m_bDelete_Mode_Check);

						char TextNow22[MAX_PATH];
						sprintf_s(TextNow22, u8" : 사용 안함!!!!따로 뺌");

						char TextNow2[MAX_PATH];
						sprintf_s(TextNow2, u8" : 언젠가 움직일지도 모르는 큐브");

						ImGui::NewLine();

						ImGui::RadioButton(u8"파괴OBJ", &m_forObjAttribute, 0);
						ImGui::RadioButton(u8"상호작용OBJ", &m_forObjAttribute, 1);
						ImGui::RadioButton(u8"조명OBJ", &m_forObjAttribute, 2);
						ImGui::RadioButton(u8"빌보드OBJ", &m_forObjAttribute, 3);
// 						ImGui::RadioButton(u8"트리거OBJ", &m_forObjAttribute, 4); 
// 						ImGui::SameLine();
// 						ImGui::Text(TextNow22);
						ImGui::RadioButton(u8"C_Point OBJ", &m_forObjAttribute, 5);
						ImGui::RadioButton(u8"무빙 OBJ", &m_forObjAttribute, 6);
						ImGui::SameLine();
						ImGui::Text(TextNow2);
						ImGui::RadioButton(u8"단순 장식OBJ", &m_forObjAttribute, 7);

						switch (m_forObjAttribute)
						{
						case 0:
							m_eOBJ_Attribute = OBJ_ATTRIBUTE::DES_OBJ;
							break;
						case 1:
							m_eOBJ_Attribute = OBJ_ATTRIBUTE::INTER_OBJ;
							break;
						case 2:
							m_eOBJ_Attribute = OBJ_ATTRIBUTE::LIGHT_OBJ;
							break;
						case 3:
							m_eOBJ_Attribute = OBJ_ATTRIBUTE::BILL_OBJ;
							break;
// 						case 4:
// 							m_eOBJ_Attribute = OBJ_ATTRIBUTE::TRIGGER_OBJ;
// 							break;
						case 5:
							m_eOBJ_Attribute = OBJ_ATTRIBUTE::C_POINT_OBJ;
							break;
						case 6:
							m_eOBJ_Attribute = OBJ_ATTRIBUTE::NONE_OBJ;
							break;
						case 7:
							m_eOBJ_Attribute = OBJ_ATTRIBUTE::MOVING_OBJ;
							break;

						default:
							m_eOBJ_Attribute = OBJ_ATTRIBUTE::NONE_OBJ;
							break;
						}

						if (m_forObjAttribute == 5) // CPoint OBJ 인 경우
						{
							ImGui::Separator(); // 가로 줄 추가
							ImGui::RadioButton(u8"BIGDADDY", &m_forMobType, 0);
							ImGui::SameLine();
							ImGui::RadioButton(u8"DULLSUIT", &m_forMobType, 1);
							ImGui::SameLine();
							ImGui::RadioButton(u8"KCIKBOARD", &m_forMobType, 2);
						}

						switch (m_forMobType)
						{
						case 0:
							m_eMonsterType = MonsterType::BIGDADDY;
							break;
						case 1:
							m_eMonsterType = MonsterType::DULLSUIT;
							break;
						case 2:
							m_eMonsterType = MonsterType::KCIKBOARD;
							break;

						default:
							m_eMonsterType = MonsterType::MOBTYPE_END;
							break;
						}

						ImGui::NewLine();


						if (ImGui::Button(u8"시계방향 회전")) //90도 단위로 회전
						{
							m_Rotate_Count_CW++;
						}

						ImGui::SameLine();
						ImGui::Text(u8"회전 횟수 : %d", (m_Rotate_Count_CW));

						ImGui::SameLine();
						if (ImGui::Button(u8"회전 카운트 Reset"))
						{
							Set_OBJ_RotateCountCW_Zero();
						};

						ImGui::NewLine();

						ImGui::Checkbox("unnormalized", &m_bNotNormal_Check);
						if (m_bNotNormal_Check == false)
						{
							m_fCubesize = { 1.f, 1.f, 1.f };
						}
						else if (m_bNotNormal_Check == true) {
							ImGui::SetNextItemWidth(70.0f);
							if (ImGui::InputFloat("sizeX", &m_fCubesize.fX))
								m_fCubesize.fX = (roundf(m_fCubesize.fX / step) * step);
							ImGui::SameLine();

							ImGui::SetNextItemWidth(70.0f);
							if (ImGui::InputFloat("SizeY", &m_fCubesize.fY))
								m_fCubesize.fY = (roundf(m_fCubesize.fY / step) * step);
							ImGui::SameLine();

							ImGui::SetNextItemWidth(70.0f);
							if (ImGui::InputFloat("SizeZ", &m_fCubesize.fZ))
								m_fCubesize.fZ = (roundf(m_fCubesize.fZ / step) * step);
						}

						ImGui::NewLine();

						ImGui::Separator(); // 가로 줄 추가
						ImGui::Text(u8"OBJ - Cube Texture");

						ImGui::Checkbox(u8"Cube", &m_bCubeType);
						if (true == m_bCubeType)
						{
							m_bPlaneType = false;
							Set_OBJType(OBJ_TYPE::CUBE_TYPE);

							for (int i = 0; i < 1; i++) //가로줄(행) 갯수
							{
								for (int j = 0; j < 10; j++) //세로줄(열) 갯수
								{
									_int iIndex = (10 * i + j) + cubeTextureStartIndex;

									if (iIndex >= cubeTextureStartIndex && iIndex < cubeTextureStartIndex + m_pCubeTextureObj.size())
									{
										ImGui::PushID(iIndex);

										if (ImGui::ImageButton("", m_pCubeTextureObj[iIndex - cubeTextureStartIndex], size1))
										{
											selected_texture1 = m_pCubeTextureObj[iIndex - cubeTextureStartIndex];
											selected_texture_index1 = iIndex;
											m_iOBJTextureNum = iIndex;
										}

										ImGui::PopID();
										ImGui::SameLine();
									}
								}
								ImGui::NewLine();
							}
						}

						ImGui::Separator(); // 가로 줄 추가
						ImGui::Text(u8"OBJ - Plane Texture");

						ImGui::Checkbox(u8"Plane", &m_bPlaneType);
						if (true == m_bPlaneType)
						{
							m_bCubeType = false;
							Set_OBJType(OBJ_TYPE::PLANE_TYPE);


							for (int i = 0; i < 1; i++) //가로줄(행) 갯수
							{
								for (int j = 0; j < 60; j++) //세로줄(열) 갯수
								{
									_int iIndex = (60 * i + j) + planeTextureStartIndex;

									if (iIndex >= planeTextureStartIndex && iIndex < planeTextureStartIndex + m_pPlaneTextureObj.size())
									{
										ImGui::PushID(iIndex);

										if (ImGui::ImageButton("", m_pPlaneTextureObj[iIndex - planeTextureStartIndex], size1))
										{
											selected_texture1 = m_pPlaneTextureObj[iIndex - planeTextureStartIndex];
											selected_texture_index1 = iIndex;
											m_iOBJTextureNum = iIndex;
										}

										ImGui::PopID();
										ImGui::SameLine();
									}
									if (j % 5 == 4) { ImGui::NewLine(); }
								}
							}
						}
					}
					ImGui::EndTabItem();
				}
		        ImGui::EndTabBar();
		   }
		  ImGui::Separator();
		  ImGui::TreePop();
		}

#pragma endregion


#pragma region 트리거 툴

		if (ImGui::TreeNode(u8"트리거"))
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar(u8"트리거", tab_bar_flags))
			{
				if (ImGui::BeginTabItem(u8"트리거 오브젝트"))
				{
					ImGui::Checkbox(u8"트리거 OBJ 모드", &m_bTrigger_Mode_Check);

					if (m_bTrigger_Mode_Check)
					{
						m_bBuild_Mode_Check = false;
						m_bOBJ_Mode_Check = false;

						ImGui::Columns(2);

						ImGui::Checkbox("Delete Mode", &m_bDelete_Mode_Check);
						ImGui::NewLine();

						float step = 0.5f; // 단위 설정

						if (ImGui::Button("Trigger Save"))
						{
							Save_TriggerData();
						}
						ImGui::SameLine();

						if (ImGui::Button("Trigger Load"))
						{
							m_bTriggerLoad_Check = true;
						}
						ImGui::NewLine();


						if (ImGui::Button(" +5 ")) { m_fCubeHeightLevel += 5; }
						ImGui::SameLine();
						if (ImGui::Button(" -5 ")) { m_fCubeHeightLevel -= 5; }
						ImGui::SameLine();
						if (ImGui::Button(" + ")) { m_fCubeHeightLevel += 1; }
						ImGui::SameLine();
						if (ImGui::Button(" - ")) { m_fCubeHeightLevel -= 1; }

						ImGui::SameLine();
						char TextNow[MAX_PATH];
						sprintf_s(TextNow, u8"높이: %.f   ", m_fCubeHeightLevel);
						ImGui::Text(TextNow);
						ImGui::SameLine();
						if (ImGui::Button(u8"높이 초기화"))
						{
							m_fCubeHeightLevel = 0;
						}

						ImGui::NewLine();

						ImGui::Checkbox("unnormalized", &m_bNotNormal_Check);
						if (m_bNotNormal_Check == false)
						{
							m_fCubesize = { 1.f, 1.f, 1.f };
						}
						else if (m_bNotNormal_Check == true) {
							ImGui::SetNextItemWidth(70.0f);
							if (ImGui::InputFloat("sizeX", &m_fCubesize.fX))
								m_fCubesize.fX = (roundf(m_fCubesize.fX / step) * step);
							ImGui::SameLine();

							ImGui::SetNextItemWidth(70.0f);
							if (ImGui::InputFloat("SizeY", &m_fCubesize.fY))
								m_fCubesize.fY = (roundf(m_fCubesize.fY / step) * step);
							ImGui::SameLine();

							ImGui::SetNextItemWidth(70.0f);
							if (ImGui::InputFloat("SizeZ", &m_fCubesize.fZ))
								m_fCubesize.fZ = (roundf(m_fCubesize.fZ / step) * step);
						}
						ImGui::Checkbox(u8"사이즈 세팅 완료", &m_SizeSet_Check);
						
						ImGui::NewLine();




						int iComboTemp0 = static_cast<int>(m_eTR_NUM);
						ImGui::SetNextItemWidth(150.0f);
						ImGui::Combo(u8"Number 선택", &iComboTemp0, Trigger_Number, static_cast<int>(TRIGGER_NUMBER::TR_END));

						switch (iComboTemp0)
						{
						case 0:
							m_eTR_NUM = TRIGGER_NUMBER::TR0;	break;
						case 1:
							m_eTR_NUM = TRIGGER_NUMBER::TR1;	break;
						case 2:
							m_eTR_NUM = TRIGGER_NUMBER::TR2;	break;
						case 3:
							m_eTR_NUM = TRIGGER_NUMBER::TR3;	break;
						case 4:
							m_eTR_NUM = TRIGGER_NUMBER::TR4;	break;
						case 5:
							m_eTR_NUM = TRIGGER_NUMBER::TR5;	break;
						case 6:
							m_eTR_NUM = TRIGGER_NUMBER::TR6;	break;
						case 7:
							m_eTR_NUM = TRIGGER_NUMBER::TR7;	break;
						case 8:
							m_eTR_NUM = TRIGGER_NUMBER::TR8;	break;
						case 9:
							m_eTR_NUM = TRIGGER_NUMBER::TR9;	break;
						case 10:
							m_eTR_NUM = TRIGGER_NUMBER::TR10;	break;
						case 11:
							m_eTR_NUM = TRIGGER_NUMBER::TR11;	break;
						case 12:
							m_eTR_NUM = TRIGGER_NUMBER::TR12;	break;
						case 13:
							m_eTR_NUM = TRIGGER_NUMBER::TR13;	break;
						case 14:
							m_eTR_NUM = TRIGGER_NUMBER::TR14;	break;
						case 15:
							m_eTR_NUM = TRIGGER_NUMBER::TR15;	break;
						case 16:
							m_eTR_NUM = TRIGGER_NUMBER::TR16;	break;
						case 17:
							m_eTR_NUM = TRIGGER_NUMBER::TR17;	break;
						case 18:
							m_eTR_NUM = TRIGGER_NUMBER::TR18;	break;
						case 19:
							m_eTR_NUM = TRIGGER_NUMBER::TR19;	break;
						case 20:
							m_eTR_NUM = TRIGGER_NUMBER::TR20;	break;

						default:
							m_eTR_NUM = TRIGGER_NUMBER::TR_END;
							break;
						}


						int iComboTemp = static_cast<int>(m_eTR_CASE);
						ImGui::SetNextItemWidth(150.0f);
						ImGui::Combo(u8"Case 선택", &iComboTemp, Trigger_Case, static_cast<int>(TRIGGER_CASE::TR_CASE_END));

						switch (iComboTemp)
						{
						case 0:
							m_eTR_CASE = TRIGGER_CASE::TR_ONCE;
							break;
						case 1:
							m_eTR_CASE = TRIGGER_CASE::TR_TWICE;
							break;
						case 2:
							m_eTR_CASE = TRIGGER_CASE::TR_REPEAT;
							break;
						default:
							m_eTR_CASE = TRIGGER_CASE::TR_CASE_END;
							break;
						}


						int iComboTemp2 = static_cast<int>(m_eTR_TYPE);
						ImGui::SetNextItemWidth(150.0f);
						ImGui::Combo(u8"Type 선택", &iComboTemp2, Trigger_Type, static_cast<int>(TRIGGER_TYPE::TR_TYPE_END));

						switch (iComboTemp2)
						{
						case 0:
							m_eTR_TYPE = TRIGGER_TYPE::TR_MOVING;
							break;
						case 1:
							m_eTR_TYPE = TRIGGER_TYPE::TR_UI;
							break;
						case 2:
							m_eTR_TYPE = TRIGGER_TYPE::TR_DAMAGE;
							break;
						case 3:
							m_eTR_TYPE = TRIGGER_TYPE::TR_MONSTER;
							break;
						case 4:
							m_eTR_TYPE = TRIGGER_TYPE::TR_NOTHING;
							break;
						default:
							m_eTR_TYPE = TRIGGER_TYPE::TR_TYPE_END;
							break;
						}

						if ((ImGui::Button(u8"이름 List 추가")) && (m_bTR_Set_Ready_Check == true))
						{
							m_TriggerTemp = new TRIGGER;

							m_TriggerTemp->eTrName = m_eTR_NUM;
							m_TriggerTemp->eTrCase = m_eTR_CASE;
							m_TriggerTemp->eTrType = m_eTR_TYPE;
							m_TriggerTemp->vSize = { m_fCubesize.fX, m_fCubesize.fY, m_fCubesize.fZ };

							m_VecListbox.push_back(m_TriggerTemp);

							Trigger_Info_Clear();
						}	

						ImGui::SameLine();
						if (ImGui::Button(u8"선택 해제"))
						{
							Trigger_Info_Clear();
							m_bTriggerReady_Check = false;
						}

						ImGui::NextColumn();

						ImGui::Text(u8"내용 입력 > 세팅 완료 > 추가 > 리스트 선택 > 맵툴 상 피킹");
						ImGui::NewLine();

						if ((m_eTR_CASE != TRIGGER_CASE::TR_CASE_END)
							&& (m_eTR_TYPE != TRIGGER_TYPE::TR_TYPE_END) 
							&& (m_SizeSet_Check == true))
						{
							ImGui::Text(u8" ! 트리거 세팅 준비 완료");
							m_bTR_Set_Ready_Check = true;
						}
						else {
							ImGui::Text(u8" ! 트리거 세팅 불완전. 다시 확인바람.");
							m_bTR_Set_Ready_Check = false;
						}
						ImGui::NewLine();

						if (m_bTriggerReady_Check == true)
						{
							ImGui::Text(u8"트리거 추가 가능! Ready!");
						}
						else if (m_bTriggerReady_Check == false)
						{
							ImGui::Text(u8"NameList에서 선택바람. 추가 불가.");
						}
						ImGui::NewLine();
						ImGui::BeginListBox("NameList", ImVec2(200, 350));
						{
							for (int i = 0; i < m_VecListbox.size(); ++i)
							{
								if (ImGui::Selectable(to_string(i).c_str()))
								{
									selected_UIIndex = i;

									m_eTR_NUM = m_VecListbox[i]->eTrName;
									m_eTR_CASE = m_VecListbox[i]->eTrCase;
									m_eTR_TYPE = m_VecListbox[i]->eTrType;
									m_fCubesize.fX = m_VecListbox[i]->vSize.x;
									m_fCubesize.fY = m_VecListbox[i]->vSize.y;
									m_fCubesize.fZ = m_VecListbox[i]->vSize.z;

									m_bTriggerReady_Check = true;
								}
							}
							ImGui::EndListBox();
						}


					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::Separator();
			ImGui::TreePop();
		}
           ImGui::Separator();
           ImGui::TreePop();
        }

		ImGui::Columns(1); 

#pragma endregion


#pragma region UI 툴
// if (ImGui::TreeNode(u8"UI 툴"))
// {
//
// 	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
// 	if (ImGui::BeginTabBar("UI 툴", tab_bar_flags))
// 	{
// 		if (ImGui::BeginTabItem(u8"UI_1"))
// 		{
//
// 			ImGui::SeparatorText(u8"추가한 데이터 (인덱스기준)");
// 			{
// 				if (ImGui::Button(u8"빈 데이터 추가"))
// 				{
// 					CMyUI* pMyUI = CMyUI::Create(Engine::Get_GraphicDev(), nullptr, UI_TYPE::BASIC);
//
// 					m_vecUIList.push_back(pMyUI);
// 					m_vecUIName.push_back(L"test");
//
// 				}
// 				ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
//
// 				ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
//
// 				ImGui::BeginListBox("Red", ImVec2(200, 800));
// 				{
// 					for (int i = 0; i < m_vecUIList.size(); ++i)
// 					{
// 						if (ImGui::Selectable(to_string(i).c_str()))
// 						{
// 							selected_UIIndex = i;
// 						}
//
// 					}
// 				ImGui::EndListBox();
// 				}
// 				ImGui::PopStyleColor();
// 			}
//
// 			ImGui::SameLine();
//
// 			if (m_vecUIList.size() != 0 && m_vecUIList[selected_UIIndex] && !m_bClick)
// 			{
// 				ImGui::BeginListBox(u8"이미지 박스", ImVec2(800, 800));
// 				{
//
// 					int UI_Index = 0;
// 					for (auto iter : m_mapLoadUI1)
// 					{
// 						ImGui::PushID(UI_Index);
// 						if (ImGui::ImageButton("", iter.second->Get_Info()->pTexture, ImVec2(96.f, 96.f)))
// 						{
// 							//lstrcmp(selected_UI_FileName, iter.first);
// 							m_vecUIName[selected_UIIndex] = iter.first;
// 							m_vecUIList[selected_UIIndex] = iter.second;
// 							m_bClick = true;
// 						}
//
// 						++UI_Index;
// 						ImGui::PopID();
//
// 						if (UI_Index % 6 == 0)
// 						{
// 							ImGui::NewLine();
// 						}
// 						else
// 						{
// 							ImGui::SameLine();
// 						}
// 					}
// 				ImGui::EndListBox();
// 				}
//
// 				if (ImGui::Button(u8"저장하기"))
// 				{
// 					for (int i = 0; i < m_vecUIList.size(); ++i)
// 					{
// 						m_mapChoiceUI.emplace(m_vecUIName[i], m_vecUIList[i]);
// 					}
//
// 					SaveData(L"UI");
// 				}
//
// 			}
// 			else if (m_bClick)
// 			{
// 				ImGui::SameLine();
// 				ImGui::BeginListBox(u8"데이터 입력", ImVec2(800, 800));
// 				{
//
// 					ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
// 					ImVec2 uv1 = ImVec2(1.0f, 1.0f);                          // UV coordinates for (32,32) in our texture
// 					ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
// 					ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
// 					ImGui::SameLine();
// 					ImGui::Image(m_vecUIList[selected_UIIndex]->Get_Info()->pTexture, ImVec2(96.f, 96.f), uv0, uv1, tint_col, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
// 					ImGui::InputFloat3(u8"위치", m_vecUIList[selected_UIIndex]->Get_Info()->vPos);
// 					ImGui::InputFloat3(u8"크기", m_vecUIList[selected_UIIndex]->Get_Info()->vSize);
//
// 					if (ImGui::Button(u8"선택 취소"))
// 					{
// 						m_bClick = false;
// 					}
// 				ImGui::EndListBox();
// 				}
// 			}
// 			ImGui::EndTabItem();
// 		}
// 		ImGui::EndTabBar();
// 	}
// 	ImGui::Separator();
// 	ImGui::TreePop();
// }
#pragma endregion

#pragma region 카메라툴

        if (ImGui::TreeNode(u8"카메라 툴"))
        {

            ImGui::Separator();
            ImGui::TreePop();
        }
#pragma endregion

        ImGui::End();
    }
}

void CImGuiManager::Render_ImGui(LPDIRECT3DDEVICE9 pGraphicDev)
{
    ImGui::EndFrame();



    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    if (m_pIo->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

    }
}


#pragma region 승용 오빠 파일 Save/Load 관련 함수

// HRESULT CImGuiManager::ReadImgPath(const _tchar* folderPath, LPDIRECT3DDEVICE9 pGraphicDev)
// {
// 	파일 및 디렉토리 정보를 저장하기 위한 구조체
// 		WIN32_FIND_DATA findData;
//
// 		//floderPath의 값을 wFolderPath에 저장 (폴더 경로를 유니코드문자열로 처리)
//
// 		wstring wfolderPath = (wstring)folderPath + L"\\*.*";
//
// 		HANDLE hFind = FindFirstFileW(wfolderPath.c_str(), &findData);
//
// 		if (hFind != INVALID_HANDLE_VALUE)
// 		{
// 			//폴더 내의 모든 파일과 디렉토리를 검색 FindNextFile함수를 사용해서 다음파일 또는 디렉토리를 찾는다
// 			do
// 			{
// 				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
// 				{
// 					// 디렉토리인 경우, "."(현재 디렉토리)와 ".."(상위 디렉토리)를 제외
// 					if (lstrcmp(findData.cFileName, L".") != 0 && lstrcmp(findData.cFileName, L"..") != 0)
// 					{
// 						// 하위 폴더의 경로 생성
// 						wstring subFolderPath = (wstring)folderPath + L"\\" + findData.cFileName;
// 						// 재귀 호출로 하위 디렉토리 검사
//
// 						ReadImgPath(subFolderPath.c_str(), pGraphicDev);
//
// 					}
// 				}
// 				else
// 				{
// 					// 파일인 경우, 이미지 파일인지 확인하고 로드
// 					wstring filePath = (wstring)folderPath + L"\\" + findData.cFileName;
//
// 					if (wcsstr(findData.cFileName, L".png") || wcsstr(findData.cFileName, L".jpg") ||
// 						wcsstr(findData.cFileName, L".bmp") || wcsstr(findData.cFileName, L".tga"))
// 					{
// 						IDirect3DBaseTexture9* pTexture = nullptr;
// 						if (SUCCEEDED(D3DXCreateTextureFromFile(Engine::Get_GraphicDev(), filePath.c_str(), (LPDIRECT3DTEXTURE9*)&pTexture)))
// 						{
// 							const _tchar* szKey = findData.cFileName;
// 							CMyUI* pMyUI = nullptr;
//
// 							if (!FindUI(szKey))
// 							{
// 								// szKey를 복사하여 동적으로 할당
// 								_tchar* copiedKey = new _tchar[_tcslen(szKey) + 1];
// 								_tcscpy_s(copiedKey, _tcslen(szKey) + 1, szKey);
// 								Engine::Ready_Proto(L"Proto_UITex", CUITex::Create(pGraphicDev));
// 								Engine::Ready_Proto(L"Proto_BaseUI", CTexture::Create(pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/all wheels.png"));
//
// 								if (wcsstr(findData.cFileName, L"HP"))
// 								{
// 									pMyUI = CMyUI::Create(pGraphicDev, pTexture, UI_TYPE::HP);
//
// 								}
// 								else
// 								{
// 									pMyUI = CMyUI::Create(pGraphicDev, pTexture, UI_TYPE::BASIC);
// 								}
//
//
// 								// 동적으로 할당한 copiedKey를 맵에 넣어야 합니다.
// 								m_mapLoadUI1.emplace(copiedKey, pMyUI);
// 							}
// 						}
// 					}
// 				}
// 			} while (FindNextFile(hFind, &findData));
//
// 			FindClose(hFind); // 파일 핸들을 닫음
//
// 		}
//
// 		return S_OK;
// }

// HRESULT CImGuiManager::SaveData(const _tchar* mapTag)
// {
// 	wstring m_strText = L"UIData.dat";
//
//
// 	OPENFILENAME    open;
// 	TCHAR   lpstrFile[MAX_PATH] = L"";
// 	static TCHAR filter[] = L"*.dat";
//
// 	ZeroMemory(&open, sizeof(OPENFILENAME));
// 	open.lStructSize = sizeof(OPENFILENAME);
// 	open.lpstrFilter = filter;
// 	open.lpstrFile = lpstrFile;
// 	open.nMaxFile = 2562;
// 	open.lpstrInitialDir = L"";
//
// 	GetModuleFileName(NULL, lpstrFile, MAX_PATH);
// 	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Client.exe
//
// 	PathRemoveFileSpec(lpstrFile);
// 	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin
//
// 	lstrcat(lpstrFile, L"\\Data\\UI");
// 	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Data\UI
//
// 	//basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());
// 	const _tchar* aa = m_strText.c_str();
//
// 	wcscat_s(lpstrFile, L"\\");
// 	wcscat_s(lpstrFile, aa);
//
//
// 	if (GetSaveFileName(&open) != 0) {
//
// 		SaveUI(m_strText, lpstrFile);
//
//
// 		MSG_BOX("저장 완료");
// 		return S_OK;
// 	}
// }


//
// void CImGuiManager::SaveUI(wstring wstrFileName, wstring wstrFilePath)
// {
// 	FILE* op = NULL;
// 	//lpstrFile
//
// 	_wfopen_s(&op, wstrFileName.c_str(), L"w");
//
// 	if (op == NULL)
// 		return;
//
// 	//        fwprintf(op, L"#KEY             TEXTURE                  SIZE(X,Y,Z)                   POS(X,Y,Z)\n");
//
// 	for (const auto& iter : m_mapChoiceUI)
// 	{
//
// 		wstring wstrkey = iter.first;
// 		_vec3                  vSize = iter.second->Get_Info()->vSize;
// 		_vec3                  vPos = iter.second->Get_Info()->vPos;
//
//
// 		fwprintf(op, L"%s,%f,%f,%f,%f,%f,%f\n", wstrkey.c_str(), vSize.x, vSize.y, vSize.z, vPos.x, vPos.y, vPos.z);
// 	}
//
// 	if (op == NULL)
// 	{
// 		MSG_BOX("op 널이다~");
// 	}
//
// 	fclose(op);
// }

// CMyUI* CImGuiManager::FindUI(const _tchar* szKey)
// {
// 	auto iter = find_if(m_mapLoadUI1.begin(), m_mapLoadUI1.end(), CTag_Finder(szKey));
//
// 	if (iter == m_mapLoadUI1.end())
// 		return nullptr;
//
// 	return iter->second;
// }

#pragma endregion


#pragma region 유진 파일 Save/Load 관련 함수


//////////////////////////Data 초반 Load//////////////////////////////

void CImGuiManager::LoadTexturesFromDirectory(const wchar_t* folderPath, vector<IDirect3DCubeTexture9*>& textureVector)
{
	WIN32_FIND_DATA findData;
	wstring wfolderPath = (wstring)folderPath + L"\\*";
	string	m_NameTemp;

	HANDLE hFind = FindFirstFileW(wfolderPath.c_str(), &findData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			int bufferSize = WideCharToMultiByte(CP_UTF8, 0, findData.cFileName, -1, nullptr, 0, nullptr, nullptr);
			char* buffer = new char[bufferSize];
			WideCharToMultiByte(CP_UTF8, 0, findData.cFileName, -1, buffer, bufferSize, nullptr, nullptr);

			wstring filePath = (wstring)folderPath + L"\\" + findData.cFileName;

			// 파일인 경우, dds 파일인지 확인하고 로드
			if (wcsstr(findData.cFileName, L".dds"))
			{
				IDirect3DCubeTexture9* pTexture = nullptr;
				if (SUCCEEDED(D3DXCreateTextureFromFile(Engine::Get_GraphicDev(), filePath.c_str(), (LPDIRECT3DTEXTURE9*)&pTexture)))
				{
					m_defSortCube = new SORTCUBE;

					m_NameTemp = buffer;
					string	stFileTemp = ".dds";

					m_NameTemp = m_NameTemp.substr(8);
					size_t  found = m_NameTemp.find(stFileTemp);

					m_defSortCube->stFileName = buffer;
					m_defSortCube->tTexture = pTexture;

					if (found != std::string::npos) //Find가 안 된게 아니라면
					{
						m_NameTemp.erase(found, stFileTemp.length());
					}

					m_defSortCube->iNameNumber = stoi(m_NameTemp);

					m_pCubeForSort.push_back(m_defSortCube);

					m_iIndex++;
				}
			}

		} while (FindNextFile(hFind, &findData));

		FindClose(hFind);
	}

	sort(m_pCubeForSort.begin(), m_pCubeForSort.end(),
		[](const SORTCUBE* pCube1, const SORTCUBE* pCube2)
		{	return pCube1->iNameNumber < pCube2->iNameNumber;	});

	for (auto& iter : m_pCubeForSort)
	{
		textureVector.push_back(iter->tTexture);
	}

	for (auto& iter : m_pCubeForSort)
	{
		Safe_Delete(iter);
	}
	m_pCubeForSort.clear();
}

void CImGuiManager::LoadTexturesFromDirectory(const wchar_t* folderPath, vector<IDirect3DBaseTexture9*>& textureVector)
{
	WIN32_FIND_DATA findData;
	wstring wfolderPath = (wstring)folderPath + L"\\*";
	string	m_NameTemp;

	HANDLE hFind = FindFirstFileW(wfolderPath.c_str(), &findData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			int bufferSize = WideCharToMultiByte(CP_UTF8, 0, findData.cFileName, -1, nullptr, 0, nullptr, nullptr);
			char* buffer = new char[bufferSize];
			WideCharToMultiByte(CP_UTF8, 0, findData.cFileName, -1, buffer, bufferSize, nullptr, nullptr);

			wstring filePath = (wstring)folderPath + L"\\" + findData.cFileName;

			// 파일인 경우, png 파일인지 확인하고 로드
			if (wcsstr(findData.cFileName, L".png"))
			{
				IDirect3DCubeTexture9* pTexture = nullptr;
				if (SUCCEEDED(D3DXCreateTextureFromFile(Engine::Get_GraphicDev(), filePath.c_str(), (LPDIRECT3DTEXTURE9*)&pTexture)))
				{

					m_defSortTex = new SORTTEX;

					m_NameTemp = buffer;
					string	stFileTemp = ".dds";

					m_NameTemp = m_NameTemp.substr(10);
					size_t  found = m_NameTemp.find(stFileTemp);

					m_defSortTex->stFileName = buffer;
					m_defSortTex->tTexture = pTexture;

					if (found != std::string::npos) //Find가 안 된게 아니라면
					{
						m_NameTemp.erase(found, stFileTemp.length());
					}

					m_defSortTex->iNameNumber = stoi(m_NameTemp);

					m_pTexForSort.push_back(m_defSortTex);

					m_iIndex++;

				}
			}

		} while (FindNextFile(hFind, &findData));

		FindClose(hFind);
	}

	sort(m_pTexForSort.begin(), m_pTexForSort.end(),
		[](const SORTTEX* pTex1, const SORTTEX* pTex2)
		{	return pTex1->iNameNumber < pTex2->iNameNumber;	});

	for (auto& iter : m_pTexForSort)
	{
		textureVector.push_back(iter->tTexture);
	}

	for (auto& iter : m_pTexForSort)
	{
		Safe_Delete(iter);
	}
	m_pTexForSort.clear();
}




//////////////////////////MapData//////////////////////////////

void CImGuiManager::Save_MapData()
{
    string m_strText = "MapData";
    vector<CUBE*> m_BuildCube = {};
    OPENFILENAME    open;
    TCHAR   lpstrFile[MAX_PATH] = L"";
    static TCHAR filter[] = L"*.dat";


    ZeroMemory(&open, sizeof(OPENFILENAME));
    open.lStructSize = sizeof(OPENFILENAME);
    open.lpstrFilter = filter;
    open.lpstrFile = lpstrFile;
    open.nMaxFile = 256;
    open.lpstrInitialDir = L"";

    GetModuleFileName(NULL, lpstrFile, MAX_PATH);
    //C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Client.exe

    PathRemoveFileSpec(lpstrFile);
    //C:\Users\wnqj4\Desktop\SR_Project\Client\Bin

    lstrcat(lpstrFile, L"\\Data\\Map");
    //C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Data\Map

    basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());
    const _tchar* aa = converted.c_str();

    wcscat_s(lpstrFile, L"\\");
    wcscat_s(lpstrFile, aa);


    if (GetSaveFileName(&open) != 0) {

        HANDLE hFile = CreateFile(lpstrFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

        if (INVALID_HANDLE_VALUE == hFile)
            return;

        DWORD   dwByte = 0;
        DWORD   dwStrByte = 0;

        dwStrByte = sizeof(CHAR) * (m_strText.length() + 1);

        WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
        //문자열 데이터의 크기 dwStrByte를 파일에 쓰고, 데이터의 크기를 알려주는 역할
        WriteFile(hFile, m_strText.c_str(), dwStrByte, &dwByte, nullptr);
        //문자열 m_strText를 파일에 쓰는데, 직전 단계에서 계산한 크기만큼 쓰여짐

        vector<CUBE*> vectorCubeTemp = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecCubeData();
		for (auto& iter : vectorCubeTemp) {  WriteFile(hFile, iter, sizeof(CUBE), &dwByte, nullptr);   }

        CloseHandle(hFile);
        MSG_BOX("Save Complete.");
    }

}

void CImGuiManager::Load_MapData()
{
	string m_strText = "MapData";
	OPENFILENAME    open;
	TCHAR   lpstrFile[MAX_PATH] = L"";
	static TCHAR filter[] = L"*.dat";
	auto& MapSceneTemp = Engine::Management()->Get_MapScene();

	ZeroMemory(&open, sizeof(OPENFILENAME));
	open.lStructSize = sizeof(OPENFILENAME);
	open.lpstrFilter = filter;
	open.lpstrFile = lpstrFile;
	open.nMaxFile = 256;
	open.lpstrInitialDir = L"";
	open.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	GetModuleFileName(NULL, lpstrFile, MAX_PATH);
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Client.exe
	PathRemoveFileSpec(lpstrFile);
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin
	lstrcat(lpstrFile, L"\\Data\\Map");
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Data\Map

	if (GetOpenFileName(&open) != 0) {

		for (auto& iter : MapSceneTemp)
		{
			if (iter.first == SCENETAG::MAPTOOL)
			{
				vectorCubeTemp = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecCubeData();

				for_each(vectorCubeTemp.begin(), vectorCubeTemp.end(), CDeleteObj2());
				vectorCubeTemp.clear();
			}
			//Stage 들은 직접 그 내부에서 Load 함수 만들어서 쓸 것.
		}
		HANDLE hFile = CreateFile(lpstrFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD   dwByte = 0;
		DWORD   dwStrByte = 0;
		CUBE* pCube = nullptr;

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		CHAR* pTag = new CHAR[dwStrByte];

		ReadFile(hFile, pTag, dwStrByte, &dwByte, nullptr);
		m_strText = pTag;

		basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());
		const _tchar* aa = converted.c_str();

		while (true)
		{
			pCube = new CUBE;
			ReadFile(hFile, pCube, sizeof(CUBE), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pCube);
				break;
			}
			vectorCubeTemp.push_back(pCube);
		}
		CloseHandle(hFile);

		for (auto& iter : MapSceneTemp)
		{
			if (iter.first == SCENETAG::MAPTOOL)
				dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Set_VecCubeData(&vectorCubeTemp);

			//Stage 들은 직접 그 내부에서 Load 함수 만들어서 쓸 것.
		}

		m_bLoad_Check = true;
		MSG_BOX("Load Complete.");
	}
}



///////////////////////////OBJ데이터//////////////////////////////

void CImGuiManager::Save_ObjData()
{
	string m_strText = "OBJData";
	vector<OBJData*> m_BuildOBJ = {};
	OPENFILENAME    open;
	TCHAR   lpstrFile[MAX_PATH] = L"";
	static TCHAR filter[] = L"*.dat";


	ZeroMemory(&open, sizeof(OPENFILENAME));
	open.lStructSize = sizeof(OPENFILENAME);
	open.lpstrFilter = filter;
	open.lpstrFile = lpstrFile;
	open.nMaxFile = 256;
	open.lpstrInitialDir = L"";

	GetModuleFileName(NULL, lpstrFile, MAX_PATH);
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Client.exe

	PathRemoveFileSpec(lpstrFile);
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin

	lstrcat(lpstrFile, L"\\Data\\OBJ");
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Data\OBJ

	basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());
	const _tchar* aa = converted.c_str();

	wcscat_s(lpstrFile, L"\\");
	wcscat_s(lpstrFile, aa);


	if (GetSaveFileName(&open) != 0) {

		{HANDLE hFile = CreateFile(lpstrFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			if (INVALID_HANDLE_VALUE == hFile)
				return;

			DWORD   dwByte = 0;
			DWORD   dwStrByte = 0;

			dwStrByte = sizeof(CHAR) * (m_strText.length() + 1);

			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			//문자열 데이터의 크기 dwStrByte를 파일에 쓰고, 데이터의 크기를 알려주는 역할
			WriteFile(hFile, m_strText.c_str(), dwStrByte, &dwByte, nullptr);
			//문자열 m_strText를 파일에 쓰는데, 직전 단계에서 계산한 크기만큼 쓰여짐

			vector<OBJData*> vectorOBJTemp = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecOBJData();
			for (auto& iter : vectorOBJTemp) { WriteFile(hFile, iter, sizeof(OBJData), &dwByte, nullptr); }

		CloseHandle(hFile); }

		MSG_BOX("Save Complete.");
	}
}


void CImGuiManager::Save_CPointData()
{
	string m_strText = "CPointData";
	vector<C_POINT*> m_CPOINT = {};
	OPENFILENAME    open;
	TCHAR   lpstrFile[MAX_PATH] = L"";
	static TCHAR filter[] = L"*.dat";


	ZeroMemory(&open, sizeof(OPENFILENAME));
	open.lStructSize = sizeof(OPENFILENAME);
	open.lpstrFilter = filter;
	open.lpstrFile = lpstrFile;
	open.nMaxFile = 256;
	open.lpstrInitialDir = L"";

	GetModuleFileName(NULL, lpstrFile, MAX_PATH);
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Client.exe

	PathRemoveFileSpec(lpstrFile);
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin

	lstrcat(lpstrFile, L"\\Data\\CPoint");
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Data\CPoint

	basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());
	const _tchar* aa = converted.c_str();

	wcscat_s(lpstrFile, L"\\");
	wcscat_s(lpstrFile, aa);


	if (GetSaveFileName(&open) != 0) {

		{HANDLE hFile = CreateFile(lpstrFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD   dwByte = 0;
		DWORD   dwStrByte = 0;

		dwStrByte = sizeof(CHAR) * (m_strText.length() + 1);

		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		//문자열 데이터의 크기 dwStrByte를 파일에 쓰고, 데이터의 크기를 알려주는 역할
		WriteFile(hFile, m_strText.c_str(), dwStrByte, &dwByte, nullptr);
		//문자열 m_strText를 파일에 쓰는데, 직전 단계에서 계산한 크기만큼 쓰여짐

		vector<C_POINT*> vectorCPointTemp = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecCreatePoint();
		for (auto& iter : vectorCPointTemp) { WriteFile(hFile, iter, sizeof(C_POINT), &dwByte, nullptr); }

		CloseHandle(hFile); }

		MSG_BOX("Save Complete.");
	}


}

void CImGuiManager::Save_TriggerData()
{
	string m_strText = "TriggerData";
// 	vector<TRIGGER*> m_VectempTrigger = {};
	OPENFILENAME    open;
	TCHAR   lpstrFile[MAX_PATH] = L"";
	static TCHAR filter[] = L"*.dat";


	ZeroMemory(&open, sizeof(OPENFILENAME));
	open.lStructSize = sizeof(OPENFILENAME);
	open.lpstrFilter = filter;
	open.lpstrFile = lpstrFile;
	open.nMaxFile = 256;
	open.lpstrInitialDir = L"";

	GetModuleFileName(NULL, lpstrFile, MAX_PATH);
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Client.exe

	PathRemoveFileSpec(lpstrFile);
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin

	lstrcat(lpstrFile, L"\\Data\\Trigger");
	//C:\Users\wnqj4\Desktop\SR_Project\Client\Bin\Data\Trigger

	basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());
	const _tchar* aa = converted.c_str();

	wcscat_s(lpstrFile, L"\\");
	wcscat_s(lpstrFile, aa);


	if (GetSaveFileName(&open) != 0) {

		HANDLE hFile = CreateFile(lpstrFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD   dwByte = 0;
		DWORD   dwStrByte = 0;

		dwStrByte = sizeof(CHAR) * (m_strText.length() + 1);

		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		//문자열 데이터의 크기 dwStrByte를 파일에 쓰고, 데이터의 크기를 알려주는 역할
		WriteFile(hFile, m_strText.c_str(), dwStrByte, &dwByte, nullptr);
		//문자열 m_strText를 파일에 쓰는데, 직전 단계에서 계산한 크기만큼 쓰여짐

		m_VecListbox = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecTrigger();
		for (auto& iter : m_VecListbox) { WriteFile(hFile, iter, sizeof(TRIGGER), &dwByte, nullptr); }

		CloseHandle(hFile);
		MSG_BOX("Save Complete.");
	}

}



///////////////////////////////////////////////////////////////////////

#pragma endregion



void CImGuiManager::Free()
{

	////while (!vectorCubeTemp.empty())
	////{
	////	delete vectorCubeTemp.back();
 //   //  vectorCubeTemp.back() = nullptr;

 //   //    vectorCubeTemp.pop_back();
	////}
 //   //vectorCubeTemp.clear();


	//
	// for (int i = 0; i < m_vecUIList.size(); ++i)
	// {
	//	 Safe_Release(m_vecUIList[i]);
	// }

	// for (int i = 0; i < m_vecUIName.size(); ++i)
	// {
	//	 delete[] m_vecUIName[i];
	// }

	// for_each(m_mapLoadUI1.begin(), m_mapLoadUI1.end(), CDeleteMap());
	// for (auto iter : m_mapLoadUI1)
	// {
	//	 delete[] iter.first;
	// }

	// m_mapLoadUI1.clear();

	Safe_Delete(m_TriggerTemp);
	Safe_Delete(m_defSortCube);
	Safe_Delete(m_defSortTex);

	 ImGui_ImplDX9_Shutdown();
	 ImGui_ImplWin32_Shutdown();
	 ImGui::DestroyContext();

}
