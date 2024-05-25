#include "pch.h"
#include "CMineScript.h"

#include "CPlayerScript.h"
#include "CEffectScript.h"

#include <Engine\CAnim2D.h>

void CMineScript::begin()
{
	SetLifeSpan(2.3f);
}

void CMineScript::tick()
{
	m_fTime += DT;

	if (!m_bBoom && m_fTime > 2.f)
	{
		m_bBoom = true;

		Vec3 Pos = Transform()->GetRelativePos();

		CreateExplosion(Pos + Vec3(-25.f, 25.f, 0.f));
		CreateExplosion(Pos + Vec3(25.f, -25.f, 0.f));
	}
}

void CMineScript::BeginOverlap(CCollider2D* _Other)
{
}

void CMineScript::OnOverlap(CCollider2D* _Other)
{
	if (m_bBoom)
	{
		if (_Other->GetOwner()->GetLayerIndex() == 2)
		{
			CPlayerScript* PlayerScript = _Other->GetOwner()->GetScript<CPlayerScript>();

			// 구르는 중이면 안맞음. 점프 중일 때도 안맞음. 
			if (PlayerScript->GetState() == 0 && PlayerScript->GetAnimNum() != 5 && PlayerScript->GetAnimNum() != 1 && PlayerScript->GetAnimNum() != 3)
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

					Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_blood_wall.pref");
					CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
					Vec3 vEffectPos = _Other->Transform()->GetRelativePos();
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
}

void CMineScript::EndOverlap(CCollider2D* _Other)
{
}

void CMineScript::CreateExplosion(Vec3 _OtherPos)
{
	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Explosion.pref");
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();

	SpawnGameObject(pCloneEffect, _OtherPos, L"Effect");
	
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_explosion_01.wav");
	pSound->Play(1, 0.6f, false);

	// NextLevel 
	Ptr<CPrefab> pEffectPrefab2 = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\NextLevel.pref");
	CGameObject* pCloneEffect2 = pEffectPrefab2->Instantiate();
	Vec3 vEffectPos2 = _OtherPos;
	SpawnGameObject(pCloneEffect2, vEffectPos2, L"MonsterProjectile");
	pCloneEffect2->Collider2D()->SetOffsetScale(Vec3(100.f, 50.f, 0.f));
}

CMineScript::CMineScript()
	: CScript((UINT)SCRIPT_TYPE::MINESCRIPT)
	, m_fTime(0.f)
	, m_bBoom(false)
{
}

CMineScript::~CMineScript()
{
}
