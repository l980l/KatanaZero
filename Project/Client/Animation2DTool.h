#pragma once
#include "UI.h"

#include <Engine\ptr.h>
#include <Engine\CRes.h>

class Animation2DTool :
    public UI
{
public:
    Animation2DTool();
    ~Animation2DTool();

private:
    class CGameObject*      m_pTarget;
    class CAnim2D*          m_SelectedAnim2D;
    int                     m_SelectedIndex;

    int                     m_FrameSlice[2];
    Vec2                    m_FrameLT;

    ImVec2                  m_FrameLTUV;
    ImVec2                  m_FrameRBUV;

    int                     m_TempAnimID;

    bool                    m_bAnimChange;

public:
    void SetTargetObject(class CGameObject* _Target) { m_pTarget = _Target; }
    void SetAnim2D(class CAnim2D* Anim2D) { m_SelectedAnim2D = Anim2D; m_bAnimChange = true; }

public:
    void SelectAnimation(DWORD_PTR _Key);
    void CreateAnim2D(DWORD_PTR _Key);

public:
    virtual void finaltick() override;
    virtual int render_update() override;

public:
    void Reset();

private:
    void Clear();
};

