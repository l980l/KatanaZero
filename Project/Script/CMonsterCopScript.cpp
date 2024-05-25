#include "pch.h"
#include "CMonsterCopScript.h"

#include "CPlayerScript.h"
#include "CEffectScript.h"
#include "CGravityScript.h"
#include "CMonsterAttackScript.h"
#include "CBulletScript.h"

#include <Engine\CAnim2D.h>

CMonsterCopScript::CMonsterCopScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERCOPSCRIPT)
	, m_fSpeed(33.f)
	, m_iGround(0)
	, m_iGroundToFall(0)
	, m_iLWall(0)
	, m_iRWall(0)
	, m_iCeiling(0)
	, m_iLStair(0)
	, m_iRStair(0)
	, m_AnimNum(0)
	, m_RewindTime(0)
	, m_iPlayerDetect(0)
	, m_bRewind(false)
	, m_IsLeft(false)
	, m_TargetPlayer(nullptr)
	, m_iFloorLevel(1)
	, m_iPrevFloorLevel(1)
	, m_bGoToStair(false)
	, m_bAttackReady(false)
	, m_iState(0)
	, m_fBleedingTime(0.f)
	, m_fBloodWallTime(0.f)
	, m_bArms(false)
	, m_fShotTime(0.f)
	, m_RewindScale(1.f)
	, m_bBossMap(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iFloorLevel, "FloorLevel");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iPlayerDetect, "PlayerDetect");

	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed      ");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iStandByType, "StandByType");
	AddScriptParam(SCRIPT_PARAM::VEC4, &m_arrStairData[0], "Stair1     ");
	AddScriptParam(SCRIPT_PARAM::VEC4, &m_arrStairData[1], "Stair2     ");
	AddScriptParam(SCRIPT_PARAM::VEC4, &m_arrStairData[2], "Stair3     ");
}

CMonsterCopScript::~CMonsterCopScript()
{
}

void CMonsterCopScript::SetDetect(int _iDetect)
{
	m_iPlayerDetect = _iDetect;

	// 느낌표 띄우기
	if (m_iPlayerDetect == 1)
	{
		// LandCloud Effect 프리팹 참조
		Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_MonsterExpression.pref");
		CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
		Vec3 vEffectPos = Transform()->GetRelativePos();
		SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

		CEffectScript* EffectScript = pCloneEffect->GetScript<CEffectScript>();
		EffectScript->SetLoop(true);
		EffectScript->SetTarget(GetOwner());
		EffectScript->SetEmoation(true);

		CAnim2D* Anim = pCloneEffect->Animator2D()->FindAnim(pCloneEffect->Animator2D()->GetCurAnimationName());
		Anim->SetCurIndex(4);
		Anim->SetStop(true);
	}
}

