

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <vector>
#include <string>
#include <d3d11.h>
#include <d3dx11tex.h>
#include <functional>
#include <animations.hpp>
#include "managers/FontManager.hpp"
#include "managers/ImageManager.hpp"
#include "compbuilder/CompBuilder.hpp"
#include "managers/WidgetsManager.hpp"
#include "managers/StyleManager.hpp"
#include "fonts.h"
#include "unicodes.hpp"
#include "managers/TabsManager.hpp"
#include "managers/ChildManager.hpp"
#include "managers/PopupManager.hpp"
#include "managers/LangManager.hpp"
#include "managers/SearchManager.hpp"
#include "../../util/settings.h"

#include <gui.hpp>

#pragma comment(lib, "freetype64.lib")

using namespace ImGui;

#include "managers/NotifyManager.hpp"

void GUI::initialize(ID3D11Device* device) {
	size = ImVec2{ 750, 500 };

	GImGui->IO.IniFilename = "";

	StyleManager::get().Styles();
	StyleManager::get().Colors();
	fonts.resize(fonts_size);

	fonts.at(font).setup(b_font, sizeof(b_font),
		{ 14, 12 },
		GetIO().Fonts->GetGlyphRangesCyrillic());

	const static ImWchar icons_ranges[] = { 0x1 + 16000, 0x1 + 17170, 0 };
	fonts.at(icons).setup(glyphter, sizeof(glyphter),
		{ 14, 12 },
		icons_ranges);

	LangManager::get().initialize();

	ChildManager& cm = ChildManager::get();
	auto& widgets = WidgetsManager::get();

	static bool bools[100];
	static int ints[100];
	static float floats[100];
	static bool multicombo_values[5];
	static float col[100][4];
	static char buf[16];

	TabsManager::get().add_page(0, [&]() {
		BeginGroup();
		{
			cm.beginchild("General");
			{
				widgets.Checkbox("Enable", &settings::aimbot::aimbot);

				if (settings::aimbot::aimbot) {
					widgets.SliderInt("Smoothness", &settings::aimbot::smoothness, 50, 95);
					widgets.Checkbox("Magic Bullet", &settings::aimbot::magicbullet);
					widgets.Checkbox("Trigger Bot", &settings::aimbot::magicbullet);

				}
			}
			cm.endchild();

		}
		EndGroup();

		SameLine();

		BeginGroup();
		{
			cm.beginchild("Miscellaneous");
			{
				widgets.Checkbox("Draw FOV", &settings::aimbot::fov);
				widgets.Checkbox("Draw Crosshair", &settings::misc::croshair);
				widgets.SliderInt("Fov Size", &settings::aimbot::fovsize, 20, 360);
				widgets.ColorEdit(("Fov Colors"), reinterpret_cast<float*>(&settings::aimbot::fovCollors));

			}
			cm.endchild();
		}
		EndGroup();
		});

	TabsManager::get().add_page(1, [&]() {
		BeginGroup();
		{
			cm.beginchild("General");
			{
				widgets.Checkbox("Enable", &settings::esp::enable);

				widgets.Checkbox("Box", &settings::esp::Box);
				widgets.Checkbox("Skeleton", &settings::esp::Skelleton);
				widgets.Checkbox("Name", &settings::esp::PlayerName);
				widgets.Checkbox("Rank", &settings::esp::Rank);
				widgets.Checkbox("Platform", &settings::esp::Platforms);
				widgets.Checkbox("Radar", &settings::esp::radar);

				widgets.Checkbox("ChinaHat", &settings::esp::ChinaHat);
				widgets.Checkbox("Snapline", &settings::esp::Snapline);
				widgets.Checkbox("Distance", &settings::esp::Distance);
				widgets.Checkbox("VSyn", &settings::esp::VSync);

			}
			cm.endchild();

		}
		EndGroup();

		SameLine();

		BeginGroup();
		{
			cm.beginchild("Miscellaneous");
			{
				widgets.ColorEdit(("Skeleton Colors"), reinterpret_cast<float*>(&settings::esp::skeletonColor));
				widgets.ColorEdit(("Box Colors"), reinterpret_cast<float*>(&settings::esp::boxColor));
				widgets.ColorEdit(("Snapline Colors"), reinterpret_cast<float*>(&settings::esp::snipelineColor));
				widgets.ColorEdit(("ChinaHat Colors"), reinterpret_cast<float*>(&settings::esp::chinaHatColor));

				widgets.ColorEdit(("Visible Colors"), reinterpret_cast<float*>(&settings::esp::VisibleColor));
				widgets.ColorEdit(("NVisible Colors"), reinterpret_cast<float*>(&settings::esp::NVisibleColor));

			}
			cm.endchild();
		}
		EndGroup();
		});

	TabsManager::get().add_page(2, [&]() {
		BeginGroup();
		{
			cm.beginchild("Miscellaneous");
			{

				widgets.Checkbox("WaterMark", &settings::misc::watermark);

				widgets.Checkbox("StreamProof", &settings::misc::streamproof);

				widgets.Checkbox("No Recoil", &settings::misc::NoRecoil);
				widgets.Checkbox("Rapid Fire", &settings::misc::RapidFire);
				widgets.Checkbox("Vehicule Fly", &settings::misc::vehiculefly);

				//widgets.Checkbox("Fov Changer", &settings::misc::fovchanger);

				//if (settings::misc::fovchanger) {
				//	widgets.SliderInt("Fov Size", &settings::misc::fovchangersize, 80, 360);
				//}


			}
			cm.endchild();

		}
		EndGroup();

		SameLine();

		BeginGroup();
		{
			cm.beginchild("Credits Kaillaka Base");
			{
				Text("Made by Kaillaka (@hypedbykai)");

				Text("Last update 09/11/2025");

			}
			cm.endchild();
		}
		EndGroup();
		});

	TabsManager::get().add_page(3, [&]() {
		static int cfg = 0;

		BeginGroup();
		{
			PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			BeginChild("configs", { GetWindowWidth() / 2 - 14 - GImGui->Style.ItemSpacing.x / 2, 0 }, ImGuiChildFlags_Border);
			{
				SetCursorPos({ 14, 14 });
				static char buf[16];
				PushItemFlag(ImGuiItemFlags_NoNav, true);
				widgets.TextField("##name", buf, sizeof(buf), { GetWindowWidth() - 28 - GetFrameHeight() - 10, GetFrameHeight() }, "Enter the name...", i_tag_01);
				PopItemFlag();
				SameLine(0, 10);
				if (CompBuilder::get().Button("add", { GetFrameHeight(), GetFrameHeight() }, [&](CompBuilder::ButtonEnv env) {
					auto col = col_anim(col_anim(GetColorU32(ImGuiCol_Button), GetColorU32(ImGuiCol_ButtonHovered), env.anim.hover), GetColorU32(ImGuiCol_ButtonActive), env.anim.held);
					GetWindowDrawList()->AddRectFilled(env.bb.Min, env.bb.Max, col, 2);

					GetWindowDrawList()->AddText(fonts[icons].get(14), 14, env.bb.GetCenter() - ImVec2{ 7, 7 }, GetColorU32(ImGuiCol_TextButton), i_plus);
					})) {

					if (strlen(buf) == 0) {
						NotifyManager::get().add("Enter the name!", NotifyManager::notify_error);
					}
					else {
						NotifyManager::get().add("Successfully created a config", NotifyManager::notify_success);
					}
				}

				Separator();
				SetCursorPosY(GetCursorPosY() - GImGui->Style.ItemSpacing.y);

				PushStyleVar(ImGuiStyleVar_WindowPadding, { 14, 14 });
				BeginChild("list", { 0, 0 }, ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
				{
					cm.smoothscroll();

					for (int i = 0; i < 10; ++i) {
						char label[64];
						ImFormatString(label, sizeof(label), "Config #%d", i);

						ImRect bb{ GetCurrentWindow()->DC.CursorPos, GetCurrentWindow()->DC.CursorPos + ImVec2{ GetWindowWidth() - GImGui->Style.WindowPadding.x * 2, GetFrameHeight() } };
						if (CompBuilder::get().Selectable(label, cfg == i, bb, [&](const CompBuilder::SelectableEnv& env) {
							ImColor col = col_anim(col_anim(GetColorU32(ImGuiCol_TextDisabled), GetColorU32(ImGuiCol_TextHovered), env.anim.hover), GetColorU32(ImGuiCol_Text), env.anim.selected);
							ImColor icon_col = col_anim(col_anim(GetColorU32(ImGuiCol_TextDisabled), GetColorU32(ImGuiCol_TextHovered), env.anim.hover), GetColorU32(ImGuiCol_Scheme), env.anim.selected);

							GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, env.anim.selected), GImGui->Style.FrameRounding);

							GetWindowDrawList()->AddText(fonts[icons].get(14), 14, bb.Min + GImGui->Style.FramePadding, icon_col, i_file_04);
							GetWindowDrawList()->AddText(bb.Min + GImGui->Style.FramePadding + ImVec2{ 24, 0 }, col, label);
							})) {
							cfg = i;
						}
					}
				}
				EndChild();
				PopStyleVar();
			}
			EndChild();
			PopStyleVar();
		}
		EndGroup();

		SameLine();

		BeginGroup();
		{
			cm.beginchild("CONFIG");
			{
				char label[64];
				ImFormatString(label, sizeof(label), "Config #%d", cfg);

				TextDisabled(LangManager::get().translate("Name:"));
				SameLine(GetWindowWidth() - CalcTextSize(label).x - GImGui->Style.WindowPadding.x);
				Text(label);

				TextDisabled(LangManager::get().translate("Created:"));
				SameLine(GetWindowWidth() - CalcTextSize("22.03.2025").x - GImGui->Style.WindowPadding.x);
				Text("22.03.2025");

				TextDisabled(LangManager::get().translate("Created by:"));
				SameLine(GetWindowWidth() - CalcTextSize("Michael Conors").x - GImGui->Style.WindowPadding.x);
				Text("Michael Conors");

				widgets.Separator();

				if (widgets.Button("Load")) {
					NotifyManager::get().add("Loaded the config", NotifyManager::notify_info);
				}

				ImColor red;
				float h, s, v;
				ColorConvertRGBtoHSV(GetStyleColorVec4(ImGuiCol_Scheme).x, GetStyleColorVec4(ImGuiCol_Scheme).y, GetStyleColorVec4(ImGuiCol_Scheme).z, h, s, v);
				ColorConvertHSVtoRGB(0.f, s, v, red.Value.x, red.Value.y, red.Value.z);
				red.Value.w = GImGui->Style.Alpha;

				PushStyleColor(ImGuiCol_Button, red.Value);
				PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ red.Value.x / 1.15f, red.Value.y / 1.15f, red.Value.z / 1.15f, GImGui->Style.Alpha });
				PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ red.Value.x / 1.35f, red.Value.y / 1.35f, red.Value.z / 1.35f, GImGui->Style.Alpha });
				if (widgets.Button("Delete")) {
					NotifyManager::get().add("Removed the config", NotifyManager::notify_info);
				}
				PopStyleColor(3);
			}
			cm.endchild();
		}
		EndGroup();
		});
}

