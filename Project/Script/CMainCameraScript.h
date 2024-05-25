#pragma once
#include <Engine\CScript.h>

class CMainCameraScript :
    public CScript
{
private:
    Vec3    m_MapSize;
    Vec3    m_MapOffset;
    Vec3    m_MapLT;
    Vec3    m_MapRB;
    bool    m_bStart;
    bool    m_bColliderCamera;
    bool    m_bColliderOn;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMainCameraScript);
public:
    CMainCameraScript();
    ~CMainCameraScript();
};

