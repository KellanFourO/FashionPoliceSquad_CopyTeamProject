#pragma once

#include "Engine_Define.h"
#include "Base.h"


class CFileMgr : public CBase
{

public:
	explicit	CFileMgr();
	explicit	CFileMgr(const TCHAR* pFilePath);
	virtual		~CFileMgr();

	bool		IsOpen() const;
	void		ReadData(void* pData, DWORD dwSize);
	void		Seek(DWORD dwPosition);
	bool		IsEndOfFile() const;

	DWORD		GetFilePointer() const;
	DWORD		GetFileSize() const;

private:
	HANDLE		m_hFile; // 파일 핸들

private:
	virtual	void	Free();

};

