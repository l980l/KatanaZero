#include "pch.h"
#include "CEffectScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"

#include <Engine\CAnim2D.h>

void CEffectScript::begin()
{
	MeshRender()->GetDynamicMaterial();
}

void CEffectScript::tick()
{
	CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());

	if (CurAnim->GetCurFrameIndex() >= CurAnim->GetEndFrameIndex() && !m_bLoop)
	{
		Destroy();
		return;
	}

	// Target이 있으면 Target 위치로 이동.
	if (m_Target)
	{
		if (m_bEmotion)
		{
			vector<CScript*> vecObjScript = m_Target->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CMonsterCopScript")
				{
					if (((CMonsterCopScript*)vecObjScript[i])->GetState() != 0)
					{
						Destroy();
						return;
					}
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (((CMonsterGruntScript*)vecObjScript[i])->GetState() != 0)
					{
						Destroy();
						return;
					}
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					if (((CMonsterPompScript*)vecObjScript[i])->GetState() != 0)
					{
						Destroy();
						return;
					}
				}
			}
		}

		Vec3 TargetPos = m_Target->Transform()->GetRelativePos();
		Vec3 Move = TargetPos - m_PrevTargetPos;
		
		m_PrevTargetPos = TargetPos;

		Transform()->SetRelativePos(Transform()->GetRelativePos() + Move);
	}

	if (m_bPlayerDustCloud)
	{
		Vec3 NewPos = Transform()->GetRelativePos();

		if (Animator2D()->IsLeft())
		{
			float MoveX = (float)rand() / RAND_MAX;
			float MoveY = (float)rand() / RAND_MAX;

			NewPos.x -= DT * 50.f * 2.f * MoveX;
			NewPos.y += DT * 50.f * 2.f * MoveY;
		}

		else
		{
			float MoveX = (float)rand() / RAND_MAX;
			float MoveY = (float)rand() / RAND_MAX;

			NewPos.x += DT * 50.f * 2.f * MoveX;
			NewPos.y += DT * 50.f * 2.f * MoveY;
		}

		Transform()->SetRelativePos(NewPos);
	}
}

void CEffectScript::BeginOverlap(CCollider2D* _Other)
{
}

CEffectScript::CEffectScript()
	: CScript((UINT)SCRIPT_TYPE::EFFECTSCRIPT)
	, m_bPlayerDustCloud(false)
	, m_Target(nullptr)
	, m_bLoop(false)
	, m_bEmotion(false)
{
}

CEffectScript::~CEffectScript()
{
}
