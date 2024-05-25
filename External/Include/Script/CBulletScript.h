#pragma once
#include <Engine\CScript.h>

class CBulletScript :
    public CScript
{
private:
    bool        m_bReflected; // 플레이어의 것인지 몬스터의 것인지.
    Vec3        m_vDir;     // 이동 방향
    float       m_fSpeed;   // 이동 속력
    bool        m_bBossMap;

public:
    void SetReflected(bool _Reflected) { m_bReflected = _Reflected; }
    void SetSpeed(float _Speed) { m_fSpeed = _Speed; }
    void SetDir(Vec3 _Dir) { m_vDir = _Dir; }
    Vec3 GetDir() { return m_vDir; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

private:
    void CreateSlash(Vec3 _OtherPos);
    void CreateReflectFx(Vec3 _OtherPos);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CBulletScript);
public:
    CBulletScript();
    ~CBulletScript();

    friend class CMonsterGruntScript;
};

