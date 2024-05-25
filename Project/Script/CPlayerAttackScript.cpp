#include "pch.h"
#include "CPlayerAttackScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CEffectScript.h"

#include <Engine\CAnim2D.h>

void CPlayerAttackScript::begin()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	wstring temp = CurLevel->GetName();
	
	if (temp == L"Level\\Boss1.lv" || temp == L"Level\\Boss2.lv")
	{
		m_bBossMap = true;
	}

	else
		m_bBossMap = false;

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\slash_1.wav");
	pSound->Play(1, 0.5f, true);
}

void CPlayerAttackScript::tick()
{
}

void CPlayerAttackScript::BeginOverlap(CCollider2D* _Other)
{
	// Monster Layer에 있는 물체와 충돌한 경우 공격되어야 한다. 근데 이거 여기 말고 몬스터 스크립트에서 처리하는게 좋을 거 같기도. 
	if (_Other->GetOwner()->GetLayerIndex() == 3)
	{
		vector<CScript*> MonsterScript = _Other->GetOwner()->GetScripts();
		int size = (int)MonsterScript.size();

		for (int i = 0; i < size; ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(MonsterScript[i]);

			if (ScriptName == L"CMonsterCopScript")
			{
				if (((CMonsterCopScript*)MonsterScript[i])->GetState() == 0)
				{
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat_1.wav");
					pSound->Play(1, 0.5f, true);
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_sword.wav");
					pSound2->Play(1, 0.5f, true);

					Vec3 KnockBackDir = _Other->Transform()->GetRelativePos() - Transform()->GetRelativePos();
					KnockBackDir = Normalize(KnockBackDir);

					((CMonsterCopScript*)MonsterScript[i])->SetState(1);
					((CMonsterCopScript*)MonsterScript[i])->SetAnimNum(8);		// 피격 시작 애니메이션.
					((CMonsterCopScript*)MonsterScript[i])->SetKnockBackDir(KnockBackDir);
					
					if (!m_bBossMap)
					{
						Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_blood_wall.pref");
						CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
						Vec3 vEffectPos = _Other->Transform()->GetRelativePos();
						vEffectPos.z = 999.f;
						SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

						CEffectScript* EffectScript = pCloneEffect->GetScript<CEffectScript>();
						EffectScript->SetLoop(true);

						CAnim2D* Anim = pCloneEffect->Animator2D()->FindAnim(pCloneEffect->Animator2D()->GetCurAnimationName());

						Anim->SetStop(true);
						int Index = rand() % 6 + 3;
						Anim->SetCurIndex(Index);

						if (KnockBackDir.x > 0)
							pCloneEffect->Animator2D()->SetIsLeft(0);

						else if (KnockBackDir.x < 0)
							pCloneEffect->Animator2D()->SetIsLeft(1);
					}
					
					Vec3 Pos = _Other->Transform()->GetRelativePos();

					CreateSlash(Pos);
				}
			}

			if (ScriptName == L"CMonsterGruntScript")
			{
				if (((CMonsterGruntScript*)MonsterScript[i])->GetState() == 0)
				{
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat_2.wav");
					pSound->Play(1, 0.5f, true);
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_sword_1.wav");
					pSound2->Play(1, 0.5f, true);

					Vec3 KnockBackDir = _Other->Transform()->GetRelativePos() - Transform()->GetRelativePos();
					KnockBackDir = Normalize(KnockBackDir);

					((CMonsterGruntScript*)MonsterScript[i])->SetState(1);
					((CMonsterGruntScript*)MonsterScript[i])->SetAnimNum(8);		// 피격 시작 애니메이션.
					((CMonsterGruntScript*)MonsterScript[i])->SetKnockBackDir(KnockBackDir);

					if (!m_bBossMap)
					{
						Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_blood_wall.pref");
						CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
						Vec3 vEffectPos = _Other->Transform()->GetRelativePos();
						vEffectPos.z = 999.f;
						SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

						CEffectScript* EffectScript = pCloneEffect->GetScript<CEffectScript>();
						EffectScript->SetLoop(true);

						CAnim2D* Anim = pCloneEffect->Animator2D()->FindAnim(pCloneEffect->Animator2D()->GetCurAnimationName());

						Anim->SetStop(true);
						int Index = rand() % 6 + 3;
						Anim->SetCurIndex(Index);

						if (KnockBackDir.x > 0)
							pCloneEffect->Animator2D()->SetIsLeft(0);

						else if (KnockBackDir.x < 0)
							pCloneEffect->Animator2D()->SetIsLeft(1);
					}

					Vec3 Pos = _Other->Transform()->GetRelativePos();

					CreateSlash(Pos);
				}
			}

			if (ScriptName == L"CMonsterPompScript")
			{
				if (((CMonsterPompScript*)MonsterScript[i])->GetState() == 0)
				{
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat_3.wav");
					pSound->Play(1, 0.5f, true);
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_sword_2.wav");
					pSound2->Play(1, 0.5f, true);

					Vec3 KnockBackDir = _Other->Transform()->GetRelativePos() - Transform()->GetRelativePos();
					KnockBackDir = Normalize(KnockBackDir);

					((CMonsterPompScript*)MonsterScript[i])->SetState(1);
					((CMonsterPompScript*)MonsterScript[i])->SetAnimNum(8);		// 피격 시작 애니메이션.
					((CMonsterPompScript*)MonsterScript[i])->SetKnockBackDir(KnockBackDir);

					if (!m_bBossMap)
					{
						Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_blood_wall.pref");
						CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
						Vec3 vEffectPos = _Other->Transform()->GetRelativePos();
						vEffectPos.z = 999.f;
						SpawnGameObject(pCloneEffect, vEffectPos, L"Effect");

						CEffectScript* EffectScript = pCloneEffect->GetScript<CEffectScript>();
						EffectScript->SetLoop(true);

						CAnim2D* Anim = pCloneEffect->Animator2D()->FindAnim(pCloneEffect->Animator2D()->GetCurAnimationName());

						Anim->SetStop(true);
						int Index = rand() % 6 + 3;
						Anim->SetCurIndex(Index);

						if (KnockBackDir.x > 0)
							pCloneEffect->Animator2D()->SetIsLeft(0);

						else if (KnockBackDir.x < 0)
							pCloneEffect->Animator2D()->SetIsLeft(1);
					}

					Vec3 Pos = _Other->Transform()->GetRelativePos();

					CreateSlash(Pos);
				}
			}
		}
	}
}

void CPlayerAttackScript::OnOverlap(CCollider2D* _Other)
{
}

void CPlayerAttackScript::EndOverlap(CCollider2D* _Other)
{
}

void CPlayerAttackScript::CreateSlash(Vec3 _OtherPos)
{
	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Slash.pref");
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();

	Vec3 Dir = _OtherPos - Transform()->GetWorldPos();
	float theta = atan2f(Dir.y, Dir.x);

	SpawnGameObject(pCloneEffect, _OtherPos, L"Effect");

	pCloneEffect->Transform()->SetRelativeRot(0.f, 0.f, theta);

	pCloneEffect->SetLifeSpan(0.2f);
}

CPlayerAttackScript::CPlayerAttackScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERATTACKSCRIPT)
	, m_bBossMap(false)
{
}

CPlayerAttackScript::~CPlayerAttackScript()
{
}
