#include "pch.h"
#include "CBulletScript.h"
#include "CPlayerScript.h"
#include "CEffectScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CPlayerAttackScript.h"
#include "CDescendableFloorScript.h"

#include <Engine\CAnim2D.h>

void CBulletScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	wstring temp = CurLevel->GetName();

	if (temp == L"Level\\Boss1.lv" || temp == L"Level\\Boss2.lv")
	{
		m_bBossMap = true;
	}
}

void CBulletScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	vPos += m_vDir * DT * m_fSpeed * 10.f;

	Transform()->SetRelativePos(vPos);
}

void CBulletScript::BeginOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == 1)
	{
		CDescendableFloorScript* DescendableFloorScript = _Other->GetOwner()->GetScript<CDescendableFloorScript>();

		// DescendableFloorScript이면 return;
		if (DescendableFloorScript)
		{
			return;
		}

		Destroy();
		return;
	}

	// 플레이어에 타격
	if (!m_bReflected)
	{
		if (_Other->GetOwner()->GetLayerIndex() == 4)
		{
			CPlayerAttackScript* PlayerAttackScript = _Other->GetOwner()->GetScript<CPlayerAttackScript>();

			if (PlayerAttackScript)
			{
				Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Bullet.pref");
				CGameObject* pCloneBullet = pEffectPrefab->Instantiate();
				Vec3 vEffectPos = Transform()->GetRelativePos();
				SpawnGameObject(pCloneBullet, vEffectPos, L"PlayerProjectile");

				pCloneBullet->Transform()->SetRelativeRot(Transform()->GetRelativeRot());

				CBulletScript* BulletScript = pCloneBullet->GetScript<CBulletScript>();

				if (BulletScript)
				{
					BulletScript->SetReflected(true);
					BulletScript->SetDir(-m_vDir);
				}

				CreateReflectFx(Transform()->GetWorldPos());

				m_bReflected = true;

				Destroy();
				return;
			}
		}

		else if (_Other->GetOwner()->GetLayerIndex() == 2)
		{
			CPlayerScript* PlayerScript = _Other->GetOwner()->GetScript<CPlayerScript>();

			// 구르는 중이면 안맞음.
			if (PlayerScript->GetState() == 0 && PlayerScript->GetAnimNum() != 5)
			{
				// 불사 상태면 그냥 나감. 대신 소리는 나야함.
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\playerdie.wav");
				pSound->Play(1, 0.5f, false);

				Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_bullet.wav");
				pSound2->Play(1, 0.5f, false);

				if (!PlayerScript->GetImmortal())
				{
					Vec3 KnockBackDir = _Other->Transform()->GetRelativePos() - Transform()->GetRelativePos();
					KnockBackDir = Normalize(KnockBackDir);

					PlayerScript->SetState(1);
					PlayerScript->SetAnimNum(8);
					PlayerScript->SetKnockBackDir(KnockBackDir);

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

				Destroy();
				return;
			}
		}
	}

	else
	{
		// Monster Layer에 있는 물체와 충돌한 경우 공격되어야 한다. 
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
						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_bullet.wav");
						pSound->Play(1, 0.5f, false);
						Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat_3.wav");
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

						CreateSlash(_Other->Transform()->GetRelativePos());

						Destroy();
						return;
					}
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (((CMonsterGruntScript*)MonsterScript[i])->GetState() == 0)
					{
						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_bullet.wav");
						pSound->Play(1, 0.5f, false);
						Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat_1.wav");
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

						CreateSlash(_Other->Transform()->GetRelativePos());

						Destroy();
						return;
					}
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					if (((CMonsterPompScript*)MonsterScript[i])->GetState() == 0)
					{
						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_bullet.wav");
						pSound->Play(1, 0.5f, false);
						Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat_2.wav");
						pSound2->Play(1, 0.5f, true);

						Vec3 KnockBackDir = _Other->Transform()->GetRelativePos() - Transform()->GetRelativePos();
						KnockBackDir = Normalize(KnockBackDir);

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

						CreateSlash(_Other->Transform()->GetRelativePos());

						Destroy();
						return;
					}
				}
			}
		}
	}
}

void CBulletScript::OnOverlap(CCollider2D* _Other)
{
}

void CBulletScript::EndOverlap(CCollider2D* _Other)
{
}

void CBulletScript::CreateSlash(Vec3 _OtherPos)
{
	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Slash.pref");
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();

	Vec3 Dir = _OtherPos - Transform()->GetWorldPos();
	float theta = atan2f(Dir.y, Dir.x);

	SpawnGameObject(pCloneEffect, _OtherPos, L"Effect");

	pCloneEffect->Transform()->SetRelativeRot(0.f, 0.f, theta);

	pCloneEffect->SetLifeSpan(0.2f);
}

void CBulletScript::CreateReflectFx(Vec3 _OtherPos)
{
	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_reflect.pref");
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();

	SpawnGameObject(pCloneEffect, _OtherPos, L"Effect");

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\slash_bullet.wav");
	pSound->Play(1, 0.5f, false);

	//CEffectScript* EffectScript = pCloneEffect->GetScript<CEffectScript>();
}

void CBulletScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_bReflected, sizeof(bool), 1, _File);
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CBulletScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_bReflected, sizeof(bool), 1, _FILE);
	fread(&m_fSpeed, sizeof(float), 1, _FILE);
}

CBulletScript::CBulletScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT)
	, m_vDir(Vec3(1.f, 0.f, 0.f))
	, m_fSpeed(100.f)
	, m_bBossMap(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_bReflected, "Reflected");
}

CBulletScript::~CBulletScript()
{
}
