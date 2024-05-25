#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>
#include <Engine\CAnim2D.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CEventMgr.h>

#include <Engine\CDevice.h>

#include "CMissileScript.h"
#include "CGravityScript.h"
#include "CEffectScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CHeadhunterScript.h"

//
#include "CLevelSaveLoad2.h"


CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_iState(0)
	, m_fSpeed(100.f)
	, m_iGround(0)
	, m_iGroundToFall(0)
	, m_iLWall(0)
	, m_iRWall(0)
	, m_iCeiling(0)
	, m_iLStair(0)
	, m_iRStair(0)
	, m_bJump(false)
	, m_iAttackJump(0)
	, m_AnimNum(0)
	, m_RewindTime(0)
	, m_RewindScale(1.f)
	, m_bRewind(false)
	, m_iFloorLevel(1)
	, m_fBleedingTime(0.f)
	, m_fBloodWallTime(0.f)
	, m_bImmortal(false)
	, m_bVideoPlayEnd(false)
	/*, m_iBGMChannel(-1)
	, m_bTimeSlow(false)
	, m_bBattery(true)*/
{ 
	AddScriptParam(SCRIPT_PARAM::INT, &m_iFloorLevel, "FloorLevel");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");

	m_PrevPlayerPos = Vec3(-9999.f, 0.f, 0.f);
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	//CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	//wstring temp = CurLevel->GetName();

	//// Level에 맞게 BGM 설정.
	//if (temp == L"Level\\Motel1.lv" || temp == L"Level\\Motel2.lv" || temp == L"Level\\Motel3.lv")
	//{
	//	Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_fullconfession.ogg");
	//	pSound2->Stop();

	//	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_sneaky_driver.ogg");
	//	m_iBGMChannel = pSound->Play(0, 0.5f, false);
	//}
	//
	//else if (temp == L"Level\\Boss1.lv" || temp == L"Level\\Boss2.lv")
	//{
	//	Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_sneaky_driver.ogg");
	//	pSound2->Stop();

	//	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_fullconfession.ogg");
	//	m_iBGMChannel = pSound->Play(0, 0.5f, false);
	//}
	

	for (int i = 0; i < 22; ++i)
	{
		m_vecNextAnim.push_back(-1);
	}

	m_vecNextAnim[2] = 3;
	m_vecNextAnim[5] = 0;
	m_vecNextAnim[6] = 7;
	m_vecNextAnim[8] = 9;
	m_vecNextAnim[9] = 10;
	m_vecNextAnim[11] = 0;

	// 마우스 안보이게 하기
	ShowCursor(false);

	// Level 시작될 때 잠시 XDistortion
	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\XDistortion.pref");
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
	SpawnGameObject(pCloneEffect, Vec3(0.f, 0.f, 0.f), L"Effect");
	pCloneEffect->SetLifeSpan(0.3f);

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pause.wav");
	pSound->Play(1, 0.5f, false);
}

