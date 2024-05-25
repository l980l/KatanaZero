#pragma once
#include <Engine\CScript.h>
class CTimerScript :
    public CScript
{
private:
    float       m_fTimelimit;
    float       m_fPassedTime;
    Vec3       m_fPos;
    Vec3       m_fScale;
    bool        m_bTimeEnd;
    float       m_fPurpleShaderTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CTimerScript);
public:
    CTimerScript();
    ~CTimerScript();
};

