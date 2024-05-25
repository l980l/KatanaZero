#pragma once
#include <Engine\CScript.h>

class CStartMenuScript :
    public CScript
{
private:
    int         m_iSelectedMenu;

public:
    virtual void begin() override;
    virtual void tick() override;

public:

    CLONE(CStartMenuScript);
public:
    CStartMenuScript();
    ~CStartMenuScript();
};

