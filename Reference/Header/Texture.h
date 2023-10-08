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

	IDirect3DBaseTexture9*	Get_Texture(_uint iIndex) { return m_vecTexture[iIndex]; }
	void					Set_Texture(IDirect3DBaseTexture9* pTexture, _uint iIndex) { m_vecTexture[iIndex] = pTexture; } // �¿� �߰�
private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;

public:
	//////////////////���� - OBJ�ؽ�ó ����//////////////////
	HRESULT					Ready_Texture(TEXTUREID eType, const _tchar* pPath, 
										  const _uint& iCnt, OBJ_TYPE eOBJType);

	void					Render_ObjCubeTex(const _uint& iIndex = 0);
	void					Render_ObjPlaneTex(const _uint& iIndex = 0);
	
	static	CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev,	TEXTUREID eType, 
							_tchar* pPath, OBJ_TYPE eOBJType, const _uint& iCnt = 1);

private:

	vector<IDirect3DCubeTexture9*>		m_vecCubeTexture;
	vector<IDirect3DBaseTexture9*>		m_vecPlaneTexture;

	const int			cubeTextureStartIndex = 1000;
	const int			planeTextureStartIndex = 3000;
	////////////////////////////////////////////////////////////////

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev,
			TEXTUREID eType, _tchar* pPath,	const _uint& iCnt = 1);

	virtual CComponent* Clone();

private:
	virtual void		Free();
};



END