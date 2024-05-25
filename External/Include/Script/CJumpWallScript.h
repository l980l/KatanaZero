#pragma once
#include <Engine\CScript.h>
class CJumpWallScript :
    public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

    CLONE(CJumpWallScript);
public:
    CJumpWallScript();
    ~CJumpWallScript();
};

