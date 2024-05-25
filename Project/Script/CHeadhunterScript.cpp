#include "pch.h"
#include "CHeadhunterScript.h"

#include "CPlayerScript.h"
#include "CEffectScript.h"
#include "CGravityScript.h"
#include "CMonsterAttackScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CBulletScript.h"
#include "CGrenadeScript.h"

#include <Engine\CAnim2D.h>

CHeadhunterScript::CHeadhunterScript()
	: CScript((UINT)SCRIPT_TYPE::HEADHUNTERSCRIPT)
	, m_iPhase(1)
	, m_iHitCount(0)
	, m_iState(0)
	, m_fSpeed(100.f)
	, m_iGround(0)
	, m_iLWall(0)
	, m_iRWall(0)
	, m_iCeiling(0)
	, m_AnimNum(17)
	, m_RewindTime(0)
	, m_bRewind(false)
	, m_IsLeft(true)
	, m_TargetPlayer(nullptr)
	, m_fBleedingTime(0.f)
	, m_fBloodWallTime(0.f)
	, m_iLaser1Count(0)
	, m_iDodgeCount(0)
	, m_fRecoverTime(0.f)
	, m_iAttack(0)
	, m_fLaser1Time(0.f)
	, m_fLaser2Time(0.f)
	, m_fJumpGunTime(0.f)
	, m_Phase1FirstSpawn(false)
	, m_Phase1SecondSpawn(false)
	, m_Phase1Shot(false)
	, m_Laser2FirstJump(false)
	, m_Laser2SecondJump(false)
	, m_Laser2LeftWallJump(false)
	, m_Laser2Theta(-3.14f)
	, m_JumpGunFirstJump(false)
	, m_JumpGunSecondJump(false)
	, m_JumpGunLeftWallJump(false)
	, m_JumpGunTheta(-3.14f)
	, m_fJumpGunShotTime(0.f)
	, m_iGrenade1Count(0)
	, m_iGrenadeShot(false)
	, m_iLaser3Step(0)
	, m_bDashLine(false)
	, m_bDash(false)
	, m_RewindScale(1.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iPhase, "Phase      ");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed      ");
}

CHeadhunterScript::~CHeadhunterScript()
{
}

void CHeadhunterScript::Rewind(float Scale, bool Backward)
{
	m_RewindTime += DT;

	m_RewindScale += DT * 120.f;

	if (Backward)
	{
		if (m_RewindTime >= 1.f / (60.f * Scale * m_RewindScale))
		{
			m_RewindTime -= m_RewindTime;

			CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
			CurAnim->SetStop(false);

			int End = (int)m_MonsterRewindDataList.size();

			if (End <= 0)
			{
				m_bRewind = false;

				vector<CScript*> vecObjScript = GetOwner()->GetScripts();

				for (size_t i = 0; i < vecObjScript.size(); ++i)
				{
					wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

					if (ScriptName == L"CGravityScript")
					{
						((CGravityScript*)vecObjScript[i])->CGravityScript::SetOnOff(true);
					}
				}

				return;
			}

			HeadhunterRewindData TempData = {};

			if (Backward)
			{
				TempData = m_MonsterRewindDataList.back();
				m_MonsterRewindDataList.pop_back();
			}

			else
			{
				TempData = m_MonsterRewindDataList.front();
				m_MonsterRewindDataList.pop_front();
			}

			Transform()->SetRelativePos(TempData.MonsterPos);
			Transform()->SetRelativeScale(TempData.MonsterScale);
			Transform()->SetRelativeRot(TempData.MonsterRot);

			SetAnimNum(TempData.AnimNum);
			Animator2D()->SetIsLeft(TempData.AnimIsLeft);

			// 애니메이션 세팅
			CHeadhunterScript::SetAnimation();

			CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
			CurAnim->SetCurIndex(TempData.AnimFrame);
			CurAnim->SetStop(true);
		}
	}
}

void CHeadhunterScript::StartRewind()
{
	m_bRewind = true;

	vector<CScript*> vecObjScript = GetOwner()->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CGravityScript")
		{
			((CGravityScript*)vecObjScript[i])->CGravityScript::SetOnOff(false);
		}
	}
}

void CHeadhunterScript::SetAnimation()
{
	switch (m_AnimNum)
	{
	case 0:
		Animator2D()->Play(L"animation\\Headhunter_afterhurt.anim", true);
		break;
	case 1:
		Animator2D()->Play(L"animation\\Headhunter_afterhurt_smoke.anim", true);
		break;
	case 2:
		Animator2D()->Play(L"animation\\Headhunter_anim_rifle.anim", true);
		break;
	case 3:
		Animator2D()->Play(L"animation\\Headhunter_bomb_run.anim", true);
		break;
	case 4:
		Animator2D()->Play(L"animation\\Headhunter_Dash.anim", true);
		break;
	case 5:
		Animator2D()->Play(L"animation\\Headhunter_dash_end_ground.anim", true);
		break;
	case 6:
		Animator2D()->Play(L"animation\\Headhunter_dead.anim", false);
		break;
	case 7:
		Animator2D()->Play(L"animation\\Headhunter_diefly.anim", true);
		break;
	case 8:
		Animator2D()->Play(L"animation\\Headhunter_dieland.anim", true);
		break;
	case 9:
		Animator2D()->Play(L"animation\\Headhunter_dodgeroll.anim", true);
		break;
	case 10:
		Animator2D()->Play(L"animation\\Headhunter_exit_door.anim", true);
		break;
	case 11:
		Animator2D()->Play(L"animation\\Headhunter_hat_hurtfly.anim", true);
		break;
	case 12:
		Animator2D()->Play(L"animation\\Headhunter_head_hurtfly.anim", true);
		break;
	case 13:
		Animator2D()->Play(L"animation\\Headhunter_head_hurtground.anim", true);
		break;
	case 14:
		Animator2D()->Play(L"animation\\Headhunter_hurt.anim", true);
		break;
	case 15:
		Animator2D()->Play(L"animation\\Headhunter_hurtfly.anim", true);
		break;
	case 16:
		Animator2D()->Play(L"animation\\Headhunter_hurtground.anim", true);
		break;
	case 17:
		Animator2D()->Play(L"animation\\Headhunter_idle.anim", true);
		break;
	case 18:
		Animator2D()->Play(L"animation\\Headhunter_jump.anim", true);
		break;
	case 19:
		Animator2D()->Play(L"animation\\Headhunter_nohead.anim", true);
		break;
	case 20:
		Animator2D()->Play(L"animation\\Headhunter_predash.anim", true);
		break;
	case 21:
		Animator2D()->Play(L"animation\\Headhunter_prejump.anim", true);
		break;
	case 22:
		Animator2D()->Play(L"animation\\Headhunter_putarifle.anim", true);
		break;
	case 23:
		Animator2D()->Play(L"animation\\Headhunter_putbackgun.anim", true);
		break;
	case 24:
		Animator2D()->Play(L"animation\\Headhunter_putoutrifle.anim", true);
		break;
	case 25:
		Animator2D()->Play(L"animation\\Headhunter_recover.anim", true);
		break;
	case 26:
		Animator2D()->Play(L"animation\\Headhunter_reveal_bomb.anim", true);
		break;
	case 27:
		Animator2D()->Play(L"animation\\Headhunter_shoot.anim", true);
		break;
	case 28:
		Animator2D()->Play(L"animation\\Headhunter_sweep.anim", true);
		break;
	case 29:
		Animator2D()->Play(L"animation\\Headhunter_takeoutgun.anim", true);
		break;
	case 30:
		Animator2D()->Play(L"animation\\Headhunter_teleport_in.anim", true);
		break;
	case 31:
		Animator2D()->Play(L"animation\\Headhunter_teleport_in_ground.anim", true);
		break;
	case 32:
		Animator2D()->Play(L"animation\\Headhunter_teleport_in_sweep.anim", true);
		break;
	case 33:
		Animator2D()->Play(L"animation\\Headhunter_teleport_out.anim", true);
		break;
	case 34:
		Animator2D()->Play(L"animation\\Headhunter_teleport_out_ground.anim", true);
		break;
	case 35:
		Animator2D()->Play(L"animation\\Headhunter_teleport_out_sweep.anim", true);
		break;
	case 36:
		Animator2D()->Play(L"animation\\Headhunter_walk.anim", true);
		break;
	case 37:
		Animator2D()->Play(L"animation\\Headhunter_wall_idle.anim", true);
		break;
	case 38:
		Animator2D()->Play(L"animation\\Headhunter_walljump.anim", true);
		break;
	case 39:
		Animator2D()->Play(L"animation\\Headhunter_walljump_land.anim", true);
		break;
	case 40:
		Animator2D()->Play(L"animation\\Headhunter_invisible.anim", true);
		break;
	}
}

void CHeadhunterScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	m_fRecoverTime = 0.f;

	for (int i = 0; i < 41; ++i)
	{
		m_vecNextAnim.push_back(-1);
	}

	// 0 Headhunter_afterhurt 1 Headhunter_afterhurt_smoke 2 Headhunter_anim_rifle 3 Headhunter_bomb_run 4 Headhunter_Dash 5 Headhunter_dash_end_ground 6 Headhunter_dead 7 Headhunter_diefly 8 Headhunter_dieland 9 Headhunter_dodgeroll 10 Headhunter_exit_door 11 Headhunter_hat_hurtfly 12 Headhunter_head_hurtfly 13 Headhunter_head_hurtground 14 Headhunter_hurt 15 Headhunter_hurtfly 16 Headhunter_hurtground 17 Headhunter_idle 18 Headhunter_jump 19 Headhunter_nohead 20 Headhunter_predash 21 Headhunter_prejump 22 Headhunter_putarifle 23 Headhunter_putbackgun 24 Headhunter_putoutrifle 25 Headhunter_recover 26 Headhunter_reveal_bomb 27 Headhunter_shoot 28 Headhunter_sweep 29 Headhunter_takeoutgun 30 Headhunter_teleport_in 31 Headhunter_teleport_in_ground 32 Headhunter_teleport_in_sweep 33 Headhunter_teleport_out 34 Headhunter_teleport_out_ground 35 Headhunter_teleport_out_sweep 36 Headhunter_walk 37 Headhunter_wall_idle 38 Headhunter_walljump 39 Headhunter_walljump_land 40 Headhunter_invisible

	m_vecNextAnim[9] = 17;	// 구르기 다음 idle
	m_vecNextAnim[10] = 17;	// Headhunter_exit_door 다음 idle

	m_vecNextAnim[14] = 1;	// hurt 다음 연막
	m_vecNextAnim[1] = 25;	// 연막 다음 recover
	m_vecNextAnim[25] = 40;	// recover 다음 사라지기

	m_vecNextAnim[24] = 2;	// rifle 꺼낸 다음 조준
	m_vecNextAnim[22] = 17;	// rifle 집어 넣고 idle

	m_vecNextAnim[21] = 18;	// Headhunter_prejump 다음 Headhunter_jump
	m_vecNextAnim[39] = 17;	// Headhunter_walljump_land 다음 idle

	m_vecNextAnim[29] = 27;	// Headhunter_takeoutgun 다음 Headhunter_shoot
	m_vecNextAnim[23] = 17;	// Headhunter_putbackgun 다음 idle

	m_vecNextAnim[20] = 4;	// Headhunter_predash 다음 Headhunter_Dash

	m_vecNextAnim[26] = 3;	// Headhunter_reveal_bomb 다음 Headhunter_bomb_run

	m_vecNextAnim[7] = 8;	// Headhunter_diefly 다음 Headhunter_dieland
	m_vecNextAnim[8] = 6;	// Headhunter_dieland 다음 Headhunter_dead
}

void CHeadhunterScript::tick()
{
	if (m_bRewind)
	{
		Rewind(0.1f, true);
		return;
	}

	m_NewMonsterPos = Transform()->GetRelativePos();

	// 비반복 애니메이션인지 확인
	if (m_vecNextAnim[GetAnimNum()] != -1)
	{
		// 애니메이션이 종료되었는지 확인
		CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());

		if (CurAnim->IsFinish())
		{
			CurAnim->Reset();
			SetAnimNum(m_vecNextAnim[GetAnimNum()]);
		}
	}

	// 애니메이션 세팅
	CHeadhunterScript::SetAnimation();

	// hurt
	if (m_iState == 1)
	{
		if (GetAnimNum() == 15)
		{
			if (m_iGround > 0)
				SetAnimNum(14);

			if (m_KnockBackDir.x > 0)
			{
				m_IsLeft = true;

				if (!m_iRWall)
				{
					m_NewMonsterPos.x += DT * m_fSpeed * 3.f;
				}
			}

			else if (m_KnockBackDir.x < 0)
			{
				m_IsLeft = false;

				if (!m_iLWall)
				{
					m_NewMonsterPos.x -= DT * m_fSpeed * 3.f;
				}
			}
		}

		// recover 상태면 다시 0으로
		else if (GetAnimNum() == 40)
		{
			m_iState = 0;
		}
	}

	else if (m_iState == 2)
	{
	}

	// m_iState == 0 인 경우
	else if (m_TargetPlayer)
	{
		if (m_iPhase == 1)
			Phase1();

		else
			Phase2();
	}


	Transform()->SetRelativePos(m_NewMonsterPos);

	m_PrevMonsterPos = Transform()->GetRelativePos();

	Animator2D()->SetIsLeft(m_IsLeft);

	// 애니메이션 세팅
	CHeadhunterScript::SetAnimation();


	// Rewind를 위한 PlayerRewindData 저장
	m_RewindTime += DT;

	if (m_RewindTime >= 1.f / 60.f)
	{
		HeadhunterRewindData TempData = {};
		TempData.AnimNum = GetAnimNum();
		CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
		TempData.AnimFrame = CurAnim->GetCurFrameIndex();
		TempData.AnimIsLeft = Animator2D()->IsLeft();
		TempData.MonsterPos = Transform()->GetRelativePos();
		TempData.MonsterScale = Transform()->GetRelativeScale();
		TempData.MonsterRot = Transform()->GetRelativeRot();

		m_MonsterRewindDataList.push_back(TempData);

		m_RewindTime -= m_RewindTime;
	}
}

void CHeadhunterScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_iState == 0)
	{
		// 구르는 중이거나 recover 상태가 아니어야 한다.
		if (GetAnimNum() != 9 && GetAnimNum() != 40)
		{
			// Player 공격이랑 충돌한 경우.
			if (_Other->GetOwner()->GetLayerIndex() == 4)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_voiceboss_huntress_hurt_1.wav");
				pSound->Play(1, 0.5f, false);

				SetAnimNum(15);
				m_iState = 1;
				m_iHitCount++;

				Vec3 KnockBackDir = Transform()->GetRelativePos() - _Other->Transform()->GetRelativePos();

				KnockBackDir = Normalize(Vec3(KnockBackDir.x, KnockBackDir.y, 0.f));

				m_KnockBackDir = KnockBackDir;
			}

			// Dash
			else if (GetAnimNum() == 4)
			{
				// Player Layer에 있는 물체와 충돌한 경우 공격되어야 한다. KnockBackDir을 설정해주고, State를 Hurt로 변환시킨다. 애니메이션도 Hurt_begin으로 변경해준다.
				if (_Other->GetOwner()->GetLayerIndex() == 2)
				{
					CPlayerScript* PlayerScript = _Other->GetOwner()->GetScript<CPlayerScript>();

					// 구르는 중이면 안맞음.
					if (PlayerScript->GetState() == 0 && PlayerScript->GetAnimNum() != 5)
					{
						// 불사 상태면 그냥 나감. 대신 소리는 나야함.
						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\playerdie.wav");
						pSound->Play(1, 0.5f, false);

						if (!PlayerScript->GetImmortal())
						{
							Vec3 KnockBackDir = _Other->Transform()->GetRelativePos() - Transform()->GetRelativePos();
							KnockBackDir = Normalize(KnockBackDir);

							PlayerScript->SetState(1);
							PlayerScript->SetAnimNum(8);
							PlayerScript->SetKnockBackDir(KnockBackDir);
						}
					}
				}
			}

			// 폭탄 자살
			else if (GetAnimNum() == 3)
			{
				// Player Layer에 있는 물체와 충돌한 경우 공격되어야 한다. KnockBackDir을 설정해주고, State를 Hurt로 변환시킨다. 애니메이션도 Hurt_begin으로 변경해준다.
				if (_Other->GetOwner()->GetLayerIndex() == 2)
				{
					CPlayerScript* PlayerScript = _Other->GetOwner()->GetScript<CPlayerScript>();

					// 구르는 중이면 안맞음.
					if (PlayerScript->GetState() == 0 && PlayerScript->GetAnimNum() != 5)
					{
						// 불사 상태면 그냥 나감. 대신 소리는 나야함.
						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\playerdie.wav");
						pSound->Play(1, 0.5f, false);

						if (!PlayerScript->GetImmortal())
						{
							Vec3 KnockBackDir = _Other->Transform()->GetRelativePos() - Transform()->GetRelativePos();
							KnockBackDir = Normalize(KnockBackDir);

							PlayerScript->SetState(1);
							PlayerScript->SetAnimNum(8);
							PlayerScript->SetKnockBackDir(KnockBackDir);
						}

						Vec3 NewPos = Transform()->GetRelativePos();

						for (int i = 0; i < 7; ++i)
						{
							CreateExplosion(NewPos + Vec3(cosf(3.14f * 2.f / 7.f * i), sinf(3.14f * 2.f / 7.f * i), 0.f) * 50.f);
							CreateExplosion(NewPos + Vec3(cosf(3.14f * 2.f / 7.f * i), sinf(3.14f * 2.f / 7.f * i), 0.f) * 120.f);
						}
					}
				}
			}
		}
	}
}

void CHeadhunterScript::OnOverlap(CCollider2D* _Other)
{
}

void CHeadhunterScript::EndOverlap(CCollider2D* _Other)
{
}

