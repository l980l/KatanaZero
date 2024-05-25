#include "pch.h"
#include "CGrenadeScript.h"

#include "CPlayerScript.h"
#include "CEffectScript.h"

void CGrenadeScript::begin()
{
	MeshRender()->GetDynamicMaterial();
}

void CGrenadeScript::tick()
{
	if (m_bGrenade)
	{
		m_fSpeed -= DT * 100.f;

		// 날아가야 함.
		if (m_fSpeed > 0.f)
		{
			// 벽이나 천장에 닿았을 경우 방향 반전
			if (m_iGround > 0 || m_iCeiling > 0)
			{
				m_vDir.y *= -1.f;
			}

			if (m_iLWall > 0 || m_iRWall > 0)
			{
				m_vDir.x *= -1.f;
			}

			Vec3 NewPos = Transform()->GetRelativePos();

			NewPos.x += m_vDir.x * m_fSpeed * DT * 10.f;
			NewPos.y += m_vDir.y * m_fSpeed * DT * 10.f;

			Transform()->SetRelativePos(NewPos);
		}

		// 속도가 다 떨어졌으면 
		else
		{
			if (!m_bBomb)
			{
				// 원 프리펩을 점점 작아지게 띄우면 될듯?
				Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Effect_Circle.pref");
				CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
				Vec3 NewPos = Transform()->GetRelativePos();
				SpawnGameObject(pCloneEffect, NewPos, L"Effect");

				m_vScale.x += DT * 400.f;
				m_vScale.y += DT * 400.f;
				m_vScale.z = 1;

				pCloneEffect->Transform()->SetRelativeScale(m_vScale);
				pCloneEffect->SetLifeSpan(0.f);		// 1 프레임동안만 보여주기.

				// 반지름 0 될즘에 폭발하게 하고.
				if (m_vScale.x >= 300.f)
				{
					m_bBomb = true;

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_boss_huntress_explosion_01.wav");
					pSound->Play(1, 0.5f, true);

					Vec3 NewPos = Transform()->GetRelativePos();

					for (int i = 0; i < 7; ++i)
					{
						CreateExplosion(NewPos + Vec3(cosf(3.14f * 2.f / 7.f * i), sinf(3.14f * 2.f / 7.f * i), 0.f) * 50.f);
						CreateExplosion(NewPos + Vec3(cosf(3.14f * 2.f / 7.f * i), sinf(3.14f * 2.f / 7.f * i), 0.f) * 120.f);
					}

					SetLifeSpan(0.3f);
				}
			}
		}
	}
}

void CGrenadeScript::BeginOverlap(CCollider2D* _Other)
{
}

void CGrenadeScript::OnOverlap(CCollider2D* _Other)
{
	if (!m_bGrenade)
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

					PlayerScript->SetState(1);
					PlayerScript->SetAnimNum(8);
					PlayerScript->SetKnockBackDir(KnockBackDir);
				}
			}
		}
	}
}

void CGrenadeScript::EndOverlap(CCollider2D* _Other)
{
}

void CGrenadeScript::CreateExplosion(Vec3 _OtherPos)
{
	Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Grenade_Explosion.pref");
	CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
	_OtherPos.z = 100.f;
	SpawnGameObject(pCloneEffect, _OtherPos, L"MonsterProjectile");

}

CGrenadeScript::CGrenadeScript()
	: CScript((UINT)SCRIPT_TYPE::GRENADESCRIPT)
	, m_fSpeed(0.f)
	, m_iGround(0)
	, m_iLWall(0)
	, m_iRWall(0)
	, m_iCeiling(0)
	, m_bBomb(false)
	, m_bGrenade(false)
{
}

CGrenadeScript::~CGrenadeScript()
{
}
