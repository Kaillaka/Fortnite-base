#pragma once

#include <imgui.h>

namespace settings {
    namespace esp {
        extern ImColor VisibleColor;
        extern ImColor NVisibleColor;
        extern ImColor skeletonColor;
        extern ImColor boxColor;
        extern ImColor snipelineColor;
        extern ImColor chinaHatColor;

        extern bool enable;

        extern bool Box;
        extern bool Snapline;
        extern bool Skelleton;
        extern bool PlayerName;
        extern bool ChinaHat;
        extern bool Distance;
        extern bool Platforms;
        extern bool radar;
        extern bool Rank;
        extern bool Menu;
        extern bool VSync;
    }

    namespace aimbot {
        extern ImColor fovCollors;

        extern bool magicbullet;
        extern bool aimbot;
        extern bool mouse;
        extern int smoothness;
        extern int Hitbox;
        extern bool prediction;
        extern bool vischeck;
        extern int aimkey;
        extern bool fov;
        extern int fovsize;
    }
    namespace misc {
        extern bool watermark;
        extern bool streamproof;
        extern bool croshair;
        extern bool NoRecoil;
        extern bool RapidFire;
        extern bool vehiculefly;
        extern bool bannguys;

        //extern bool RapidFire;



        extern bool fovchanger;
        extern int fovchangersize;
    }

}