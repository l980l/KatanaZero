#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine\CParticleSystem.h>

ParticleSystemUI::ParticleSystemUI()
    : ComponentUI("##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
{
    SetName("ParticleSystem");
}

ParticleSystemUI::~ParticleSystemUI()
{
}

int ParticleSystemUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    static float SpawnColor[4] = { GetTarget()->ParticleSystem()->GetSpawnColor().x, GetTarget()->ParticleSystem()->GetSpawnColor().y, GetTarget()->ParticleSystem()->GetSpawnColor().z, 0.f};
    static float SpawnScaleMin[3] = { GetTarget()->ParticleSystem()->GetSpawnScaleMin().x, GetTarget()->ParticleSystem()->GetSpawnScaleMin().y, GetTarget()->ParticleSystem()->GetSpawnScaleMin().z };
    static float SpawnScaleMax[3] = { GetTarget()->ParticleSystem()->GetSpawnScaleMax().x, GetTarget()->ParticleSystem()->GetSpawnScaleMax().y, GetTarget()->ParticleSystem()->GetSpawnScaleMax().z };
    static float BoxShapeScale[3] = { GetTarget()->ParticleSystem()->GetBoxShapeScale().x, GetTarget()->ParticleSystem()->GetBoxShapeScale().y, GetTarget()->ParticleSystem()->GetBoxShapeScale().z };
    static float SphereShapeRadius = GetTarget()->ParticleSystem()->GetSphereShapeRadius();
    static int SpawnShapeType = GetTarget()->ParticleSystem()->GetSpawnShapeType();
    static int SpawnRate = GetTarget()->ParticleSystem()->GetSpawnRate();
    static int Space = GetTarget()->ParticleSystem()->GetSpace();
    static float MinLifeTime = GetTarget()->ParticleSystem()->GetMinLifeTime();
    static float MaxLifeTime = GetTarget()->ParticleSystem()->GetMaxLifeTime();
    static float StartScale = GetTarget()->ParticleSystem()->GetStartScale();
    static float EndScale = GetTarget()->ParticleSystem()->GetEndScale();
    static int MaxParticleCount = GetTarget()->ParticleSystem()->GetMaxParticleCount();
    static float VelocityDir[3] = { GetTarget()->ParticleSystem()->GetVelocityDir().x, GetTarget()->ParticleSystem()->GetVelocityDir().y, GetTarget()->ParticleSystem()->GetVelocityDir().z};
    static int AddVelocityType = GetTarget()->ParticleSystem()->GetAddVelocityType();
    static float OffsetAngle = GetTarget()->ParticleSystem()->GetOffsetAngle();
    static float Speed = GetTarget()->ParticleSystem()->GetSpeed();
    static float StartDrag = GetTarget()->ParticleSystem()->GetStartDrag();
    static float EndDrag = GetTarget()->ParticleSystem()->GetEndDrag();
    static float NoiseTerm = GetTarget()->ParticleSystem()->GetNoiseTerm();
    static float NoiseForce = GetTarget()->ParticleSystem()->GetNoiseForce();
    static int VelocityAlignment = GetTarget()->ParticleSystem()->GetVelocityAlignment();
    static int VelocityScale = GetTarget()->ParticleSystem()->GetVelocityScale();
    static float MaxSpeed = GetTarget()->ParticleSystem()->GetMaxSpeed();
    static float MaxVelocityScale[3] = { GetTarget()->ParticleSystem()->GetMaxVelocityScale().x, GetTarget()->ParticleSystem()->GetMaxVelocityScale().y, GetTarget()->ParticleSystem()->GetMaxVelocityScale().z };
    
    // Save
    ImGui::Text("ModuleData Save  ");
    ImGui::SameLine();
    if (ImGui::Button("##ModuleData Save", ImVec2(18.f, 18.f)))
    {
        wstring Path = (L"particle\\" + GetTarget()->GetName() + L".ptcl");

        GetTarget()->ParticleSystem()->Save(Path);
    }

    // ModuleCheck
    ImGui::Text("PARTICLE_SPAWN   ");
    ImGui::SameLine();
    bool ParticleSpawnOn = GetTarget()->ParticleSystem()->GetModuleCheck(PARTICLE_MODULE::PARTICLE_SPAWN);
    ImGui::Checkbox("##PARTICLE_SPAWN", &ParticleSpawnOn);
    GetTarget()->ParticleSystem()->SetModuleCheck(PARTICLE_MODULE::PARTICLE_SPAWN, ParticleSpawnOn ? 1 : 0);

    if (ParticleSpawnOn)
    {

        ImGui::Text("SpawnColor       ");
        ImGui::SameLine();
        static ImVec4 spawnColor = ImVec4(GetTarget()->ParticleSystem()->GetSpawnColor().x, GetTarget()->ParticleSystem()->GetSpawnColor().y, GetTarget()->ParticleSystem()->GetSpawnColor().z, GetTarget()->ParticleSystem()->GetSpawnColor().w);
        ImGui::ColorEdit4("##SpawnColor", (float*)&spawnColor);
        ImGui::SameLine();
        if (ImGui::Button("##Apply SpawnColor", ImVec2(18.f, 18.f))) {
            Vec4 vSpawnColor(spawnColor.x, spawnColor.y, spawnColor.z, spawnColor.w);
            GetTarget()->ParticleSystem()->SetSpawnColor(vSpawnColor);
        }

        ImGui::Text("SpawnScaleMin    ");
        ImGui::SameLine();
        ImGui::DragFloat3("##SpawnScaleMin", SpawnScaleMin);
        ImGui::SameLine();
        if (ImGui::Button("##Apply SpawnScaleMin", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetSpawnScaleMin(Vec4(SpawnScaleMin[0], SpawnScaleMin[1], SpawnScaleMin[2], 0));
        }

        ImGui::Text("SpawnScaleMax    ");
        ImGui::SameLine();
        ImGui::DragFloat3("##SpawnScaleMax", SpawnScaleMax);
        ImGui::SameLine();
        if (ImGui::Button("##Apply SpawnScaleMax", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetSpawnScaleMax(Vec4(SpawnScaleMax[0], SpawnScaleMax[1], SpawnScaleMax[2], 0));
        }

        ImGui::Text("BoxShapeScale    ");
        ImGui::SameLine();
        ImGui::DragFloat3("##BoxShapeScale", BoxShapeScale);
        ImGui::SameLine();
        if (ImGui::Button("##Apply BoxShapeScale", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetBoxShapeScale(Vec3(BoxShapeScale[0], BoxShapeScale[1], BoxShapeScale[2]));
        }

        ImGui::Text("SphereShapeRadius");
        ImGui::SameLine();
        ImGui::DragFloat("##SphereShapeRadius", &SphereShapeRadius);
        ImGui::SameLine();
        if (ImGui::Button("##Apply SphereShapeRadius", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetSphereShapeRadius(SphereShapeRadius);
        }

        ImGui::Text("SpawnShapeType   ");
        ImGui::SameLine();
        const char* spawnShapeTypes[] = { "Box", "Sphere" };
        const int numSpawnShapeTypes = sizeof(spawnShapeTypes) / sizeof(spawnShapeTypes[0]);
        static int selectedSpawnShapeType = 0;
        ImGui::Combo("##SpawnShapeType", &selectedSpawnShapeType, spawnShapeTypes, numSpawnShapeTypes);
        ImGui::SameLine();
        if (ImGui::Button("##Apply SpawnShapeType", ImVec2(18.f, 18.f))) {
            GetTarget()->ParticleSystem()->SetSpawnShapeType(selectedSpawnShapeType);
        }

        // SetSpawnRate
        ImGui::Text("SpawnRate        ");
        ImGui::SameLine();
        ImGui::DragInt("##SpawnRate", &SpawnRate);
        ImGui::SameLine();
        if (ImGui::Button("##Apply SpawnRate", ImVec2(18.f, 18.f))) {
            GetTarget()->ParticleSystem()->SetSpawnRate(SpawnRate);
        }

        // SetSpace
        ImGui::Text("Space            ");
        ImGui::SameLine();
        ImGui::DragInt("##Space", &Space);
        ImGui::SameLine();
        if (ImGui::Button("##Apply Space", ImVec2(18.f, 18.f))) {
            GetTarget()->ParticleSystem()->SetSpace(Space);
        }

        // SetMinLifeTime
        ImGui::Text("MinLifeTime      ");
        ImGui::SameLine();
        ImGui::DragFloat("##MinLifeTime", &MinLifeTime);
        ImGui::SameLine();
        if (ImGui::Button("##Apply MinLifeTime", ImVec2(18.f, 18.f))) {
            GetTarget()->ParticleSystem()->SetMinLifeTime(MinLifeTime);
        }

        // SetMaxLifeTime
        ImGui::Text("MaxLifeTime      ");
        ImGui::SameLine();
        ImGui::DragFloat("##MaxLifeTime", &MaxLifeTime);
        ImGui::SameLine();
        if (ImGui::Button("##Apply MaxLifeTime", ImVec2(18.f, 18.f))) {
            GetTarget()->ParticleSystem()->SetMaxLifeTime(MaxLifeTime);
        }
    }

    ImGui::Text("COLOR_CHANGE     ");
    ImGui::SameLine();
    bool ParticleColorChangeOn = GetTarget()->ParticleSystem()->GetModuleCheck(PARTICLE_MODULE::COLOR_CHANGE);
    ImGui::Checkbox("##COLOR_CHANGE", &ParticleColorChangeOn);
    GetTarget()->ParticleSystem()->SetModuleCheck(PARTICLE_MODULE::COLOR_CHANGE, ParticleColorChangeOn ? 1 : 0);
    
    if (ParticleColorChangeOn)
    {
        // SetStartColor
        ImGui::Text("StartColor       ");
        ImGui::SameLine();
        static ImVec4 startColor = ImVec4(GetTarget()->ParticleSystem()->GetStartColor().x, GetTarget()->ParticleSystem()->GetStartColor().y, GetTarget()->ParticleSystem()->GetStartColor().z, GetTarget()->ParticleSystem()->GetStartColor().w);
        ImGui::ColorEdit4("##StartColor", (float*)&startColor);
        ImGui::SameLine();
        if (ImGui::Button("##Apply StartColor", ImVec2(18.f, 18.f))) {
            Vec4 vStartColor(startColor.x, startColor.y, startColor.z, startColor.w);
            GetTarget()->ParticleSystem()->SetStartColor(vStartColor);
        }

        // SetEndColor
        ImGui::Text("EndColor         ");
        ImGui::SameLine();
        static ImVec4 endColor = ImVec4(GetTarget()->ParticleSystem()->GetEndColor().x, GetTarget()->ParticleSystem()->GetEndColor().y, GetTarget()->ParticleSystem()->GetEndColor().z, GetTarget()->ParticleSystem()->GetEndColor().w);
        ImGui::ColorEdit4("##EndColor", (float*)&endColor);
        ImGui::SameLine();
        if (ImGui::Button("##Apply EndColor", ImVec2(18.f, 18.f))) {
            Vec4 vEndColor(endColor.x, endColor.y, endColor.z, endColor.w);
            GetTarget()->ParticleSystem()->SetEndColor(vEndColor);
        }
    }

    ImGui::Text("SCALE_CHANGE     ");
    ImGui::SameLine();
    bool ParticleScaleChangeOn = GetTarget()->ParticleSystem()->GetModuleCheck(PARTICLE_MODULE::SCALE_CHANGE);
    ImGui::Checkbox("##SCALE_CHANGE", &ParticleScaleChangeOn);
    GetTarget()->ParticleSystem()->SetModuleCheck(PARTICLE_MODULE::SCALE_CHANGE, ParticleScaleChangeOn ? 1 : 0);

    if (ParticleScaleChangeOn)
    {
        // StartScale
        ImGui::Text("StartScale       ");
        ImGui::SameLine();
        ImGui::DragFloat("##StartScale", &StartScale);
        ImGui::SameLine();
        if (ImGui::Button("##ApplyStartScale", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetStartScale(StartScale);
        }

        // EndScale
        ImGui::Text("EndScale         ");
        ImGui::SameLine();
        ImGui::DragFloat("##EndScale", &EndScale);
        ImGui::SameLine();
        if (ImGui::Button("##ApplyEndScale", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetEndScale(EndScale);
        }

        // MaxParticleCount
        ImGui::Text("MaxParticleCount ");
        ImGui::SameLine();
        ImGui::DragInt("##MaxParticleCount", &MaxParticleCount);
        ImGui::SameLine();
        if (ImGui::Button("##ApplyMaxParticleCount", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetMaxParticleCount(MaxParticleCount);
        }
    }

    ImGui::Text("ADD_VELOCITY     ");
    ImGui::SameLine();
    bool ParticleAddVelocityOn = GetTarget()->ParticleSystem()->GetModuleCheck(PARTICLE_MODULE::ADD_VELOCITY);
    ImGui::Checkbox("##ADD_VELOCITY", &ParticleAddVelocityOn);
    GetTarget()->ParticleSystem()->SetModuleCheck(PARTICLE_MODULE::ADD_VELOCITY, ParticleAddVelocityOn ? 1 : 0);

    if (ParticleAddVelocityOn)
    {
        // VelocityDir
        ImGui::Text("VelocityDirection");
        ImGui::SameLine();
        ImGui::DragFloat3("##VelocityDir", VelocityDir);
        ImGui::SameLine();
        if (ImGui::Button("##ApplyVelocityDir", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetVelocityDir(Vec4(VelocityDir[0], VelocityDir[1], VelocityDir[2], 0.f));
        }

        // AddVelocityType
        ImGui::Text("AddVelocityType  ");
        ImGui::SameLine();
        ImGui::Combo("##AddVelocityType", &AddVelocityType, "From Center\0To Center\0Fixed Direction\0");
        ImGui::SameLine();
        if (ImGui::Button("##ApplyAddVelocityType", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetAddVelocityType(AddVelocityType);
        }

        // OffsetAngle
        ImGui::Text("OffsetAngle      ");
        ImGui::SameLine();
        ImGui::DragFloat("##OffsetAngle", &OffsetAngle);
        ImGui::SameLine();
        if (ImGui::Button("##ApplyOffsetAngle", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetOffsetAngle(OffsetAngle);
        }

        // Speed
        ImGui::Text("Speed            ");
        ImGui::SameLine();
        ImGui::DragFloat("##Speed", &Speed);
        ImGui::SameLine();
        if (ImGui::Button("##ApplySpeed", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetSpeed(Speed);
        }
    }

    ImGui::Text("DRAG             ");
    ImGui::SameLine();
    bool ParticleDRAGOn = GetTarget()->ParticleSystem()->GetModuleCheck(PARTICLE_MODULE::DRAG);
    ImGui::Checkbox("##DRAG", &ParticleDRAGOn);
    GetTarget()->ParticleSystem()->SetModuleCheck(PARTICLE_MODULE::DRAG, ParticleDRAGOn ? 1 : 0);
   
    if (ParticleDRAGOn)
    {
        // StartDrag
        ImGui::Text("StartDrag        ");
        ImGui::SameLine();
        ImGui::DragFloat("##StartDrag", &StartDrag);
        ImGui::SameLine();
        if (ImGui::Button("##ApplyStartDrag", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetStartDrag(StartDrag);
        }

        // EndDrag
        ImGui::Text("EndDrag          ");
        ImGui::SameLine();
        ImGui::DragFloat("##EndDrag", &EndDrag);
        ImGui::SameLine();
        if (ImGui::Button("##ApplyEndDrag", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetEndDrag(EndDrag);
        }
    }

    ImGui::Text("NOISE_FORCE      ");
    ImGui::SameLine();
    bool ParticleNoiseForceOn = GetTarget()->ParticleSystem()->GetModuleCheck(PARTICLE_MODULE::NOISE_FORCE);
    ImGui::Checkbox("##NOISE_FORCE", &ParticleNoiseForceOn);
    GetTarget()->ParticleSystem()->SetModuleCheck(PARTICLE_MODULE::NOISE_FORCE, ParticleNoiseForceOn ? 1 : 0);

    if (ParticleNoiseForceOn)
    {
        // NoiseTerm
        ImGui::Text("NoiseTerm        ");
        ImGui::SameLine();
        ImGui::DragFloat("##NoiseTerm", &NoiseTerm);
        ImGui::SameLine();
        if (ImGui::Button("##ApplyNoiseTerm", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetNoiseTerm(NoiseTerm);
        }

        // NoiseForce
        ImGui::Text("NoiseForce       ");
        ImGui::SameLine();
        ImGui::DragFloat("##NoiseForce", &NoiseForce);
        ImGui::SameLine();
        if (ImGui::Button("##ApplyNoiseForce", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetNoiseForce(NoiseForce);
        }
    }
    
    ImGui::Text("RENDER           ");
    ImGui::SameLine();
    bool ParticleRenderOn = GetTarget()->ParticleSystem()->GetModuleCheck(PARTICLE_MODULE::RENDER);
    ImGui::Checkbox("##RENDER", &ParticleRenderOn);
    GetTarget()->ParticleSystem()->SetModuleCheck(PARTICLE_MODULE::RENDER, ParticleRenderOn ? 1 : 0);
    
    if (ParticleRenderOn)
    {
        // VelocityAlignment
        ImGui::Text("VelocityAlignment");
        ImGui::SameLine();
        const char* velocityAlignmentItems[] = { "No", "Yes" };
        ImGui::Combo("##VelocityAlignment", &VelocityAlignment, velocityAlignmentItems, 2);
        ImGui::SameLine();
        if (ImGui::Button("##Apply VelocityAlignment", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetVelocityAlignment(VelocityAlignment);
        }

        // VelocityScale
        ImGui::Text("VelocityScale    ");
        ImGui::SameLine();
        const char* velocityScaleItems[] = { "No", "Yes" };
        ImGui::Combo("##VelocityScale", &VelocityScale, velocityScaleItems, 2);
        ImGui::SameLine();
        if (ImGui::Button("##Apply VelocityScale", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetVelocityScale(VelocityScale);
        }

        // MaxSpeed
        ImGui::Text("MaxSpeed         ");
        ImGui::SameLine();
        ImGui::DragFloat("##MaxSpeed", &MaxSpeed);
        ImGui::SameLine();
        if (ImGui::Button("##Apply MaxSpeed", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetMaxSpeed(MaxSpeed);
        }

        // MaxVelocityScale
        ImGui::Text("MaxVelocityScale ");
        ImGui::SameLine();
        ImGui::DragFloat3("##MaxVelocityScale", MaxVelocityScale);
        ImGui::SameLine();
        if (ImGui::Button("##Apply MaxVelocityScale", ImVec2(18.f, 18.f)))
        {
            GetTarget()->ParticleSystem()->SetMaxVelocityScale(Vec4(MaxVelocityScale[0], MaxVelocityScale[1], MaxVelocityScale[2], 0.f));
        }
    }

    return TRUE;
}

