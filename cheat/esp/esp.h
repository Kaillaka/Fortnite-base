// MADE BY hypedbykai

#pragma once
#include "../../util/util.h"
#include "../sdk/sdk.h"
#include "../sdk/offsets.hpp"
#include "../../util/settings.h"
#include "../../aimbot/aimbot.h"
#include "unicodes.hpp"
#include "../render.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

//
//inline void TRIGGERBOT() {
//    if (globals::triggerbot::enabled) {
//        auto tCurrentTime = std::chrono::steady_clock::now();
//        auto fTimeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(tCurrentTime - tCachedTime).count();
//
//        if (fTimeElapsed > 250)
//            bCanClick = true;
//
//        fvector iBoneIndex;
//        auto TargetedPawn = read<uintptr_t>(tocache::player_controller + 0x18b0);
//        if (TargetedPawn && SpoofCall->GetAsyncKeyState(globals::triggerbot::key))
//        {
//            if (globals::triggerbot::shotgun_only)
//            {
//                if (HeldWeaponType == EFortWeaponType::Shotgun)
//                {
//                    if (bHasRecentlyClicked)
//                    {
//                        tpBeginning = std::chrono::steady_clock::now();
//                        bHasRecentlyClicked = false;
//                    }
//
//                    tpEnding = std::chrono::steady_clock::now();
//                    iTimeSince = std::chrono::duration_cast<std::chrono::milliseconds>(tpEnding - tpBeginning).count();
//                    if (globals::triggerbot::zero) {
//                        globals::triggerbot::wait = 0.01f;
//                    }
//                    if (iTimeSince >= globals::triggerbot::wait)
//                    {
//                        if (bCanClick)
//                        {
//
//
//
//                            bHasRecentlyClicked = true;
//                            bCanClick = false;
//                            tCachedTime = std::chrono::steady_clock::now();
//
//                        }
//                    }
//                }
//            }
//            else
//            {
//                if (bHasRecentlyClicked)
//                {
//                    tpBeginning = std::chrono::steady_clock::now();
//                    bHasRecentlyClicked = false;
//                }
//
//                tpEnding = std::chrono::steady_clock::now();
//                iTimeSince = std::chrono::duration_cast<std::chrono::milliseconds>(tpEnding - tpBeginning).count();
//                if (globals::triggerbot::zero) {
//                    globals::triggerbot::wait = 0.01f;
//                }
//                if (iTimeSince >= globals::triggerbot::wait)
//                {
//                    if (bCanClick)
//                    {
//
//
//
//                        bHasRecentlyClicked = true;
//                        bCanClick = false;
//                        tCachedTime = std::chrono::steady_clock::now();
//                    }
//                }
//            }
//
//        }
//    }
//
//
//
//}
//

class FVector
{

public:
    FVector() : x(0.f), y(0.f), z(0.f)
    {

    }

    FVector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
    {

    }
    ~FVector()
    {

    }

    double x;
    double y;
    double z;

    inline float Dot(FVector v)
    {
        return x * v.x + y * v.y + z * v.z;
    }

    inline double Distance(FVector v)
    {
        return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
    }

    FVector operator+(FVector v)
    {
        return FVector(x + v.x, y + v.y, z + v.z);
    }

    FVector operator-(FVector v)
    {
        return FVector(x - v.x, y - v.y, z - v.z);
    }

    FVector operator*(float number) const {
        return FVector(x * number, y * number, z * number);
    }

};

class FRotator
{
public:
    FRotator() : Pitch(0.f), Yaw(0.f), Roll(0.f) {}
    FRotator(double _Pitch, double _Yaw, double _Roll) : Pitch(_Pitch), Yaw(_Yaw), Roll(_Roll) {}
    ~FRotator() {}

    double Pitch;
    double Yaw;
    double Roll;

    inline FRotator get() { return FRotator(Pitch, Yaw, Roll); }
    inline void set(double _Pitch, double _Yaw, double _Roll) { Pitch = _Pitch; Yaw = _Yaw; Roll = _Roll; }

    inline FRotator Clamp() {
        FRotator result = get();
        if (result.Pitch > 180) result.Pitch -= 360;
        else if (result.Pitch < -180) result.Pitch += 360;
        if (result.Yaw > 180) result.Yaw -= 360;
        else if (result.Yaw < -180) result.Yaw += 360;
        if (result.Pitch < -89) result.Pitch = -89;
        if (result.Pitch > 89) result.Pitch = 89;
        while (result.Yaw < -180.0f) result.Yaw += 360.0f;
        while (result.Yaw > 180.0f) result.Yaw -= 360.0f;
        result.Roll = 0;
        return result;
    }

