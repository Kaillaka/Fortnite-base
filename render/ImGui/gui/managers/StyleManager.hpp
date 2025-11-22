#pragma once

class StyleManager {
public:
	void Styles( ) {
		auto& style = GImGui->Style;

		style.WindowRounding = 4;
        style.WindowPadding = ImVec2{ 0, 0 };
        style.WindowBorderSize = 0;

        style.FrameRounding = 3;
        style.FramePadding = ImVec2{ 14, 12 };
        style.FrameBorderSize = 0;

        style.PopupRounding = 3;
        style.PopupBorderSize = 0;

        style.ChildRounding = 4;
        style.ChildBorderSize = 1;

        style.ItemSpacing = ImVec2{ 14, 14 };
        style.ItemInnerSpacing = ImVec2{ 10, 10 };

        style.ScrollbarRounding = 4;
        style.ScrollbarSize = 4;
        style.WindowMinSize = ImVec2{ 1, 1 };
	}

	void Colors( ) {
		ImVec4* colors = GImGui->Style.Colors;

        colors[ImGuiCol_Scheme]                 = ImColor( 142, 132, 255 );

        colors[ImGuiCol_Text]                   = ImColor( 215, 211, 255 );
        colors[ImGuiCol_TextHovered]            = ImColor( 69, 69, 81, int( 0.6f * 255 ) );
        colors[ImGuiCol_TextDisabled]           = ImColor( 69, 69, 81 );
        colors[ImGuiCol_TextButton]             = ImColor( 19, 20, 26 );

        colors[ImGuiCol_WindowBg]               = ImColor( 12, 13, 18 );
        colors[ImGuiCol_ChildBg]                = ImColor( 19, 20, 26 );
        colors[ImGuiCol_FrameBg]                = ImColor( 25, 26, 34 );
        colors[ImGuiCol_FrameBgHovered]         = ImColor( 31, 32, 42 );
        colors[ImGuiCol_FrameBgActive]          = ImColor( 36, 37, 48 );
        colors[ImGuiCol_PopupBg]                = colors[ImGuiCol_FrameBg];
        colors[ImGuiCol_Border]                 = ImColor( 212, 202, 255, 10 );
        colors[ImGuiCol_CheckMark]              = ImColor( 255, 255, 255 );
        colors[ImGuiCol_SliderGrab]             = ImColor( 255, 255, 255 );
        colors[ImGuiCol_TextSelectedBg]         = ImColor( colors[ImGuiCol_Scheme].x, colors[ImGuiCol_Scheme].y, colors[ImGuiCol_Scheme].z, 0.07f );
        colors[ImGuiCol_Tab]                    = ImColor( 17, 17, 23 );

        colors[ImGuiCol_Button]                 = colors[ImGuiCol_Scheme];
        colors[ImGuiCol_ButtonHovered]          = ImColor{ colors[ImGuiCol_Scheme].x / 1.15f, colors[ImGuiCol_Scheme].y / 1.15f, colors[ImGuiCol_Scheme].z / 1.15f, 1.f };
        colors[ImGuiCol_ButtonActive]           = ImColor{ colors[ImGuiCol_Scheme].x / 1.35f, colors[ImGuiCol_Scheme].y / 1.35f, colors[ImGuiCol_Scheme].z / 1.35f, 1.f };

        colors[ImGuiCol_ScrollbarGrab]          = colors[ImGuiCol_Scheme];
        colors[ImGuiCol_ScrollbarGrabHovered]   = colors[ImGuiCol_Scheme];
        colors[ImGuiCol_ScrollbarGrabActive]    = colors[ImGuiCol_Scheme];
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.0f);
        colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
	}

	static StyleManager& get( ) {
		static StyleManager s{ };
		return s;
	}
};