void CPlayerScript::tick()
{
	// 불사 모드
	if (m_bImmortal)
		m_iState = 0;
	
	// I 키를 눌러서 불사모드를 키고 끌 수 있다.
	if (KEY_TAP(KEY::I))
	{
		if (m_bImmortal)
			m_bImmortal = false;
		else
			m_bImmortal = true;
	}

	if (m_bRewind)
	{
		Rewind(0.1f, true);
		return;
	}

	m_NewPlayerPos = Transform()->GetRelativePos();

	// 비반복 애니메이션인지 확인
	if (m_vecNextAnim[GetAnimNum()] != -1)
	{
		// 애니메이션이 종료되었는지 확인
		CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());

		if (CurAnim->IsFinish())
		{
			SetAnimNum(m_vecNextAnim[GetAnimNum()]);
			CurAnim->Reset();
		}
	}

	// hurt
	if (m_iState == 1)
	{
		if (GetAnimNum() == 10)
		{
			CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());

			// Hurt animation이 끝났다면 Dead 상태. 
			if (CurAnim->IsFinish())
			{
				m_iState = 2;
				CurAnim->SetStop(true);
			}
		}

		// 맞은 방향으로 뒤로 밀려나면서 피가 나와야 하고 그에 맞게 피자국이 벽에 남아야 한다. 피자국은 2개로 구성한다. 하나는 Attack 스크립트에서 생성함.
		else
		{
			if (m_KnockBackDir.x > 0)
			{
				Animator2D()->SetIsLeft(1);

				if (m_iRWall <= 0)
				{
					m_NewPlayerPos.x += DT * m_fSpeed * 4.f;
				}
			}

			else if (m_KnockBackDir.x < 0)
			{
				Animator2D()->SetIsLeft(0);

				if (m_iLWall <= 0)
				{
					m_NewPlayerPos.x -= DT * m_fSpeed * 4.f;
				}
			}

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
				//EffectScript->SetTarget(GetOwner());

				if (m_KnockBackDir.x > 0)
					pCloneEffect->Animator2D()->SetIsLeft(1);

				else if (m_KnockBackDir.x < 0)
					pCloneEffect->Animator2D()->SetIsLeft(0);
			}

			// 핏자국 이펙트
			m_fBloodWallTime += DT;

			if (m_fBloodWallTime > 0.5f)
			{
				m_fBloodWallTime -= m_fBloodWallTime;

				// 가로로 긴 핏자국
				Ptr<CPrefab> pEffectPrefab2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_blood_wall.pref");
				CGameObject* pCloneEffect2 = pEffectPrefab2->Instantiate();
				Vec3 vEffectPos2 = Transform()->GetRelativePos();
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
				float theta = atan2f(m_PlayerDir.y, m_PlayerDir.x);

				pCloneEffect2->Transform()->SetRelativeRot(Vec3(0.f, 0.f, theta));
			}
		}
	}

	// dead
	else if (m_iState == 2)
	{
		// Rewind 하는 경우 충돌체 비활성화 시키고, 중력 스크립트를 꺼줘야 한다.
		if (KEY_TAP(KEY::LBTN))
		{
			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RewindDistortion.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Vec3(0.f, 0.f, 0.f), L"Effect");

			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\Rewind.wav");
			pSound->Play(1, 0.5f, false);

			CLevel* Level = CLevelMgr::GetInst()->GetCurLevel();
			
			// MonsterLayer의 모든 옵젝들
			CLayer* MonsterLayer = Level->GetLayer(3);
			vector<CGameObject*> vecMonster = MonsterLayer->GetParentObject();
			int Size = (int)vecMonster.size();

			for (int i = 0; i < Size; ++i)
			{
				CGameObject* MonsterObj = vecMonster[i];
				vector<CScript*> vecScript = MonsterObj->GetScripts();

				int vecScriptSize = (int)vecScript.size();

				for (size_t i = 0; i < vecScriptSize; ++i)
				{
					wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);

					if (ScriptName == L"CHeadhunterScript")
					{
						((CHeadhunterScript*)vecScript[i])->CHeadhunterScript::StartRewind();
					}

					if (ScriptName == L"CMonsterCopScript")
					{
						((CMonsterCopScript*)vecScript[i])->CMonsterCopScript::StartRewind();
					}

					if (ScriptName == L"CMonsterGruntScript")
					{
						((CMonsterGruntScript*)vecScript[i])->CMonsterGruntScript::StartRewind();
					}

					if (ScriptName == L"CMonsterPompScript")
					{
						((CMonsterPompScript*)vecScript[i])->CMonsterPompScript::StartRewind();
					}
				}
			}

			////
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
	}

	// alive
	else
	{
		// Attack 상태인 경우의 이동 처리 구현.
		if (GetAnimNum() == 2)
		{
			Attack();
		}

		// 추락 상태인 경우 바닥에 닿으면 Idle 상태로 변경. 착지.
		if (GetAnimNum() == 3)
		{
			if (m_iGround > 0 || m_iGroundToFall > 0 || m_iLStair > 0 || m_iRStair > 0)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_land.wav");
				pSound->Play(1, 0.5f, true);

				SetAnimNum(0);

				// LandCloud Effect 프리팹 참조
				Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_landCloud.pref");
				Vec3 vEffectPos = Transform()->GetRelativePos();
				CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
				SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");
			}
		}

		// Crouch 상태인 경우 s를 떼면 일어나.
		if (GetAnimNum() == 4)
		{
			if (KEY_RELEASE(KEY::S))
			{
				SetAnimNum(0);
			}
		}

		// Dash 상태인 경우의 이동 처리 구현.
		if (GetAnimNum() == 5)
		{
			Dash();
		}


		// ----------------------------------------------- 키입력 -----------------------------------------------
		if (KEY_TAP(KEY::LBTN))
		{
			// 이미 공격중이면 넘김.
			if (GetAnimNum() == 2)
			{
			}

			else
			{
				++m_iAttackJump;

				Vec3 MainCameraPos = CRenderMgr::GetInst()->GetMainCam()->GetOwner()->Transform()->GetRelativePos();
				Vec3 MainCameraSize = Vec3(1280.f, 768.f, 0.f) * CRenderMgr::GetInst()->GetMainCam()->GetScale();
				Vec3 ClientLTPos = MainCameraPos + Vec3(-MainCameraSize.x / 2.f, MainCameraSize.y / 2.f, 0.f);

				m_AttackDir = ClientLTPos + Vec3(CKeyMgr::GetInst()->GetMousePos().x, -CKeyMgr::GetInst()->GetMousePos().y, 0.f) - m_NewPlayerPos;
				m_AttackDir = Normalize(Vec3(m_AttackDir.x, m_AttackDir.y, 0.f));

				SetAnimNum(2);

				vector<CScript*> vecObjScript = GetOwner()->GetScripts();

				for (size_t i = 0; i < vecObjScript.size(); ++i)
				{
					wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

					if (ScriptName == L"CGravityScript")
					{
						((CGravityScript*)vecObjScript[i])->CGravityScript::Attack();
					}
				}

				// PlayerSlash Effect 프리팹 참조
				Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_player_slash.pref");
				Vec3 vEffectPos = Transform()->GetRelativePos();
				CGameObject* pCloneEffect = pEffectPrefab->Instantiate();

				// 레벨에 추가
				SpawnGameObject(pCloneEffect, vEffectPos, L"PlayerProjectile");

				// 각도 구하기
				float theta = atan2f(m_AttackDir.y, m_AttackDir.x);

				// 90도 이상
				if (theta > 3.14f / 2.f)
				{
					pCloneEffect->Animator2D()->SetIsLeft(true);
					theta = -3.14f + theta;
				}

				// -90도 이하
				else if (theta < -3.14f / 2.f)
				{
					pCloneEffect->Animator2D()->SetIsLeft(true);
					theta = 3.14f + theta;
				}

				Vec3 Rot = { 0.f, 0.f, theta };
				pCloneEffect->Transform()->SetRelativeRot(Rot);


				vector<CScript*> vecObjScript2 = pCloneEffect->GetScripts();

				for (size_t i = 0; i < vecObjScript2.size(); ++i)
				{
					wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript2[i]);

					if (ScriptName == L"CEffectScript")
					{
						((CEffectScript*)vecObjScript2[i])->CEffectScript::SetTarget(GetOwner());
					}
				}
			}
		}

		if (KEY_TAP(KEY::W))
		{
			// 공격 상태 혹은 점프 상태면 그냥 넘김
			if (GetAnimNum() == 2 || GetAnimNum() == 1)
			{
			}

			// 바닥에 착지한 상태에서만 점프가 됨.
			else if (m_iGround > 0 || m_iGroundToFall > 0 || m_iLStair > 0 || m_iRStair > 0)
			{
				Jump();
			}
		}

		if (KEY_PRESSED(KEY::S))
		{
			// 공격 중이거나 구르기 중이면 안걸려야 함.
			if (GetAnimNum() == 5 || GetAnimNum() == 2)
			{
			}

			// 착지한 상태. S를 누르면 앉는다. 이 상태에서 A나 D를 누르면 해당 방향으로 구른다.
			else if (m_iGround > 0 || m_iLStair > 0 || m_iRStair > 0)
			{
				// 공격중이 아니고, 이동중도 아닐 때.
				if (GetAnimNum() != 2 && KEY_PRESSED(KEY::A) == false && KEY_PRESSED(KEY::D) == false)
					SetAnimNum(4);

				// 왼쪽으로 구르기
				if (KEY_TAP(KEY::A))
				{
					m_NewPlayerPos.x -= DT * m_fSpeed;	// Dir을 잡아주기 위한 이동량
					SetAnimNum(5);
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_roll.wav");
					pSound->Play(1, 0.5f, true);
				}

				// 오른쪽으로 구르기
				if (KEY_TAP(KEY::D))
				{
					m_NewPlayerPos.x += DT * m_fSpeed;	// Dir을 잡아주기 위한 이동량
					SetAnimNum(5);
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_roll.wav");
					pSound->Play(1, 0.5f, true);
				}
			}

			// 일반 땅이나 계단에 착지된 상태가 아니고, 낙하가능 바닥 위라면 s를 눌렀을 때 떨어져야 함.
			else if (m_iGroundToFall > 0)
			{
				SetAnimNum(3);
			}

			// 공중에서 S를 누르게 되면 빠르게 착지하는데 이건 나중에.
			else
			{
			}
		}

		if (KEY_PRESSED(KEY::A))
		{
			// 공격 중이거나 구르기 중이면 안걸려야 함.
			if (GetAnimNum() == 5 || GetAnimNum() == 2 || GetAnimNum() == 11)
			{
			}

			// 왼쪽 벽에 부딪힌 상태
			else if (m_iLWall > 0)
			{
			}

			// 계단 위
			else if (m_iLStair > 0 || m_iRStair > 0)
			{
				// 왼쪽으로 구르기
				if (KEY_TAP(KEY::S))
				{
					m_NewPlayerPos.x -= DT * m_fSpeed;	// Dir을 잡아주기 위한 이동량
					SetAnimNum(5);
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_roll.wav");
					pSound->Play(1, 0.5f, true);
				}

				// 달리기
				else
				{
					// Idle 상태였다면 idle_to_run
					if (GetAnimNum() == 0)
					{
						SetAnimNum(6);
						DirtCloudEffect(false, false);
					}

					m_NewPlayerPos.x -= DT * m_fSpeed * 0.7f * 4.f;	//대각선으로 갈 때는 x축으로 0.7배씩만 이동
				}
			}

			// 일반 바닥 위
			else if (m_iGround > 0)
			{
				// 왼쪽으로 구르기
				if (KEY_TAP(KEY::S))
				{
					m_NewPlayerPos.x -= DT * m_fSpeed;	// Dir을 잡아주기 위한 이동량
					SetAnimNum(5);
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_roll.wav");
					pSound->Play(1, 0.5f, true);
				}

				// 달리기
				for (int i = 0; i < 4; ++i)
				{
					// Idle 상태였다면 idle_to_run
					if (GetAnimNum() == 0)
					{
						SetAnimNum(6);
						DirtCloudEffect(false, false);
					}

					m_NewPlayerPos.x -= DT * m_fSpeed;
				}
			}

			// 낙하가능 바닥 위나 공중인 경우
			else
			{
				// Idle 상태였다면 idle_to_run
				if (GetAnimNum() == 0)
				{
					SetAnimNum(6);
					DirtCloudEffect(false, false);
				}

				m_NewPlayerPos.x -= DT * m_fSpeed * 4.f;
			}
		}

		if (KEY_PRESSED(KEY::D))
		{
			// 공격 중이거나 구르기 중이면 안걸려야 함.
			if (GetAnimNum() == 5 || GetAnimNum() == 2 || GetAnimNum() == 11)
			{
			}

			// 오른쪽 벽에 부딪힌 상태
			else if (m_iRWall > 0)
			{
			}

			// 계단 위
			else if (m_iLStair > 0 || m_iRStair > 0)
			{
				// 왼쪽으로 구르기
				if (KEY_TAP(KEY::S))
				{
					m_NewPlayerPos.x += DT * m_fSpeed;	// Dir을 잡아주기 위한 이동량
					SetAnimNum(5);
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_roll.wav");
					pSound->Play(1, 0.5f, true);
				}

				// 달리기
				else
				{
					for (int i = 0; i < 4; ++i)
					{
						// Idle 상태였다면 idle_to_run
						if (GetAnimNum() == 0)
						{
							SetAnimNum(6);
							DirtCloudEffect(true, false);
						}

						m_NewPlayerPos.x += DT * m_fSpeed * 0.7f;	//대각선으로 갈 때는 x축으로 0.7배씩만 이동
					}
				}
			}

			// 일반 바닥 위
			else if (m_iGround > 0)
			{
				// 왼쪽으로 구르기
				if (KEY_TAP(KEY::S))
				{
					m_NewPlayerPos.x += DT * m_fSpeed;	// Dir을 잡아주기 위한 이동량
					SetAnimNum(5);
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_roll.wav");
					pSound->Play(1, 0.5f, true);
				}

				// 달리기
				for (int i = 0; i < 4; ++i)
				{
					// Idle 상태였다면 idle_to_run
					if (GetAnimNum() == 0)
					{
						SetAnimNum(6);
						DirtCloudEffect(true, false);
					}

					m_NewPlayerPos.x += DT * m_fSpeed;
				}
			}

			// 낙하가능 바닥 위나 공중인 경우
			else
			{
				// Idle 상태였다면 idle_to_run
				if (GetAnimNum() == 0)
				{
					SetAnimNum(6);
					DirtCloudEffect(true, false);
				}

				m_NewPlayerPos.x += DT * m_fSpeed * 4.f;
			}
		}
	}

	Vec3 PrevDir = m_PlayerDir;

	m_PlayerDir = m_NewPlayerPos - m_PrevPlayerPos;

	if (m_iCeiling > 0)
	{
		if (m_PlayerDir.y > 0)
			m_NewPlayerPos.y = m_PrevPlayerPos.y;
	}

	// 달리기 상태인데, x 이동량이 없다면 Idle로 변경.

	if (GetAnimNum() == 7)
	{
		if (m_PlayerDir.x == 0)
		{
			SetAnimNum(0);
		}

		// 달리다가 방향이 바뀌는 경우
		else if (PrevDir.x* m_PlayerDir.x < 0)
		{
			bool CloudLeft = m_PlayerDir.x > 0;
			DirtCloudEffect(CloudLeft, false);
		}
	}

	if (m_iState == 0)
	{
		if (m_PlayerDir.x > 0)
			Animator2D()->SetIsLeft(0);

		else if (m_PlayerDir.x < 0)
			Animator2D()->SetIsLeft(1);
	}

	Transform()->SetRelativePos(m_NewPlayerPos);

	m_PrevPlayerPos = Transform()->GetRelativePos();

	switch (m_AnimNum)
	{
	case 0:
		Animator2D()->Play(L"animation\\Player_idle.anim", true);
		break;
	case 1:
		Animator2D()->Play(L"animation\\Player_jump.anim", true);
		break;
	case 2:
		Animator2D()->Play(L"animation\\Player_attack.anim", false);
		break;
	case 3:
		Animator2D()->Play(L"animation\\Player_fall.anim", true);
		break;
	case 4:
		Animator2D()->Play(L"animation\\Player_crouch.anim", true);
		break;
	case 5:
		Animator2D()->Play(L"animation\\Player_roll.anim", true);
		break;
	case 6:
		Animator2D()->Play(L"animation\\Player_idle_to_run.anim", true);
		break;
	case 7:
		Animator2D()->Play(L"animation\\Player_run.anim", true);
		break;
	case 8:
		Animator2D()->Play(L"animation\\Player_hurtfly_begin.anim", true);
		break;
	case 9:
		Animator2D()->Play(L"animation\\Player_hurtfly_loop.anim", true);
		break;
	case 10:
		Animator2D()->Play(L"animation\\Player_hurtgrond.anim", false);
		break;
	case 11:
		Animator2D()->Play(L"animation\\Player_doorbreak.anim", false);
		break;
	}

	// Rewind를 위한 PlayerRewindData 저장
	m_RewindTime += DT;

	if (m_RewindTime >= 1.f / 60.f)
	{
		PlayerRewindData TempData = {};
		TempData.AnimNum = GetAnimNum();
		CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
		TempData.AnimFrame = CurAnim->GetCurFrameIndex();
		TempData.AnimIsLeft = Animator2D()->IsLeft();
		TempData.PlayerPos = Transform()->GetRelativePos();
		TempData.PlayerScale = Transform()->GetRelativeScale();
		TempData.PlayerRot = Transform()->GetRelativeRot();

		m_PlayerRewindDataList.push_back(TempData);

		m_RewindTime -= m_RewindTime;
	}
}

