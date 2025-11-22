#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>
#include <imgui_internal.h>
#include <functional>
#include <vector>
#include <string>
#include <compbuilder/CompBuilder.hpp>
#include <animations.hpp>
#include <unicodes.hpp>
#include "ChildManager.hpp"
#include "FontManager.hpp"
#include "LangManager.hpp"

using namespace ImGui;

void ChildManager::beginchild( const char* label, ImVec2 size ) {
	current = label;

	auto window = GetCurrentWindow( );

	PushStyleVar( ImGuiStyleVar_WindowPadding, { 0, 0 } );
	Begin( label, 0, ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_NoDecoration );
	PopStyleVar( );
	SetWindowSize( CalcItemSize( size, window->Size.x / 2 - GImGui->Style.WindowPadding.x - GImGui->Style.ItemSpacing.x / 2, GetCurrentWindow( )->ContentSize.y ) );

	window = GetCurrentWindow( );

	window->DrawList->AddText( GetWindowPos( ) + ImVec2{ 14, 14 }, GetColorU32( ImGuiCol_TextDisabled ), LangManager::get( ).translate( label ), FindRenderedTextEnd( LangManager::get( ).translate( label ) ) );

	SetCursorPos( { 0, 32 } );
	char temp[64];
	ImFormatString( temp, sizeof( temp ), "child %s", label );
	PushStyleVar( ImGuiStyleVar_WindowPadding, { 14, 14 } );
	PushStyleVar( ImGuiStyleVar_ItemSpacing, { 12, 12 } );
	Begin( temp, 0, ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_NoBackground | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoDecoration );
	SetWindowSize( { window->Size.x, size.y == 0 ? GetCurrentWindow( )->ContentSize.y + GImGui->Style.WindowPadding.y * 2 : size.y - GetCursorPos( ).y } );
	PushItemWidth( GetWindowWidth( ) - GImGui->Style.WindowPadding.x * 2 );
}

void ChildManager::endchild( ) {
	PopItemWidth( );
	EndChild( );
	PopStyleVar( 2 );
	EndChild( );
}

void ChildManager::smoothscroll( ) {
	struct s {
		float scroll;
		float scroll_anim;
	}; auto& obj = anim_obj( GetCurrentWindow( )->Name, 23123, s{ } );

	obj.scroll = ImClamp( obj.scroll, 0.f, GetCurrentWindow( )->ScrollMax.y );
	obj.scroll_anim = ImClamp( obj.scroll_anim, 0.f, GetCurrentWindow( )->ScrollMax.y );

	ImGuiWindow* wheeling_window = nullptr;
	if ( GImGui->HoveredWindow ) {
		for ( ImGuiWindow* window = GImGui->HoveredWindow; window->Flags & ImGuiWindowFlags_ChildWindow; window = window->ParentWindow ) {
			if ( window->ScrollMax[ImGuiAxis_Y] == 0 )
				continue;

			wheeling_window = window;
		}
	}

	if ( !wheeling_window ) return;

	if ( wheeling_window == GetCurrentWindow( ) )
		obj.scroll = ImClamp( obj.scroll - GetIO( ).MouseWheel * 50, 0.f, GetCurrentWindow( )->ScrollMax.y );
	obj.scroll_anim = ImLerp( obj.scroll_anim, obj.scroll, GetIO( ).DeltaTime * 40 );
	GetCurrentWindow( )->Scroll.y = obj.scroll_anim;
}