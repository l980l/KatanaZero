#pragma once
#include <Engine\CScript.h>
class CGravityScript :
    public CScript
{
private:
    int     m_iGround;
    float   m_fGravityVelocity;
    bool    m_bJump;
    bool    m_bAttack;

    bool    m_bOn;

public:
    void AddGround() { ++m_iGround; }
    void ReleaseGround() { --m_iGround; }
    void SetGravityVelocity(float _fGravityVelocity) { m_fGravityVelocity = _fGravityVelocity; }
    void ClearGravityVelocity();
    
    void Jump();
    void HeadHunterJump();
    void Attack();
    void SetOnOff(bool _bOn) { m_bOn = _bOn; }

public:
    virtual void tick() override;



private:
    CLONE(CGravityScript);
public:
    CGravityScript();
    ~CGravityScript();
};

