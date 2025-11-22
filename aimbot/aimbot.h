#pragma once
#include "../util/util.h"
#include "../util/settings.h"
#include "../cheat/sdk/sdk.h"


Vector3 PredictLocation(Vector3 target, Vector3 targetVelocity, float projectileSpeed, float projectileGravityScale, float distance)
{
    float horizontalTime = distance / projectileSpeed;
    float verticalTime = distance / projectileSpeed;

    target.x += targetVelocity.x * horizontalTime;
    target.y += targetVelocity.y * horizontalTime;
    target.z += targetVelocity.z * verticalTime +
        abs(-980 * projectileGravityScale) * 0.5f * (verticalTime * verticalTime);

    return target;
}

bool memory_event(Vector3 newpos)
{
    write<Vector3>(cache::PlayerController + offsets::PlayerAim, newpos);
    return true;
}

//void memoryaim(uintptr_t targetmeshs, Vector2 target2d)
//{
//    Vector2 target{};
//    float speedPercent = settings::aimbot::aimbot;
//    float smoothness = 100.0f - (speedPercent - 1.0f);
//    if (smoothness < 1.0f) smoothness = 1.0f;
//
//    float screen_center_x = static_cast<float>(Width) / 2;
//    float screen_center_y = static_cast<float>(Height) / 2;
//
//    if (target2d.x > screen_center_x) {
//        target.x = -(screen_center_x - target2d.x) / smoothness;
//        if (target.x + screen_center_x > screen_center_x * 2)
//            target.x = 0;
//    }
//    else if (target2d.x < screen_center_x) {
//        target.x = (target2d.x - screen_center_x) / smoothness;
//        if (target.x + screen_center_x < 0)
//            target.x = 0;
//    }
//
//    if (target2d.y > screen_center_y) {
//        target.y = -(screen_center_y - target2d.y) / smoothness;
//        if (target.y + screen_center_y > screen_center_y * 2)
//            target.y = 0;
//    }
//    else if (target2d.y < screen_center_y) {
//        target.y = (target2d.y - screen_center_y) / smoothness;
//        if (target.y + screen_center_y < 0)
//            target.y = 0;
//    }
//
//    float scale = 5.0f;
//    memory_event(Vector3(-target.y / scale, target.x / scale, 0));
//}

void memory(uintptr_t target_mesh)
{
    Vector3 head3d = GetBoneWithRotation(target_mesh, 110);
    Vector2 head2d = ProjectWorldToScreen(head3d);
    Vector2 target{};
    float screen_center_x = Width / 2.0f;
    float screen_center_y = Height / 2.0f;

    if (head2d.x != 0)
    {
        if (head2d.x > screen_center_x)
        {
            target.x = -(screen_center_x - head2d.x);
            target.x /= 20;
            if (target.x + screen_center_x > Width * 2) target.x = 0;
        }
        if (head2d.x < Width)
        {
            target.x = head2d.x - screen_center_x;
            target.x /= 20;
            if (target.x + screen_center_x < 0) target.x = 0;
        }
    }
    if (head2d.y != 0)
    {
        if (head2d.y > screen_center_y)
        {
            target.y = -(screen_center_y - head2d.y);
            target.y /= 20;
            if (target.y + screen_center_y > screen_center_y * 2) target.y = 0;
        }
        if (head2d.y < screen_center_y)
        {
            target.y = head2d.y - screen_center_y;
            target.y /= 20;
            if (target.y + screen_center_y < 0) target.y = 0;
        }
    }
    memory_event(Vector3(-target.y / 5, target.x / 5, 0));
}
