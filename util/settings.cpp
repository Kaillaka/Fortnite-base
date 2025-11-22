#include "settings.h"
#include <Windows.h>  

namespace settings {
    namespace esp {
        ImColor VisibleColor = ImColor(255, 255, 255, 255);
        ImColor NVisibleColor = ImColor(255, 0, 0, 255);
        ImColor skeletonColor = ImColor(255, 255, 255, 255);
        ImColor boxColor = ImColor(255, 255, 255, 255);
        ImColor snipelineColor = ImColor(255, 255, 255, 255);
        ImColor chinaHatColor = ImColor(255, 255, 255, 255);

        bool enable = false;

        bool Box = true;
        bool Snapline = false;
        bool Distance = false;
        bool Skelleton = false;
        bool PlayerName = false;
        bool ChinaHat = false;
        bool Rank = false;
        bool Platforms = false;
        bool radar = false;

        bool Menu = false;
        bool VSync = true;
    }

    namespace aimbot {
        ImColor fovCollors = ImColor(255, 255, 255, 255);

        bool magicbullet = false;

        bool aimbot = true;
        bool mouse = true;
        int smoothness = 95;
        int Hitbox = 0;
        bool prediction = true;
        bool vischeck = false;
        int aimkey = VK_RBUTTON;
        bool fov = false;
        int fovsize = 150;
    }
    namespace misc {
        bool watermark = true;
        bool streamproof = false;
        bool croshair = false;
        bool fovchanger = false;
        bool NoRecoil = false;
        bool RapidFire = false;
        bool vehiculefly = false;
        bool bannguys = false;
        int fovchangersize = 80;
    }

}