#pragma once
#include <Engine\CScript.h>

class CGrenadeScript :
    public CScript
{
private:
    float       m_fSpeed;
    Vec3        m_vDir;
    int         m_iGround;
    int         m_iLWall;
    int         m_iRWall;
    int         m_iCeiling;
    bool        m_bBomb;
    Vec3        m_vScale;
    bool        m_bGrenade;

public:
    void SetSpeed(float _Speed) { m_fSpeed = _Speed; }

    void SetDir(Vec3 _Dir) { m_vDir = _Dir; }
    Vec3 GetDir() { return m_vDir; }
    void SetGrenade(bool Grenade) { m_bGrenade = Grenade; }

public:
    void AddGround() { ++m_iGround; }
    void ReleaseGround() { --m_iGround; }
    void AddCeiling() { ++m_iCeiling; }
    void ReleaseCeiling() { --m_iCeiling; }
    void AddLWall() { ++m_iLWall; }
    void ReleaseLWall() { --m_iLWall; }
    void AddRWall() { ++m_iRWall; }
    void ReleaseRWall() { --m_iRWall; }

private:
    void CreateExplosion(Vec3 _OtherPos);

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

    CLONE(CGrenadeScript);
public:
    CGrenadeScript();
    ~CGrenadeScript();

    friend class CHeadhunterScript;
};

