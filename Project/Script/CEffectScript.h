#pragma once
#include <Engine\CScript.h>

class CEffectScript :
    public CScript
{
private:
    bool            m_bPlayerDustCloud;
    bool            m_bLoop;
    bool            m_bEmotion;

    CGameObject*    m_Target;
    Vec3            m_PrevTargetPos;

private:
    void SetPlayerDustCloud(bool _PlayerDustCloud) { m_bPlayerDustCloud = _PlayerDustCloud; }
    void SetTarget(CGameObject* _Target) { m_Target = _Target; m_PrevTargetPos = _Target->Transform()->GetRelativePos(); }
    void SetEmoation(bool _bool) { m_bEmotion = _bool;}

    void SetLoop(bool Loop) { m_bLoop = Loop; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

public:
    CLONE(CEffectScript);

public:
    CEffectScript();
    ~CEffectScript();

    friend class CPlayerScript;
    friend class CPlayerAttackScript;
    friend class CMonsterAttackScript;
    friend class CMonsterGruntScript;
    friend class CMonsterPompScript;
    friend class CMonsterCopScript;
    friend class CHeadhunterScript;
    friend class CBulletScript;
    friend class CRifleScript;
    friend class CMineScript;
};