void CPlayerScript::Shoot()
{
	// 미사일 프리팹 참조
	Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
	Vec3 vMissilePos = Transform()->GetRelativePos() + Vec3(0.f, 0.5f, 0.f) * Transform()->GetRelativeScale();
	CGameObject* pCloneMissile = pMissilePrefab->Instantiate();

	// 레벨에 추가
	SpawnGameObject(pCloneMissile, vMissilePos, L"PlayerProjectile");
}

void CPlayerScript::Jump()
{
	vector<CScript*> vecObjScript = GetOwner()->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CGravityScript")
		{
			((CGravityScript*)vecObjScript[i])->CGravityScript::Jump();
		}
	}

	SetAnimNum(1);

	m_NewPlayerPos.y += 4.f;

	// Jump Effect 프리팹 참조
	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_playerJump.pref");
	Vec3 vEffectPos = Transform()->GetRelativePos();
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
	SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");
}

void CPlayerScript::Attack()
{
	Vec2 Move = {};

	Move.x = m_AttackDir.x * DT * m_fSpeed * 5.f;
	Move.y = m_AttackDir.y * DT * m_fSpeed * 5.f;

	if(m_iRWall > 0)
	{
		if(Move.x > 0)
			Move.x = 0.f;
	}

	if(m_iLWall > 0)
	{
		if (Move.x < 0)
			Move.x = 0.f;
	}

	if (m_iGround > 0)
	{
		if (Move.y < 0)
			Move.y = 0.f;
	}

	if (m_iLStair > 0 || m_iRStair > 0)
	{
		Move.x *= 0.7f;

		if (Move.y < 0)
			Move.y = 0.f;
	}

	if (m_iGroundToFall > 0)
	{
		if (Move.y < 0)
			Move.y = 0.f;
	}

	// 천장에 부딪히는 중이거나, 공격으로 한번 공중에 뜬 이후의 공격이면 더이상 위로 가지 않는다.
	if (m_iCeiling > 0 || m_iAttackJump > 1)
	{
		if (Move.y > 0)
			Move.y = 0.f;
	}

	m_NewPlayerPos.x += Move.x;
	m_NewPlayerPos.y += Move.y;
}

