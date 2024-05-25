#pragma once
#include <Engine\CScript.h>
class CNextLevelScript :
    public CScript
{
private:
    class CPlayerScript* m_PlayerScript;
    int         m_iLevel;
    bool        m_bVideoPlay;
    float       m_fGridPostprocess;

private:
    void NextLevel();
    bool CheckMonsterAllDead();

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;

    CLONE(CNextLevelScript);
public:
    CNextLevelScript();
    ~CNextLevelScript();
};