void CHeadhunterScript::Phase1()
{
	CPlayerScript* PlayerScript = m_TargetPlayer->GetScript<CPlayerScript>();
	int PlayerState = PlayerScript->GetState();

	// Player 사망시 공격 x
	if (PlayerState == 2)
	{
		SetAnimNum(17);
		SetAnimation();
		return;
	}

	// Recover 상태가 아니라면.
	if (GetAnimNum() != 40)
	{
		// 공격 상태가 아니라면
		if (m_iAttack == 0)
		{
			// 구르기 상태면 
			if (GetAnimNum() == 9)
			{
				Dodge();
			}

			// Recover도 아니고 공격 중도 아니고, 구르기 상태도 아니라면.
			else
			{
				// 이미 공격 중이거나 Dodge 상태가 아니라면 Player 방향에 따라 m_IsLeft를 설정해주자.
				Vec3 PlayerPos = m_TargetPlayer->Transform()->GetRelativePos();
				float Distance = PlayerPos.x - m_NewMonsterPos.x;

				// 좌우 애니메이션 세팅
				if (Distance > 0.f)
					m_IsLeft = false;
				else if (Distance < 0.f)
					m_IsLeft = true;

				Distance = abs(PlayerPos.x - m_NewMonsterPos.x);

				// 1 번 레이저 공격.
				if (Distance > 350.f)
				{
					Laser1();
					m_iAttack = 1;
					m_iDodgeCount = 0;
				}

				// 거리가 가까우면 Dodge를 최대 2번까지 하고, 2 번 레이저 공격. 
				else
				{
					if (m_iDodgeCount >= 2)
					{
						Laser2();
						m_iAttack = 2;
						m_iDodgeCount = 0;
					}

					else
					{
						// 구르기
						SetAnimNum(9);
						++m_iDodgeCount;

						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_roll.wav");
						pSound->Play(1, 0.5f, true);
					}
				}
			}
		}

		if (m_iAttack == 1)
		{
			Laser1();
		}

		else if (m_iAttack == 2)
		{
			Laser2();
		}
	}

	// Recover 상태인 경우
	else
	{
		m_fRecoverTime += DT;

		Vec3 PlayerPos = m_TargetPlayer->Transform()->GetRelativePos();
	
		if (m_iHitCount == 1)
		{
			if (!m_Phase1FirstSpawn)
			{
				// Cop 두마리
				if (m_fRecoverTime > 0.5f)
				{
					m_Phase1FirstSpawn = true;

					Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Monster_Cop.pref");
					CGameObject* pCloneMonster = pEffectPrefab->Instantiate();
					SpawnGameObject(pCloneMonster, Vec3(-446.f, -178, m_PrevMonsterPos.z), L"Monster");
					CMonsterCopScript* MonsterScript = pCloneMonster->GetScript<CMonsterCopScript>();
					if (MonsterScript)
					{
						MonsterScript->SetLeft(false);
						MonsterScript->SetDetect(1);
						MonsterScript->SetTargetPlayer(m_TargetPlayer);
					}

					Ptr<CPrefab> pEffectPrefab2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Monster_Cop.pref");
					CGameObject* pCloneMonster2 = pEffectPrefab2->Instantiate();
					SpawnGameObject(pCloneMonster2, Vec3(443.f, -178, m_PrevMonsterPos.z), L"Monster");
					CMonsterCopScript* MonsterScript2 = pCloneMonster2->GetScript<CMonsterCopScript>();
					if (MonsterScript2)
					{
						MonsterScript2->SetLeft(true);
						MonsterScript2->SetDetect(1);
						MonsterScript2->SetTargetPlayer(m_TargetPlayer);
					}
				}
			}

			if (!m_Phase1SecondSpawn)
			{
				// Grunt 두마리
				if (m_fRecoverTime > 2.5f)
				{
					m_Phase1SecondSpawn = true;

					Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Monster_Grunt.pref");
					CGameObject* pCloneMonster = pEffectPrefab->Instantiate();
					SpawnGameObject(pCloneMonster, Vec3(-159.f, -178, m_PrevMonsterPos.z), L"Monster");
					CMonsterGruntScript* MonsterScript = pCloneMonster->GetScript<CMonsterGruntScript>();
					if (MonsterScript)
					{
						MonsterScript->SetLeft(false);
						MonsterScript->SetDetect(1);
						MonsterScript->SetTargetPlayer(m_TargetPlayer);
					}

					Ptr<CPrefab> pEffectPrefab2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Monster_Grunt.pref");
					CGameObject* pCloneMonster2 = pEffectPrefab2->Instantiate();
					SpawnGameObject(pCloneMonster2, Vec3(193.f, -178, m_PrevMonsterPos.z), L"Monster");
					CMonsterGruntScript* MonsterScript2 = pCloneMonster2->GetScript<CMonsterGruntScript>();
					if (MonsterScript2)
					{
						MonsterScript2->SetLeft(true);
						MonsterScript2->SetDetect(1);
						MonsterScript2->SetTargetPlayer(m_TargetPlayer);
					}
				}
			}
		}

		else if (m_iHitCount == 2)
		{
			if (!m_Phase1FirstSpawn)
			{
				// Pomp 두마리
				if (m_fRecoverTime > 0.5f)
				{
					m_Phase1FirstSpawn = true;

					Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Monster_Pomp.pref");
					CGameObject* pCloneMonster = pEffectPrefab->Instantiate();
					SpawnGameObject(pCloneMonster, Vec3(-446.f, -178, m_PrevMonsterPos.z), L"Monster");
					CMonsterPompScript* MonsterScript = pCloneMonster->GetScript<CMonsterPompScript>();
					if (MonsterScript)
					{
						MonsterScript->SetLeft(false);
						MonsterScript->SetDetect(1);
						MonsterScript->SetTargetPlayer(m_TargetPlayer);
					}

					Ptr<CPrefab> pEffectPrefab2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Monster_Pomp.pref");
					CGameObject* pCloneMonster2 = pEffectPrefab2->Instantiate();
					SpawnGameObject(pCloneMonster2, Vec3(443.f, -178, m_PrevMonsterPos.z), L"Monster");
					CMonsterPompScript* MonsterScript2 = pCloneMonster2->GetScript<CMonsterPompScript>();
					if (MonsterScript2)
					{
						MonsterScript2->SetLeft(true);
						MonsterScript2->SetDetect(1);
						MonsterScript2->SetTargetPlayer(m_TargetPlayer);
					}
				}
			}

			if (!m_Phase1SecondSpawn)
			{
				// Cop 두마리
				if (m_fRecoverTime > 2.5f)
				{
					m_Phase1SecondSpawn = true;

					Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Monster_Cop.pref");
					CGameObject* pCloneMonster = pEffectPrefab->Instantiate();
					SpawnGameObject(pCloneMonster, Vec3(-159.f, -178, m_PrevMonsterPos.z), L"Monster");
					CMonsterCopScript* MonsterScript = pCloneMonster->GetScript<CMonsterCopScript>();
					if (MonsterScript)
					{
						MonsterScript->SetLeft(false);
						MonsterScript->SetDetect(1);
						MonsterScript->SetTargetPlayer(m_TargetPlayer);
					}

					Ptr<CPrefab> pEffectPrefab2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Monster_Cop.pref");
					CGameObject* pCloneMonster2 = pEffectPrefab2->Instantiate();
					SpawnGameObject(pCloneMonster2, Vec3(193.f, -178, m_PrevMonsterPos.z), L"Monster");
					CMonsterCopScript* MonsterScript2 = pCloneMonster2->GetScript<CMonsterCopScript>();
					if(MonsterScript2)
					{
						MonsterScript2->SetLeft(true);
						MonsterScript2->SetDetect(1);
						MonsterScript2->SetTargetPlayer(m_TargetPlayer);
					}
				}
			}
		}

		// Phase에 맞게 엔딩 동작이 나가야지. 1번에선 맵 하단에서 폭발이 생겨야 하고, 2번에선 칼을 떨어뜨리고, 폭탄 테러 하고.
		if (m_iHitCount >= 3)
		{
			m_iState = 2;

			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntressmines_armed_01.wav");
			pSound->Play(1, 0.5f, false);

			for (int i = 0; i < 16; ++i)
			{
				Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Mine.pref");
				CGameObject* pCloneMonster = pEffectPrefab->Instantiate();
				SpawnGameObject(pCloneMonster, Vec3(-480.f + 75.f * i, -196.f, m_PrevMonsterPos.z), L"MonsterProjectile");
			}
		}

		// 5초가 지나면 나타나게 해야함.
		if (m_fRecoverTime > 5.f)
		{
			m_iState = 0;
			m_iAttack = 0;

			// Phase1 에서는 Recover 이후에 exit_door 만 사용함.
			SetAnimNum(10);

			// Player 위치에서 더 먼쪽의 문에서 나타나도록 위치를 설정해줘야 함.
			if (PlayerPos.x > 15.f)
			{
				m_NewMonsterPos = Vec3(-446.f, -178.f, m_PrevMonsterPos.z);
				m_IsLeft = false;
			}

			else
			{
				m_NewMonsterPos = Vec3(443.f, -178.f, m_PrevMonsterPos.z);
				m_IsLeft = true;
			}
			
			m_fRecoverTime = 0.f;
			m_Phase1FirstSpawn = false;
			m_Phase1SecondSpawn = false;
		}
	}

	if (m_iRWall > 0)
	{
		if (m_NewMonsterPos.x > m_PrevMonsterPos.x)
			m_NewMonsterPos.x = m_PrevMonsterPos.x;
	}

	if (m_iLWall > 0)
	{
		if (m_NewMonsterPos.x < m_PrevMonsterPos.x)
			m_NewMonsterPos.x = m_PrevMonsterPos.x;
	}

	if (m_iCeiling > 0)
	{
		if (m_NewMonsterPos.y > m_PrevMonsterPos.y)
			m_NewMonsterPos.y = m_PrevMonsterPos.y;
	}
}