void CPlayerScript::Dash()
{
	if (Animator2D()->IsLeft() <= 0)
	{
		if (!m_iRWall)
		{
			// 계단 위면 70퍼로.
			if (m_iLStair > 0 || m_iRStair > 0)
			{
				m_NewPlayerPos.x += DT * m_fSpeed * 6.f * 0.7f;
			}

			else
			{
				m_NewPlayerPos.x += DT * m_fSpeed * 6.f;
			}
		}
	}

	else
	{
		if (!m_iLWall)
		{
			// 계단 위면 70퍼로.
			if (m_iLStair > 0 || m_iRStair > 0)
			{
				for (int i = 0; i < 4; ++i)
				{
					m_NewPlayerPos.x -= DT * m_fSpeed * 1.5f * 0.7f;
				}
			}

			else
			{
				for (int i = 0; i < 4; ++i)
				{
					m_NewPlayerPos.x -= DT * m_fSpeed * 1.5f;
				}
			}
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

void CPlayerScript::DirtCloudEffect(bool _bLeft, bool _bDash)
{
	// 달리기
	if (!_bDash)
	{
		// 한번에 4개씩 생성
		for (int i = 0; i < 4; ++i)
		{
			// Cloud Effect 프리팹 참조
			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_player_cloud.pref");
			Vec3 vEffectPos = Transform()->GetRelativePos();
			vEffectPos.y += Collider2D()->GetOffsetPos().y - Collider2D()->GetOffsetScale().y / 2.f;
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();

			// 범위 내에서 Random한 위치에 생성
			float Range = 40.f;
			Vec3 RandomOffsetPos = Vec3((float)rand() / RAND_MAX * Range - Range / 2.f, (float)rand() / RAND_MAX * Range - Range / 2.f, 0.f);

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
	}
	
	// 대쉬. 1개씩 만들고, 범위를 키움.
	else
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
}

void CPlayerScript::Rewind(float Scale, bool Backward)
{
	// 좌클릭이나 우클릭시, 그냥 바로 끝내기.
	if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::RBTN))
	{
		if (Backward)
		{
			// 현재 Level 다시 불러오기
			CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
			wstring temp = CurLevel->GetName();
			CLevel* pLoadedLevel = CLevelSaveLoad2::LoadLevel(CurLevel->GetName());

			tEvent evn = {};
			evn.Type = EVENT_TYPE::LEVEL_CHANGE;
			evn.wParam = (DWORD_PTR)pLoadedLevel;

			CEventMgr::GetInst()->AddEvent(evn);

			return;
		}

		else
		{
			m_bVideoPlayEnd = true;
			return;
		}
	}

	// 
	m_RewindTime += DT;
	m_RewindScale += DT * 120.f;

	if (Backward)
	{
		if (m_RewindTime >= 1.f / (60.f * Scale * m_RewindScale))
		{
			m_RewindTime -= m_RewindTime;

			CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
			CurAnim->SetStop(false);

			int End = (int)m_PlayerRewindDataList.size();

			// 뒤로 감기 중 다됐다면.
			if (End <= 0 && Backward)
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

				// 현재 Level 다시 불러오기
				CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
				wstring temp = CurLevel->GetName();
				CLevel* pLoadedLevel = CLevelSaveLoad2::LoadLevel(CurLevel->GetName());

				tEvent evn = {};
				evn.Type = EVENT_TYPE::LEVEL_CHANGE;
				evn.wParam = (DWORD_PTR)pLoadedLevel;

				CEventMgr::GetInst()->AddEvent(evn);

				return;
			}

			PlayerRewindData TempData = {};

			TempData = m_PlayerRewindDataList.back();
			m_PlayerRewindDataList.pop_back();


			Transform()->SetRelativePos(TempData.PlayerPos);
			Transform()->SetRelativeScale(TempData.PlayerScale);
			Transform()->SetRelativeRot(TempData.PlayerRot);

			SetAnimNum(TempData.AnimNum);
			Animator2D()->SetIsLeft(TempData.AnimIsLeft);

			switch (m_AnimNum)
			{
			case 0:
				Animator2D()->Play(L"animation\\Player_idle.anim", true);
				break;
			case 1:
				Animator2D()->Play(L"animation\\Player_jump.anim", true);
				break;
			case 2:
				Animator2D()->Play(L"animation\\Player_attack.anim", true);
				break;
			case 3:
				Animator2D()->Play(L"animation\\Player_fall.anim", true);
				break;
			case 4:
				Animator2D()->Play(L"animation\\Player_crouch.anim", true);
				break;
			case 5:
				Animator2D()->Play(L"animation\\Player_roll.anim", true);
				break;
			case 6:
				Animator2D()->Play(L"animation\\Player_idle_to_run.anim", true);
				break;
			case 7:
				Animator2D()->Play(L"animation\\Player_run.anim", true);
				break;
			case 8:
				Animator2D()->Play(L"animation\\Player_hurtfly_begin.anim", true);
				break;
			case 9:
				Animator2D()->Play(L"animation\\Player_hurtfly_loop.anim", true);
				break;
			case 10:
				Animator2D()->Play(L"animation\\Player_hurtgrond.anim", false);
				break;
			case 11:
				Animator2D()->Play(L"animation\\Player_doorbreak.anim", false);
				break;
			}

			CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
			CurAnim->SetCurIndex(TempData.AnimFrame);
			CurAnim->SetStop(true);
		}
	}

	if (!Backward)
	{
		if (m_RewindTime >= 1.f / (60.f * Scale))
		{
			m_RewindTime -= m_RewindTime;

			CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
			CurAnim->SetStop(false);

			int End = (int)m_PlayerRewindDataList.size();

			if (End <= 0 && !Backward)
			{
				m_bVideoPlayEnd = true;
				return;
			}

			PlayerRewindData TempData = {};

			TempData = m_PlayerRewindDataList.front();
			m_PlayerRewindDataList.pop_front();

			Transform()->SetRelativePos(TempData.PlayerPos);
			Transform()->SetRelativeScale(TempData.PlayerScale);
			Transform()->SetRelativeRot(TempData.PlayerRot);

			SetAnimNum(TempData.AnimNum);
			Animator2D()->SetIsLeft(TempData.AnimIsLeft);

			switch (m_AnimNum)
			{
			case 0:
				Animator2D()->Play(L"animation\\Player_idle.anim", true);
				break;
			case 1:
				Animator2D()->Play(L"animation\\Player_jump.anim", true);
				break;
			case 2:
				Animator2D()->Play(L"animation\\Player_attack.anim", true);
				break;
			case 3:
				Animator2D()->Play(L"animation\\Player_fall.anim", true);
				break;
			case 4:
				Animator2D()->Play(L"animation\\Player_crouch.anim", true);
				break;
			case 5:
				Animator2D()->Play(L"animation\\Player_roll.anim", true);
				break;
			case 6:
				Animator2D()->Play(L"animation\\Player_idle_to_run.anim", true);
				break;
			case 7:
				Animator2D()->Play(L"animation\\Player_run.anim", true);
				break;
			case 8:
				Animator2D()->Play(L"animation\\Player_hurtfly_begin.anim", true);
				break;
			case 9:
				Animator2D()->Play(L"animation\\Player_hurtfly_loop.anim", true);
				break;
			case 10:
				Animator2D()->Play(L"animation\\Player_hurtgrond.anim", false);
				break;
			case 11:
				Animator2D()->Play(L"animation\\Player_doorbreak.anim", false);
				break;
			}

			CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
			CurAnim->SetCurIndex(TempData.AnimFrame);
			CurAnim->SetStop(true);
		}
	}
}


void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}