#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>
#include <imgui_internal.h>
#include <functional>
#include "PopupManager.hpp"

using namespace ImGui;

void PopupManager::open_popup( std::function< void( ) > code ) {
	anim_dest = 1.f;
	popup = code;
}

void PopupManager::close_popup( ) {
	anim_dest = 0.f;
}

void PopupManager::handle( ) {
	anim = ImLerp( anim, anim_dest, GetIO( ).DeltaTime * 24 );

	if ( anim < 0.05f ) {
		popup = 0;
	} else {
		SetNextWindowPos( GetWindowPos( ) );
		SetNextWindowSize( GetWindowSize( ) );
		PushStyleVar( ImGuiStyleVar_Alpha, anim );
		PushStyleColor( ImGuiCol_WindowBg, GetColorU32( ImGuiCol_WindowBg, 0.96f ) );
		Begin( "popup", 0, ImGuiWindowFlags_NoDecoration );
		{
			BringWindowToFocusFront( GetCurrentWindow( ) );
			BringWindowToDisplayFront( GetCurrentWindow( ) );

			popup( );
		}
		End( );
		PopStyleColor( );
		PopStyleVar( );
	}
}