void GUI::draw() {
	static bool bools[100];
	static int ints[100];
	static bool multicombo_values[4];
	static float col[100][4];
	static char buf[16];

	SetNextWindowSize(size);
	Begin("GUI", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
	{
		BeginChild("navbar", { 180, 0 }, 0, ImGuiWindowFlags_NoBackground);
		{
			SetCursorPos({ 14, 14 });
			PushStyleColor(ImGuiCol_FrameBg, GetColorU32(ImGuiCol_FrameBg, 0.f));
			PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
			PushItemFlag(ImGuiItemFlags_NoNav, true);
			Text("Kaillaka Base");

			PopItemFlag();
			PopStyleVar();
			PopStyleColor();
			WidgetsManager::get().Separator();
			SetCursorPosX(14);
			TabsManager::get().render_tabs(8);

			GetWindowDrawList()->AddRectFilled(GetWindowPos() + ImVec2{ GetWindowWidth() - 1, 0 }, GetWindowPos() + GetWindowSize(), GetColorU32(ImGuiCol_Border));

			SetCursorPos({ 14, GetWindowHeight() - 38 });

		}
		EndChild();

		SameLine(0, 0);

		if (strlen(SearchManager::get().search_buf) > 0 || SearchManager::get().get_anim() > 0.05f) PushStyleVar(ImGuiStyleVar_WindowPadding, { 14, 14 });
		BeginChild("main", { 0, 0 }, ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			if (strlen(SearchManager::get().search_buf) > 0 || SearchManager::get().get_anim() > 0.05f) PopStyleVar();

			ChildManager::get().smoothscroll();

			if (strlen(SearchManager::get().search_buf) == 0 && SearchManager::get().get_anim() < 0.05f) {
				TabsManager::get().render_subtabs(20);

				auto window = GetCurrentWindow();

				PushStyleVar(ImGuiStyleVar_Alpha, GImGui->Style.Alpha * TabsManager::get().get_anim() * GImGui->Style.Alpha);
				PushStyleVar(ImGuiStyleVar_WindowPadding, { 14, 14 });
				BeginChild("content", { 0, 0 }, ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
				{
					TabsManager::get().draw_page(window);
				}
				EndChild();
				PopStyleVar(2);
			}
			else {
				SearchManager::get().draw();
			}
		}
		EndChild();

		PopupManager::get().handle();
	}
	End();

	NotifyManager::get().draw();
}