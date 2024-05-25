#pragma once
#include <Engine\CScript.h>

struct HeadhunterRewindData
{
    int AnimNum;
    int AnimFrame;
    int AnimIsLeft;
    Vec3 MonsterPos;
    Vec3 MonsterScale;
    Vec3 MonsterRot;
};

class CHeadhunterScript :
    public CScript
{
private:
    int         m_iPhase;
    int         m_iHitCount;

    int         m_iState;       // 0 alive 1 hurt 2 dead
    float       m_fSpeed;
    int         m_iGround;
    int         m_iLWall;
    int         m_iRWall;
    int         m_iCeiling;

    Vec3        m_PrevMonsterPos;
    Vec3        m_NewMonsterPos;
    Vec3        m_MonsterDir;

    bool        m_IsLeft;

    // 0 Headhunter_afterhurt 1 Headhunter_afterhurt_smoke 2 Headhunter_anim_rifle 3 Headhunter_bomb_run 4 Headhunter_Dash 5 Headhunter_dash_end_ground 6 Headhunter_dead 7 Headhunter_diefly 8 Headhunter_dieland 9 Headhunter_dodgeroll 10 Headhunter_exit_door 11 Headhunter_hat_hurtfly 12 Headhunter_head_hurtfly 13 Headhunter_head_hurtground 14 Headhunter_hurt 15 Headhunter_hurtfly 16 Headhunter_hurtground 17 Headhunter_idle 18 Headhunter_jump 19 Headhunter_nohead 20 Headhunter_predash 21 Headhunter_prejump 22 Headhunter_putarifle 23 Headhunter_putbackgun 24 Headhunter_putoutrifle 25 Headhunter_recover 26 Headhunter_reveal_bomb 27 Headhunter_shoot 28 Headhunter_sweep 29 Headhunter_takeoutgun 30 Headhunter_teleport_in 31 Headhunter_teleport_in_ground 32 Headhunter_teleport_in_sweep 33 Headhunter_teleport_out 34 Headhunter_teleport_out_ground 35 Headhunter_teleport_out_sweep 36 Headhunter_walk 37 Headhunter_wall_idle 38 Headhunter_walljump 39 Headhunter_walljump_land 40 Headhunter_invisible
    int         m_AnimNum;
    float       m_RewindTime;
    bool        m_bRewind;

    vector<int>   m_vecNextAnim;                                 // 애니메이션 별로 다음 애니메이션을 저장. 비반복 애니메이션에만 적용. 반복 애니메이션은 -1
    list<HeadhunterRewindData> m_MonsterRewindDataList;          // Rewind를 위한 데이터

    // 공격을 해야 하는 경우
    CGameObject* m_TargetPlayer;
    Vec3        m_KnockBackDir;     // 뒤로 밀려나는 방향
    float       m_fBleedingTime;
    float       m_fBloodWallTime;

    int         m_iLaser1Count;
    int         m_iDodgeCount;
    float       m_fRecoverTime;

    int         m_iAttack;      // 공격상태인지. 0이면 공격 아님. 1이면 Laser1 2면 Laser2

    float       m_fLaser1Time;
    float       m_fLaser2Time;
    float       m_fJumpGunTime;

    Vec3        m_Laser1Pos;
    Vec3        m_Laser1Rot;

    bool        m_Phase1FirstSpawn;
    bool        m_Phase1SecondSpawn;
    bool        m_Phase1Shot;
    bool        m_Laser2FirstJump;
    bool        m_Laser2SecondJump;
    bool        m_Laser2LeftWallJump;
    float       m_Laser2Theta;

    bool        m_JumpGunFirstJump;
    bool        m_JumpGunSecondJump;
    bool        m_JumpGunLeftWallJump;
    float       m_JumpGunTheta;
    float       m_fJumpGunShotTime;

    int         m_iGrenade1Count;
    bool        m_iGrenadeShot;

    int         m_iLaser3Step;

    bool        m_bDashLine;
    bool        m_bDash;
    float       m_RewindScale;

public:
    void AddGround() { ++m_iGround; }
    void ReleaseGround() { --m_iGround; }
    void AddCeiling() { ++m_iCeiling; }
    void ReleaseCeiling() { --m_iCeiling; }
    void AddLWall() { ++m_iLWall; }
    void ReleaseLWall() { --m_iLWall; }
    void AddRWall() { ++m_iRWall; }
    void ReleaseRWall() { --m_iRWall; }

    Vec3 GetPrevMonsterPos() { return m_PrevMonsterPos; }
    void SetNewMonsterPos(Vec3 _Pos) { m_NewMonsterPos = _Pos; }

public:
    void SetTargetPlayer(CGameObject* Player) { m_TargetPlayer = Player; }
    CGameObject* GetTargetPlayer() { return m_TargetPlayer; }

    // 0 Headhunter_afterhurt 1 Headhunter_afterhurt_smoke 2 Headhunter_anim_rifle 3 Headhunter_bomb_run 4 Headhunter_Dash 5 Headhunter_dash_end_ground 6 Headhunter_dead 7 Headhunter_diefly 8 Headhunter_dieland 9 Headhunter_dodgeroll 10 Headhunter_exit_door 11 Headhunter_hat_hurtfly 12 Headhunter_head_hurtfly 13 Headhunter_head_hurtground 14 Headhunter_hurt 15 Headhunter_hurtfly 16 Headhunter_hurtground 17 Headhunter_idle 18 Headhunter_jump 19 Headhunter_nohead 20 Headhunter_predash 21 Headhunter_prejump 22 Headhunter_putarifle 23 Headhunter_putbackgun 24 Headhunter_putoutrifle 25 Headhunter_recover 26 Headhunter_reveal_bomb 27 Headhunter_shoot 28 Headhunter_sweep 29 Headhunter_takeoutgun 30 Headhunter_teleport_in 31 Headhunter_teleport_in_ground 32 Headhunter_teleport_in_sweep 33 Headhunter_teleport_out 34 Headhunter_teleport_out_ground 35 Headhunter_teleport_out_sweep 36 Headhunter_walk 37 Headhunter_wall_idle 38 Headhunter_walljump 39 Headhunter_walljump_land 40 Headhunter_invisible
    void SetAnimNum(int _AnimNum) { m_AnimNum = _AnimNum; }
    // 0 Headhunter_afterhurt 1 Headhunter_afterhurt_smoke 2 Headhunter_anim_rifle 3 Headhunter_bomb_run 4 Headhunter_Dash 5 Headhunter_dash_end_ground 6 Headhunter_dead 7 Headhunter_diefly 8 Headhunter_dieland 9 Headhunter_dodgeroll 10 Headhunter_exit_door 11 Headhunter_hat_hurtfly 12 Headhunter_head_hurtfly 13 Headhunter_head_hurtground 14 Headhunter_hurt 15 Headhunter_hurtfly 16 Headhunter_hurtground 17 Headhunter_idle 18 Headhunter_jump 19 Headhunter_nohead 20 Headhunter_predash 21 Headhunter_prejump 22 Headhunter_putarifle 23 Headhunter_putbackgun 24 Headhunter_putoutrifle 25 Headhunter_recover 26 Headhunter_reveal_bomb 27 Headhunter_shoot 28 Headhunter_sweep 29 Headhunter_takeoutgun 30 Headhunter_teleport_in 31 Headhunter_teleport_in_ground 32 Headhunter_teleport_in_sweep 33 Headhunter_teleport_out 34 Headhunter_teleport_out_ground 35 Headhunter_teleport_out_sweep 36 Headhunter_walk 37 Headhunter_wall_idle 38 Headhunter_walljump 39 Headhunter_walljump_land 40 Headhunter_invisible
    int GetAnimNum() { return m_AnimNum; }

    bool GetRewind() { return m_bRewind; }
    void SetLeft(bool _IsLeft) { m_IsLeft = _IsLeft; }
    bool GetLeft() { return m_IsLeft; }
    int  GetState() { return m_iState; }
    void SetState(int iState) { m_iState = iState; }
    Vec3 GetKnockBackDir() { return m_KnockBackDir; }
    void SetKnockBackDir(Vec3 _Dir) { m_KnockBackDir = _Dir; }

private:
    void Rewind(float Scale, bool Backward);
    void StartRewind();
    void SetAnimation();

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

private:
    void Phase1();
    void Phase2();

private:
    void Grenade();
    void Dash();
    void Laser3();
    void JumpGun();
    void BombRun();

    void Laser1();
    void Laser2();
    void Dodge();

private:
    void DirtCloudEffect(bool _bLeft, bool _bDash); // DirtCloudEffect 생성하는 함수.
    void CreateExplosion(Vec3 _OtherPos);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CHeadhunterScript);

    friend class CPlayerScript;

public:
    CHeadhunterScript();
    ~CHeadhunterScript();
};

