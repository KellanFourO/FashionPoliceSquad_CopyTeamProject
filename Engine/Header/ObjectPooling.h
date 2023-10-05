#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CObjectPooling : public CBase
{
	DECLARE_SINGLETON(CObjectPooling)
private:
	explicit CObjectPooling();
	virtual ~CObjectPooling();

public:
	void	Initialize();
	void	Reset();


};

END