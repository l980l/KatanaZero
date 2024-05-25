#include "pch.h"
#include "CNormalWallScript.h"
#include "CGravityScript.h"
#include "CPlayerScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CHeadhunterScript.h"
#include "CGrenadeScript.h"

#include "CScriptMgr.h"

void CNormalWallScript::begin()
{
}

void CNormalWallScript::tick()
{
}

void CNormalWallScript::BeginOverlap(CCollider2D* _Other)
{
	CGameObject* pGameObject = _Other->GetOwner();

	vector<CScript*> vecObjScript = pGameObject->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CGrenadeScript")
		{
			if (m_bLeft)
				((CGrenadeScript*)vecObjScript[i])->CGrenadeScript::AddLWall();
			else
				((CGrenadeScript*)vecObjScript[i])->CGrenadeScript::AddRWall();
		}

		if (ScriptName == L"CHeadhunterScript")
		{
			if (!((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::GetRewind())
			{
				if (m_bLeft)
					((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::AddLWall();
				else
					((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::AddRWall();
			}
		}

		if (ScriptName == L"CMonsterCopScript")
		{
			if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
			{
				if (m_bLeft)
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::AddLWall();
				else
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::AddRWall();
			}
		}

		if (ScriptName == L"CMonsterGruntScript")
		{
			if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
			{
				if (m_bLeft)
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::AddLWall();
				else
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::AddRWall();
			}
		}

		if (ScriptName == L"CMonsterPompScript")
		{
			if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
			{
				if (m_bLeft)
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::AddLWall();
				else
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::AddRWall();
			}
		}

		if (ScriptName == L"CPlayerScript")
		{
			if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
			{
				if (m_bLeft)
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::AddLWall();
				else
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::AddRWall();
			}
		}
	}
}

void CNormalWallScript::OnOverlap(CCollider2D* _Other)
{
}

void CNormalWallScript::EndOverlap(CCollider2D* _Other)
{
	CGameObject* pGameObject = _Other->GetOwner();

	vector<CScript*> vecObjScript = pGameObject->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CGrenadeScript")
		{
			if (m_bLeft)
				((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::ReleaseLWall();
			else
				((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::ReleaseRWall();
		}

		if (ScriptName == L"CHeadhunterScript")
		{
			if (!((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::GetRewind())
			{
				if (m_bLeft)
					((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::ReleaseLWall();
				else
					((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::ReleaseRWall();
			}
		}

		if (ScriptName == L"CMonsterCopScript")
		{
			if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
			{
				if (m_bLeft)
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseLWall();
				else
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseRWall();
			}
		}

		if (ScriptName == L"CMonsterGruntScript")
		{
			if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
			{
				if (m_bLeft)
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseLWall();
				else
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseRWall();
			}
		}

		if (ScriptName == L"CMonsterPompScript")
		{
			if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
			{
				if (m_bLeft)
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseLWall();
				else
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseRWall();
			}
		}

		if (ScriptName == L"CPlayerScript")
		{
			if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
			{
				if (m_bLeft)
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseLWall();
				else
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseRWall();
			}
		}
	}
}

void CNormalWallScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_bLeft, sizeof(bool), 1, _File);
}

void CNormalWallScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_bLeft, sizeof(bool), 1, _FILE);
}

CNormalWallScript::CNormalWallScript()
	: CScript((UINT)SCRIPT_TYPE::NORMALWALLSCRIPT)
	, m_bLeft(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_bLeft, "IsLeftWall");
}

CNormalWallScript::~CNormalWallScript()
{
}
