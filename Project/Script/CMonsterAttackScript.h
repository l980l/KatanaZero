#pragma once
#include <Engine\CScript.h>

class CMonsterAttackScript :
    public CScript
{
private:
    bool                    m_bIsLeft;
    CGameObject*            m_Target;
    Vec3                    m_PrevTargetPos;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

public:
    void SetLeft(bool Left) { m_bIsLeft = Left; }
    bool GetLeft() { return m_bIsLeft; }
    void SetTarget(CGameObject* _Target) { m_Target = _Target; m_PrevTargetPos = _Target->Transform()->GetRelativePos(); }

    CLONE(CMonsterAttackScript);
public:
    CMonsterAttackScript();
    ~CMonsterAttackScript();

    friend class CMonsterGruntScript;
};

