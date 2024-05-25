#include "pch.h"
#include "CStairScript.h"
#include "CGravityScript.h"
#include "CPlayerScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"

#include "CScriptMgr.h"

void CStairScript::begin()
{
	Vec3 vecRot = Transform()->GetRelativeRot();

	float ascend = sin(abs(vecRot.z) * 3.141592f / 180.f);

	if (vecRot.z < 0)
	{
		if (ascend < 0)
			m_bAcsend = true;

		else
			m_bAcsend = false;
	}

	else
	{
		if (ascend < 0)
			m_bAcsend = false;

		else
			m_bAcsend = true;
	}
}

void CStairScript::tick()
{
}

void CStairScript::BeginOverlap(CCollider2D* _Other)
{
	CGameObject* pGameObject = _Other->GetOwner();
	
	Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

	float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);

	vector<CScript*> vecObjScript = pGameObject->GetScripts();

	for (size_t i = 0; i < vecObjScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecObjScript[i]);

		if (ScriptName == L"CGravityScript")
		{
			((CGravityScript*)vecObjScript[i])->CGravityScript::AddGround();
		}

		if (ScriptName == L"CMonsterCopScript")
		{
			if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
			{
				if (m_bAcsend)
				{
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::AddRStair();
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetFloorLevel(m_iFloorLevel);

					// 직선의 방정식을 이용하여 물체의 y축 위치를 정한다.
					float ZRot = Transform()->GetRelativeRot().z;
					float StairLength = Collider2D()->GetOffsetScale().x;
					Vec3 StairPos = Transform()->GetRelativePos();

					float dx = ObjPos.x + _Other->GetOffsetPos().x + _Other->GetOffsetScale().x / 2.f - StairPos.x;
					float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
					ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}

				else
				{
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::AddLStair();
					((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::SetFloorLevel(m_iFloorLevel);

					float ZRot = Transform()->GetRelativeRot().z;
					float StairLength = Collider2D()->GetOffsetScale().x;
					Vec3 StairPos = Transform()->GetRelativePos();

					float dx = ObjPos.x + _Other->GetOffsetPos().x - _Other->GetOffsetScale().x / 2.f - StairPos.x;
					float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
					ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}
			}
		}

		if (ScriptName == L"CMonsterPompScript")
		{
			if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
			{
				if (m_bAcsend)
				{
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::AddRStair();
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetFloorLevel(m_iFloorLevel);

					// 직선의 방정식을 이용하여 물체의 y축 위치를 정한다.
					float ZRot = Transform()->GetRelativeRot().z;
					float StairLength = Collider2D()->GetOffsetScale().x;
					Vec3 StairPos = Transform()->GetRelativePos();

					float dx = ObjPos.x + _Other->GetOffsetPos().x + _Other->GetOffsetScale().x / 2.f - StairPos.x;
					float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
					ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}

				else
				{
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::AddLStair();
					((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::SetFloorLevel(m_iFloorLevel);

					float ZRot = Transform()->GetRelativeRot().z;
					float StairLength = Collider2D()->GetOffsetScale().x;
					Vec3 StairPos = Transform()->GetRelativePos();

					float dx = ObjPos.x + _Other->GetOffsetPos().x - _Other->GetOffsetScale().x / 2.f - StairPos.x;
					float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
					ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}
			}
		}

		if (ScriptName == L"CMonsterGruntScript")
		{
			if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
			{
				if (m_bAcsend)
				{
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::AddRStair();
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetFloorLevel(m_iFloorLevel);

					// 직선의 방정식을 이용하여 물체의 y축 위치를 정한다.
					float ZRot = Transform()->GetRelativeRot().z;
					float StairLength = Collider2D()->GetOffsetScale().x;
					Vec3 StairPos = Transform()->GetRelativePos();

					float dx = ObjPos.x + _Other->GetOffsetPos().x + _Other->GetOffsetScale().x / 2.f - StairPos.x;
					float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
					ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}

				else
				{
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::AddLStair();
					((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::SetFloorLevel(m_iFloorLevel);

					float ZRot = Transform()->GetRelativeRot().z;
					float StairLength = Collider2D()->GetOffsetScale().x;
					Vec3 StairPos = Transform()->GetRelativePos();

					float dx = ObjPos.x + _Other->GetOffsetPos().x - _Other->GetOffsetScale().x / 2.f - StairPos.x;
					float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
					ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}
			}
		}

		if (ScriptName == L"CPlayerScript")
		{
			if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
			{
				if (m_bAcsend)
				{
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::AddRStair();
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::SetFloorLevel(m_iFloorLevel);

					// 직선의 방정식을 이용하여 물체의 y축 위치를 정한다.
					float ZRot = Transform()->GetRelativeRot().z;
					float StairLength = Collider2D()->GetOffsetScale().x;
					Vec3 StairPos = Transform()->GetRelativePos();

					float dx = ObjPos.x + _Other->GetOffsetPos().x + _Other->GetOffsetScale().x / 2.f - StairPos.x;
					float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
					ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}

				else
				{
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::AddLStair();
					((CPlayerScript*)vecObjScript[i])->CPlayerScript::SetFloorLevel(m_iFloorLevel);

					float ZRot = Transform()->GetRelativeRot().z;
					float StairLength = Collider2D()->GetOffsetScale().x;
					Vec3 StairPos = Transform()->GetRelativePos();

					float dx = ObjPos.x + _Other->GetOffsetPos().x - _Other->GetOffsetScale().x / 2.f - StairPos.x;
					float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
					ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

					pGameObject->Transform()->SetRelativePos(ObjPos);
				}
			}
		}
	}

	m_Collider2DList.push_back(_Other);

}

void CStairScript::OnOverlap(CCollider2D* _Other)
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

				if (ScriptName == L"CMonsterPompScript")
				{
					if (!((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::GetRewind())
					{
						Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

						float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);

						if (m_bAcsend)
						{
							// 직선의 방정식을 이용하여 물체의 y축 위치를 정한다.
							float ZRot = Transform()->GetRelativeRot().z;
							float StairLength = Collider2D()->GetOffsetScale().x;
							Vec3 StairPos = Transform()->GetRelativePos();

							float dx = ObjPos.x + _Other->GetOffsetPos().x + _Other->GetOffsetScale().x / 2.f - StairPos.x;
							float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
							ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

							pGameObject->Transform()->SetRelativePos(ObjPos);
						}

						else
						{
							float ZRot = Transform()->GetRelativeRot().z;
							float StairLength = Collider2D()->GetOffsetScale().x;
							Vec3 StairPos = Transform()->GetRelativePos();

							float dx = ObjPos.x + _Other->GetOffsetPos().x - _Other->GetOffsetScale().x / 2.f - StairPos.x;
							float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
							ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;


							pGameObject->Transform()->SetRelativePos(ObjPos);
						}
					}
				}

				if (ScriptName == L"CMonsterCopScript")
				{
					if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
					{
						Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

						float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);

						if (m_bAcsend)
						{
							// 직선의 방정식을 이용하여 물체의 y축 위치를 정한다.
							float ZRot = Transform()->GetRelativeRot().z;
							float StairLength = Collider2D()->GetOffsetScale().x;
							Vec3 StairPos = Transform()->GetRelativePos();

							float dx = ObjPos.x + _Other->GetOffsetPos().x + _Other->GetOffsetScale().x / 2.f - StairPos.x;
							float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
							ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

							pGameObject->Transform()->SetRelativePos(ObjPos);
						}

						else
						{
							float ZRot = Transform()->GetRelativeRot().z;
							float StairLength = Collider2D()->GetOffsetScale().x;
							Vec3 StairPos = Transform()->GetRelativePos();

							float dx = ObjPos.x + _Other->GetOffsetPos().x - _Other->GetOffsetScale().x / 2.f - StairPos.x;
							float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
							ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;


							pGameObject->Transform()->SetRelativePos(ObjPos);
						}
					}
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
					{
						Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

						float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);

						if (m_bAcsend)
						{
							// 직선의 방정식을 이용하여 물체의 y축 위치를 정한다.
							float ZRot = Transform()->GetRelativeRot().z;
							float StairLength = Collider2D()->GetOffsetScale().x;
							Vec3 StairPos = Transform()->GetRelativePos();

							float dx = ObjPos.x + _Other->GetOffsetPos().x + _Other->GetOffsetScale().x / 2.f - StairPos.x;
							float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
							ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

							pGameObject->Transform()->SetRelativePos(ObjPos);
						}

						else
						{
							float ZRot = Transform()->GetRelativeRot().z;
							float StairLength = Collider2D()->GetOffsetScale().x;
							Vec3 StairPos = Transform()->GetRelativePos();

							float dx = ObjPos.x + _Other->GetOffsetPos().x - _Other->GetOffsetScale().x / 2.f - StairPos.x;
							float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
							ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;


							pGameObject->Transform()->SetRelativePos(ObjPos);
						}
					}
				}

				if (ScriptName == L"CPlayerScript")
				{
					if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
					{
						Vec3 ObjPos = pGameObject->Transform()->GetRelativePos();

						float ObjFoot = ObjPos.y + _Other->GetOffsetPos().y - (_Other->GetOffsetScale().y / 2.f);

						if (m_bAcsend)
						{
							// 직선의 방정식을 이용하여 물체의 y축 위치를 정한다.
							float ZRot = Transform()->GetRelativeRot().z;
							float StairLength = Collider2D()->GetOffsetScale().x;
							Vec3 StairPos = Transform()->GetRelativePos();

							float dx = ObjPos.x + _Other->GetOffsetPos().x + _Other->GetOffsetScale().x / 2.f - StairPos.x;
							float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
							ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;

							pGameObject->Transform()->SetRelativePos(ObjPos);
						}

						else
						{
							float ZRot = Transform()->GetRelativeRot().z;
							float StairLength = Collider2D()->GetOffsetScale().x;
							Vec3 StairPos = Transform()->GetRelativePos();

							float dx = ObjPos.x + _Other->GetOffsetPos().x - _Other->GetOffsetScale().x / 2.f - StairPos.x;
							float dy = dx * tan(ZRot) + StairLength / 2.f * sin(ZRot);
							ObjPos.y += StairPos.y - StairLength / 2.f * sin(ZRot) + dy - ObjFoot - 2.f;


							pGameObject->Transform()->SetRelativePos(ObjPos);

						}
					}
				}
			}
		}
	}
}

