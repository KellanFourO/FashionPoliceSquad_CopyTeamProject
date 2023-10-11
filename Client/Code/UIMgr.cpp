#include "stdafx.h"
#include "UIMgr.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include <commdlg.h>
#include <Shlwapi.h>
#include <sstream>
#include <utility>
#include "Loading.h"

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
	Free();
}


void CUIMgr::Ready_UIMgr()
{
	ReadImgPath(L"..\\Bin\\Resource\\Texture\\UI");
	Load_UI();

}

void CUIMgr::Load_UI()
{
	wifstream fin;

	fin.open(L"../Bin/Data/UI/UIdata.dat");

	if (!fin.fail())
	{
		wstring line;

		while (getline(fin, line)) // �� �پ� �б�
		{
			wistringstream iss(line);
			wstring token;

			_tchar szKey[MAX_PATH] = L"";
			_tchar szSizeX[MAX_PATH] = L"";
			_tchar szSizeY[MAX_PATH] = L"";
			_tchar szSizeZ[MAX_PATH] = L"";
			_tchar szPosX[MAX_PATH] = L"";
			_tchar szPosY[MAX_PATH] = L"";
			_tchar szPosZ[MAX_PATH] = L"";

			getline(iss, token, L','); // ','�� �����ڷ� ����Ͽ� ���� �ڸ�
			_tcscpy_s(szKey, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szSizeX, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szSizeY, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szSizeZ, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szPosX, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szPosY, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szPosZ, MAX_PATH, token.c_str());

			_vec3 vSize = { (float)_ttof(szSizeX), (float)_ttof(szSizeY), (float)_ttof(szSizeZ) };
			_vec3 vPos = { (float)_ttof(szPosX), (float)_ttof(szPosY), (float)_ttof(szPosZ) };


			CMyUI* pMyUI = Get_UI(szKey);
			pMyUI->SetData(vSize, vPos);

		}
	}
	fin.close();
}

void CUIMgr::ReadImgPath(const _tchar* folderPath)
{
	//���� �� ���丮 ������ �����ϱ� ���� ����ü
	WIN32_FIND_DATA findData;

	//floderPath�� ���� wFolderPath�� ���� (���� ��θ� �����ڵ幮�ڿ��� ó��)

	wstring wfolderPath = (wstring)folderPath + L"\\*.*";

	HANDLE hFind = FindFirstFileW(wfolderPath.c_str(), &findData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		//���� ���� ��� ���ϰ� ���丮�� �˻� FindNextFile�Լ��� ����ؼ� �������� �Ǵ� ���丮�� ã�´�
		do
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// ���丮�� ���, "."(���� ���丮)�� ".."(���� ���丮)�� ����
				if (lstrcmp(findData.cFileName, L".") != 0 && lstrcmp(findData.cFileName, L"..") != 0)
				{
					// ���� ������ ��� ����
					wstring subFolderPath = (wstring)folderPath + L"\\" + findData.cFileName;
					// ��� ȣ��� ���� ���丮 �˻�

					ReadImgPath(subFolderPath.c_str());

				}
			}
			else
			{
				// ������ ���, �̹��� �������� Ȯ���ϰ� �ε�
				wstring filePath = (wstring)folderPath + L"\\" + findData.cFileName;

				if (wcsstr(findData.cFileName, L".png") || wcsstr(findData.cFileName, L".jpg") ||
					wcsstr(findData.cFileName, L".bmp") || wcsstr(findData.cFileName, L".tga"))
				{
					IDirect3DBaseTexture9* pTexture = nullptr;
					if (SUCCEEDED(D3DXCreateTextureFromFile(Engine::Get_GraphicDev(), filePath.c_str(), (LPDIRECT3DTEXTURE9*)&pTexture)))
					{
						const _tchar* szKey = findData.cFileName;
						CMyUI* pMyUI = nullptr;

						if (!Get_UI(szKey))
						{
							// szKey�� �����Ͽ� �������� �Ҵ�
							_tchar* copiedKey = new _tchar[_tcslen(szKey) + 1];
							_tcscpy_s(copiedKey, _tcslen(szKey) + 1, szKey);

							if (wcsstr(findData.cFileName, L"HP"))
							{
								pMyUI = CMyUI::Create(Engine::Get_GraphicDev(), pTexture, UI_TYPE::HP);
							}

							else if (wcsstr(findData.cFileName, L"VALUE"))
							{
								pMyUI = CMyUI::Create(Engine::Get_GraphicDev(), pTexture, UI_TYPE::VALUE);
							}

							else if (wcsstr(findData.cFileName, L"SPRITE"))
							{
								pMyUI = CMyUI::Create(Engine::Get_GraphicDev(), pTexture, UI_TYPE::SPRITE);
							}
							else if (wcsstr(findData.cFileName, L"BULLET"))
							{
								pMyUI = CMyUI::Create(Engine::Get_GraphicDev(), pTexture, UI_TYPE::BULLET);
							}
							else
							{
								pMyUI = CMyUI::Create(Engine::Get_GraphicDev(), pTexture, UI_TYPE::BASIC);
							}

							// �������� �Ҵ��� copiedKey�� �ʿ� �־�� �մϴ�.
							m_mapLoadUI.emplace(copiedKey, pMyUI);
						}


					}
				}
			}
		} while (FindNextFile(hFind, &findData));

		FindClose(hFind); // ���� �ڵ��� ����

	}

}

void CUIMgr::Free()
{

	for_each(m_mapLoadUI.begin(), m_mapLoadUI.end(), CDeleteMap());

	for (auto iter : m_mapLoadUI)
	{

		delete[] iter.first;
	}

	m_mapLoadUI.clear();


	//for_each(m_mapLoadUI.begin(), m_mapLoadUI.end(),
	//	[](std::pair<const _tchar*, CMyUI*> pair)
	//	{
	//		delete[] pair.first; /
	//
	// / ���ڿ� Ű�� �޸� ����
	//	});


}
