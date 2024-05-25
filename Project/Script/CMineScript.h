#pragma once
#include <Engine\CScript.h>
class CMineScript :
    public CScript
{
private:
    float   m_fTime;
    bool    m_bBoom;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

private:
    void CreateExplosion(Vec3 _OtherPos);

public:

    CLONE(CMineScript);
public:
    CMineScript();
    ~CMineScript();
};

