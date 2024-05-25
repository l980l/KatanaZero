#pragma once
#include <Engine\CScript.h>
class CDoorScript :
    public CScript
{
private:
    bool                    m_bLeft;

    list<CCollider2D*>      m_Collider2DList;
    float                   m_fDoorTime;
    bool                    m_bOpen;

public:
    bool IsOpen() { return m_bOpen; }

private:
    void ReleaseDoorAll();

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CDoorScript);

public:
    CDoorScript();
    ~CDoorScript();
};

