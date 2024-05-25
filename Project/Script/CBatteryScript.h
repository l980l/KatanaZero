#pragma once
#include <Engine\CScript.h>

class CBatteryScript :
    public CScript
{
private:
    float   m_fBattery;
    bool    m_bTimeSlow;
    class CPlayerScript* m_PlayerScript;
    class CGameObject* m_DarkPostprocess;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CBatteryScript);

public:
    CBatteryScript();
    ~CBatteryScript();
};

