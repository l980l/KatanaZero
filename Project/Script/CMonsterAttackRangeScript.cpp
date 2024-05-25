#include "pch.h"
#include "CMonsterAttackRangeScript.h"
#include "CDoorScript.h"

#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CDescendableFloorScript.h"

void CMonsterAttackRangeScript::begin()
{
}

void CMonsterAttackRangeScript::tick()
{
	if (GetOwner()->GetParent())
	{
		vector<CScript*> vecObjScript = GetOwner()->GetParent()->GetScripts();

		for (size_t i = 0; i < vecObjScript.size(); ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

			if (ScriptName == L"CMonsterCopScript")
			{
				m_bGun = true;
			}

			if (ScriptName == L"CMonsterGruntScript")
			{
				m_bIsLeft = ((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetLeft();
			}

			if (ScriptName == L"CMonsterPompScript")
			{
				m_bIsLeft = ((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetLeft();
			}
		}
	}

	if (!m_bGun)
	{
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
}

void CMonsterAttackRangeScript::BeginOverlap(CCollider2D* _Other)
{
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

		CDescendableFloorScript* DescendableFloorScript = _Other->GetOwner()->GetScript<CDescendableFloorScript>();

		// DescendableFloorScript이면 return;
		if (DescendableFloorScript)
		{
			return;
		}

		m_Collider2DList.push_back(_Other);
	}

	// Player Layer에 있는 물체와 충돌한 경우 공격신호를 보내야 한다.
	if (_Other->GetOwner()->GetLayerIndex() == 2)
	{
		if (GetOwner()->GetParent())
		{
			vector<CScript*> vecObjScript = GetOwner()->GetParent()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetDetect() == 1)
					{
						((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetAttackReady(true);
					}
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					if (((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetDetect() == 1)
					{
						((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetAttackReady(true);
					}
				}
			}
		}
	}
}

void CMonsterAttackRangeScript::OnOverlap(CCollider2D* _Other)
{
	// Player Layer에 있는 물체와 충돌한 경우 공격신호를 보내야 한다.
	if (_Other->GetOwner()->GetLayerIndex() == 2)
	{
		if (GetOwner()->GetParent())
		{
			vector<CScript*> vecObjScript = GetOwner()->GetParent()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CMonsterCopScript")
				{
					if (((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetDetect() == 1)
					{
						if (m_Collider2DList.size())
							((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetAttackReady(false);
						else
							((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetAttackReady(true);
					}
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetDetect() == 1)
					{
						((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetAttackReady(true);
					}
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					if (((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetDetect() == 1)
					{
						((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetAttackReady(true);
					}
				}
			}
		}
	}
}

void CMonsterAttackRangeScript::EndOverlap(CCollider2D* _Other)
{
	// 타일인 경우, 충돌체 목록에 
	if (_Other->GetOwner()->GetLayerIndex() == 1)
	{
		auto iter = m_Collider2DList.begin();
		auto iterEnd = m_Collider2DList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (_Other == (*iter))
			{
				m_Collider2DList.erase(iter);
				break;
			}
		}
	}

	// Player Layer에 있는 물체와 충돌한 경우 공격신호를 취소해야 한다.
	if (_Other->GetOwner()->GetLayerIndex() == 2)
	{
		if (GetOwner()->GetParent())
		{
			vector<CScript*> vecObjScript = GetOwner()->GetParent()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CMonsterCopScript")
				{
					if (((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetDetect() == 1)
					{
						((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetAttackReady(false);
					}
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetDetect() == 1)
					{
						((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetAttackReady(false);
					}
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					if (((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetDetect() == 1)
					{
						((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetAttackReady(false);
					}
				}
			}
		}
	}
}

void CMonsterAttackRangeScript::SaveToLevelFile(FILE* _File)
{
}

void CMonsterAttackRangeScript::LoadFromLevelFile(FILE* _FILE)
{
}

CMonsterAttackRangeScript::CMonsterAttackRangeScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERATTACKRANGESCRIPT)
	, m_bIsLeft(false)
	, m_bGun(false)
{
}

CMonsterAttackRangeScript::~CMonsterAttackRangeScript()
{
}