    double Length() { return sqrt(Pitch * Pitch + Yaw * Yaw + Roll * Roll); }
    FRotator operator+(FRotator angB) { return FRotator(Pitch + angB.Pitch, Yaw + angB.Yaw, Roll + angB.Roll); }
    FRotator operator-(FRotator angB) { return FRotator(Pitch - angB.Pitch, Yaw - angB.Yaw, Roll - angB.Roll); }
    FRotator operator/(float flNum) { return FRotator(Pitch / flNum, Yaw / flNum, Roll / flNum); }
    FRotator operator*(float flNum) { return FRotator(Pitch * flNum, Yaw * flNum, Roll * flNum); }
    bool operator==(FRotator angB) { return Pitch == angB.Pitch && Yaw == angB.Yaw && Yaw == angB.Yaw; }
    bool operator!=(FRotator angB) { return Pitch != angB.Pitch || Yaw != angB.Yaw || Yaw != angB.Yaw; }
};

void RadarRange(float* x, float* y, float range) {
    if (fabs((*x)) > range || fabs((*y)) > range) {
        if ((*y) > (*x)) {
            if ((*y) > -(*x)) { (*x) = range * (*x) / (*y); (*y) = range; }
            else { (*y) = -range * (*y) / (*x); (*x) = -range; }
        }
        else {
            if ((*y) > -(*x)) { (*y) = range * (*y) / (*x); (*x) = range; }
            else { (*x) = -range * (*x) / (*y); (*y) = -range; }
        }
    }
}