void CMonsterCopScript::Rewind(float Scale, bool Backward)
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

			MonsterCopRewindData TempData = {};

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

			// 팔
			vector<CGameObject*> vecChild = GetOwner()->GetChild();

			CGameObject* Gun = nullptr;

			int Size = (int)vecChild.size();

			for (int i = 0; i < Size; ++i)
			{
				wstring Name = vecChild[i]->GetName();

				if (Name == L"Cop_gun")
				{
					Gun = vecChild[i];
				}
			}

			if (Gun)
			{
				if (TempData.AnimNum == 7)
				{
					CAnim2D* Anim = Gun->Animator2D()->FindAnim(Gun->Animator2D()->GetCurAnimationName());
					Anim->SetStop(true);
					Anim->SetCurIndex(0);

					Gun->Transform()->SetRelativeRot(TempData.GunRot);

					if (TempData.AnimIsLeft)
					{
						Gun->Transform()->SetRelativePos(Vec3(11.f, 14.f, -1.f));
						Gun->Animator2D()->SetIsLeft(true);
					}

					else
					{
						Gun->Transform()->SetRelativePos(Vec3(-11.f, 14.f, -1.f));
						Gun->Animator2D()->SetIsLeft(false);
					}
				}

				else
				{
					CAnim2D* Anim = Gun->Animator2D()->FindAnim(Gun->Animator2D()->GetCurAnimationName());
					Anim->SetStop(true);
					Anim->SetCurIndex(1);
				}
			}

			switch (m_AnimNum)
			{
			case 0:
				Animator2D()->Play(L"animation\\Cop_walk.anim", true);
				break;
			case 1:
				Animator2D()->Play(L"animation\\Cop_leavestair.anim", true);
				break;
			case 2:
				Animator2D()->Play(L"animation\\Cop_run.anim", true);
				break;
			case 3:
				Animator2D()->Play(L"animation\\Cop_turn.anim", true);
				break;
			case 4:
				Animator2D()->Play(L"animation\\Cop_enterstair.anim", true);
				break;
			case 5:
				Animator2D()->Play(L"animation\\Cop_hurtground.anim", true);
				break;
			case 6:
				Animator2D()->Play(L"animation\\Cop_idle.anim", true);
				break;
			case 7:
				Animator2D()->Play(L"animation\\Cop_aim.anim", true);
				break;
			case 8:
				Animator2D()->Play(L"animation\\Cop_hurtground.anim", false);
				break;
			}

			CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
			CurAnim->SetCurIndex(TempData.AnimFrame);
			CurAnim->SetStop(true);
		}
	}
}

void CMonsterCopScript::StartRewind()
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

void CMonsterCopScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	for (int i = 0; i < 8; ++i)
	{
		m_vecNextAnim.push_back(-1);
	}

	// 0 Cop_walk 1 Cop_leavestair 2 Cop_run 3 Cop_turn 4 Cop_enterstair 5 Cop_hurtground 6 Cop_idle 7 Cop_aim 8 Cop_hurtground
	m_vecNextAnim[3] = 0;
}

