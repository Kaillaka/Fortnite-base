#pragma once
#include "render.h"
#include <Windows.h>
#include <iostream>
#include "../util/util.h"
#include "../cheat/esp/esp.h"
#include <d3d11.h>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <gui.hpp>

struct resources {
	HWND hwnd{ nullptr };
	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* device_context{ nullptr };
	ID3D11RenderTargetView* render_target_view{ nullptr };
	IDXGISwapChain* swap_chain{ nullptr };
} static inline resources_t;



static inline bool setup_d3d() {
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
	swap_chain_desc.BufferCount = 2;
	swap_chain_desc.BufferDesc.Width = 0;
	swap_chain_desc.BufferDesc.Height = 0;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = resources_t.hwnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	D3D_FEATURE_LEVEL selected_feature_level;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, feature_levels, _countof(feature_levels), D3D11_SDK_VERSION, &swap_chain_desc, &resources_t.swap_chain, &resources_t.device, &selected_feature_level, &resources_t.device_context)))return false;

	ID3D11Texture2D* back_buffer = nullptr;
	if (FAILED(resources_t.swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer))) || !back_buffer) {
		return false;
	}

	if (FAILED(resources_t.device->CreateRenderTargetView(back_buffer, nullptr, &resources_t.render_target_view))) {
		return false;
	}

	back_buffer->Release();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowTitleAlign.x = 0.50f;
	style->WindowRounding = 0;

	ImGui_ImplWin32_Init(resources_t.hwnd);
	ImGui_ImplDX11_Init(resources_t.device, resources_t.device_context);
	resources_t.device->Release();
	return true;
}

void CreateOverlay() {
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0, 0, nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		"Overlay",
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RegisterClassExA(&wcex);

	HWND target = FindWindowA("Chrome_WidgetWin_1", "Discord Overlay");
	RECT rect;
	if (target)
		GetWindowRect(target, &rect);
	else
		GetWindowRect(GetDesktopWindow(), &rect); // fallback plein écran

	resources_t.hwnd = CreateWindowExA(
		WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
		"Overlay", "Overlay",
		WS_POPUP,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, wcex.hInstance, nullptr
	);

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(resources_t.hwnd, &margin);
	SetLayeredWindowAttributes(resources_t.hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	ShowWindow(resources_t.hwnd, SW_SHOW);
	SetWindowPos(resources_t.hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	UpdateWindow(resources_t.hwnd);
}


void Menu() {
	//const DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

	//ImGui::SetNextWindowSize({ 620.f,350.f });
	//ImGui::Begin("Base", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);


	//ImGui::Text("Aimbot");

	//ImGui::Checkbox("Aimbot", &settings::aimbot::aimbot);
	//if (settings::aimbot::aimbot)
	//{
	//	ImGui::SliderInt("Smoothness", &settings::aimbot::smoothness, 50, 95);
	//}

	//ImGui::Text("ESP");
	//ImGui::Text("Visible Check");
	//ImGui::ColorEdit4(("Visible"), reinterpret_cast<float*>(&settings::esp::VisibleColor), picker_flags);
	//ImGui::ColorEdit4(("NVisible"), reinterpret_cast<float*>(&settings::esp::NVisibleColor), picker_flags);

	//ImGui::Checkbox("Box", &settings::esp::Box);
	//ImGui::Checkbox("Snapline", &settings::esp::Snapline);
	//ImGui::Checkbox("Distance", &settings::esp::Distance);
	//ImGui::Checkbox("VSyn", &settings::esp::VSync);
	//ImGui::End();
}
static inline void loop() {
	constexpr float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	MSG msg{};

	while (msg.message != WM_QUIT) {
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;

		if (GetAsyncKeyState(VK_INSERT) & 1) {
			settings::esp::Menu = !settings::esp::Menu;
		}

		if (settings::esp::Menu) {
			POINT cursor_pos;
			GetCursorPos(&cursor_pos);
			io.MousePos = ImVec2(static_cast<float>(cursor_pos.x), static_cast<float>(cursor_pos.y));
			io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame(); {
			ActorLoop();

			if (settings::esp::Menu) {
				//Menu();
				GUI::get().draw();

			}
		}
		ImGui::Render();

		resources_t.device_context->OMSetRenderTargets(1, &resources_t.render_target_view, nullptr);
		resources_t.device_context->ClearRenderTargetView(resources_t.render_target_view, clear_color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		resources_t.swap_chain->Present(settings::esp::VSync, 0);
	}


	Sleep(150);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

static inline void setup() {
	CreateOverlay();
	setup_d3d();
	GUI::get().initialize(resources_t.device);
	loop();
}