void Draw_radar() {
    if (!settings::esp::radar) return;

    auto drawList = ImGui::GetBackgroundDrawList();
    static float animTime = 0.0f;
    static float pulseTime = 0.0f;
    static float scanAngle = 0.0f;
    animTime += ImGui::GetIO().DeltaTime;
    pulseTime += ImGui::GetIO().DeltaTime * 2.0f;
    scanAngle += ImGui::GetIO().DeltaTime * 1.5f;

    const float radarSize = 220.0f;
    const float radarX = Width - radarSize - 30.0f;
    const float radarY = 30.0f;
    const ImVec2 radarCenter(radarX + radarSize / 2.0f, radarY + radarSize / 2.0f);
    const float radarRadius = radarSize / 2.0f;
    const float radarRange = 150.0f;

    for (int i = 0; i < 3; i++) {
        float alpha = 100 - (i * 30);
        drawList->AddCircleFilled(radarCenter, radarRadius - (i * 2),
            IM_COL32(10, 15, 25, (int)alpha), 64);
    }

    drawList->AddCircle(radarCenter, radarRadius, IM_COL32(0, 180, 255, 255), 64, 3.0f);
    drawList->AddCircle(radarCenter, radarRadius - 2, IM_COL32(0, 120, 200, 150), 64, 1.5f);

    for (int i = 1; i <= 4; i++) {
        float circleRadius = (radarRadius / 4.0f) * i;
        float pulse = sinf(pulseTime + i * 0.5f) * 0.3f + 0.7f;
        int alpha = (int)(60 * pulse);
        drawList->AddCircle(radarCenter, circleRadius, IM_COL32(0, 150, 220, alpha), 64, 1.5f);
    }

    ImU32 gridColor = IM_COL32(0, 140, 200, 100);
    drawList->AddLine(ImVec2(radarCenter.x - radarRadius, radarCenter.y),
        ImVec2(radarCenter.x + radarRadius, radarCenter.y), gridColor, 1.5f);
    drawList->AddLine(ImVec2(radarCenter.x, radarCenter.y - radarRadius),
        ImVec2(radarCenter.x, radarCenter.y + radarRadius), gridColor, 1.5f);

    float diag = radarRadius * 0.707f;
    drawList->AddLine(ImVec2(radarCenter.x - diag, radarCenter.y - diag),
        ImVec2(radarCenter.x + diag, radarCenter.y + diag), IM_COL32(0, 140, 200, 60), 1.0f);
    drawList->AddLine(ImVec2(radarCenter.x - diag, radarCenter.y + diag),
        ImVec2(radarCenter.x + diag, radarCenter.y - diag), IM_COL32(0, 140, 200, 60), 1.0f);

    float scanEndX = radarCenter.x + cosf(scanAngle) * radarRadius;
    float scanEndY = radarCenter.y + sinf(scanAngle) * radarRadius;

    for (int i = 0; i < 30; i++) {
        float trailAngle = scanAngle - (i * 0.05f);
        float trailX = radarCenter.x + cosf(trailAngle) * radarRadius;
        float trailY = radarCenter.y + sinf(trailAngle) * radarRadius;
        int alpha = 150 - (i * 5);
        if (alpha > 0) {
            drawList->AddLine(radarCenter, ImVec2(trailX, trailY),
                IM_COL32(0, 255, 180, alpha), 2.0f);
        }
    }

    drawList->AddLine(radarCenter, ImVec2(scanEndX, scanEndY),
        IM_COL32(0, 255, 200, 255), 3.0f);

    Vector3 localPos = read<Vector3>(cache::RootComponent + offsets::RelativeLocation);
    uintptr_t CameraManager = read<uintptr_t>(cache::PlayerController + offsets::PlayerCameraManager);
    if (!CameraManager) return;
    FRotator localRot = read<FRotator>(CameraManager + offsets::CameraRotation);

    float playerPulse = sinf(animTime * 3.0f) * 0.3f + 0.7f;
    drawList->AddCircleFilled(radarCenter, 6.0f * playerPulse, IM_COL32(0, 255, 100, 200), 16);
    drawList->AddCircle(radarCenter, 6.0f * playerPulse, IM_COL32(0, 255, 100, 255), 16, 2.0f);
    drawList->AddCircle(radarCenter, 8.0f, IM_COL32(0, 255, 100, 80), 16, 1.0f);

    float yawRad = localRot.Yaw * (M_PI / 180.0f);
    float arrowLength = 12.0f;
    ImVec2 arrowTip(radarCenter.x + cosf(yawRad - M_PI / 2.0f) * arrowLength,
        radarCenter.y + sinf(yawRad - M_PI / 2.0f) * arrowLength);
    ImVec2 arrowLeft(radarCenter.x + cosf(yawRad - M_PI / 2.0f + 2.8f) * (arrowLength * 0.6f),
        radarCenter.y + sinf(yawRad - M_PI / 2.0f + 2.8f) * (arrowLength * 0.6f));
    ImVec2 arrowRight(radarCenter.x + cosf(yawRad - M_PI / 2.0f - 2.8f) * (arrowLength * 0.6f),
        radarCenter.y + sinf(yawRad - M_PI / 2.0f - 2.8f) * (arrowLength * 0.6f));

    drawList->AddTriangleFilled(arrowTip, arrowLeft, arrowRight, IM_COL32(0, 255, 100, 255));
    drawList->AddTriangle(arrowTip, arrowLeft, arrowRight, IM_COL32(255, 255, 255, 150), 1.5f);

    uintptr_t GameState = read<uintptr_t>(cache::Gworld + offsets::GameState);
    if (!GameState) return;
    uintptr_t PlayerArray = read<uintptr_t>(GameState + offsets::PlayerArray);
    int Num = read<int>(GameState + (offsets::PlayerArray + sizeof(uintptr_t)));

    for (int i = 0; i < Num; i++) {
        uintptr_t PlayerState = read<uintptr_t>(PlayerArray + (i * sizeof(uintptr_t)));
        if (!PlayerState) continue;
        uintptr_t Player = read<uintptr_t>(PlayerState + offsets::PawnPrivate);
        if (!Player || Player == cache::Player) continue;
        uintptr_t RootComp = read<uintptr_t>(Player + offsets::RootComponent);
        if (!RootComp) continue;

        Vector3 enemyPos = read<Vector3>(RootComp + offsets::RelativeLocation);
        float dx = enemyPos.x - localPos.x;
        float dy = enemyPos.y - localPos.y;
        float distance = sqrtf(dx * dx + dy * dy) / 100.0f;
        if (distance > radarRange) continue;

        float angle = atan2f(dy, dx) - yawRad;
        float radarX = cosf(angle) * distance;
        float radarY = sinf(angle) * distance;
        RadarRange(&radarX, &radarY, radarRange);

        float scale = radarRadius / radarRange;
        float screenX = radarCenter.x + (radarX * scale);
        float screenY = radarCenter.y + (radarY * scale);

        float distFromCenter = sqrtf(powf(screenX - radarCenter.x, 2) + powf(screenY - radarCenter.y, 2));
        if (distFromCenter > radarRadius - 4.0f) continue;

        float distNorm = distance / radarRange;
        ImU32 enemyColor = IM_COL32(255, (int)(50 + distNorm * 100), 50, 255);

        float enemyPulse = sinf(animTime * 4.0f + i * 0.3f) * 0.2f + 0.8f;

        drawList->AddCircleFilled(ImVec2(screenX, screenY), 6.0f * enemyPulse,
            IM_COL32(255, 50, 50, 60), 12);

        drawList->AddCircleFilled(ImVec2(screenX, screenY), 4.5f, enemyColor, 12);
        drawList->AddCircle(ImVec2(screenX, screenY), 4.5f, IM_COL32(255, 255, 255, 200), 12, 1.5f);

        if (distance < 50.0f) {
            drawList->AddLine(radarCenter, ImVec2(screenX, screenY),
                IM_COL32(255, 100, 100, 80), 1.0f);

            char distText[16];
            snprintf(distText, sizeof(distText), "%.0fm", distance);
            ImVec2 textSize = ImGui::CalcTextSize(distText);
            ImVec2 textPos(screenX - textSize.x / 2.0f, screenY - 20.0f);

            drawList->AddRectFilled(
                ImVec2(textPos.x - 3, textPos.y - 2),
                ImVec2(textPos.x + textSize.x + 3, textPos.y + textSize.y + 2),
                IM_COL32(0, 0, 0, 180), 3.0f);

            drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), distText);
        }
    }

    const char* radarLabel = "TACTICAL RADAR";
    ImVec2 labelSize = ImGui::CalcTextSize(radarLabel);
    ImVec2 labelPos(radarCenter.x - labelSize.x / 2.0f, radarY + radarSize + 8.0f);

    // Fond du label
    drawList->AddRectFilled(
        ImVec2(labelPos.x - 8, labelPos.y - 3),
        ImVec2(labelPos.x + labelSize.x + 8, labelPos.y + labelSize.y + 3),
        IM_COL32(0, 20, 30, 200), 4.0f);
    drawList->AddRect(
        ImVec2(labelPos.x - 8, labelPos.y - 3),
        ImVec2(labelPos.x + labelSize.x + 8, labelPos.y + labelSize.y + 3),
        IM_COL32(0, 180, 255, 255), 4.0f, 0, 1.5f);

    drawList->AddText(labelPos, IM_COL32(0, 220, 255, 255), radarLabel);

    char rangeText[32];
    snprintf(rangeText, sizeof(rangeText), "%.0fm", radarRange);
    ImVec2 rangeSize = ImGui::CalcTextSize(rangeText);
    drawList->AddText(
        ImVec2(radarCenter.x - rangeSize.x / 2.0f, radarY - 18.0f),
        IM_COL32(0, 200, 255, 200), rangeText);
}

