#pragma once
#include <Engine\CScript.h>

struct PlayerRewindData
{
    int AnimNum;
    int AnimFrame;
    int AnimIsLeft;
    Vec3 PlayerPos;
    Vec3 PlayerScale;
    Vec3 PlayerRot;
};

class CPlayerScript : 
    public CScript
{
private:
    bool        m_bImmortal;
    int         m_iState;       // 0 alive 1 hurt 2 dead

    float       m_fSpeed;
    int         m_iGround;
    int         m_iGroundToFall;
    int         m_iLWall;
    int         m_iRWall;
    int         m_iCeiling;
    int         m_iLStair;
    int         m_iRStair;

    bool        m_bJump;
    Vec3        m_PrevPlayerPos;
    Vec3        m_NewPlayerPos;
    Vec3        m_PlayerDir;
    Vec3        m_AttackDir;
    int         m_iAttackJump;

    // 0 Player_idle, 1 Player_jump, 2 Player_attack, 3 Player_fall, 4 Player_crouch, 5 Player_roll, 6 Player_idle_to_run, 7 Player_run
    // 8 Player_hurtfly_begin, 9 Player_hurtfly_loop, 10 Player_hurtgrond, 11 Player_doorbreak
    int         m_AnimNum;
    float       m_RewindTime;
    float       m_RewindScale;
    bool        m_bRewind;

    vector<int>   m_vecNextAnim;         // 애니메이션 별로 다음 애니메이션을 저장. 비반복 애니메이션에만 적용. 반복 애니메이션은 -1
    list<PlayerRewindData> m_PlayerRewindDataList;     // Rewind를 위한 데이터. 재생하거나 역재생하기 위해 list로 구현.

    int         m_iFloorLevel;

    Vec3        m_KnockBackDir;     // 뒤로 밀려나는 방향
    float       m_fBleedingTime;
    float       m_fBloodWallTime;

    bool        m_bVideoPlayEnd;

   /* int         m_iBGMChannel;

    bool        m_bTimeSlow;
    bool        m_bBattery;*/

public:
    // 0 Player_idle, 1 Player_jump, 2 Player_attack, 3 Player_fall, 4 Player_crouch, 5 Player_roll, 6 Player_idle_to_run, 7 Player_run
    // 8 Player_hurtfly_begin, 9 Player_hurtfly_loop, 10 Player_hurtgrond, 11 Player_doorbreak
    void SetAnimNum(int _AnimNum) { m_AnimNum = _AnimNum; }
    // 0 Player_idle, 1 Player_jump, 2 Player_attack, 3 Player_fall, 4 Player_crouch, 5 Player_roll, 6 Player_idle_to_run, 7 Player_run
    // 8 Player_hurtfly_begin, 9 Player_hurtfly_loop, 10 Player_hurtgrond, 11 Player_doorbreak
    int GetAnimNum() { return m_AnimNum; }
    void ResetAttackJump() { m_iAttackJump = 0; }
    bool GetRewind() { return m_bRewind; }
    void SetState(int iState) { m_iState = iState; }
    int GetState() { return m_iState; }
    void SetImmortal(bool _bImmortal) { m_bImmortal = _bImmortal; }
    int GetImmortal() { return m_bImmortal; }
    void SetVideoPlayEnd(bool VideoPlayEnd) { m_bVideoPlayEnd = VideoPlayEnd; }
    int GetVideoPlayEnd() { return m_bVideoPlayEnd; }
   /* void SetBattery(bool Battery) { m_bBattery = Battery; }
    int GetBattery() { return m_bBattery; }*/

public:
    void AddGround()                { ++m_iGround; }
    void ReleaseGround()            { --m_iGround; }
    void AddGroundToFall()          { ++m_iGroundToFall; }
    void ReleaseGroundToFall()      { --m_iGroundToFall; }
    void AddCeiling()               { ++m_iCeiling;}
    void ReleaseCeiling()           { --m_iCeiling;}
    void AddLWall()                 { ++m_iLWall;  }
    void ReleaseLWall()             { --m_iLWall;  }
    void AddRWall()                 { ++m_iRWall;  }
    void ReleaseRWall()             { --m_iRWall;  }
    void AddLStair()                { ++m_iLStair; }
    void ReleaseLStair()            { --m_iLStair; }
    void AddRStair()                { ++m_iRStair; }
    void ReleaseRStair()            { --m_iRStair; }

    int GetGroundToFall() { return m_iGroundToFall; }

    Vec3 GetPrevPlayerPos() { return m_PrevPlayerPos; }
    void SetNewPlayerPos(Vec3 _Pos) { m_NewPlayerPos = _Pos; }

    Vec3 GetKnockBackDir() { return m_KnockBackDir; }
    void SetKnockBackDir(Vec3 _Dir) { m_KnockBackDir = _Dir; }

    int GetFloorLevel() { return m_iFloorLevel; }
    void SetFloorLevel(int FloorLevel) { m_iFloorLevel = FloorLevel; }

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Shoot();
    void Jump();    // Jump 상태로 변환 하는 작업.
    void Attack();  // Attack 상태인 경우 매 프레임마다 처리할 작업
    void Dash();    // Dash 상태인 경우 매 프레임마다 처리할 작업
    void DirtCloudEffect(bool _bLeft, bool _bDash); // DirtCloudEffect 생성하는 함수.

public:
    void Rewind(float Scale, bool Backward);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

