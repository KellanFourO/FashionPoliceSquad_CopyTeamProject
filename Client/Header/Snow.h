#pragma once

#include "Engine_Define.h"
#include "../../Engine/Header/PSystem.h"

BEGIN(Engine)

class BoundingBox;

END
class CSnow :public Engine::CPSystem
{
public:
	CSnow(BoundingBox* boundingBox, int numParticles);
	void resetParticle(Attribute* attribute);
	void Update_Particle(_float fTimeDelta);
};