void RenderWatermark() {
    auto drawList = ImGui::GetBackgroundDrawList();

    const float padding = 10.0f;
    const float spacing = 15.0f;
    const float iconSize = 16.0f;
    const float height = 30.0f;

    ImVec2 startPos(padding, padding);

    ImU32 bgColor = IM_COL32(20, 20, 25, 200);
    ImU32 textColor = IM_COL32(255, 255, 255, 255);
    ImU32 iconColor = IM_COL32(120, 120, 255, 255);

    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    char timeStr[32];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d %s",
        (ltm.tm_hour % 12 == 0) ? 12 : ltm.tm_hour % 12,
        ltm.tm_min,
        (ltm.tm_hour >= 12) ? "PM" : "AM");

    float fps = ImGui::GetIO().Framerate;
    char fpsStr[32];
    snprintf(fpsStr, sizeof(fpsStr), "%.0f FPS", fps);

    ImVec2 text1Size = ImGui::CalcTextSize("Kaillaka Base");
    ImVec2 text2Size = ImGui::CalcTextSize(fpsStr);
    ImVec2 text3Size = ImGui::CalcTextSize(timeStr);

    float totalWidth = padding * 2 +
        iconSize + spacing + text1Size.x + spacing +
        iconSize + spacing + text2Size.x + spacing +
        iconSize + spacing + text3Size.x;

    drawList->AddRectFilled(
        startPos,
        ImVec2(startPos.x + totalWidth, startPos.y + height),
        bgColor,
        8.0f
    );

    float currentX = startPos.x + padding;
    float centerY = startPos.y + height / 2.0f;

    ImVec2 iconCenter(currentX + iconSize / 2, centerY);
    drawList->AddCircleFilled(iconCenter, iconSize / 2, iconColor, 12);
    drawList->AddCircle(iconCenter, iconSize / 2, IM_COL32(255, 255, 255, 100), 12, 1.5f);

    currentX += iconSize + spacing;

    ImVec2 textPos1(currentX, centerY - text1Size.y / 2);
    drawList->AddText(textPos1, textColor, "Kaillaka Base");
    currentX += text1Size.x + spacing;

    ImVec2 cameraPos(currentX + iconSize / 2, centerY);
    drawList->AddRectFilled(
        ImVec2(cameraPos.x - iconSize / 2.5f, cameraPos.y - iconSize / 3.5f),
        ImVec2(cameraPos.x + iconSize / 2.5f, cameraPos.y + iconSize / 3.5f),
        iconColor,
        2.0f
    );
    drawList->AddCircleFilled(
        ImVec2(cameraPos.x + iconSize / 2.5f, cameraPos.y),
        iconSize / 5,
        iconColor,
        8
    );

    currentX += iconSize + spacing;

    ImVec2 textPos2(currentX, centerY - text2Size.y / 2);
    drawList->AddText(textPos2, textColor, fpsStr);
    currentX += text2Size.x + spacing;


    ImVec2 clockCenter(currentX + iconSize / 2, centerY);
    drawList->AddCircle(clockCenter, iconSize / 2, iconColor, 12, 1.5f);
    drawList->AddLine(
        clockCenter,
        ImVec2(clockCenter.x, clockCenter.y - iconSize / 3),
        iconColor,
        1.5f
    );
    drawList->AddLine(
        clockCenter,
        ImVec2(clockCenter.x + iconSize / 4, clockCenter.y),
        iconColor,
        1.5f
    );

    currentX += iconSize + spacing;

    ImVec2 textPos3(currentX, centerY - text3Size.y / 2);
    drawList->AddText(textPos3, textColor, timeStr);
}

std::string GetPlayerName(uintptr_t player_state) {
    __int64 FString = read<__int64>(player_state + 0xB08);
    int Length = read<int>(FString + 16);
    uintptr_t FText = read<__int64>(FString + 8);
    __int64 v6 = Length;
    if (!v6) return std::string("BOT");
    wchar_t* NameBuffer = new wchar_t[Length];
    mem::read_physical((PVOID)FText, NameBuffer, Length * sizeof(wchar_t));
    char v21;
    int v22;
    int i;
    int v25;
    WORD* v23;
    v21 = v6 - 1;
    if (!(DWORD)v6) v21 = 0;
    v22 = 0;
    v23 = (WORD*)NameBuffer;
    for (i = (v21) & 3; ; *v23++ += i & 7) {
        v25 = v6 - 1;
        if (!(DWORD)v6) v25 = 0;
        if (v22 >= v25) break;
        i += 3;
        ++v22;
    }
    std::wstring wbuffer{ NameBuffer };
    delete[] NameBuffer;
    return std::string(wbuffer.begin(), wbuffer.end());
}

