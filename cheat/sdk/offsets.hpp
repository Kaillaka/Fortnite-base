#pragma once
#include <cstdint>
#include <unordered_map>
#include <fstream>

namespace offsets {
    uintptr_t UWorld = 0x175448b8;                 

    uintptr_t GNames = 0x17136200;
    uintptr_t LocalPlayers = 0x38;
    uintptr_t PlayerController = 0x30;
    uintptr_t PlayerCameraManager = 0x360;
    uintptr_t AcknowledgedPawn = 0x358;
    uintptr_t PlayerState = 0x2d0;
    uintptr_t TeamIndex = 0x11a9;
    uintptr_t b_is_dying = 0x728;
    uintptr_t B_IS_DBNO = 0x841;                    
    uintptr_t Platform = 0x440;
    uintptr_t TargetedFortPawn = 0x1820;            
    uintptr_t KillScore = 0x11A8;
    uintptr_t PlayerName = 0xA00;
    uintptr_t PlayerNamePrivate = 0xA00;
    uintptr_t ranked_progress = 0xD8;
    uintptr_t OwningGameInstance = 0x248;
    uintptr_t GameState = 0x1D0;                    
    uintptr_t PlayerArray = 0x2c8;
    uintptr_t RootComponent = 0x1b0;                
    uintptr_t PawnPrivate = 0x328;
    uintptr_t AActor = 0xA0;
    uintptr_t PersistentLevel = 0x38;               

    uintptr_t PlayerAim = 0x2BC0;



    uintptr_t Mesh = 0x330;                         
    uintptr_t BoneArray = 0x5E8;
    uintptr_t bonecache = 0x5F8;
    uintptr_t bonestide = 0x60;
    uintptr_t ComponentToWorld = 0x1e0;
    uintptr_t RelativeLocation = 0x140;             
    uintptr_t RelativeRotation = 0x158;
    uintptr_t AdditionalAimOffset = 0x2BC0;
    uintptr_t CameraFOV = 0x3B4;                    
    uintptr_t CameraLocation = 0x178;              
    uintptr_t CameraRotation = 0x188;              
    uintptr_t current_vehicle = 0x2C18;             
    uintptr_t current_weapon = 0x990;               
    uintptr_t weapon_data = 0x598;                 
    uintptr_t AmmoCount = 0x1174;
    uintptr_t ItemName = 0x40;                      
    uintptr_t projectile_speed = 0x24F4;
    uintptr_t projectile_gravity = 0x24F8;
    uintptr_t ComponentVelocity = 0x188;             
    uintptr_t PrimaryPickupItemEntry = 0x3a8;        
    uintptr_t ItemDefinition = 0x10;
    uintptr_t ItemRarity = 0xAA;
    uintptr_t Tier = 0xA2;
    uintptr_t Rarity = 0xaa;                        
    uintptr_t bAlreadySearched = 0xD52;
    uintptr_t HabaneroComponent = 0x940;            
    uintptr_t Levels = 0x1e8;
    uintptr_t CustomTimeDilation = 0x68;
    uintptr_t LastRenderTime = 0x32c;              
    uintptr_t LastRenderTimeOnScreen = 0x328;
    uintptr_t PlayerInput = 0x428;
    uintptr_t ServerTime = 0x2e8;                    
    uintptr_t ViewLocations = 0x180;
    uintptr_t ViewPort = 0x78;
    uintptr_t Velocity = 0x188;                    
    uintptr_t ProjectileGravityScale = 0x21d8;
    uintptr_t ProjectileSpeed = 0x2488;
    uintptr_t PlayerBannedScreen = 0x350;

    namespace UOBJECT {
        constexpr uintptr_t NamePrivate = 0x18;
        constexpr uintptr_t ClassPrivate = 0x10;
        constexpr uintptr_t OuterPrivate = 0x20;
    }
};


enum bone : int
{
    // Top Bones
    Head = 110,
    Neck = 67, // SixSeven
    Chest = 66,

    // Unknown Region / Think its BackPack?
    BackPackUpper = 70,
    BackPackLower = 69,

    // Right Arm
    RightShoulder = 9,
    RightShoulderLower = 35,
    RightElBowUpper = 36,
    RightElBow = 10,
    RightArm = 34,
    RightArmLower = 33,

    // Right Fingers

    // Left Arm
    LeftShoulder = 38,
    LeftShoulderLower = 64,
    LeftElBowUpper = 65,
    LeftElBow = 39,
    LeftArm = 63,
    LeftArmLower = 62,

    // spine
    SpineEighth = 37, // top Left
    SpineSixth = 8, // top Right
    SpineFifth = 7,
    SpineFourth = 6,
    SpineThird = 5,
    SpineSecond = 4,
    SpineFirst = 3,

    HipMid = 2,

    // right foot
    RightHip = 71,
    RightHipLower = 77,
    RightKnee = 72,
    RightUpperCalf = 74,
    RightCalf = 73,
    RightHeel = 75,
    RightToe = 76,

    // left foot
    LeftHip = 78,
    LeftHipLower = 84,
    LeftKnee = 79,
    LeftUpperCalf = 81,
    leftCalf = 80,
    LeftHeel = 82,
    LeftToe = 83

};

