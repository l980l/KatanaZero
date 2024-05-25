#pragma once
#include <Engine\CScript.h>

class CCameraZoomScript :
    public CScript
{
private:
    float       m_Zoom;
    bool        m_StartMenu;
    Vec3        m_StartPos;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CCameraZoomScript);
public:
    CCameraZoomScript();
    ~CCameraZoomScript();
};

