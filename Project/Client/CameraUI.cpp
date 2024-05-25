#include "pch.h"
#include "CameraUI.h"

#include <Engine\components.h>

CameraUI::CameraUI()
    : ComponentUI("##Camera", COMPONENT_TYPE::CAMERA)
{
    SetName("Camera");
}

CameraUI::~CameraUI()
{
}

int CameraUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

	//if (ImGui::Button("##CreateUICamera", ImVec2(18, 18)))
	//{
	//	// UI cameara
	//	CGameObject* pUI2Cam = new CGameObject;
	//	pUI2Cam->SetName(L"UI2Camera");

	//	pUI2Cam->AddComponent(new CTransform);
	//	pUI2Cam->AddComponent(new CCamera);

	//	pUI2Cam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//	pUI2Cam->Camera()->SetCameraIndex(2);
	//	pUI2Cam->Camera()->SetLayerMask(31, true);

	//	SpawnGameObject(pUI2Cam, Vec3(0.f, 4828.f, 0.f), L"ViewPort UI");
	//}

	//if (ImGui::Button("##CreateUICamera", ImVec2(18, 18)))
	//{
	//	// UI cameara
	//	CGameObject* pUI2Cam = new CGameObject;
	//	pUI2Cam->SetName(L"ColliderCamera");

	//	pUI2Cam->AddComponent(new CTransform);
	//	pUI2Cam->AddComponent(new CCamera);

	//	pUI2Cam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//	pUI2Cam->Camera()->SetCameraIndex(3);
	//	pUI2Cam->Camera()->SetLayerMask(31, true);

	//	SpawnGameObject(pUI2Cam, Vec3(0.f, 0.f, 0.f), L"ViewPort UI");
	//}

    return TRUE;
}
