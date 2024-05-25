#include "pch.h"
#include "CMonsterAttackScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CPlayerScript.h"
#include "CEffectScript.h"

#include <Engine\CAnim2D.h>

void CMonsterAttackScript::begin()
{
}

void CMonsterAttackScript::tick()
{
	bool PrevIsLeft = m_bIsLeft;

	if (m_Target)
		m_bIsLeft = m_Target->Animator2D()->IsLeft();

	Vec3 OffSetPos = Collider2D()->GetOffsetPos();

	if (m_bIsLeft ^ PrevIsLeft)
	{
		Collider2D()->SetOffsetPos(Vec3(-OffSetPos.x, OffSetPos.y, OffSetPos.z));
	}

	else
	{
		Collider2D()->SetOffsetPos(Vec3(OffSetPos.x, OffSetPos.y, OffSetPos.z));
	}

	if (Animator2D())
	{
		Animator2D()->SetIsLeft(m_bIsLeft);
	}
}

void CMonsterAttackScript::BeginOverlap(CCollider2D* _Other)
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
			Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\punch_hit.wav");
			pSound2->Play(1, 0.5f, false);

			if (PlayerScript->GetImmortal())
				return;

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
	}
}

void CMonsterAttackScript::OnOverlap(CCollider2D* _Other)
{
}

void CMonsterAttackScript::EndOverlap(CCollider2D* _Other)
{
}

CMonsterAttackScript::CMonsterAttackScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERATTACKSCRIPT)
{
}

CMonsterAttackScript::~CMonsterAttackScript()
{
}