struct BonePair { bone start, end; };
static const BonePair skeleton_bones[] = {
   {bone::Head, bone::Neck},
   {bone::Neck, bone::Chest},
   {bone::Chest, bone::SpineEighth},
   {bone::SpineEighth, bone::HipMid},
   {bone::Chest, bone::RightShoulder},
   {bone::RightShoulder, bone::RightShoulderLower},
   {bone::RightShoulderLower, bone::RightElBowUpper},
   {bone::RightElBowUpper, bone::RightElBow},
   {bone::RightElBow, bone::RightArmLower},
   {bone::Chest, bone::LeftShoulder},
   {bone::LeftShoulder, bone::LeftShoulderLower},
   {bone::LeftShoulderLower, bone::LeftElBowUpper},
   {bone::LeftElBowUpper, bone::LeftElBow},
   {bone::LeftElBow, bone::LeftArmLower},
   {bone::HipMid, bone::RightHip},
   {bone::RightHip, bone::RightHipLower},
   {bone::RightHipLower, bone::RightKnee},
   {bone::RightKnee, bone::RightUpperCalf},
   {bone::RightUpperCalf, bone::RightHeel},
   {bone::RightHeel, bone::RightToe},
   {bone::HipMid, bone::LeftHip},
   {bone::LeftHip, bone::LeftHipLower},
   {bone::LeftHipLower, bone::LeftKnee},
   {bone::LeftKnee, bone::LeftUpperCalf},
   {bone::LeftUpperCalf, bone::LeftHeel},
   {bone::LeftHeel, bone::LeftToe}
};
static constexpr int skeleton_bone_count = sizeof(skeleton_bones) / sizeof(BonePair);

inline std::string GetPlatformText(uintptr_t state) {
    uintptr_t test_platform = read<uintptr_t>(state + 0x430);
    if (!is_valid(test_platform)) return "";
    wchar_t platform_wchar[64] = { 0 };
    mem::read_physical((PVOID)test_platform, reinterpret_cast<PVOID>(platform_wchar), sizeof(platform_wchar));
    std::wstring platform_wstring(platform_wchar);
    return WStringToUTF8(platform_wstring);
}

inline ImColor PlatColor(const std::string& platform) {
    if (platform.find("XBL") != std::string::npos || platform.find("XSX") != std::string::npos)
        return ImColor(128, 224, 0, 200);
    if (platform.find("PSN") != std::string::npos || platform.find("PS5") != std::string::npos)
        return ImColor(0, 96, 192, 255);
    if (platform.find("WIN") != std::string::npos)
        return ImColor(126, 208, 252);
    if (platform.find("SWT") != std::string::npos)
        return ImColor(255, 0, 0);
    if (platform.find("AND") != std::string::npos)
        return ImColor(50, 168, 82);
    if (platform.find("MAC") != std::string::npos)
        return ImColor(255, 255, 255);
    if (platform.find("LNX") != std::string::npos)
        return ImColor(255, 233, 87);
    if (platform.find("IOS") != std::string::npos)
        return ImColor(255, 255, 255);
    return ImColor(255, 255, 255);
}

inline int32_t Rank(uintptr_t state) {
    uintptr_t habenero = read<uintptr_t>(state + 0xa48);
    if (!is_valid(habenero)) return 0;
    return read<int32_t>(habenero + 0xD0 + 0x10);
}

inline std::string realRank(int32_t real) {
    if (real == 0) return ("Bronze I");
    else if (real == 1) return ("Bronze II");
    else if (real == 2) return ("Bronze III");
    else if (real == 3) return ("Sliver I");
    else if (real == 4) return ("Sliver II");
    else if (real == 5) return ("Sliver III");
    else if (real == 6) return ("Gold I");
    else if (real == 7) return ("Gold II");
    else if (real == 8) return ("Gold III");
    else if (real == 9) return ("Platinum I");
    else if (real == 10) return ("Platinum II");
    else if (real == 11) return ("Platinum III");
    else if (real == 12) return ("Diamond I");
    else if (real == 13) return ("Diamond II");
    else if (real == 14) return ("Diamond III");
    else if (real == 15) return ("Elite");
    else if (real == 16) return ("Champion");
    else if (real == 17) return ("Unreal");
    else return ("Unranked");
}

