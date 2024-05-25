#include "pch.h"
#include "CGravityScript.h"

#include "CPlayerScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterCopScript.h"
#include "CHeadhunterScript.h"

CGravityScript::CGravityScript()
	: CScript((UINT)SCRIPT_TYPE::GRAVITYSCRIPT),
	m_fGravityVelocity(0.f),
	m_iGround(false),
	m_bJump(false),
	m_bAttack(false),
	m_bOn(true)
{
}

CGravityScript::~CGravityScript()
{
}


void CGravityScript::tick()
{
	if (!m_bOn)
		return;

	float fG = 11.f;

	// PlayerState 가져오기. 몬스터들도 해줘야 함.
	int State = 0;
	vector<CScript*> vecObjScript = GetOwner()->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CPlayerScript")
		{
			State = ((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetState();
		}

		if (ScriptName == L"CMonsterCopScript")
		{
			State = ((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetState();
		}

		if (ScriptName == L"CMonsterGruntScript")
		{
			State = ((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetState();
		}

		if (ScriptName == L"CMonsterPompScript")
		{
			State = ((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetState();
		}

		if (ScriptName == L"CHeadhunterScript")
		{
			State = ((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::GetState();
		}
	}


	if (m_bAttack && State == 0)
	{
		int AnimNum = -1;
		
		vector<CScript*> vecObjScript = GetOwner()->GetScripts();

		for (size_t i = 0; i < vecObjScript.size(); ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

			if (ScriptName == L"CPlayerScript")
			{
				AnimNum = ((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetAnimNum();
			}
		}

		if (AnimNum != 2)
		{
			m_bAttack = false;
		}
	}

	// 점프상태인 경우
	else if (m_bJump && State == 0)
	{
		Vec3 fObjPos = Transform()->GetRelativePos();

		m_fGravityVelocity += fG * DT;

		fObjPos.y -= m_fGravityVelocity * DT * 85.f;

		Transform()->SetRelativePos(fObjPos);
		
		// 떨어지기 시작하는 경우
		if (m_fGravityVelocity >= 0)
		{
			m_bJump = false;

			vector<CScript*> vecObjScript = GetOwner()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CPlayerScript")
				{
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::SetAnimNum(3);
				}
			}
		}
	}

	else if (m_iGround > 0)
	{
		ClearGravityVelocity();

		vector<CScript*> vecObjScript = GetOwner()->GetScripts();

		for (size_t i = 0; i < vecObjScript.size(); ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

			if (ScriptName == L"CPlayerScript")
			{
				((CPlayerScript*)vecObjScript[i])->CPlayerScript::ResetAttackJump();	
			}
		}

		return;
	}

	else
	{
		Vec3 fObjPos = Transform()->GetRelativePos();

		m_fGravityVelocity += fG * DT;

		fObjPos.y -= m_fGravityVelocity * DT * 85.f;

		Transform()->SetRelativePos(fObjPos);
	}
}

void CGravityScript::ClearGravityVelocity()
{
	SetGravityVelocity(6.f);
}

void CGravityScript::Jump()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_jump.wav");
	pSound->Play(1, 0.5f, true);

	m_bJump = true;
	SetGravityVelocity(-5.f);
}

void CGravityScript::HeadHunterJump()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\player_jump.wav");
	pSound->Play(1, 0.5f, true);

	m_bJump = true;
	SetGravityVelocity(-7.f);
}

void CGravityScript::Attack()
{
	m_bAttack = true;
	SetGravityVelocity(2.f);
}


