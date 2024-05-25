#include "pch.h"
#include "CScriptMgr.h"

#include "CBatteryScript.h"
#include "CBulletScript.h"
#include "CCameraMoveScript.h"
#include "CCameraZoomScript.h"
#include "CCeilingScript.h"
#include "CDescendableFloorScript.h"
#include "CDoorScript.h"
#include "CEffectScript.h"
#include "CGravityScript.h"
#include "CGrenadeScript.h"
#include "CHeadhunterScript.h"
#include "CJumpWallScript.h"
#include "CMainCameraScript.h"
#include "CMineScript.h"
#include "CMissileScript.h"
#include "CMonsterAttackRangeScript.h"
#include "CMonsterAttackScript.h"
#include "CMonsterCopScript.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterRayScript.h"
#include "CMonsterScript.h"
#include "CMouseScript.h"
#include "CNextLevelScript.h"
#include "CNormalFloorScript.h"
#include "CNormalWallScript.h"
#include "CPlayerAttackScript.h"
#include "CPlayerScript.h"
#include "CRifleScript.h"
#include "CStairScript.h"
#include "CStartMenuScript.h"
#include "CTimerScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBatteryScript");
	_vec.push_back(L"CBulletScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCameraZoomScript");
	_vec.push_back(L"CCeilingScript");
	_vec.push_back(L"CDescendableFloorScript");
	_vec.push_back(L"CDoorScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CGrenadeScript");
	_vec.push_back(L"CHeadhunterScript");
	_vec.push_back(L"CJumpWallScript");
	_vec.push_back(L"CMainCameraScript");
	_vec.push_back(L"CMineScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterAttackRangeScript");
	_vec.push_back(L"CMonsterAttackScript");
	_vec.push_back(L"CMonsterCopScript");
	_vec.push_back(L"CMonsterGruntScript");
	_vec.push_back(L"CMonsterPompScript");
	_vec.push_back(L"CMonsterRayScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMouseScript");
	_vec.push_back(L"CNextLevelScript");
	_vec.push_back(L"CNormalFloorScript");
	_vec.push_back(L"CNormalWallScript");
	_vec.push_back(L"CPlayerAttackScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CRifleScript");
	_vec.push_back(L"CStairScript");
	_vec.push_back(L"CStartMenuScript");
	_vec.push_back(L"CTimerScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBatteryScript" == _strScriptName)
		return new CBatteryScript;
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCameraZoomScript" == _strScriptName)
		return new CCameraZoomScript;
	if (L"CCeilingScript" == _strScriptName)
		return new CCeilingScript;
	if (L"CDescendableFloorScript" == _strScriptName)
		return new CDescendableFloorScript;
	if (L"CDoorScript" == _strScriptName)
		return new CDoorScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CGrenadeScript" == _strScriptName)
		return new CGrenadeScript;
	if (L"CHeadhunterScript" == _strScriptName)
		return new CHeadhunterScript;
	if (L"CJumpWallScript" == _strScriptName)
		return new CJumpWallScript;
	if (L"CMainCameraScript" == _strScriptName)
		return new CMainCameraScript;
	if (L"CMineScript" == _strScriptName)
		return new CMineScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterAttackRangeScript" == _strScriptName)
		return new CMonsterAttackRangeScript;
	if (L"CMonsterAttackScript" == _strScriptName)
		return new CMonsterAttackScript;
	if (L"CMonsterCopScript" == _strScriptName)
		return new CMonsterCopScript;
	if (L"CMonsterGruntScript" == _strScriptName)
		return new CMonsterGruntScript;
	if (L"CMonsterPompScript" == _strScriptName)
		return new CMonsterPompScript;
	if (L"CMonsterRayScript" == _strScriptName)
		return new CMonsterRayScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMouseScript" == _strScriptName)
		return new CMouseScript;
	if (L"CNextLevelScript" == _strScriptName)
		return new CNextLevelScript;
	if (L"CNormalFloorScript" == _strScriptName)
		return new CNormalFloorScript;
	if (L"CNormalWallScript" == _strScriptName)
		return new CNormalWallScript;
	if (L"CPlayerAttackScript" == _strScriptName)
		return new CPlayerAttackScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CRifleScript" == _strScriptName)
		return new CRifleScript;
	if (L"CStairScript" == _strScriptName)
		return new CStairScript;
	if (L"CStartMenuScript" == _strScriptName)
		return new CStartMenuScript;
	if (L"CTimerScript" == _strScriptName)
		return new CTimerScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BATTERYSCRIPT:
		return new CBatteryScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAZOOMSCRIPT:
		return new CCameraZoomScript;
		break;
	case (UINT)SCRIPT_TYPE::CEILINGSCRIPT:
		return new CCeilingScript;
		break;
	case (UINT)SCRIPT_TYPE::DESCENDABLEFLOORSCRIPT:
		return new CDescendableFloorScript;
		break;
	case (UINT)SCRIPT_TYPE::DOORSCRIPT:
		return new CDoorScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
		break;
	case (UINT)SCRIPT_TYPE::GRENADESCRIPT:
		return new CGrenadeScript;
		break;
	case (UINT)SCRIPT_TYPE::HEADHUNTERSCRIPT:
		return new CHeadhunterScript;
		break;
	case (UINT)SCRIPT_TYPE::JUMPWALLSCRIPT:
		return new CJumpWallScript;
		break;
	case (UINT)SCRIPT_TYPE::MAINCAMERASCRIPT:
		return new CMainCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::MINESCRIPT:
		return new CMineScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERATTACKRANGESCRIPT:
		return new CMonsterAttackRangeScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERATTACKSCRIPT:
		return new CMonsterAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERCOPSCRIPT:
		return new CMonsterCopScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERGRUNTSCRIPT:
		return new CMonsterGruntScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERPOMPSCRIPT:
		return new CMonsterPompScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERRAYSCRIPT:
		return new CMonsterRayScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MOUSESCRIPT:
		return new CMouseScript;
		break;
	case (UINT)SCRIPT_TYPE::NEXTLEVELSCRIPT:
		return new CNextLevelScript;
		break;
	case (UINT)SCRIPT_TYPE::NORMALFLOORSCRIPT:
		return new CNormalFloorScript;
		break;
	case (UINT)SCRIPT_TYPE::NORMALWALLSCRIPT:
		return new CNormalWallScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERATTACKSCRIPT:
		return new CPlayerAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::RIFLESCRIPT:
		return new CRifleScript;
		break;
	case (UINT)SCRIPT_TYPE::STAIRSCRIPT:
		return new CStairScript;
		break;
	case (UINT)SCRIPT_TYPE::STARTMENUSCRIPT:
		return new CStartMenuScript;
		break;
	case (UINT)SCRIPT_TYPE::TIMERSCRIPT:
		return new CTimerScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BATTERYSCRIPT:
		return L"CBatteryScript";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CAMERAZOOMSCRIPT:
		return L"CCameraZoomScript";
		break;

	case SCRIPT_TYPE::CEILINGSCRIPT:
		return L"CCeilingScript";
		break;

	case SCRIPT_TYPE::DESCENDABLEFLOORSCRIPT:
		return L"CDescendableFloorScript";
		break;

	case SCRIPT_TYPE::DOORSCRIPT:
		return L"CDoorScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::GRENADESCRIPT:
		return L"CGrenadeScript";
		break;

	case SCRIPT_TYPE::HEADHUNTERSCRIPT:
		return L"CHeadhunterScript";
		break;

	case SCRIPT_TYPE::JUMPWALLSCRIPT:
		return L"CJumpWallScript";
		break;

	case SCRIPT_TYPE::MAINCAMERASCRIPT:
		return L"CMainCameraScript";
		break;

	case SCRIPT_TYPE::MINESCRIPT:
		return L"CMineScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERATTACKRANGESCRIPT:
		return L"CMonsterAttackRangeScript";
		break;

	case SCRIPT_TYPE::MONSTERATTACKSCRIPT:
		return L"CMonsterAttackScript";
		break;

	case SCRIPT_TYPE::MONSTERCOPSCRIPT:
		return L"CMonsterCopScript";
		break;

	case SCRIPT_TYPE::MONSTERGRUNTSCRIPT:
		return L"CMonsterGruntScript";
		break;

	case SCRIPT_TYPE::MONSTERPOMPSCRIPT:
		return L"CMonsterPompScript";
		break;

	case SCRIPT_TYPE::MONSTERRAYSCRIPT:
		return L"CMonsterRayScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MOUSESCRIPT:
		return L"CMouseScript";
		break;

	case SCRIPT_TYPE::NEXTLEVELSCRIPT:
		return L"CNextLevelScript";
		break;

	case SCRIPT_TYPE::NORMALFLOORSCRIPT:
		return L"CNormalFloorScript";
		break;

	case SCRIPT_TYPE::NORMALWALLSCRIPT:
		return L"CNormalWallScript";
		break;

	case SCRIPT_TYPE::PLAYERATTACKSCRIPT:
		return L"CPlayerAttackScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::RIFLESCRIPT:
		return L"CRifleScript";
		break;

	case SCRIPT_TYPE::STAIRSCRIPT:
		return L"CStairScript";
		break;

	case SCRIPT_TYPE::STARTMENUSCRIPT:
		return L"CStartMenuScript";
		break;

	case SCRIPT_TYPE::TIMERSCRIPT:
		return L"CTimerScript";
		break;

	}
	return nullptr;
}