#include "pch.h"
#include "CCeilingScript.h"
#include "CGravityScript.h"
#include "CPlayerScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CHeadhunterScript.h"
#include "CGrenadeScript.h"

#include "CScriptMgr.h"

void CCeilingScript::begin()
{
}

void CCeilingScript::tick()
{
}

void CCeilingScript::BeginOverlap(CCollider2D* _Other)
{
	CGameObject* pGameObject = _Other->GetOwner();

	vector<CScript*> vecObjScript = pGameObject->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CGrenadeScript")
		{
			((CGrenadeScript*)vecObjScript[i])->CGrenadeScript::AddCeiling();
		}

		if (ScriptName == L"CHeadhunterScript")
		{
			if (!((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::GetRewind())
				((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::AddCeiling();
		}

		if (ScriptName == L"CMonsterPompScript")
		{
			if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
				((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::AddCeiling();
		}

		if (ScriptName == L"CMonsterCopScript")
		{
			if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
				((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::AddCeiling();
		}

		if (ScriptName == L"CMonsterGruntScript")
		{
			if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
				((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::AddCeiling();
		}

		if (ScriptName == L"CPlayerScript")
		{
			if(!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
				((CPlayerScript*)vecObjScript[i])->CPlayerScript::AddCeiling();
		}
	}
}

void CCeilingScript::OnOverlap(CCollider2D* _Other)
{
}

void CCeilingScript::EndOverlap(CCollider2D* _Other)
{
	CGameObject* pGameObject = _Other->GetOwner();

	vector<CScript*> vecObjScript = pGameObject->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CGrenadeScript")
		{
			((CGrenadeScript*)vecObjScript[i])->CGrenadeScript::ReleaseCeiling();
		}

		if (ScriptName == L"CHeadhunterScript")
		{
			if (!((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::GetRewind())
				((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::ReleaseCeiling();
		}

		if (ScriptName == L"CMonsterPompScript")
		{
			if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
				((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseCeiling();
		}

		if (ScriptName == L"CMonsterCopScript")
		{
			if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
				((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseCeiling();
		}

		if (ScriptName == L"CMonsterGruntScript")
		{
			if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
				((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseCeiling();
		}

		if (ScriptName == L"CPlayerScript")
		{
			if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
				((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseCeiling();
		}
	}
}

CCeilingScript::CCeilingScript()
	: CScript((UINT)SCRIPT_TYPE::CEILINGSCRIPT)
{
}

CCeilingScript::~CCeilingScript()
{
}
