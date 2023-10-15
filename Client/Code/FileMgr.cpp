#include "FileMgr.h"

CFileMgr::CFileMgr() 
{

}

CFileMgr::CFileMgr(const TCHAR* pFilePath) 
{
	m_hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
}

CFileMgr::~CFileMgr() 
{
	if (IsOpen()) {
		CloseHandle(m_hFile);
	}
}

bool CFileMgr::IsOpen() const 
{
	return m_hFile != INVALID_HANDLE_VALUE;
}

void CFileMgr::ReadData(void* pData, DWORD dwSize) 
{
	if (IsOpen()) 
	{
		DWORD bytesRead;
		ReadFile(m_hFile, pData, dwSize, &bytesRead, nullptr);
	}
}

void CFileMgr::Seek(DWORD dwPosition) 
{
	if (IsOpen()) 
	{
		SetFilePointer(m_hFile, dwPosition, nullptr, FILE_BEGIN);
	}
}

bool CFileMgr::IsEndOfFile() const 
{
	if (IsOpen()) 
	{
		return GetFilePointer() == GetFileSize();
	}
	return false;
}

void CFileMgr::Free()
{
}

DWORD CFileMgr::GetFilePointer() const 
{
	if (IsOpen()) 
	{
		return SetFilePointer(m_hFile, 0, nullptr, FILE_CURRENT);
	}
	return 0;
}

DWORD CFileMgr::GetFileSize() const 
{
	if (IsOpen()) 
	{
		//return GetFileSize(m_hFile, nullptr);
	}
	return 0;
}
