#include "pch.h"
#include "CDescendableFloorScript.h"
#include "CGravityScript.h"
#include "CPlayerScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"

#include "CScriptMgr.h"



void CDescendableFloorScript::begin()
{
}

void CDescendableFloorScript::tick()
{
}

void CDescendableFloorScript::Decend(CCollider2D* _Other)
{
	auto iter = m_Collider2DList.begin();
	auto iterEnd = m_Collider2DList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == _Other)
		{
			CGameObject* pGameObject = _Other->GetOwner();

			vector<CScript*> vecObjScript = pGameObject->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CGravityScript")
				{
					((CGravityScript*)vecObjScript[i])->CGravityScript::ReleaseGround();

				}

				if (ScriptName == L"CMonsterCopScript")
				{
					if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
						((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseGroundToFall();
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
						((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseGroundToFall();
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
						((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseGroundToFall();
				}

				if (ScriptName == L"CPlayerScript")
				{
					if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
						((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseGroundToFall();
				}
			}
			m_Collider2DList.erase(iter);
			return;
		}
	}
}

void CDescendableFloorScript::BeginOverlap(CCollider2D* _Other)
{
	CGameObject* pGameObject = _Other->GetOwner();

	vector<CScript*> vecObjScript = pGameObject->GetScripts();

	bool bOn = false;

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CMonsterPompScript")
		{
			if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
			{
				Vec3 PrevPos = ((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetPrevMonsterPos();
				float PrevFoot = PrevPos.y + _Other->GetOffsetPos().y - _Other->GetOffsetScale().y / 2.f;
				float FloorHead = Transform()->GetRelativePos().y + Collider2D()->GetOffsetScale().y / 4.f;

				// 이전 프레임의 Obj의 발이 낙하 가능 바닥의 위쪽보다 위에 있던 경우에만 밟은거로 처리.
				if (PrevFoot >= FloorHead)
				{
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::AddGroundToFall();
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetFloorLevel(m_iFloorLevel);
					bOn = true;
					m_Collider2DList.push_back(_Other);
				}
			}
		}

		if (ScriptName == L"CMonsterCopScript")
		{
			if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
			{
				Vec3 PrevPos = ((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetPrevMonsterPos();
				float PrevFoot = PrevPos.y + _Other->GetOffsetPos().y - _Other->GetOffsetScale().y / 2.f;
				float FloorHead = Transform()->GetRelativePos().y + Collider2D()->GetOffsetScale().y / 4.f;

				// 이전 프레임의 Obj의 발이 낙하 가능 바닥의 위쪽보다 위에 있던 경우에만 밟은거로 처리.
				if (PrevFoot >= FloorHead)
				{
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::AddGroundToFall();
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetFloorLevel(m_iFloorLevel);
					bOn = true;
					m_Collider2DList.push_back(_Other);
				}
			}
		}

		if (ScriptName == L"CMonsterGruntScript")
		{
			if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
			{
				Vec3 PrevPos = ((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetPrevMonsterPos();
				float PrevFoot = PrevPos.y + _Other->GetOffsetPos().y - _Other->GetOffsetScale().y / 2.f;
				float FloorHead = Transform()->GetRelativePos().y + Collider2D()->GetOffsetScale().y / 4.f;

				// 이전 프레임의 Obj의 발이 낙하 가능 바닥의 위쪽보다 위에 있던 경우에만 밟은거로 처리.
				if (PrevFoot >= FloorHead)
				{
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::AddGroundToFall();
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetFloorLevel(m_iFloorLevel);
					bOn = true;
					m_Collider2DList.push_back(_Other);
				}
			}
		}

		if (ScriptName == L"CPlayerScript")
		{
			if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
			{
				Vec3 PrevPos = ((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetPrevPlayerPos();
				float PrevFoot = PrevPos.y + _Other->GetOffsetPos().y - _Other->GetOffsetScale().y / 2.f;
				float FloorHead = Transform()->GetRelativePos().y + Collider2D()->GetOffsetScale().y / 4.f;

				// 이전 프레임의 Obj의 발이 낙하 가능 바닥의 위쪽보다 위에 있던 경우에만 밟은거로 처리.
				if (PrevFoot >= FloorHead)
				{
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::AddGroundToFall();
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::SetFloorLevel(m_iFloorLevel);
					bOn = true;
					m_Collider2DList.push_back(_Other);
				}
			}
		}
	}

	if (bOn)
	{
		vecObjScript = pGameObject->GetScripts();

		for (size_t i = 0; i < vecObjScript.size(); ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

			if (ScriptName == L"CGravityScript")
			{
				((CGravityScript*)vecObjScript[i])->CGravityScript::AddGround();
			}
		}
	}
}

void CDescendableFloorScript::OnOverlap(CCollider2D* _Other)
{
	if (KEY_PRESSED(KEY::S))
	{
		CGameObject* pGameObject = _Other->GetOwner();

		vector<CScript*> vecObjScript = pGameObject->GetScripts();

		for (size_t i = 0; i < vecObjScript.size(); ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

			// PlayerScript가 있는 경우에만 Decend 처리.
			if (ScriptName == L"CPlayerScript")
			{
				Decend(_Other);
				return;
			}
		}
	}

	auto iter = m_Collider2DList.begin();
	auto iterEnd = m_Collider2DList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == _Other)
		{
			CGameObject* pGameObject = _Other->GetOwner();

			vector<CScript*> vecObjScript = pGameObject->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CMonsterPompScript")
				{
					if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
					{
						// GoToStair 가 true면 Decend해서 계단으로 이동하게 함.
						if (((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetGoToStair())
						{
							((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetGoToStair(false);
							Decend(_Other);
							return;
						}

						Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

						float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);
						float FloorHead = Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 4.f);

						vector<CScript*> vecObjScript = pGameObject->GetScripts();

						// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
						ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

						pGameObject->Transform()->SetRelativePos(ObjPos);
					}
				}

				if (ScriptName == L"CMonsterCopScript")
				{
					if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
					{
						// GoToStair 가 true면 Decend해서 계단으로 이동하게 함.
						if (((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetGoToStair())
						{
							((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetGoToStair(false);
							Decend(_Other);
							return;
						}

						Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

						float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);
						float FloorHead = Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 4.f);

						vector<CScript*> vecObjScript = pGameObject->GetScripts();

						// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
						ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

						pGameObject->Transform()->SetRelativePos(ObjPos);
					}
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
					{
						// GoToStair 가 true면 Decend해서 계단으로 이동하게 함.
						if (((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetGoToStair())
						{
							((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetGoToStair(false);
							Decend(_Other);
							return;
						}

						Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

						float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);
						float FloorHead = Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 4.f);

						vector<CScript*> vecObjScript = pGameObject->GetScripts();

						// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
						ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

						pGameObject->Transform()->SetRelativePos(ObjPos);
					}
				}

				if (ScriptName == L"CPlayerScript")
				{
					if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
					{
						Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

						float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);
						float FloorHead = Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 4.f);

						vector<CScript*> vecObjScript = pGameObject->GetScripts();

						// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
						ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

						pGameObject->Transform()->SetRelativePos(ObjPos);
					}
				}
			}
		}
	}
}

void CDescendableFloorScript::EndOverlap(CCollider2D* _Other)
{
	auto iter = m_Collider2DList.begin();
	auto iterEnd = m_Collider2DList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == _Other)
		{
			CGameObject* pGameObject = _Other->GetOwner();

			vector<CScript*> vecObjScript = pGameObject->GetScripts();

			for (size_t i = 0; i < vecObjScript.size(); ++i)
			{
				wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

				if (ScriptName == L"CGravityScript")
				{
					((CGravityScript*)vecObjScript[i])->CGravityScript::ReleaseGround();

				}

				if (ScriptName == L"CMonsterPompScript")
				{
					if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
						((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseGroundToFall();
				}

				if (ScriptName == L"CMonsterCopScript")
				{
					if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
						((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseGroundToFall();
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
						((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseGroundToFall();
				}

				if (ScriptName == L"CPlayerScript")
				{
					if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
						((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseGroundToFall();
				}
			}
			m_Collider2DList.erase(iter);
			return;
		}
	}
}

void CDescendableFloorScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_iFloorLevel, sizeof(int), 1, _File);
}

void CDescendableFloorScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_iFloorLevel, sizeof(int), 1, _FILE);
}

CDescendableFloorScript::CDescendableFloorScript()
	: CScript((UINT)SCRIPT_TYPE::DESCENDABLEFLOORSCRIPT)
	, m_iFloorLevel(1)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iFloorLevel, "FloorLevel");
}

CDescendableFloorScript::~CDescendableFloorScript()
{
}