ImU32 GetRankColor(int32_t rank) {
    switch (rank) {
    case 17: return ImColor(255, 0, 255, 255);
    case 16: return ImColor(255, 100, 255, 255);
    case 15: return ImColor(255, 50, 50, 255);
    case 14: return ImColor(0, 255, 255, 255);
    case 13: return ImColor(0, 255, 255, 255);
    case 12: return ImColor(0, 255, 255, 255);
    case 11: return ImColor(255, 215, 0, 255);
    case 10: return ImColor(255, 215, 0, 255);
    case 9:  return ImColor(255, 215, 0, 255);
    case 8:  return ImColor(255, 223, 0, 255);
    case 7:  return ImColor(255, 223, 0, 255);
    case 6:  return ImColor(255, 223, 0, 255);
    case 5:  return ImColor(192, 192, 192, 255);
    case 4:  return ImColor(192, 192, 192, 255);
    case 3:  return ImColor(192, 192, 192, 255);
    case 2:  return ImColor(205, 127, 50, 255);
    case 1:  return ImColor(205, 127, 50, 255);
    case 0:  return ImColor(205, 127, 50, 255);
    default: return ImColor(150, 150, 150, 255);
    }
}

void DrawSkeleton(uintptr_t Mesh, ImU32 color) {
    static thread_local Vector2 bone_screen[128];
    static thread_local bool bone_valid[128];
    std::memset(bone_valid, 0, sizeof(bone_valid));
    auto* draw = ImGui::GetBackgroundDrawList();
    for (const auto& pair : skeleton_bones) {
        int s = static_cast<int>(pair.start);
        int e = static_cast<int>(pair.end);
        if (!bone_valid[s]) {
            Vector3 pos = GetBoneWithRotation(Mesh, s);
            Vector2 scr = ProjectWorldToScreen(pos);
            if (scr.x > 0 && scr.y > 0 && scr.x < Width && scr.y < Height) {
                bone_screen[s] = scr;
                bone_valid[s] = true;
            }
        }
        if (!bone_valid[e]) {
            Vector3 pos = GetBoneWithRotation(Mesh, e);
            Vector2 scr = ProjectWorldToScreen(pos);
            if (scr.x > 0 && scr.y > 0 && scr.x < Width && scr.y < Height) {
                bone_screen[e] = scr;
                bone_valid[e] = true;
            }
        }
    }
    for (const auto& pair : skeleton_bones) {
        int s = static_cast<int>(pair.start);
        int e = static_cast<int>(pair.end);
        if (bone_valid[s] && bone_valid[e]) {
            draw->AddLine(ImVec2(bone_screen[s].x, bone_screen[s].y),
                ImVec2(bone_screen[e].x, bone_screen[e].y),
                IM_COL32(0, 0, 0, 255), 2.5f);
            draw->AddLine(ImVec2(bone_screen[s].x, bone_screen[s].y),
                ImVec2(bone_screen[e].x, bone_screen[e].y),
                color, 1.5f);
        }
    }
}

void DrawChinaHat(ImDrawList* drawList, uintptr_t Mesh, ImU32 hatColor) {
    const float height = 25.0f;
    const float radius = 30.0f;
    const float offset = 7.0f;
    const int segments = 80;
    Vector3 head_pos = GetBoneWithRotation(Mesh, bone::Head);
    if (head_pos.x == 0 && head_pos.y == 0 && head_pos.z == 0) return;
    head_pos.z += offset;
    std::vector<Vector3> base_points;
    base_points.reserve(segments);
    for (int i = 0; i < segments; ++i) {
        float angle = (2.0f * M_PI / segments) * i;
        float x = head_pos.x + cosf(angle) * radius;
        float y = head_pos.y + sinf(angle) * radius;
        base_points.push_back({ x, y, head_pos.z });
    }
    Vector3 apex_3d = head_pos;
    apex_3d.z += height;
    Vector2 apex_2d = ProjectWorldToScreen(apex_3d);
    if (apex_2d.x <= 0 || apex_2d.y <= 0 || apex_2d.x >= Width || apex_2d.y >= Height) return;
    bool any_visible = false;
    for (auto& pt : base_points) {
        Vector2 base_2d = ProjectWorldToScreen(pt);
        if (base_2d.x > 0 && base_2d.y > 0 && base_2d.x < Width && base_2d.y < Height) {
            drawList->AddLine(ImVec2(apex_2d.x, apex_2d.y), ImVec2(base_2d.x, base_2d.y), hatColor, 2.0f);
            any_visible = true;
        }
    }
    if (!any_visible) return;
    for (int i = 0; i < segments; ++i) {
        Vector2 a = ProjectWorldToScreen(base_points[i]);
        Vector2 b = ProjectWorldToScreen(base_points[(i + 1) % segments]);
        if ((a.x > 0 && a.y > 0 && a.x < Width && a.y < Height) &&
            (b.x > 0 && b.y > 0 && b.x < Width && b.y < Height)) {
            drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), hatColor, 1.8f);
        }
    }
}

