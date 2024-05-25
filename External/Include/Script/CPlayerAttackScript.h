#pragma once
#include <Engine\CScript.h>

class CPlayerAttackScript :
    public CScript
{
private:
    bool    m_bBossMap;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

private:
    void CreateSlash(Vec3 _OtherPos);

    CLONE(CPlayerAttackScript);
public:
    CPlayerAttackScript();
    ~CPlayerAttackScript();
};

