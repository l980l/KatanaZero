#include "pch.h"
#include "CDoorScript.h"
#include "CPlayerScript.h"
#include "CPlayerAttackScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"

#include <Engine\CAnim2D.h>

CDoorScript::CDoorScript()
	: CScript((UINT)SCRIPT_TYPE::DOORSCRIPT)
	, m_bLeft(false)
	, m_fDoorTime(0.f)
	, m_bOpen(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_bLeft, "IsLeftDoor");
}

CDoorScript::~CDoorScript()
{
}

void CDoorScript::ReleaseDoorAll()
{
	auto iter = m_Collider2DList.begin();
	auto iterEnd = m_Collider2DList.end();

	for (; iter != iterEnd; ++iter)
	{
		vector<CScript*> vecObjScript = (*iter)->GetOwner()->GetScripts();

		for (size_t i = 0; i < vecObjScript.size(); ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

			if (m_bLeft)
			{
				if (ScriptName == L"CMonsterCopScript")
				{
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseRWall();
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseRWall();
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseRWall();
				}

				if (ScriptName == L"CPlayerScript")
				{
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseLWall();
				}
			}

			else
			{
				if (ScriptName == L"CMonsterCopScript")
				{
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseLWall();
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseLWall();
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseLWall();
				}

				if (ScriptName == L"CPlayerScript")
				{
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseRWall();
				}
			}
		}
	}

	m_Collider2DList.clear();
}

void CDoorScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	CAnim2D* Anim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());

	if (m_bLeft)
		Animator2D()->SetIsLeft(true);

	else
		Animator2D()->SetIsLeft(false);
}

void CDoorScript::tick()
{
}

void CDoorScript::BeginOverlap(CCollider2D* _Other)
{
	if (!m_bOpen)
	{
		// Player Layer에 있는 물체와 충돌한 경우 애니메이션을 확인하고, 
		if (_Other->GetOwner()->GetLayerIndex() == 2)
		{
			CPlayerScript* PlayerScript = _Other->GetOwner()->GetScript<CPlayerScript>();

			if (PlayerScript)
			{
				m_Collider2DList.push_back(_Other);

				if (m_bLeft)
					PlayerScript->AddLWall();

				else
					PlayerScript->AddRWall();
			}
		}

		// Player Projectile Layer에 있는 물체와 충돌한 경우, Player Slash인 경우에만 열려야 한다. Player Slash는 PlayerAttackScript를 갖고 있다.
		if (_Other->GetOwner()->GetLayerIndex() == 4)
		{
			CPlayerAttackScript* PlayerSlash = _Other->GetOwner()->GetScript<CPlayerAttackScript>();

			// PlayerSlash인 경우
			if (PlayerSlash)
			{
				m_bOpen = true;

				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_dooropen.wav");
				pSound->Play(1, 0.5f, false);

				Animator2D()->Play(L"animation\\Door.anim", false);

				// 모든 애들 Release 해줘야 함. 
				ReleaseDoorAll();
			}
		}

		// Monster Layer에 있는 물체와 충돌한 경우 Door 값을 올려준다. 오른쪽 문이면 왼쪽 wall을 올려주고, 왼쪽 문이면 오른쪽 wall을 올려주고. 
		if (_Other->GetOwner()->GetLayerIndex() == 3)
		{
			vector<CScript*> vecObjScript = _Other->GetOwner()->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (m_bLeft)
				{
					if (ScriptName == L"CMonsterCopScript")
					{
						((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::AddRWall();
					}

					if (ScriptName == L"CMonsterGruntScript")
					{
						((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::AddRWall();
					}

					if (ScriptName == L"CMonsterPompScript")
					{
						((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::AddRWall();
					}
				}

				else
				{
					if (ScriptName == L"CMonsterCopScript")
					{
						((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::AddLWall();
					}

					if (ScriptName == L"CMonsterGruntScript")
					{
						((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::AddLWall();
					}

					if (ScriptName == L"CMonsterPompScript")
					{
						((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::AddLWall();
					}
				}
			}

			m_Collider2DList.push_back(_Other);
		}
	}
}

void CDoorScript::OnOverlap(CCollider2D* _Other)
{
	if (!m_bOpen)
	{
		if (KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D))
		{
			auto iter = m_Collider2DList.begin();
			auto iterEnd = m_Collider2DList.end();

			for (; iter != iterEnd; ++iter)
			{
				if (*iter == _Other)
				{
					CPlayerScript* PlayerScript = _Other->GetOwner()->GetScript<CPlayerScript>();

					if (PlayerScript)
					{
						int AnimNum = PlayerScript->GetAnimNum();

						if (AnimNum == 0 || AnimNum == 4 || AnimNum == 7)
						{
							m_fDoorTime += DT;

							if (m_fDoorTime > 0.05f)
							{
								m_bOpen = true;

								Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_dooropen.wav");
								pSound->Play(1, 0.5f, false);

								Animator2D()->Play(L"animation\\Door.anim", false);

								PlayerScript->SetAnimNum(11);

								// 모든 애들 Release 해줘야 함. 
								ReleaseDoorAll();
								return;
							}
						}
					}
				}
			}
		}
	}
}

void CDoorScript::EndOverlap(CCollider2D* _Other)
{
	if (!m_bOpen)
	{
		// 떨어지게 되면 누적 시간 초기화.
		auto iter = m_Collider2DList.begin();
		auto iterEnd = m_Collider2DList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (*iter == _Other)
			{
				vector<CScript*> vecObjScript = (*iter)->GetOwner()->GetScripts();

				for (size_t i = 0; i < vecObjScript.size(); ++i)
				{
					wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

					if (m_bLeft)
					{
						if (ScriptName == L"CMonsterCopScript")
						{
							((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseRWall();
						}

						if (ScriptName == L"CMonsterGruntScript")
						{
							((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseRWall();
						}

						if (ScriptName == L"CMonsterPompScript")
						{
							((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseRWall();
						}

						if (ScriptName == L"CPlayerScript")
						{
							((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseLWall();
							m_fDoorTime = 0.f;
						}
					}

					else
					{
						if (ScriptName == L"CMonsterCopScript")
						{
							((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseLWall();
						}

						if (ScriptName == L"CMonsterGruntScript")
						{
							((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseLWall();
						}

						if (ScriptName == L"CMonsterPompScript")
						{
							((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseLWall();
						}

						if (ScriptName == L"CPlayerScript")
						{
							((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseRWall();
							m_fDoorTime = 0.f;
						}
					}
				}

				m_Collider2DList.erase(iter);
				return;
			}
		}
	}
}

void CDoorScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_bLeft, sizeof(bool), 1, _File);
}

void CDoorScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_bLeft, sizeof(bool), 1, _FILE);
}
