#pragma once
#include <Engine\CScript.h>
class CNormalWallScript :
    public CScript
{
private:
    bool        m_bLeft;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CNormalWallScript);
public:
    CNormalWallScript();
    ~CNormalWallScript();
};