void CHeadhunterScript::Phase2()
{
	CPlayerScript* PlayerScript = m_TargetPlayer->GetScript<CPlayerScript>();
	int PlayerState = PlayerScript->GetState();

	// Player 사망시 공격 x
	if (PlayerState == 2)
	{
		SetAnimNum(17);
		SetAnimation();
		return;
	}

	// Recover 상태가 아니라면.
	if (GetAnimNum() != 40)
	{
		// 공격 상태가 아니라면
		if (m_iAttack == 0)
		{
			// 구르기 상태면 
			if (GetAnimNum() == 9)
			{
				Dodge();
			}

			// Recover도 아니고 공격 중도 아니고, 구르기 상태도 아니라면.
			else
			{
				// 이미 공격 중이거나 Dodge 상태가 아니라면 Player 방향에 따라 m_IsLeft를 설정해주자.
				Vec3 PlayerPos = m_TargetPlayer->Transform()->GetRelativePos();
				float Distance = PlayerPos.x - m_NewMonsterPos.x;

				// 좌우 애니메이션 세팅
				if (Distance > 0)
					m_IsLeft = false;
				else if (Distance < 0)
					m_IsLeft = true;

				Distance = abs(PlayerPos.x - m_NewMonsterPos.x);

				// 1 번 레이저 공격.
				if (Distance > 350.f)
				{
					if (m_iGrenade1Count >= 1 && m_iGrenade1Count < 3)
					{
						m_iAttack = 3;
						m_iDodgeCount = 0;
					}

					else
					{
						m_iAttack = 1;
						m_iDodgeCount = 0;
					}
				}

				// 거리가 가까우면 Dodge를 최대 2번까지 하고, 2 번 레이저 공격. 
				else
				{
					if (m_iDodgeCount >= 2)
					{
						m_iAttack = 2;
						m_iDodgeCount = 0;
					}

					else
					{
						// 구르기
						SetAnimNum(9);
						++m_iDodgeCount;

						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_roll.wav");
						pSound->Play(1, 0.5f, true);
					}
				}
			}
		}

		if (m_iAttack == 1)
		{
			Laser1();
		}

		// Phaser2에서는 m_iAttack에 2면 JumpGun을 사용
		else if (m_iAttack == 2)
		{
			JumpGun();
		}

		else if (m_iAttack == 3)
		{
			Grenade();
		}

		else if (m_iAttack == 4)
		{
			Laser3();
		}

		else if (m_iAttack == 5)
		{
			Dash();
		}

		else if (m_iAttack == 6)
		{
			BombRun();
		}
	}

	// Recover 상태인 경우
	else
	{
		m_fRecoverTime += DT;

		// 다시 나타나게 해야함.
		if (m_fRecoverTime > 1.5f)
		{
			m_iState = 0;
			m_fRecoverTime = 0.f;

			// Player 위치에서 더 먼쪽의 문에서 나타나도록 위치를 설정해줘야 함.
			Vec3 PlayerPos = m_TargetPlayer->Transform()->GetRelativePos();

			if (PlayerPos.x < 0.f || PlayerPos.x > 445.f)
			{
				m_IsLeft = true;

				m_NewMonsterPos = Vec3(445.f, -239.f, m_PrevMonsterPos.z);
			}

			else if (PlayerPos.x >= 0.f || PlayerPos.x <= -428.f)
			{
				m_IsLeft = false;

				m_NewMonsterPos = Vec3(-428.f, -239.f, m_PrevMonsterPos.z);
			}

			// 
			if (m_iHitCount == 1)
			{
				m_iAttack = 3;
			}

			else if (m_iHitCount == 2)
			{
				m_iAttack = 4;
			}

			else if (m_iHitCount == 3)
			{
				m_iAttack = 5;
			}

			else if (m_iHitCount == 4)
			{
				// 엔딩.
				Vec3 PlayerPos = m_TargetPlayer->Transform()->GetRelativePos();

				if (PlayerPos.x < 0.f || PlayerPos.x>445.f)
				{
					m_IsLeft = true;

					m_NewMonsterPos = Vec3(445.f, -239.f, m_PrevMonsterPos.z);

					m_iState = 0;
					m_iAttack = 6;
				}

				else if (PlayerPos.x >= 0.f || PlayerPos.x <= -428.f)
				{
					m_IsLeft = false;

					m_NewMonsterPos = Vec3(-428.f, -239.f, m_PrevMonsterPos.z);

					m_iState = 0;
					m_iAttack = 6;
				}
			}

			// Phase1 에서는 Recover 이후에 exit_door 만 사용함.
			SetAnimNum(10);
		}
	}

	if (m_iRWall > 0)
	{
		if (m_NewMonsterPos.x > m_PrevMonsterPos.x)
			m_NewMonsterPos.x = m_PrevMonsterPos.x;
	}

	if (m_iLWall > 0)
	{
		if (m_NewMonsterPos.x < m_PrevMonsterPos.x)
			m_NewMonsterPos.x = m_PrevMonsterPos.x;
	}

	if (m_iCeiling > 0)
	{
		if (m_NewMonsterPos.y > m_PrevMonsterPos.y)
			m_NewMonsterPos.y = m_PrevMonsterPos.y;
	}
}

void CHeadhunterScript::Grenade()
{
	// 애니메이션이 aim이나 Putback 중이 아니라면 총을 꺼냄.
	if (GetAnimNum() != 27 && GetAnimNum() != 23)
	{
		Vec3 PlayerPos = m_TargetPlayer->Transform()->GetRelativePos();
		Vec3 Dir = PlayerPos - m_NewMonsterPos;
		if (Dir.x > 0)
			m_IsLeft = false;
		else if (Dir.x < 0)
			m_IsLeft = true;

		m_iGrenade1Count = 0;
		SetAnimNum(29);
	}

	// shoot 중인 상태.
	if (GetAnimNum() == 27)
	{
		CAnim2D* Anim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
		int CurIndex = Anim->GetCurFrameIndex();

		// 발사
		if(CurIndex == 3 && !m_iGrenadeShot)
		{
			m_iGrenadeShot = true;

			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntressbomb_shot_01.wav");
			pSound->Play(1, 0.5f, true);

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Grenade.pref");
			CGameObject* Grenade = pEffectPrefab->Instantiate();
			Vec3 NewPos = Transform()->GetRelativePos();
			SpawnGameObject(Grenade, NewPos, L"Monster");

			CGrenadeScript* GrenadeScript = Grenade->GetScript<CGrenadeScript>();
			float MinGrenadeSpeed = 100.f;
			float GrenadeSpeed = 100.f;

			if (GrenadeScript)
			{
				// 속도 설정
				GrenadeScript->SetSpeed(static_cast<float>(rand()) / RAND_MAX * GrenadeSpeed + MinGrenadeSpeed);

				// 방향 설정
				float randomX = static_cast<float>(rand()) / RAND_MAX;
				float randomY = static_cast<float>(rand()) / RAND_MAX;
				Vec3 GrenadeDir = m_IsLeft ? Vec3(-randomX, randomY, 0.f) : Vec3(randomX, randomY, 0.f);
				GrenadeDir = Normalize(GrenadeDir);

				GrenadeScript->SetDir(GrenadeDir);
				GrenadeScript->SetGrenade(true);
			}
		}

		// 발사 종료.
		int EndIndex = Anim->GetEndFrameIndex();
		
		if(Anim->IsFinish() && m_iGrenadeShot)
		{
			m_iAttack = 0;
			++m_iGrenade1Count;
			m_iGrenadeShot = false;

			if (m_iGrenade1Count >= 3)
			{
				SetAnimNum(23);
				m_iGrenade1Count = 0;
			}
		}
	}

	// idle 상태가 된 경우.
	else if (m_iGrenade1Count >= 3 && GetAnimNum() == 17)
	{
		m_iGrenade1Count = 0;
		m_iAttack = 0;
		m_iGrenadeShot = false;
	}
}

void CHeadhunterScript::Dash()
{
	if (!m_bDashLine && !m_bDash)
	{
		Vec3 PlayerPos = m_TargetPlayer->Transform()->GetRelativePos();
		Vec3 Dir = PlayerPos - m_NewMonsterPos;

		if (Dir.x > 0)
		{
			m_IsLeft = false;
			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_redline.pref");
			CGameObject* Redline = pEffectPrefab->Instantiate();
			Vec3 NewPos = Transform()->GetRelativePos();
			NewPos.y += 20.f;
			SpawnGameObject(Redline, NewPos, L"Effect");
			Redline->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 3.14f));
			float ScaleX = 507.f - m_NewMonsterPos.x;
			Redline->Transform()->SetRelativeScale(Vec3(ScaleX * 2.f, 60.f, 1.f));
			Redline->SetLifeSpan(1.f);
		}

		else if (Dir.x < 0)
		{
			m_IsLeft = true;
			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_redline.pref");
			CGameObject* Redline = pEffectPrefab->Instantiate();
			Vec3 NewPos = Transform()->GetRelativePos();
			NewPos.y += 20.f;
			SpawnGameObject(Redline, NewPos, L"Effect");
			float ScaleX = m_NewMonsterPos.x + 510.f;
			Redline->Transform()->SetRelativeScale(Vec3(ScaleX * 2.f, 60.f, 1.f));
			Redline->SetLifeSpan(1.f);
		}

		SetAnimNum(20);

		m_bDashLine = true;
	}
	
	if (m_bDashLine && !m_bDash)
	{
		if (GetAnimNum() == 20)
		{
		}

		// Dash 중
		else if (GetAnimNum() == 4)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_dash_01.wav");
			pSound->Play(1, 0.5f, false);

			if (m_IsLeft)
			{
				// 벽에 닿으면 멈춤
				if (m_iLWall > 0)
				{
					m_bDash = true;
					SetAnimNum(5);
				}

				else
				{
					m_NewMonsterPos.x -= DT * m_fSpeed * 100.f;

					if (m_NewMonsterPos.x <= -487.f)
					{
						m_NewMonsterPos.x = -487.f;
					}
				}
			}

			else if (!m_IsLeft)
			{
				// 벽에 닿으면 멈춤
				if (m_iRWall > 0)
				{
					m_bDash = true;
					SetAnimNum(5);
				}

				else
				{
					m_NewMonsterPos.x += DT * m_fSpeed * 100.f;

					if (m_NewMonsterPos.x >= 487.f)
					{
						m_NewMonsterPos.x = 487.f;
					}
				}
			}
		}
	}

	if (GetAnimNum() == 5 && m_bDash)
	{
		CAnim2D* Anim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
		if (Anim->IsFinish())
		{
			m_iAttack = 0;
			m_bDashLine = false;
			m_bDash = false;
			SetAnimNum(17);
		}
	}
}

