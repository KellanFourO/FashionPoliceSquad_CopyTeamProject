#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "MyUI.h"

class CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)

private:
	 CUIMgr();
	~CUIMgr();

public:

	const map<const _tchar*, CMyUI*>& Get_MapLoadUI() { return m_mapLoadUI; }

	void							Ready_UIMgr();
	void							Load_UI();
	void							ReadImgPath(const _tchar* folderPath);

	CMyUI* Get_UI(const _tchar* pUIKey)
	{

		auto iter = find_if(m_mapLoadUI.begin(), m_mapLoadUI.end(), CTag_Finder(pUIKey));

		if (iter == m_mapLoadUI.end())
			return nullptr;

		return iter->second;


	}


	CMyUI* Get_UI(const _tchar* pUIKey, CGameObject* pTarget)
	{
		auto iter = find_if(m_mapLoadUI.begin(), m_mapLoadUI.end(), CTag_Finder(pUIKey));

		if (iter == m_mapLoadUI.end())
			return nullptr;

		dynamic_cast<CMyUI*>(iter->second)->Set_Target(pTarget);
		return iter->second;
	}

	CMyUI* Get_UI_Clone(const _tchar* pUIKey)
	{
		auto iter = find_if(m_mapLoadUI.begin(), m_mapLoadUI.end(), CTag_Finder(pUIKey));

		if (iter == m_mapLoadUI.end())
			return nullptr;


		//TODO - 승용 // new CMyUI(*iter->second) 실패, CMyUI* pOrigin = iter->second; 까진 성공, CMyUI* pClone = new CMyUI(*pOrigin); 실패
		// TODO - 승용 아래 코드처럼 액세스권한이 있는 복사할 대상이 자신이니 자신에게 접근해서 해주는게 해결책 ㅋ
		return iter->second->Get_Clone();
	}

private:
	void Free();


private:
	map<const _tchar*, CMyUI*>			m_mapLoadUI;




};

