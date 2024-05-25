#pragma once
#include <Engine\CScript.h>

class CMonsterRayScript :
    public CScript
{
private:
    bool                    m_bDetect;
    bool                    m_bIsLeft;
    list<CCollider2D*>      m_Collider2DList;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

public:
    void SetLeft(bool Left) { m_bIsLeft = Left; }  
    bool GetLeft() { return m_bIsLeft; }

    CLONE(CMonsterRayScript);
public:
    CMonsterRayScript();
    ~CMonsterRayScript();

    friend class CMonsterGruntScript;
};

