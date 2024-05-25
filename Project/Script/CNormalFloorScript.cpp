#include "pch.h"
#include "CNormalFloorScript.h"
#include "CGravityScript.h"
#include "CPlayerScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CHeadhunterScript.h"
#include "CGrenadeScript.h"

#include "CScriptMgr.h"

void CNormalFloorScript::begin()
{
}

void CNormalFloorScript::tick()
{
}

void CNormalFloorScript::BeginOverlap(CCollider2D* _Other)
{
	CGameObject* pGameObject = _Other->GetOwner();

	Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

	float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);
	float FloorHead = Transform()->GetWorldPos().y;// +(Collider2D()->GetOffsetScale().y / 4.f);

	// 물체 충돌체의 바닥 위치가 바닥 충돌체의 3/4 위쪽인가.
	if (ObjFoot >= FloorHead)
	{
		vector<CScript*> vecObjScript = pGameObject->GetScripts();

		for (size_t i = 0; i < vecObjScript.size(); ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

			if (ScriptName == L"CGravityScript")
			{
				((CGravityScript*)vecObjScript[i])->CGravityScript::AddGround();
			}

			if (ScriptName == L"CGrenadeScript")
			{
				((CGrenadeScript*)vecObjScript[i])->CGrenadeScript::AddGround();

				// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
				ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

				pGameObject->Transform()->SetRelativePos(ObjPos);
			}

			if (ScriptName == L"CHeadhunterScript")
			{
				if (!((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::GetRewind())
				{
					((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::AddGround();

					// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
					ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}
			}

			if (ScriptName == L"CMonsterCopScript")
			{
				if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
				{
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::AddGround();
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetFloorLevel(m_iFloorLevel);

					// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
					ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}
			}

			if (ScriptName == L"CMonsterGruntScript")
			{
				if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
				{
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::AddGround();
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetFloorLevel(m_iFloorLevel);

					// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
					ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}
			}

			if (ScriptName == L"CMonsterPompScript")
			{
				if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
				{
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::AddGround();
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetFloorLevel(m_iFloorLevel);

					// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
					ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}
			}

			if (ScriptName == L"CPlayerScript")
			{
				if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
				{
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::AddGround();
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::SetFloorLevel(m_iFloorLevel);

					// 바닥의 맨 위가 플레이어의 바닥이 되도록 설정.
					ObjPos.y += (Transform()->GetWorldPos().y + (Collider2D()->GetOffsetScale().y / 2.f)) - ObjFoot;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}
			}
		}

		// 제대로 바닥을 밟은 처리를 한 경우에 충돌체 리스트에 추가.
		m_Collider2DList.push_back(_Other);
	}
}

void CNormalFloorScript::OnOverlap(CCollider2D* _Other)
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

				if (ScriptName == L"CHeadhunterScript")
				{
					if (!((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::GetRewind())
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

				if (ScriptName == L"CMonsterCopScript")
				{
					if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
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

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
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

				if (ScriptName == L"CMonsterPompScript")
				{
					if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
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

void CNormalFloorScript::EndOverlap(CCollider2D* _Other)
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

				if (ScriptName == L"CGrenadeScript")
				{
					((CGrenadeScript*)vecObjScript[i])->CGrenadeScript::ReleaseGround();
				}

				if (ScriptName == L"CHeadhunterScript")
				{
					if (!((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::GetRewind())
						((CHeadhunterScript*)vecObjScript[i])->CHeadhunterScript::ReleaseGround();
				}

				if (ScriptName == L"CMonsterCopScript")
				{
					if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
						((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseGround();
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
						((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseGround();
				}

				if (ScriptName == L"CMonsterPompScript")
				{
					if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
						((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseGround();
				}

				if (ScriptName == L"CPlayerScript")
				{
					if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
						((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseGround();
				}
			}

			m_Collider2DList.erase(iter);
			return;
		}
	}
}

void CNormalFloorScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_iFloorLevel, sizeof(int), 1, _File);
}

void CNormalFloorScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_iFloorLevel, sizeof(int), 1, _FILE);
}


CNormalFloorScript::CNormalFloorScript()
	: CScript((UINT)SCRIPT_TYPE::NORMALFLOORSCRIPT), 
	m_iFloorLevel(1)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iFloorLevel, "FloorLevel");
}

CNormalFloorScript::~CNormalFloorScript()
{
}
