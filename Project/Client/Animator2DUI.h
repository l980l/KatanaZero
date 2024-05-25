#pragma once
#include "ComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
public:
    string                  m_SelectedAnim2DName;
    class CAnim2D*          m_SelectedAnim2D;
    Ptr<class CTexture>     m_AtlasTex;
    float                   m_fTime;
    int                     m_iCurFrm;

public:
    virtual int render_update() override;

public:
    void SelectAnimation(DWORD_PTR _Key);

public:
    Animator2DUI();
    ~Animator2DUI();
};