void CHeadhunterScript::Laser3()
{
	CAnim2D* Anim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
	int CurIndex = Anim->GetCurFrameIndex();

	m_IsLeft = false;

	static float fTime0 = 0.f;
	static float fTime1 = 0.f;
	static float fTime2 = 0.f;
	static float fTime3 = 0.f;
	static float fTime4 = 0.f;
	static float fTime5 = 0.f;
	static float fTime6 = 0.f;

	// 중력 끄기.
	vector<CScript*> vecObjScript = GetOwner()->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CGravityScript")
		{
			((CGravityScript*)vecObjScript[i])->CGravityScript::SetOnOff(false);
		}
	}

	if (m_iLaser3Step == 0)
	{
		m_NewMonsterPos = Vec3(-428.f, 128.f, 700.f);

		if (GetAnimNum() != 30 && GetAnimNum() != 33)
		{
			// 순간이동
			SetAnimNum(30);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_appear_1.wav");
			pSound->Play(1, 0.5f, true);
		}

		else if (GetAnimNum() == 30)
		{
			Vec3 Rot = { 0.f, 0.f, -3.14f / 2.f };
			float OffsetX = cosf(-3.14f / 2.f) * 40.f;
			float OffsetY = sinf(-3.14f / 2.f) * 40.f;

			Vec3 Scale = { 8000.f, 3.f,0.f };
			Vec3 Color = Vec3(1.0f, 1.0f, 0.0f);

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;

			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

			pCloneEffect->Animator2D()->SetArrangeColor(Color);

			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->SetLifeSpan(0.f);		// 1 프레임동안만 보여주기.

			if (Anim->IsFinish())
			{
				SetAnimNum(33);		//teleport_out;
				fTime0 = 0.f;

				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_vertical_1.wav");
				pSound->Play(1, 0.5f, true);
			}
		}

		else if (GetAnimNum() == 33)
		{
			fTime0 += DT;
			Ptr<CPrefab> pEffectPrefa2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Rifle_Collider.pref");
			CGameObject* pCloneEffect2 = pEffectPrefa2->Instantiate();
			Vec3 Rot = { 0.f, 0.f, -3.14f / 2.f };
			float OffsetX = cosf(-3.14f / 2.f) * 40.f;
			float OffsetY = sinf(-3.14f / 2.f) * 40.f;
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;
			SpawnGameObject(pCloneEffect2, vEffectPos, L"MonsterProjectile");
			pCloneEffect2->Transform()->SetRelativeRot(Rot);
			pCloneEffect2->Collider2D()->SetOffsetScale(Vec3(2000.f, 20.f, 1.f));
			pCloneEffect2->SetLifeSpan(0.f);

			Vec3 Scale = { 8000.f, 60.f * (1.f - fTime0), 1.f };
			Vec3 Color = Vec3(1.0f, 1.0f, ( 1.f- fTime0));

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);

			if (Anim->IsFinish())
			{
				SetAnimNum(30);		//teleport_in;
				++m_iLaser3Step;
				fTime0 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_appear_2.wav");
				pSound->Play(1, 0.5f, true);
			}
		}
	}

	else if (m_iLaser3Step == 1)
	{
		m_NewMonsterPos = Vec3(445.f, 128.f, 700.f);

		if (GetAnimNum() == 30)
		{
			Vec3 Rot = { 0.f, 0.f, -3.14f / 2.f };
			float OffsetX = cosf(-3.14f / 2.f) * 40.f;
			float OffsetY = sinf(-3.14f / 2.f) * 40.f;

			Vec3 Scale = { 8000.f, 3.f,0.f };
			Vec3 Color = Vec3(1.0f, 1.0f, 0.0f);

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;

			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

			pCloneEffect->Animator2D()->SetArrangeColor(Color);

			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->SetLifeSpan(0.f);		// 1 프레임동안만 보여주기.

			if (Anim->IsFinish())
			{
				SetAnimNum(33);		//teleport_out;
				fTime1 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_vertical_1.wav");
				pSound->Play(1, 0.5f, true);
			}
		}

		else if (GetAnimNum() == 33)
		{
			fTime1 += DT;
			Ptr<CPrefab> pEffectPrefa2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Rifle_Collider.pref");
			CGameObject* pCloneEffect2 = pEffectPrefa2->Instantiate();
			Vec3 Rot = { 0.f, 0.f, -3.14f / 2.f };
			float OffsetX = cosf(-3.14f / 2.f) * 40.f;
			float OffsetY = sinf(-3.14f / 2.f) * 40.f;
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;
			SpawnGameObject(pCloneEffect2, vEffectPos, L"MonsterProjectile");
			pCloneEffect2->Transform()->SetRelativeRot(Rot);
			pCloneEffect2->Collider2D()->SetOffsetScale(Vec3(2000.f, 20.f, 1.f));
			pCloneEffect2->SetLifeSpan(0.f);

			Vec3 Scale = { 8000.f, 60.f * (1.f - fTime0), 1.f };
			Vec3 Color = Vec3(1.0f, 1.0f, (1.f - fTime0));

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);

			if (Anim->IsFinish())
			{
				SetAnimNum(30);		//teleport_in;
				++m_iLaser3Step;
				fTime1 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_appear_3.wav");
				pSound->Play(1, 0.5f, true);
			}
		}
	}

	else if (m_iLaser3Step == 2)
	{
		m_NewMonsterPos = Vec3(-291.f, 128.f, 700.f);

		if (GetAnimNum() == 30)
		{
			Vec3 Rot = { 0.f, 0.f, -3.14f / 2.f };
			float OffsetX = cosf(-3.14f / 2.f) * 40.f;
			float OffsetY = sinf(-3.14f / 2.f) * 40.f;

			Vec3 Scale = { 8000.f, 3.f,0.f };
			Vec3 Color = Vec3(1.0f, 1.0f, 0.0f);

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;

			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

			pCloneEffect->Animator2D()->SetArrangeColor(Color);

			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->SetLifeSpan(0.f);		// 1 프레임동안만 보여주기.

			if (Anim->IsFinish())
			{
				SetAnimNum(33);		//teleport_out;
				fTime2 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_vertical_1.wav");
				pSound->Play(1, 0.5f, true);
			}
		}

		else if (GetAnimNum() == 33)
		{
			fTime2 += DT;
			Ptr<CPrefab> pEffectPrefa2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Rifle_Collider.pref");
			CGameObject* pCloneEffect2 = pEffectPrefa2->Instantiate();
			Vec3 Rot = { 0.f, 0.f, -3.14f / 2.f };
			float OffsetX = cosf(-3.14f / 2.f) * 40.f;
			float OffsetY = sinf(-3.14f / 2.f) * 40.f;
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;
			SpawnGameObject(pCloneEffect2, vEffectPos, L"MonsterProjectile");
			pCloneEffect2->Transform()->SetRelativeRot(Rot);
			pCloneEffect2->Collider2D()->SetOffsetScale(Vec3(2000.f, 20.f, 1.f));
			pCloneEffect2->SetLifeSpan(0.f);

			Vec3 Scale = { 8000.f, 60.f * (1.f - fTime0), 1.f };
			Vec3 Color = Vec3(1.0f, 1.0f, (1.f - fTime0));

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);

			if (Anim->IsFinish())
			{
				SetAnimNum(30);		//teleport_in;
				++m_iLaser3Step;
				fTime2 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_appear_1.wav");
				pSound->Play(1, 0.5f, true);
			}
		}
	}

	else if (m_iLaser3Step == 3)
	{
		m_NewMonsterPos = Vec3(316.f, 128.f, 700.f);

		if (GetAnimNum() == 30)
		{
			Vec3 Rot = { 0.f, 0.f, -3.14f / 2.f };
			float OffsetX = cosf(-3.14f / 2.f) * 40.f;
			float OffsetY = sinf(-3.14f / 2.f) * 40.f;

			Vec3 Scale = { 8000.f, 3.f,0.f };
			Vec3 Color = Vec3(1.0f, 1.0f, 0.0f);

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;

			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

			pCloneEffect->Animator2D()->SetArrangeColor(Color);

			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->SetLifeSpan(0.f);		// 1 프레임동안만 보여주기.

			if (Anim->IsFinish())
			{
				SetAnimNum(33);		//teleport_out;
				fTime3 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_vertical_1.wav");
				pSound->Play(1, 0.5f, true);
			}
		}

		else if (GetAnimNum() == 33)
		{
			fTime3 += DT;
			Ptr<CPrefab> pEffectPrefa2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Rifle_Collider.pref");
			CGameObject* pCloneEffect2 = pEffectPrefa2->Instantiate();
			Vec3 Rot = { 0.f, 0.f, -3.14f / 2.f };
			float OffsetX = cosf(-3.14f / 2.f) * 40.f;
			float OffsetY = sinf(-3.14f / 2.f) * 40.f;
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;
			SpawnGameObject(pCloneEffect2, vEffectPos, L"MonsterProjectile");
			pCloneEffect2->Transform()->SetRelativeRot(Rot);
			pCloneEffect2->Collider2D()->SetOffsetScale(Vec3(2000.f, 20.f, 1.f));
			pCloneEffect2->SetLifeSpan(0.f);

			Vec3 Scale = { 8000.f, 60.f * (1.f - fTime0), 1.f };
			Vec3 Color = Vec3(1.0f, 1.0f, (1.f - fTime0));

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);

			if (Anim->IsFinish())
			{
				SetAnimNum(32);		//teleport_in_sweep;
				++m_iLaser3Step;
				fTime3 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_appear_2.wav");
				pSound->Play(1, 0.5f, true);
			}
		}
	}

	else if (m_iLaser3Step == 4)
	{
		m_NewMonsterPos = Vec3(-421.f, 85.f, 700.f);

		if (GetAnimNum() == 32)
		{
			if (Anim->IsFinish())
			{
				SetAnimNum(28);		//Sweep;
				fTime4 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_swipe_01.wav");
				pSound->Play(1, 0.5f, false);
			}
		}

		else if (GetAnimNum() == 28)
		{
			fTime4 += DT;
			//
			float theta = -3.14f * 0.75f + fTime4 * 2.5f;
			Vec3 Rot = { 0.f, 0.f, theta };
			float OffsetX = cosf(theta) * 40.f;
			float OffsetY = sinf(theta) * 40.f;

			// 각도에 따라 위치 살짝 조정. 
			Vec3 Pos = m_NewMonsterPos;
			Pos.x += OffsetX;
			Pos.y += OffsetY + 30.f;
			Pos.z -= 100.f;
			Vec3 Color = Vec3(1.0f, 1.0f, (cosf(fTime4 * 10.f) / 2.f + 1.f));

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RiffleEffectAndCol.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Pos, L"MonsterProjectile");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);

			float unit = 3.14f / 18.f;	// 10 도

			for (int i = 0; i < 18; ++i)
			{
				if (theta < -unit * i && theta >= -unit * (i + 1))
				{
					Anim->SetCurIndex(i);
					Anim->SetStop(true);
					break;
				}
			}

			if (theta>=0.f)
			{
				SetAnimNum(32);		//teleport_in_sweep;
				++m_iLaser3Step;
				fTime4 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_appear_3.wav");
				pSound->Play(1, 0.5f, true);
			}
		}
	}

	else if (m_iLaser3Step == 5)
	{
		m_NewMonsterPos = Vec3(445.f, 85.f, 700.f);

		if (GetAnimNum() == 32)
		{
			if (Anim->IsFinish())
			{
				SetAnimNum(28);		//Sweep;
				fTime5 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_swipe_01.wav");
				pSound->Play(1, 0.5f, false);
			}
		}

		else if (GetAnimNum() == 28)
		{
			fTime5 += DT;
			//
			float theta = -3.14f * 0.25f - fTime5 * 2.5f;
			Vec3 Rot = { 0.f, 0.f, theta };
			float OffsetX = cosf(theta) * 40.f;
			float OffsetY = sinf(theta) * 40.f;

			// 각도에 따라 위치 살짝 조정. 
			Vec3 Pos = m_NewMonsterPos;
			Pos.x += OffsetX;
			Pos.y += OffsetY + 30.f;
			Pos.z -= 100.f;
			Vec3 Color = Vec3(1.0f, 1.0f, (cosf(fTime5 * 10.f) / 2.f + 1.f));

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RiffleEffectAndCol.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Pos, L"MonsterProjectile");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);

			float unit = 3.14f / 18.f;	// 10 도

			for (int i = 0; i < 18; ++i)
			{
				if (theta < - unit * i && theta >= - unit * (i + 1))
				{
					Anim->SetCurIndex(i);
					Anim->SetStop(true);
					break;
				}
			}

			if (theta <= -3.14f)
			{
				SetAnimNum(31);		//teleport_in_ground;
				++m_iLaser3Step;
				fTime5 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_appear_1.wav");
				pSound->Play(1, 0.5f, true);
			}
		}
	}

	else if (m_iLaser3Step == 6)
	{
		m_NewMonsterPos = Vec3(303.f, -239.f, 700.f);

		m_IsLeft = true;

		if (GetAnimNum() == 31)
		{
			Vec3 Rot = { 0.f, 0.f, -3.14f };
			float OffsetX = cosf(-3.14f) * 40.f;
			float OffsetY = sinf(-3.14f) * 40.f;

			Vec3 Scale = { 8000.f, 3.f,0.f };
			Vec3 Color = Vec3(1.0f, 1.0f, 0.0f);

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;

			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

			pCloneEffect->Animator2D()->SetArrangeColor(Color);

			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->SetLifeSpan(0.f);		// 1 프레임동안만 보여주기.

			if (Anim->IsFinish())
			{
				SetAnimNum(34);		//teleport_out_ground;
				fTime6 = 0.f;
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_shot_1.wav");
				pSound->Play(1, 0.5f, false);
			}
		}

		else if (GetAnimNum() == 34)
		{
			fTime6 += DT;
			Ptr<CPrefab> pEffectPrefa2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Rifle_Collider.pref");
			CGameObject* pCloneEffect2 = pEffectPrefa2->Instantiate();
			Vec3 Rot = { 0.f, 0.f, -3.14f };
			float OffsetX = cosf(-3.14f) * 40.f;
			float OffsetY = sinf(-3.14f) * 40.f;
			Vec3 vEffectPos = m_NewMonsterPos;

			// 각도에 따라 위치 살짝 조정. 
			vEffectPos.x += OffsetX;
			vEffectPos.y += OffsetY + 30.f;
			vEffectPos.z -= 100.f;
			SpawnGameObject(pCloneEffect2, vEffectPos, L"MonsterProjectile");
			pCloneEffect2->Transform()->SetRelativeRot(Rot);
			pCloneEffect2->Collider2D()->SetOffsetScale(Vec3(2000.f, 20.f, 1.f));
			pCloneEffect2->SetLifeSpan(0.f);

			Vec3 Scale = { 8000.f, 60.f * (1.f - fTime6), 1.f };
			Vec3 Color = Vec3(1.0f, 1.0f, (1.f - fTime6));

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);

			if (Anim->IsFinish())
			{
				SetAnimNum(17);		//idle;
				m_iLaser3Step = 0;
				fTime6 = 0.f;

				// 마지막에
				m_iAttack = 0;

				// 중력 다시 키기
				vector<CScript*> vecObjScript = GetOwner()->GetScripts();

				for (size_t i = 0; i < vecObjScript.size(); ++i)
				{
					wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

					if (ScriptName == L"CGravityScript")
					{
						((CGravityScript*)vecObjScript[i])->CGravityScript::SetOnOff(true);
					}
				}
			}
		}
	}
}

