#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CCamera;

END

class CFlyingCamera : public CGameObject
{
private:
    explicit        CFlyingCamera(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit        CFlyingCamera(const CFlyingCamera& rhs);
    virtual         ~CFlyingCamera();

public:
    virtual HRESULT Ready_GameObject(void)                      override;
    virtual _int    Update_GameObject(const _float& fTimeDelta) override;
    virtual void    LateUpdate_GameObject(void)                 override;
    virtual void    Render_GameObject(void) {};

public:
    CCameraCom*     Get_Camera() { return m_pCamera; }
    _int            Get_CameraMode() { return (_int)m_eCameraMode; }
    _bool           Is_MouseFree() { return m_isMouseFree; }

    void            Change_Mode();
    void            Set_MouseFix(_bool _isMouseFree) { m_isMouseFree = _isMouseFree; }

private:
    HRESULT		    Add_Component(void);
    void		    Key_Input(const _float& fTimeDelta);

    void			Mouse_Fix();

private:
    enum class CAMERA_MODE { FIRST_PERSON, THIRD_PERSON };

    _bool		    m_bCheck = false;
    _float			m_fSpeed = 30.f;

    CCameraCom*     m_pCamera = nullptr;

    CAMERA_MODE		m_eCameraMode;
    _bool			m_isMouseFree = false;


public:
    static CFlyingCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();
};

