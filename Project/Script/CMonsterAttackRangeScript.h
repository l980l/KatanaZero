#pragma once
#include <Engine\CScript.h>
class CMonsterAttackRangeScript :
    public CScript
{
private:
    bool                    m_bIsLeft;
    bool                    m_bGun;
    list<CCollider2D*>      m_Collider2DList;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CMonsterAttackRangeScript);
public:
    CMonsterAttackRangeScript();
    ~CMonsterAttackRangeScript();
};