void CHeadhunterScript::JumpGun()
{
	float MapCenter = 0.f;

	// 여기에 구르는 상태이거나 exit door 상태면 값전부 초기화 시키게 해야겠는데?
	if (GetAnimNum() == 9)
	{
		m_JumpGunFirstJump = false;
		m_JumpGunSecondJump = false;
		m_JumpGunLeftWallJump = false;
		m_JumpGunTheta = -3.14f;
		m_fJumpGunShotTime = 0.f;
	}

	if (m_iPhase == 1)
		MapCenter = 15.f;

	// 가까운 벽으로 점프.
	if (!m_JumpGunFirstJump && !m_JumpGunSecondJump)
	{
		if (GetAnimNum() == 39)
		{
		}

		else if (GetAnimNum() != 18 && GetAnimNum() != 9)
		{
			SetAnimNum(21);
		}

		else if (GetAnimNum() == 18)
		{
			// 왼쪽 벽으로 점프
			if (m_NewMonsterPos.x < MapCenter)
			{
				m_JumpGunLeftWallJump = true;
				m_JumpGunTheta = 0.f;
			}

			vector<CScript*> vecObjScript = GetOwner()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CGravityScript")
				{
					((CGravityScript*)vecObjScript[i])->CGravityScript::HeadHunterJump();
				}
			}

			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_jump_01.wav");
			pSound->Play(1, 0.5f, true);

			m_NewMonsterPos.y += 10.f;
			m_JumpGunFirstJump = true;
		}
	}

	// 벽으로 점프 중
	else if (m_JumpGunFirstJump && !m_JumpGunSecondJump)
	{
		// 벽과 충돌한 경우.
		if (m_iLWall > 0 || m_iRWall > 0)
		{
			vector<CScript*> vecObjScript = GetOwner()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CGravityScript")
				{
					((CGravityScript*)vecObjScript[i])->CGravityScript::HeadHunterJump();
				}
			}

			SetAnimNum(38);
			m_NewMonsterPos.y += 10.f;
			m_JumpGunSecondJump = true;
		}

		// 벽과 충돌하기 전.
		else
		{
			// 오른 쪽 벽으로 점프
			if (!m_JumpGunLeftWallJump)
			{
				m_fJumpGunShotTime = 0.f;
				m_NewMonsterPos.x += DT * m_fSpeed * 6.f;
			}

			// 왼쪽 벽으로 점프
			else
			{
				m_fJumpGunShotTime = 0.f;
				m_NewMonsterPos.x -= DT * m_fSpeed * 6.f;
			}
		}
	}

	// 벽 점프 중
	else if (m_JumpGunFirstJump && m_JumpGunSecondJump)
	{
		m_fJumpGunTime += DT;

		// 오른 쪽 벽에서 점프
		if (!m_JumpGunLeftWallJump)
		{
			m_NewMonsterPos.x -= DT * m_fSpeed * 6.f;

			//
			m_JumpGunTheta += DT * 2.5f;
			Vec3 Rot = { 0.f, 0.f, m_JumpGunTheta };
			float OffsetX = cosf(m_JumpGunTheta) * 60.f;
			float OffsetY = sinf(m_JumpGunTheta) * 60.f;

			// 각도에 따라 위치 살짝 조정. 
			Vec3 Pos = m_NewMonsterPos;
			Pos.x += OffsetX;
			Pos.y += OffsetY;
			Pos.z -= 100.f;
			Vec3 Color = Vec3(1.0f, 1.0f, (cosf(m_fJumpGunTime * 10.f) / 2.f + 1.f));

			// 일정 시간마다 총알이 날아가야 함. 
			m_fJumpGunShotTime += DT;

			if (m_fJumpGunShotTime > 0.1f)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_gatling_01.wav");
				pSound->Play(1, 0.5f, true);

				m_fJumpGunShotTime -= m_fJumpGunShotTime;

				Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Bullet.pref");
				CGameObject* pCloneBullet = pEffectPrefab->Instantiate();
				SpawnGameObject(pCloneBullet, Pos, L"MonsterProjectile");

				pCloneBullet->Transform()->SetRelativeRot(Rot);
				Vec3 Scale = pCloneBullet->Transform()->GetRelativeScale();
				Scale.x *= 3.5f;
				pCloneBullet->Transform()->SetRelativeScale(Scale);

				CBulletScript* BulletScript = pCloneBullet->GetScript<CBulletScript>();

				Vec3 Dir = Normalize(Vec3(cosf(m_JumpGunTheta), sinf(m_JumpGunTheta), 0.f));

				if (BulletScript)
				{
					BulletScript->SetDir(Dir);
					BulletScript->SetSpeed(200.f);
				}
			}

			/*Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RiffleEffectAndCol.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Pos, L"MonsterProjectile");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);*/
		}

		// 왼쪽 벽에서 점프
		else
		{
			m_NewMonsterPos.x += DT * m_fSpeed * 6.f;

			//
			m_JumpGunTheta -= DT * 2.5f;
			Vec3 Rot = { 0.f, 0.f, m_JumpGunTheta };
			float OffsetX = cosf(m_JumpGunTheta) * 60.f;
			float OffsetY = sinf(m_JumpGunTheta) * 60.f;

			// 각도에 따라 위치 살짝 조정. 
			Vec3 Pos = m_NewMonsterPos;
			Pos.x += OffsetX;
			Pos.y += OffsetY;
			Pos.z -= 100.f;
			Vec3 Color = Vec3(1.0f, 1.0f, (cosf(m_fJumpGunTime * 10.f) / 2.f + 1.f));

			// 일정 시간마다 총알이 날아가야 함. 
			m_fJumpGunShotTime += DT;

			if (m_fJumpGunShotTime > 0.1f)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_gatling_01.wav");
				pSound->Play(1, 0.5f, true);

				m_fJumpGunShotTime -= m_fJumpGunShotTime;
				
				Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Bullet.pref");
				CGameObject* pCloneBullet = pEffectPrefab->Instantiate();
				SpawnGameObject(pCloneBullet, Pos, L"MonsterProjectile");

				pCloneBullet->Transform()->SetRelativeRot(Rot);
				Vec3 Scale = pCloneBullet->Transform()->GetRelativeScale();
				Scale.x *= 3.5f;
				pCloneBullet->Transform()->SetRelativeScale(Scale);

				CBulletScript* BulletScript = pCloneBullet->GetScript<CBulletScript>();

				Vec3 Dir = Normalize(Vec3(cosf(m_JumpGunTheta), sinf(m_JumpGunTheta), 0.f));

				if (BulletScript)
				{
					BulletScript->SetDir(Dir);
					BulletScript->SetSpeed(200.f);
				}
			}
		}

		// 착지
		if (m_iGround > 0)
		{
			SetAnimNum(39);
			m_iAttack = 0;
			m_JumpGunFirstJump = false;
			m_JumpGunSecondJump = false;
			m_JumpGunLeftWallJump = false;
			m_JumpGunTheta = -3.14f;
			m_fJumpGunShotTime = 0.f;
		}
	}
}

