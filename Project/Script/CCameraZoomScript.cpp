#include "pch.h"
#include "CCameraZoomScript.h"

void CCameraZoomScript::begin()
{
	Camera()->SetScale(m_Zoom);
	m_StartPos = {};
}

void CCameraZoomScript::tick()
{
	if (m_StartMenu)
	{
		if (m_StartPos.y > -160.f)
		{
			m_StartPos.y -= DT * 100.f;

			Transform()->SetRelativePos(m_StartPos);
		}
	}
}

void CCameraZoomScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Zoom, sizeof(float), 1, _File);
	fwrite(&m_StartMenu, sizeof(bool), 1, _File);
}

void CCameraZoomScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_Zoom, sizeof(float), 1, _FILE);
	fread(&m_StartMenu, sizeof(bool), 1, _FILE);
}

CCameraZoomScript::CCameraZoomScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERAZOOMSCRIPT)
	, m_Zoom(1.f)
	, m_StartMenu(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Zoom, "Zoom");
	AddScriptParam(SCRIPT_PARAM::INT, &m_StartMenu, "StartMenu");
}

CCameraZoomScript::~CCameraZoomScript()
{
}
