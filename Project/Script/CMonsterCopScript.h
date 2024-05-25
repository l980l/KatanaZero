#pragma once
#include <Engine\CScript.h>

struct MonsterCopRewindData
{
    int AnimNum;
    int AnimFrame;
    int AnimIsLeft;
    Vec3 MonsterPos;
    Vec3 MonsterScale;
    Vec3 MonsterRot;
    Vec3 GunRot;
};

class CMonsterCopScript :
    public CScript
{
private:
    int         m_iState;       // 0 alive 1 hurt 2 dead

    float       m_fSpeed;
    int         m_iGround;
    int         m_iGroundToFall;
    int         m_iLWall;
    int         m_iRWall;
    int         m_iCeiling;
    int         m_iLStair;
    int         m_iRStair;

    Vec3        m_PrevMonsterPos;
    Vec3        m_NewMonsterPos;
    Vec3        m_MonsterDir;

    bool        m_IsLeft;

    // 0 Cop_walk 1 Cop_leavestair 2 Cop_run 3 Cop_turn 4 Cop_enterstair 5 Cop_hurtground 6 Cop_idle 7 Cop_aim 8 Cop_hurtground
    int         m_AnimNum;
    float       m_RewindTime;
    bool        m_bRewind;

    vector<int>   m_vecNextAnim;                                 // 애니메이션 별로 다음 애니메이션을 저장. 비반복 애니메이션에만 적용. 반복 애니메이션은 -1
    list<MonsterCopRewindData> m_MonsterRewindDataList;       // Rewind를 위한 데이터

    int         m_iFloorLevel;          // 속해있는 층.
    int         m_iPrevFloorLevel;      // 한 프레임 전에 속해있는 층.
    Vec4        m_arrStairData[3];      // 현재 Level의 Stair들의 위치를 저장. FloorLevel, 위쪽 x좌표, 아래쪽 x좌표, Left여부. 최대 3개까지만 있다고 가정하고 배열 크기를 4로 잡음.

    // 0 대기 1 찾아서 좇아감 
    // 나중에 플레이어를 놓치거나 의심하는 등의 조건을 위해 int로 선언.
    int         m_iPlayerDetect;

    // 대기 상태 동작 0 제자리에 서있기 1 일정 구간 반복 걷기
    int         m_iStandByType;

    // 계단 내려가야 되는 경우
    bool        m_bGoToStair;

    // 공격을 해야 하는 경우
    bool        m_bAttackReady;
    CGameObject* m_TargetPlayer;
    Vec3        m_KnockBackDir;     // 뒤로 밀려나는 방향
    float       m_fBleedingTime;
    float       m_fBloodWallTime;

    bool        m_bArms;

    float       m_fShotTime;
    float       m_RewindScale;
    bool        m_bBossMap;

public:
    void SetArrStairData(int index, Vec4 Data) { m_arrStairData[index] = Data; }
    void AddGround() { ++m_iGround; }
    void ReleaseGround() { --m_iGround; }
    void AddGroundToFall() { ++m_iGroundToFall; }
    void ReleaseGroundToFall() { --m_iGroundToFall; }
    void AddCeiling() { ++m_iCeiling; }
    void ReleaseCeiling() { --m_iCeiling; }
    void AddLWall() { ++m_iLWall; }
    void ReleaseLWall() { --m_iLWall; }
    void AddRWall() { ++m_iRWall; }
    void ReleaseRWall() { --m_iRWall; }
    void AddLStair() { ++m_iLStair; }
    void ReleaseLStair() { --m_iLStair; }
    void AddRStair() { ++m_iRStair; }
    void ReleaseRStair() { --m_iRStair; }

    int GetGroundToFall() { return m_iGroundToFall; }

    Vec3 GetPrevMonsterPos() { return m_PrevMonsterPos; }
    void SetNewMonsterPos(Vec3 _Pos) { m_NewMonsterPos = _Pos; }

    int GetFloorLevel() { return m_iFloorLevel; }
    void SetFloorLevel(int FloorLevel) { m_iFloorLevel = FloorLevel; }

public:
    // 0 Cop_walk 1 Cop_leavestair 2 Cop_run 3 Cop_turn 4 Cop_enterstair 5 Cop_hurtground 6 Cop_idle 7 Cop_aim 8 Cop_hurtground
    void SetAnimNum(int _AnimNum) { m_AnimNum = _AnimNum; }
    // 0 Cop_walk 1 Cop_leavestair 2 Cop_run 3 Cop_turn 4 Cop_enterstair 5 Cop_hurtground 6 Cop_idle 7 Cop_aim 8 Cop_hurtground
    int GetAnimNum() { return m_AnimNum; }
    bool GetRewind() { return m_bRewind; }
    void SetLeft(bool _IsLeft) { m_IsLeft = _IsLeft; }
    bool GetLeft() { return m_IsLeft; }
    void SetDetect(int _iDetect); // 0 대기 1 찾아서 좇아감 
    bool GetDetect() { return m_iPlayerDetect; }
    void SetTargetPlayer(CGameObject* Player) { m_TargetPlayer = Player; }
    CGameObject* GetTargetPlayer() { return m_TargetPlayer; }
    void SetGoToStair(bool GoToStair) { m_bGoToStair = GoToStair; }
    bool GetGoToStair() { return m_bGoToStair; }
    void SetAttackReady(bool AttackReady) { m_bAttackReady = AttackReady; }
    void SetState(int iState) { m_iState = iState; }
    int GetState() { return m_iState; }
    Vec3 GetKnockBackDir() { return m_KnockBackDir; }
    void SetKnockBackDir(Vec3 _Dir) { m_KnockBackDir = _Dir; }

private:
    void Rewind(float Scale, bool Backward);
    void StartRewind();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CMonsterCopScript);

    friend class CPlayerScript;

public:
    CMonsterCopScript();
    ~CMonsterCopScript();
};

