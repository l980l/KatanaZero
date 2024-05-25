#pragma once
#include <Engine\CScript.h>
class CNormalFloorScript :
    public CScript
{
private:
    list<CCollider2D*>    m_Collider2DList;
    int         m_iFloorLevel;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CNormalFloorScript);
public:
    CNormalFloorScript();
    ~CNormalFloorScript();
};