void CHeadhunterScript::BombRun()
{
	// Dead
	if (GetAnimNum() == 6)
	{
		CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());

		if (CurAnim->IsFinish())
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_voiceboss_huntress_ko_01.wav");
			pSound->Play(1, 0.5f, false);

			m_iState = 2;
			
			// Arrow
			Ptr<CPrefab> pEffectPrefab3 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\UI_Arrow.pref");
			CGameObject* pCloneEffect3 = pEffectPrefab3->Instantiate();
			Vec3 vEffectPos3 = { 0.f, -139.f,1.f };
			SpawnGameObject(pCloneEffect3, vEffectPos3, L"Effect");
			pCloneEffect3->Transform()->SetRelativeScale(Vec3(700.f, 700.f, 0.f));
			pCloneEffect3->Transform()->SetRelativeRot(Vec3(0.f, 0.f, -3.14f /2.f));

			// NextLevel 
			Ptr<CPrefab> pEffectPrefab2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\NextLevel.pref");
			CGameObject* pCloneEffect2 = pEffectPrefab2->Instantiate();
			Vec3 vEffectPos2 = { 0.f, -239.f,1.f };
			SpawnGameObject(pCloneEffect2, vEffectPos2, L"MonsterProjectile");
			pCloneEffect2->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 0.f));
		}
	}

	else if (GetAnimNum() != 3 && GetAnimNum() != 7 && GetAnimNum() != 8)
	{
		SetAnimNum(26);
	}

	// 달리는 중.
	else if (GetAnimNum() == 3)
	{
		if (m_IsLeft)
		{
			m_NewMonsterPos.x -= DT * m_fSpeed * 7.f;

			if (m_NewMonsterPos.x <= -480.f)
			{
				m_NewMonsterPos.x = -480.f;

				SetAnimNum(7);
			}
		}

		else if (!m_IsLeft)
		{
			m_NewMonsterPos.x += DT * m_fSpeed * 7.f;

			if (m_NewMonsterPos.x >= 480.f)
			{
				m_NewMonsterPos.x = 480.f;

				SetAnimNum(7);
			}
		}
	}

	
}

void CHeadhunterScript::Laser1()
{
	m_fLaser1Time += DT;

	CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());

	// 애니메이션이 aim이나 Putback 중이 아니라면 총을 꺼냄.
	if (GetAnimNum() != 2 && GetAnimNum() != 22)
	{
		m_iLaser1Count = 0;
		SetAnimNum(24);
		m_fLaser1Time = 0.f;
	}

	// aim 중인 상태.
	if (GetAnimNum() == 2)
	{
		// 조준. 프레임 조절. 선 그리기. 음... 근데 선의 시작 위치가 총구여야 하네... 음... 
		if (m_fLaser1Time < 1.f)
		{
			Vec3 PlayerPos = m_TargetPlayer->Transform()->GetRelativePos();
			Vec3 Dir = PlayerPos - m_NewMonsterPos;
			float theta = atan2f(Dir.y, Dir.x);
			
			Vec3 Rot = { 0.f, 0.f, theta };
			float OffsetX = cosf(theta) * 40.f;
			float OffsetY = sinf(theta) * 40.f;

			Vec3 Scale = { 8000.f, 3.f,0.f };
			Vec3 Color = Vec3(1.0f, 1.0f, 0.0f);

			if (m_fLaser1Time > 0.5f)
			{
				if (m_fLaser1Time <= 0.6f)
				{
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_lockon_01.wav");
					pSound->Play(1, 0.5f, false);
				}

				Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
				CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
				Vec3 vEffectPos = m_NewMonsterPos;

				// 각도에 따라 위치 살짝 조정. 
				vEffectPos.x += OffsetX;
				vEffectPos.y += OffsetY + 30.f;
				vEffectPos.z -= 100.f;

				SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

				pCloneEffect->Animator2D()->SetArrangeColor(Color);

				pCloneEffect->Transform()->SetRelativeRot(Rot);
				pCloneEffect->Transform()->SetRelativeScale(Scale);
				pCloneEffect->SetLifeSpan(0.f);		// 1 프레임동안만 보여주기.

				m_Laser1Pos = vEffectPos;
				m_Laser1Rot = Rot;
			}

			// theta 값에 따라 프레임 조정.
			if (theta > 3.14f / 2.f)
			{
				theta = 3.14f - theta;
				m_IsLeft = true;
			}
			else if (theta < -3.14f / 2.f)
			{
				theta = -3.14f - theta;
				m_IsLeft = true;
			}
			else
			{
				m_IsLeft = false;
			}

			float unit = 3.14f / 18.f;	// 10 도

			// 각도를 인덱스로 변환
			int index = static_cast<int>((theta + 3.14f / 2.f) / unit);

			// 인덱스 유효성 검사 (0 이상 18 이하)
			if (index >= 0 && index < 19)
			{
				CurAnim->SetCurIndex(index);
				CurAnim->SetStop(true);
			}
		}

		// 잠시 대기
		else if (m_fLaser1Time < 1.3f)
		{
		}

		// 발사.
		else if (m_fLaser1Time < 1.5f)
		{
			if (!m_Phase1Shot)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_shot_1.wav");
				pSound->Play(1, 0.5f, false);

				Ptr<CPrefab> pEffectPrefa2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Rifle_Collider.pref");
				CGameObject* pCloneEffect2 = pEffectPrefa2->Instantiate();
				SpawnGameObject(pCloneEffect2, m_Laser1Pos, L"MonsterProjectile");
				pCloneEffect2->Transform()->SetRelativeRot(m_Laser1Rot);
				pCloneEffect2->Collider2D()->SetOffsetScale(Vec3(2000.f, 20.f, 1.f));
				pCloneEffect2->SetLifeSpan(0.2f);

				m_Phase1Shot = true;
			}

			Vec3 Scale = { 8000.f, 60.f * (2.f - m_fLaser1Time) * 2.f, 1.f };
			Vec3 Color = Vec3(1.0f, 1.0f, (m_fLaser1Time - 1.f) * 2.f);

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Rifle.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, m_Laser1Pos, L"Effect");
			pCloneEffect->Transform()->SetRelativeRot(m_Laser1Rot);
			pCloneEffect->Transform()->SetRelativeScale(Scale);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);		
		}

		// 발사 종료.
		else
		{
			m_iAttack = 0;
			++m_iLaser1Count;
			m_fLaser1Time = 0.f;
			m_Phase1Shot = false;

			if (m_iLaser1Count >= 3)
			{
				SetAnimNum(22);
				m_iLaser1Count = 0;
			}
		}
	}

	// idle 상태가 된 경우.
	else if (m_iLaser1Count >= 3 && GetAnimNum() == 17)
	{
		m_iLaser1Count = 0;
		m_iAttack = 0;
		m_fLaser1Time = 0.f;
	}
}

