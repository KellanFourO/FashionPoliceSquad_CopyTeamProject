#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture :public CComponent
{
private:
	explicit CTexture();
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT					Ready_Texture(TEXTUREID eType, const _tchar* pPath, const _uint& iCnt);
	void					Render_Textrue(const _uint& iIndex = 0);

	void					Render_OBJTextrue(IDirect3DCubeTexture9* pTexture); //OBJ텍스처 전용
	void					Render_OBJTextrue(IDirect3DBaseTexture9* pTexture);

	IDirect3DBaseTexture9*	Get_Texture(_uint iIndex) { return m_vecTexture[iIndex]; }
	void					Set_Texture(IDirect3DBaseTexture9* pTexture, _uint iIndex) { m_vecTexture[iIndex] = pTexture; } // 승용 추가
private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	map<_uint, IDirect3DBaseTexture9*>  m_mapOBJTexture; //유진: OBJ 텍스처 구분용

	// 유진 - OBJ 텍스쳐 인덱스
	const int			cubeTextureStartIndex = 1000;
	const int			planeTextureStartIndex = 3000;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev,
			TEXTUREID eType, _tchar* pPath,	const _uint& iCnt = 1);
	virtual CComponent* Clone();

private:
	virtual void		Free();
};



END