void CStairScript::EndOverlap(CCollider2D* _Other)
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
					{
						if (m_bAcsend)
							((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseRStair();
						else
							((CMonsterPompScript*)vecObjScript[i])->CMonsterPompScript::ReleaseLStair();
					}
				}

				if (ScriptName == L"CMonsterCopScript")
				{
					if (!((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::GetRewind())
					{
						if (m_bAcsend)
							((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseRStair();
						else
							((CMonsterCopScript*)vecObjScript[i])->CMonsterCopScript::ReleaseLStair();
					}
				}

				if (ScriptName == L"CMonsterGruntScript")
				{
					if (!((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::GetRewind())
					{
						if (m_bAcsend)
							((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseRStair();
						else
							((CMonsterGruntScript*)vecObjScript[i])->CMonsterGruntScript::ReleaseLStair();
					}
				}

				if (ScriptName == L"CPlayerScript")
				{
					if (!((CPlayerScript*)vecObjScript[i])->CPlayerScript::GetRewind())
					{
						if (m_bAcsend)
							((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseRStair();
						else
							((CPlayerScript*)vecObjScript[i])->CPlayerScript::ReleaseLStair();
					}
				}
			}

			m_Collider2DList.erase(iter);
			return;
		}
	}
}

void CStairScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_iFloorLevel, sizeof(int), 1, _File);
}

void CStairScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_iFloorLevel, sizeof(int), 1, _FILE);
}


CStairScript::CStairScript()
	: CScript((UINT)SCRIPT_TYPE::STAIRSCRIPT)
	, m_bAcsend(false)
	, m_iFloorLevel(1)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iFloorLevel, "FloorLevel");
}

CStairScript::~CStairScript()
{
}