void CMonsterCopScript::tick()
{
	if (m_bRewind)
	{
		Rewind(0.1f, true);
		return;
	}

	m_NewMonsterPos = Transform()->GetRelativePos();
	
	//
	m_bArms = false;

	// Gun이랑 AttackRange 잡아두기
	vector<CGameObject*> vecChild = GetOwner()->GetChild();

	CGameObject* Gun = nullptr;
	CGameObject* AttackRange = nullptr;

	int Size = (int)vecChild.size();

	for (int i = 0; i < Size; ++i)
	{
		wstring Name = vecChild[i]->GetName();

		if (Name == L"Cop_gun")
		{
			Gun = vecChild[i];
		}

		if (Name == L"MonsterAttackRange")
		{
			AttackRange = vecChild[i];
		}
	}

	Vec3 GunRot = {};

	// hurt
	if (m_iState == 1)
	{
		if (GetAnimNum() == 8)
		{
			CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());

			// Hurt animation이 끝났다면 Dead 상태. 
			if (CurAnim->IsFinish())
			{
				m_iState = 2;
				CurAnim->SetStop(true);
			}
		}

		if (m_KnockBackDir.x > 0)
		{
			m_IsLeft = true;

			if (!m_iRWall)
			{
				m_NewMonsterPos.x += m_KnockBackDir.x * DT * m_fSpeed * 8.f;
			}
		}

		else if (m_KnockBackDir.x < 0)
		{
			m_IsLeft = false;

			if (!m_iLWall)
			{
				m_NewMonsterPos.x += m_KnockBackDir.x * DT * m_fSpeed * 8.f;
			}
		}

		// 출혈 이펙트
		m_fBleedingTime += DT;

		// 출혈 이펙트
		m_fBleedingTime += DT;

		if (m_fBleedingTime > 0.025f)
		{
			m_fBleedingTime -= m_fBleedingTime;

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_bleeding.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			Vec3 vEffectPos = Transform()->GetRelativePos();
			vEffectPos.z -= 1.f;

			// 범위 내에서 Random한 위치에 생성
			float Range = 20.f;
			Vec3 RandomOffsetPos = Vec3((float)rand() / RAND_MAX * Range / 4.f, (float)rand() / RAND_MAX * Range - Range / 2.f, 0.f);

			vEffectPos += RandomOffsetPos;

			SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

			pCloneEffect->Animator2D()->SetArrangeColor(Vec3(1.f, 0.f, 0.f));

			CEffectScript* EffectScript = pCloneEffect->GetScript<CEffectScript>();
			EffectScript->SetLoop(false);

			if (m_KnockBackDir.x > 0)
				pCloneEffect->Animator2D()->SetIsLeft(1);

			else if (m_KnockBackDir.x < 0)
				pCloneEffect->Animator2D()->SetIsLeft(0);
		}

		if (!m_bBossMap)
		{
			m_fBloodWallTime += DT;

			if (m_fBloodWallTime > 0.3f)
			{
				m_fBloodWallTime -= m_fBloodWallTime;

				// 가로로 긴 핏자국
				Ptr<CPrefab> pEffectPrefab2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_blood_wall.pref");
				CGameObject* pCloneEffect2 = pEffectPrefab2->Instantiate();
				Vec3 vEffectPos2 = Transform()->GetRelativePos();
				vEffectPos2.z = 999.f;
				SpawnGameObject(pCloneEffect2, vEffectPos2, L"Effect");

				CEffectScript* EffectScript2 = pCloneEffect2->GetScript<CEffectScript>();
				EffectScript2->SetLoop(true);

				CAnim2D* Anim = pCloneEffect2->Animator2D()->FindAnim(pCloneEffect2->Animator2D()->GetCurAnimationName());

				Anim->SetStop(true);
				int Index = rand() % 3;
				Anim->SetCurIndex(Index);

				if (m_KnockBackDir.x > 0)
					pCloneEffect2->Animator2D()->SetIsLeft(0);

				else if (m_KnockBackDir.x < 0)
					pCloneEffect2->Animator2D()->SetIsLeft(1);

				// 회전시켜주기
				Vec3 Dir = m_NewMonsterPos - m_PrevMonsterPos;
				float theta = atan2f(Dir.y, Dir.x);

				pCloneEffect2->Transform()->SetRelativeRot(Vec3(0.f, 0.f, theta));
			}
		}
	}

	// dead 그냥 누워 있어야 함.
	else if (m_iState == 2)
	{

	}

	// alive
	else
	{
		float Speed = m_fSpeed;

		if (m_iLStair > 0 || m_iRStair > 0)
			Speed *= 0.7f;


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

		// turn 중이면 맞는거 아니면 변화 없음. 
		if (GetAnimNum() == 3)
		{
		}

		// Idle 상태이거나 walk 하도록.
		else if (m_iPlayerDetect == 0)
		{
			// 서있기
			if (m_iStandByType == 0)
			{
				SetAnimNum(6);
			}

			// 일정 구간 보초
			else if (m_iStandByType == 1)
			{
				// 걷는 중인 경우에만 
				if (GetAnimNum() == 0)
				{
					if (m_IsLeft)
					{
						if (m_iLWall > 0 || m_iLStair > 0)
						{
							SetAnimNum(3);
							m_IsLeft = false;
						}

						else
							m_NewMonsterPos.x -= DT * Speed * 4.f;
					}

					if (!m_IsLeft)
					{
						if (m_iRWall > 0 || m_iRStair > 0)
						{
							SetAnimNum(3);
							m_IsLeft = true;
						}

						else
							m_NewMonsterPos.x += DT * Speed * 4.f;
					}
				}
			}
		}

		// Player 좇아가서 공격
		else if (m_iPlayerDetect == 1)
		{
			Vec3 TargetPos = m_TargetPlayer->Transform()->GetWorldPos();

			Vec3 Dir = TargetPos - m_NewMonsterPos;
			float theta = atan2f(Dir.y, Dir.x);

			Vec3 CenterPos = (TargetPos + m_NewMonsterPos) / 2.f;
			Vec3 Rot = { 0.f, 0.f, theta };
			Vec3 Scale = { Dir.Length(),0.f,0.f };

			CenterPos-=AttackRange->GetParent()->Transform()->GetRelativePos();
			AttackRange->Transform()->SetRelativePos(CenterPos);
			AttackRange->Transform()->SetRelativeRot(Rot);
			AttackRange->Collider2D()->SetOffsetScale(Scale);

			CPlayerScript* PlayerScript = m_TargetPlayer->GetScript<CPlayerScript>();
			int PlayerState = PlayerScript->GetState();

			// 공격 범위에 플레이어가 있다면 공격을 해야 함. 플레이어가 alive 상태인 경우에만 공격.
			if (m_bAttackReady)
			{
				SetAnimNum(7);

				if (Gun)
				{
					m_bArms = true;

					// 90도 이상
					if (theta > 3.14f / 2.f)
					{
						m_IsLeft = true;
						theta = -3.14f + theta;
					}
					// -90도 이하
					else if (theta < -3.14f / 2.f)
					{
						m_IsLeft = true;
						theta = 3.14f + theta;
					}
					// 오른쪽
					else
						m_IsLeft = false;

					Rot = { 0.f, 0.f, theta };
					Gun->Transform()->SetRelativeRot(Rot);

					if (m_IsLeft)
					{
						Gun->Transform()->SetRelativePos(Vec3(11.f, 14.f, -1.f));
						Gun->Animator2D()->SetIsLeft(true);
					}

					else
					{
						Gun->Transform()->SetRelativePos(Vec3(-11.f, 14.f, -1.f));
						Gun->Animator2D()->SetIsLeft(false);
					}

					GunRot = Rot;

					m_fShotTime += DT;

					if (m_fShotTime > 1.f && PlayerState == 0)
					{
						m_fShotTime -= m_fShotTime;

						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_shotgun_fire.wav");
						pSound->Play(1, 0.5f, true);

						// 일정 시간마다 총알이 날아가야 함. 
						Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Bullet.pref");
						CGameObject* pCloneBullet = pEffectPrefab->Instantiate();
						Vec3 vEffectPos = Transform()->GetRelativePos();
						SpawnGameObject(pCloneBullet, vEffectPos, L"MonsterProjectile");

						pCloneBullet->Transform()->SetRelativeRot(Rot);

						CBulletScript* BulletScript = pCloneBullet->GetScript<CBulletScript>();

						Dir = Normalize(Dir);

						if (BulletScript)
						{
							BulletScript->SetDir(Dir);
						}
					}
				}
			}

			// 공격 범위에 플레이어가 없다면 위치를 추적해야 함. 
			else
			{
				CPlayerScript* PlayerScript = m_TargetPlayer->GetScript<CPlayerScript>();

				// 같은 층에 있는 경우
				if (PlayerScript->GetFloorLevel() == GetFloorLevel())
				{
					SetGoToStair(false);

					// 이번 프레임에 계단에서 올라온 경우.
					if (m_iFloorLevel > m_iPrevFloorLevel)
					{
						m_NewMonsterPos.y += 8.f;
					}

					if (TargetPos.x > m_NewMonsterPos.x)
					{
						// 우선 플레이어 방향으로 Monster 방향도 전환
						if (m_IsLeft)
						{
							SetAnimNum(3);
							m_IsLeft = false;
						}

						else
						{
							Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
							pSound->Play(1, 0.5f, false);

							SetAnimNum(2);
							m_NewMonsterPos.x += DT * Speed * 12.f;
						}
					}

					if (TargetPos.x < m_NewMonsterPos.x)
					{
						if (!m_IsLeft)
						{
							SetAnimNum(3);
							m_IsLeft = true;
						}

						else
						{
							Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
							pSound->Play(1, 0.5f, false);

							SetAnimNum(2);
							m_NewMonsterPos.x -= DT * Speed * 12.f;
						}
					}
				}

				// Player가 위 층에 있는 경우
				else if (PlayerScript->GetFloorLevel() > GetFloorLevel())
				{
					vector<Vec4> FloorStair;

					for (int i = 0; i < 3; ++i)
					{
						// 같은 층의 계단 정보 모으기.
						if ((int)m_arrStairData[i].x == GetFloorLevel())
						{
							FloorStair.push_back(m_arrStairData[i]);
						}
					}

					// 제일 가까운 계단 찾기.
					int Size = (int)FloorStair.size();
					float MinXStairDistance = (float)INT_MAX;
					int MinXStairIndex = 0;

					for (int i = 0; i < Size; ++i)
					{
						// 계단 아래쪽의 x좌표와 현재 몬스터의 x 좌표의 차이 구하기.
						float Distance = abs(FloorStair[i].z - m_NewMonsterPos.x);

						if (Distance < MinXStairDistance)
						{
							MinXStairDistance = Distance;
							MinXStairIndex = i;
						}
					}

					// LStair인 경우
					if (m_NewMonsterPos.x > FloorStair[MinXStairIndex].y)
					{
						if (!m_IsLeft)
						{
							SetAnimNum(3);
							m_IsLeft = true;
						}

						else
						{
							Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
							pSound->Play(1, 0.5f, false);

							SetAnimNum(2);
							m_NewMonsterPos.x -= DT * Speed * 12.f;
						}
					}

					// RStair인 경우
					else
					{
						if (m_IsLeft)
						{
							SetAnimNum(3);
							m_IsLeft = false;
						}

						else
						{
							Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
							pSound->Play(1, 0.5f, false);

							SetAnimNum(2);
							m_NewMonsterPos.x += DT * Speed * 12.f;
						}
					}
				}

				// Player가 아래 층에 있는 경우
				else if (PlayerScript->GetFloorLevel() < GetFloorLevel())
				{
					vector<Vec4> FloorStair;

					for (int i = 0; i < 3; ++i)
					{
						// 아래 층의 계단 정보 모으기.
						if ((int)m_arrStairData[i].x == GetFloorLevel() - 1)
						{
							FloorStair.push_back(m_arrStairData[i]);
						}
					}

					// 제일 가까운 계단 찾기.
					int Size = (int)FloorStair.size();
					float MinXStairDistance = (float)INT_MAX;
					int MinXStairIndex = 0;

					for (int i = 0; i < Size; ++i)
					{
						// 계단 위쪽의 x좌표와 현재 몬스터의 x 좌표의 차이 구하기.
						float Distance = abs(FloorStair[i].y - m_NewMonsterPos.x);

						if (Distance < MinXStairDistance)
						{
							MinXStairDistance = Distance;
							MinXStairIndex = i;
						}
					}

					// 좌상단 계단인 경우
					if (FloorStair[MinXStairIndex].y < FloorStair[MinXStairIndex].z)
					{
						if (m_NewMonsterPos.x > FloorStair[MinXStairIndex].y + 50.f)
						{
							if (!m_IsLeft)
							{
								SetAnimNum(3);
								m_IsLeft = true;
							}

							else
							{
								Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
								pSound->Play(1, 0.5f, false);

								SetAnimNum(2);
								m_NewMonsterPos.x -= DT * Speed * 12.f;
							}
						}

						else if (m_NewMonsterPos.x < FloorStair[MinXStairIndex].y - 50.f)
						{
							if (m_IsLeft)
							{
								SetAnimNum(3);
								m_IsLeft = false;
							}

							else
							{
								Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
								pSound->Play(1, 0.5f, false);

								SetAnimNum(2);
								m_NewMonsterPos.x += DT * Speed * 12.f;
							}
						}

						else
						{
							Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
							pSound->Play(1, 0.5f, false);

							SetGoToStair(true);
							SetAnimNum(2);
							m_NewMonsterPos.x += DT * Speed * 12.f;
						}
					}

					// 우상단 계단인 경우
					else if (FloorStair[MinXStairIndex].y > FloorStair[MinXStairIndex].z)
					{
						if (m_NewMonsterPos.x > FloorStair[MinXStairIndex].y + 50.f)
						{
							if (!m_IsLeft)
							{
								SetAnimNum(3);
								m_IsLeft = true;
							}

							else
							{
								Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
								pSound->Play(1, 0.5f, false);

								SetAnimNum(2);
								m_NewMonsterPos.x -= DT * Speed * 12.f;
							}
						}

						else if (m_NewMonsterPos.x < FloorStair[MinXStairIndex].y - 50.f)
						{
							if (m_IsLeft)
							{
								SetAnimNum(3);
								m_IsLeft = false;
							}

							else
							{
								Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
								pSound->Play(1, 0.5f, false);

								SetAnimNum(2);
								m_NewMonsterPos.x += DT * Speed * 12.f;
							}
						}

						else
						{
							Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_generic_enemy_run_3.wav");
							pSound->Play(1, 0.5f, false);

							SetGoToStair(true);
							SetAnimNum(2);
							m_NewMonsterPos.x -= DT * Speed * 12.f;
						}
					}
				}
			}
		}
	}

	// 팔 세팅
	if (Gun)
	{
		CAnim2D* Anim = Gun->Animator2D()->FindAnim(Gun->Animator2D()->GetCurAnimationName());
		Anim->SetStop(true);

		if (m_bArms)
			Anim->SetCurIndex(0);

		else
			Anim->SetCurIndex(1);
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

	Animator2D()->SetIsLeft(m_IsLeft);

	Transform()->SetRelativePos(m_NewMonsterPos);

	m_PrevMonsterPos = Transform()->GetRelativePos();

	switch (m_AnimNum)
	{
	case 0:
		Animator2D()->Play(L"animation\\Cop_walk.anim", true);
		break;
	case 1:
		Animator2D()->Play(L"animation\\Cop_leavestair.anim", true);
		break;
	case 2:
		Animator2D()->Play(L"animation\\Cop_run.anim", true);
		break;
	case 3:
		Animator2D()->Play(L"animation\\Cop_turn.anim", true);
		break;
	case 4:
		Animator2D()->Play(L"animation\\Cop_enterstair.anim", true);
		break;
	case 5:
		Animator2D()->Play(L"animation\\Cop_hurtground.anim", true);
		break;
	case 6:
		Animator2D()->Play(L"animation\\Cop_idle.anim", true);
		break;
	case 7:
		Animator2D()->Play(L"animation\\Cop_aim.anim", true);
		break;
	case 8:
		Animator2D()->Play(L"animation\\Cop_hurtground.anim", false);
		break;
	}

	// PrevFloorLevel 설정
	m_iPrevFloorLevel = m_iFloorLevel;

	// Rewind를 위한 PlayerRewindData 저장
	m_RewindTime += DT;

	if (m_RewindTime >= 1.f / 60.f)
	{
		MonsterCopRewindData TempData = {};
		TempData.AnimNum = GetAnimNum();
		CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
		TempData.AnimFrame = CurAnim->GetCurFrameIndex();
		TempData.AnimIsLeft = Animator2D()->IsLeft();
		TempData.MonsterPos = Transform()->GetRelativePos();
		TempData.MonsterScale = Transform()->GetRelativeScale();
		TempData.MonsterRot = Transform()->GetRelativeRot();
		TempData.GunRot = GunRot;

		m_MonsterRewindDataList.push_back(TempData);

		m_RewindTime -= m_RewindTime;
	}
}

void CMonsterCopScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
	fwrite(&m_iStandByType, sizeof(int), 1, _File);
	fwrite(&m_arrStairData, sizeof(Vec4), 3, _File);
}

void CMonsterCopScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_fSpeed, sizeof(float), 1, _FILE);
	fread(&m_iStandByType, sizeof(int), 1, _FILE);
	fread(&m_arrStairData, sizeof(Vec4), 3, _FILE);
}

