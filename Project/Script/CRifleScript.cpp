#include "pch.h"
#include "CRifleScript.h"

#include "CPlayerScript.h"
#include "CEffectScript.h"

#include <Engine\CAnim2D.h>

void CRifleScript::begin()
{
	// 충돌체만 넣어서 사용해서 MeshRender가 없음.
}

void CRifleScript::tick()
{
}

void CRifleScript::BeginOverlap(CCollider2D* _Other)
{
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

			Vec3 Pos = _Other->Transform()->GetRelativePos();

			Pos.z -= 100.f;
			CreateExplosion(Pos + Vec3(-40.f, 3.f, 0.f));
			CreateExplosion(Pos + Vec3(42.f, -4.f, 0.f));
			CreateExplosion(Pos + Vec3(5.f, 40.f, 0.f));
			CreateExplosion(Pos + Vec3(1.f, -40.f, 0.f));
		}
	}
}

void CRifleScript::OnOverlap(CCollider2D* _Other)
{
}

void CRifleScript::EndOverlap(CCollider2D* _Other)
{
}

void CRifleScript::CreateExplosion(Vec3 _OtherPos)
{
	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Explosion.pref");
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();

	SpawnGameObject(pCloneEffect, _OtherPos, L"Effect");
}

CRifleScript::CRifleScript()
	: CScript((UINT)SCRIPT_TYPE::RIFLESCRIPT)
{
}

CRifleScript::~CRifleScript()
{
}