void CHeadhunterScript::Laser2()
{
	float MapCenter = 0.f;

	// 여기에 구르는 상태이거나 exit door 상태면 값전부 초기화 시키게 해야겠는데?
	if (GetAnimNum() == 9 || GetAnimNum() == 10)
	{
		m_Laser2FirstJump = false;
		m_Laser2SecondJump = false;
		m_Laser2LeftWallJump = false;
		m_Laser2Theta = -3.14f;
	}

	if (m_iPhase == 1)
		MapCenter = 15.f;

	// 가까운 벽으로 점프.
	if (!m_Laser2FirstJump &&!m_Laser2SecondJump)
	{
		if (GetAnimNum() == 39)
		{
		}

		else if (GetAnimNum() != 18 && GetAnimNum() != 9)
		{
			SetAnimNum(21);
		}

		else if (GetAnimNum() == 18)
		{
			// 왼쪽 벽으로 점프
			if (m_NewMonsterPos.x < MapCenter)
			{
				m_Laser2LeftWallJump = true;
				m_Laser2Theta = 0.f;
			}

			vector<CScript*> vecObjScript = GetOwner()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CGravityScript")
				{
					((CGravityScript*)vecObjScript[i])->CGravityScript::HeadHunterJump();
				}
			}

			m_NewMonsterPos.y += 10.f;
			m_Laser2FirstJump = true;
		}
	}

	// 벽으로 점프 중
	else if(m_Laser2FirstJump && !m_Laser2SecondJump)
	{
		// 벽과 충돌한 경우.
		if (m_iLWall > 0 || m_iRWall > 0)
		{
			vector<CScript*> vecObjScript = GetOwner()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CGravityScript")
				{
					((CGravityScript*)vecObjScript[i])->CGravityScript::HeadHunterJump();
				}
			}

			SetAnimNum(38);
			m_NewMonsterPos.y += 10.f;
			m_Laser2SecondJump = true;

			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntresslaser_swipe_01.wav");
			pSound->Play(1, 0.5f, false);
		}

		// 벽과 충돌하기 전.
		else
		{
			// 오른 쪽 벽으로 점프
			if (!m_Laser2LeftWallJump)
			{
				m_NewMonsterPos.x += DT * m_fSpeed * 6.f;
			}

			// 왼쪽 벽으로 점프
			else
			{
				m_NewMonsterPos.x -= DT * m_fSpeed * 6.f;
			}
		}
	}

	// 벽 점프 중
	else if (m_Laser2FirstJump && m_Laser2SecondJump)
	{
		m_fLaser2Time += DT;

		// 오른 쪽 벽에서 점프
		if (!m_Laser2LeftWallJump)
		{
			m_NewMonsterPos.x -= DT * m_fSpeed * 6.f;

			//
			m_Laser2Theta += DT * 2.5f;
			Vec3 Rot = { 0.f, 0.f, m_Laser2Theta };
			float OffsetX = cosf(m_Laser2Theta) * 40.f;
			float OffsetY = sinf(m_Laser2Theta) * 40.f;

			// 각도에 따라 위치 살짝 조정. 
			Vec3 Pos = m_NewMonsterPos;
			Pos.x += OffsetX;
			Pos.y += OffsetY + 30.f;
			Pos.z -= 100.f;
			Vec3 Color = Vec3(1.0f, 1.0f, (cosf(m_fLaser2Time * 10.f) / 2.f + 1.f));

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RiffleEffectAndCol.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Pos, L"MonsterProjectile");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);
		}

		// 왼쪽 벽에서 점프
		else
		{
			m_NewMonsterPos.x += DT * m_fSpeed * 6.f;

			//
			m_Laser2Theta -= DT * 2.5f;
			Vec3 Rot = { 0.f, 0.f, m_Laser2Theta };
			float OffsetX = cosf(m_Laser2Theta) * 40.f;
			float OffsetY = sinf(m_Laser2Theta) * 40.f;

			// 각도에 따라 위치 살짝 조정. 
			Vec3 Pos = m_NewMonsterPos;
			Pos.x += OffsetX;
			Pos.y += OffsetY + 30.f;
			Pos.z -= 100.f;
			Vec3 Color = Vec3(1.0f, 1.0f, (cosf(m_fLaser2Time * 10.f) / 2.f + 1.f));

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RiffleEffectAndCol.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Pos, L"MonsterProjectile");
			pCloneEffect->Transform()->SetRelativeRot(Rot);
			pCloneEffect->Animator2D()->SetArrangeColor(Color);
			pCloneEffect->SetLifeSpan(0.f);
		}

		// 착지
		if (m_iGround > 0)
		{
			SetAnimNum(39);
			m_iAttack = 0;
			m_Laser2FirstJump = false;
			m_Laser2SecondJump = false;
			m_Laser2LeftWallJump = false;
			m_Laser2Theta = -3.14f;
		}
	}
}

void CHeadhunterScript::Dodge()
{
	m_iAttack = 0;

	if (Animator2D()->IsLeft() <= 0)
	{
		if (!m_iRWall)
		{
			m_NewMonsterPos.x += DT * m_fSpeed * 4.8f;
		}
	}

	else
	{
		if (!m_iLWall)
		{
			m_NewMonsterPos.x -= DT * m_fSpeed * 4.8f;
		}
	}

	static float DashTime = 0.f;
	DashTime += DT;

	if (DashTime > 0.05f)
	{
		DirtCloudEffect(!Animator2D()->IsLeft(), true);
		DashTime -= 0.05f;
	}
}

void CHeadhunterScript::DirtCloudEffect(bool _bLeft, bool _bDash)
{
	// Cloud Effect 프리팹 참조
	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_player_cloud.pref");
	Vec3 vEffectPos = Transform()->GetRelativePos();
	vEffectPos.y += Collider2D()->GetOffsetPos().y - Collider2D()->GetOffsetScale().y / 2.f;
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();

	// 범위 내에서 Random한 위치에 생성
	float Range = 40.f;
	Vec3 RandomOffsetPos = Vec3((float)rand() / RAND_MAX * Range - Range / 2.f, (float)rand() / RAND_MAX * Range / 4.f, 0.f);

	vEffectPos += RandomOffsetPos;

	// 레벨에 추가
	SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

	// Animator2D 방향 정해주기
	if (_bLeft)
		pCloneEffect->Animator2D()->SetIsLeft(1);

	else
		pCloneEffect->Animator2D()->SetIsLeft(0);

	vector<CScript*> vecObjScript = pCloneEffect->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CEffectScript")
		{
			((CEffectScript*)vecObjScript[i])->CEffectScript::SetPlayerDustCloud(true);
		}
	}
}

void CHeadhunterScript::CreateExplosion(Vec3 _OtherPos)
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_explosion_01.wav");
	pSound->Play(1, 0.5f, false);

	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Explosion.pref");
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
	_OtherPos.z = 100.f;
	SpawnGameObject(pCloneEffect, _OtherPos, L"Effect");
}

void CHeadhunterScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_iPhase, sizeof(int), 1, _File);
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CHeadhunterScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_iPhase, sizeof(int), 1, _FILE);
	fread(&m_fSpeed, sizeof(float), 1, _FILE);
}

