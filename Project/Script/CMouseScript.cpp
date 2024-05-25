#include "pch.h"
#include "CMouseScript.h"
#include <Engine\CRenderMgr.h>

void CMouseScript::begin()
{
	MeshRender()->GetDynamicMaterial();
}

void CMouseScript::tick()
{
	Vec3 Pos = CRenderMgr::GetInst()->GetMainCam()->GetOwner()->Transform()->GetRelativePos() + Vec3(-640.f, +384.f, 0.f) + Vec3(CKeyMgr::GetInst()->GetMousePos().x, -CKeyMgr::GetInst()->GetMousePos().y, 0.f);
	Transform()->SetRelativePos(Pos);
}

void CMouseScript::SaveToLevelFile(FILE* _File)
{
}

void CMouseScript::LoadFromLevelFile(FILE* _FILE)
{
}

CMouseScript::CMouseScript()
	: CScript((UINT)SCRIPT_TYPE::MOUSESCRIPT)
{
}

CMouseScript::~CMouseScript()
{
}