void ActorLoop() {
    cache::Gworld = read<uint64_t>(virtualaddy + offsets::UWorld);
    if (!cache::Gworld) return;

    uintptr_t GameInstance = read<uint64_t>(cache::Gworld + offsets::OwningGameInstance);
    if (!GameInstance) return;

    cache::LocalPlayers = read<uint64_t>(read<uint64_t>(GameInstance + offsets::LocalPlayers));
    if (!cache::LocalPlayers) return;

    cache::PlayerController = read<uint64_t>(cache::LocalPlayers + offsets::PlayerController);
    if (!cache::PlayerController) return;

    cache::Player = read<uint64_t>(cache::PlayerController + offsets::AcknowledgedPawn);
    cache::RootComponent = read<uint64_t>(cache::Player + offsets::RootComponent);

    cache::closest_mesh = 0;

    uintptr_t GameState = read<uintptr_t>(cache::Gworld + offsets::GameState);
    if (!GameState) return;

    uintptr_t PlayerArrayOffset = offsets::PlayerArray;
    uintptr_t PlayerArray = read<uintptr_t>(GameState + PlayerArrayOffset);
    int Num = read<int>(GameState + (PlayerArrayOffset + sizeof(uintptr_t)));

    float bestDist = 999999.0f;
    float bestFovDist = 999999.0f;

    if (settings::aimbot::fov) {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Width / 2.0f, Height / 2.0f), settings::aimbot::fovsize, settings::aimbot::fovCollors, 60, 1.5f);
    }

    for (int i = 0; i < Num; i++) {
        uintptr_t PlayerState = read<uintptr_t>(PlayerArray + (i * sizeof(uintptr_t)));
        if (!PlayerState) continue;

        uintptr_t Player = read<uintptr_t>(PlayerState + offsets::PawnPrivate);
        if (!Player || Player == cache::Player) continue;

        uintptr_t Mesh = read<uintptr_t>(Player + offsets::Mesh);
        if (!Mesh) continue;

        Vector3 Head3D = GetBoneWithRotation(Mesh, 110);
        Vector2 Head2D = ProjectWorldToScreen({ Head3D.x, Head3D.y, Head3D.z + 20 });
        Vector3 Bottom3D = GetBoneWithRotation(Mesh, 0);
        Vector2 Bottom2D = ProjectWorldToScreen(Bottom3D);

        float Distance = vCamera.Location.Distance(Bottom3D) / 100.0f;
        float DistToCenter = sqrtf(powf(Head2D.x - Width / 2.0f, 2) + powf(Head2D.y - Height / 2.0f, 2));

        bool inFov = (!settings::aimbot::fov || DistToCenter < settings::aimbot::fovsize);
        if (inFov && Distance < bestDist) {
            bestDist = Distance;
            cache::closest_mesh = Mesh;
            bestFovDist = DistToCenter;
        }

        float BoxHeight = (float)(Head2D.y - Bottom2D.y);
        float CornerHeight = abs(Head2D.y - Bottom2D.y);
        float CornerWidth = BoxHeight * 0.60;

        ImU32 skel_color = settings::esp::skeletonColor;
        ImU32 box_color = settings::esp::boxColor;
        ImU32 Snapline_color = settings::esp::snipelineColor;
        ImU32 chinaHat_color = settings::esp::chinaHatColor;

        if (settings::misc::watermark) RenderWatermark();

        if (settings::misc::croshair) {
            float Time = ImGui::GetTime();
            int r = (int)(sin(Time * 2.0f) * 127.5f + 127.5f);
            int g = (int)(sin(Time * 2.0f + 2.0f) * 127.5f + 127.5f);
            int b = (int)(sin(Time * 2.0f + 4.0f) * 127.5f + 127.5f);
            ImColor Dynamic(r, g, b);
            float CrosshairSize = 10.0f;
            float Thickness = 1.5f;
            ImVec2 Center(Width / 2.0f, Height / 2.0f);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x - CrosshairSize, Center.y), ImVec2(Center.x + CrosshairSize, Center.y), Dynamic, Thickness);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x, Center.y - CrosshairSize), ImVec2(Center.x, Center.y + CrosshairSize), Dynamic, Thickness);
        }

        if (settings::misc::streamproof) {
            HWND hWnd = GetActiveWindow();
            SetWindowDisplayAffinity(hWnd, WDA_EXCLUDEFROMCAPTURE);
        }
        else {
            HWND hWnd = GetActiveWindow();
            SetWindowDisplayAffinity(hWnd, WDA_NONE);
        }

        if (settings::aimbot::magicbullet) {
            uintptr_t pawn_private = read<uintptr_t>(cache::LocalPlayers + offsets::AcknowledgedPawn + offsets::PawnPrivate);
            write<FVector>(pawn_private, FVector(20, 20, 20));
        }

        if (settings::misc::NoRecoil) {

            write<float>(cache::PlayerController + offsets::CustomTimeDilation, -1.0f);
        }

        if (settings::misc::fovchanger) {
            // SHIT DONT WORK AFTER THIS FUCKING UPDATE
            // SHIT DONT WORK AFTER THIS FUCKING UPDATE
            // SHIT DONT WORK AFTER THIS FUCKING UPDATE
            // SHIT DONT WORK AFTER THIS FUCKING UPDATE
            // SHIT DONT WORK AFTER THIS FUCKING UPDATE
            // SHIT DONT WORK AFTER THIS FUCKING UPDATE
            // SHIT DONT WORK AFTER THIS FUCKING UPDATE
            // SHIT DONT WORK AFTER THIS FUCKING UPDATE

            uintptr_t PlayerCameraManager = read<uintptr_t>(cache::PlayerController + offsets::PlayerCameraManager);
            write<float>(PlayerCameraManager + 0x2C4 + 0x4, settings::misc::fovchangersize);
            write(PlayerCameraManager + 0x380, settings::misc::fovchangersize);
        }

        if (settings::esp::enable) {
            if (settings::esp::radar) Draw_radar();
            if (settings::esp::Box) DrawCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y, CornerWidth, CornerHeight, box_color, 1.5);
            if (settings::esp::Snapline) ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Width / 2, Height), ImVec2(Bottom2D.x, Bottom2D.y), Snapline_color, 1);
            if (settings::esp::Distance) {
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "%d m", static_cast<int>(Distance));
                ImGui::GetBackgroundDrawList()->AddText(ImVec2(Bottom2D.x, Bottom2D.y), ImColor(255, 255, 255, 255), buffer);
            }
            if (settings::esp::Skelleton) DrawSkeleton(Mesh, skel_color);
            if (settings::esp::ChinaHat) DrawChinaHat(ImGui::GetBackgroundDrawList(), Mesh, chinaHat_color);

            if (settings::esp::PlayerName) {
                std::string playerName = GetPlayerName(PlayerState);
                if (playerName.empty() || playerName == "[ BOT/AI ]") playerName = "[ BOT/AI ]";
                char nameBuffer[128];
                snprintf(nameBuffer, sizeof(nameBuffer), "%s", playerName.c_str());
                ImVec2 textPos = ImVec2(Head2D.x, Head2D.y - 10.0f);
                ImGui::GetBackgroundDrawList()->AddText(ImVec2(textPos.x + 1, textPos.y + 1), IM_COL32(0, 0, 0, 255), nameBuffer);
                ImGui::GetBackgroundDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), nameBuffer);
            }

            if (settings::esp::Rank) {
                int32_t rankValue = Rank(PlayerState);
                std::string rankStr = realRank(rankValue);
                ImVec2 rank_size = ImGui::CalcTextSize(rankStr.c_str());
                ImVec2 rank_pos = ImVec2(Head2D.x - (rank_size.x / 2.0f), Head2D.y - 25.0f - rank_size.y);
                ImGui::GetBackgroundDrawList()->AddText(ImVec2(rank_pos.x + 1.0f, rank_pos.y + 1.0f), IM_COL32(0, 0, 0, 255), rankStr.c_str());
                ImGui::GetBackgroundDrawList()->AddText(rank_pos, GetRankColor(rankValue), rankStr.c_str());
            }

            if (settings::esp::Platforms) {
                std::string platformStr = GetPlatformText(PlayerState);
                if (platformStr.empty()) platformStr = "Unknown";
                ImVec2 platform_size = ImGui::CalcTextSize(platformStr.c_str());
                ImVec2 platform_pos = ImVec2(Bottom2D.x - (platform_size.x / 2.0f), Bottom2D.y + 8.0f);
                ImGui::GetBackgroundDrawList()->AddText(ImVec2(platform_pos.x + 1.0f, platform_pos.y + 1.0f), IM_COL32(0, 0, 0, 255), platformStr.c_str());
                ImGui::GetBackgroundDrawList()->AddText(platform_pos, PlatColor(platformStr), platformStr.c_str());
            }
        }
    }


    if (settings::misc::vehiculefly) {
        uint64_t Vehicle = read<uint64_t>(cache::Player + offsets::current_vehicle);
        Vector3 vel = read<Vector3>(Vehicle + offsets::Velocity);
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)   vel.z = 1200.f;
        else if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) vel.z = -1200.f;
        else vel.z = 0.f;
        write<Vector3>(Vehicle + offsets::Velocity, vel);

    }

    if (settings::misc::RapidFire) {
        //uint64_t weapon = read<uint64_t>(offsets::AcknowledgedPawn + offsets::current_weapon);
        uint64_t weapon = read<uint64_t>(cache::Player + offsets::current_weapon);

        write<float>(weapon + offsets::CustomTimeDilation, 0.1f);
    }


    if (settings::misc::bannguys) {
        //static uint64_t banned_flag_addr = 0;

        //// Trouve l'adresse une seule fois
        //if (!banned_flag_addr) {
        //    for (auto obj : GetAllObjects()) { // ← ta fonction existante
        //        if (obj && FNameToString(obj->Class->Name) == "FortSocialUserInfoViewModel") {
        //            banned_flag_addr = (uint64_t)obj + 0xAB;
        //            break;
        //        }
        //    }
        //}

        //// Écris directement → ÉCRAN DE BAN
        //if (banned_flag_addr && IsValidPtr(banned_flag_addr)) {
        //    write<bool>(banned_flag_addr, true);
        //}
    }

    if (settings::aimbot::aimbot && (GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
        memory(cache::closest_mesh);
    }
}