#include "pch.h"
#include "CMonsterRayScript.h"
#include "CDoorScript.h"

#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CHeadhunterScript.h"

void CMonsterRayScript::begin()
{
}

void CMonsterRayScript::tick()
{
	if (m_bDetect)
	{
		Destroy();
		return;
	}

	m_bIsLeft = GetOwner()->GetParent()->Animator2D()->IsLeft();

	float OffsetX = Collider2D()->GetOffsetScale().x / 2.f;

	if (m_bIsLeft)
	{
		Collider2D()->SetOffsetPos(Vec3(-OffsetX, 0.f, 0.f));
	}

	else
	{
		Collider2D()->SetOffsetPos(Vec3(OffsetX, 0.f, 0.f));
	}
}

void CMonsterRayScript::BeginOverlap(CCollider2D* _Other)
{
	// 이미 찾았으면 그냥 끝
	if (m_bDetect)
		return;

	// 타일인 경우, 충돌체 목록에 보관
	if (_Other->GetOwner()->GetLayerIndex() == 1)
	{
		CDoorScript* DoorScript = _Other->GetOwner()->GetScript<CDoorScript>();

		// Door인데 닫혀있다면 그냥 return;
		if (DoorScript)
		{
			if (DoorScript->IsOpen())
				return;
		}

		m_Collider2DList.push_back(_Other);
	}

	// Player Layer에 있는 물체와 충돌한 경우 알람을 보내야 한다.
	if (_Other->GetOwner()->GetLayerIndex() == 2)
	{
		// 플레이어 사이에 벽이 있는지 확인
		float PlayerPosX = _Other->Transform()->GetRelativePos().x;

		auto iter = m_Collider2DList.begin();
		auto iterEnd = m_Collider2DList.end();

		for (; iter != iterEnd; ++iter)
		{
			float WallX = (*iter)->GetOwner()->Transform()->GetRelativePos().x;

			if (m_bIsLeft)
			{
				if (WallX > PlayerPosX)
					return;
			}

			else if (!m_bIsLeft)
			{
				if (WallX < PlayerPosX)
					return;
			}
		}
		
		if (GetOwner()->GetParent())
		{
			vector<CScript*> vecObjScript = GetOwner()->GetParent()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CHeadhunterScript")
				{
					((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::SetTargetPlayer(_Other->GetOwner());
					m_bDetect = true;
				}

				if (ScriptName == L"CMonsterCopScript")
				{
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetDetect(1);
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetTargetPlayer(_Other->GetOwner());
					m_bDetect = true;
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetDetect(1);
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetTargetPlayer(_Other->GetOwner());
					m_bDetect = true;
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetDetect(1);
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetTargetPlayer(_Other->GetOwner());
					m_bDetect = true;
				}
			}
		}
	}
}

void CMonsterRayScript::OnOverlap(CCollider2D* _Other)
{
	// 이미 찾았으면 그냥 끝
	if (m_bDetect)
		return;

	// Player Layer에 있는 물체와 충돌한 경우 알람을 보내야 한다.
	if (_Other->GetOwner()->GetLayerIndex() == 2)
	{
		// 플레이어 사이에 벽이 있는지 확인
		float PlayerPosX = _Other->Transform()->GetRelativePos().x + _Other->GetOffsetScale().x / 2.f;

		auto iter = m_Collider2DList.begin();
		auto iterEnd = m_Collider2DList.end();

		for (; iter != iterEnd; ++iter)
		{
			float WallX = (*iter)->GetOwner()->Transform()->GetRelativePos().x;

			if (m_bIsLeft)
			{
				if (WallX > PlayerPosX)
					return;
			}

			else if (!m_bIsLeft)
			{
				if (WallX < PlayerPosX)
					return;
			}
		}

		if (GetOwner()->GetParent())
		{
			vector<CScript*> vecObjScript = GetOwner()->GetParent()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CMonsterCopScript")
				{
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetDetect(1);
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetTargetPlayer(_Other->GetOwner());
					m_bDetect = true;
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetDetect(1);
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetTargetPlayer(_Other->GetOwner());
					m_bDetect = true;
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetDetect(1);
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetTargetPlayer(_Other->GetOwner());
					m_bDetect = true;
				}
			}
		}
	}
}

void CMonsterRayScript::EndOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == 1)
	{
		auto iter = m_Collider2DList.begin();
		auto iterEnd = m_Collider2DList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (*iter == _Other)
			{
				m_Collider2DList.erase(iter);
				return;
			}
		}
	}
}

CMonsterRayScript::CMonsterRayScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERRAYSCRIPT),
	m_bDetect(false),
	m_bIsLeft(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_bDetect, "Detect");
}

CMonsterRayScript::~CMonsterRayScript()
{
}
