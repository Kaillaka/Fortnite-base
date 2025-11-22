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
#include "TabsManager.hpp"
#include "FontManager.hpp"
#include "ChildManager.hpp"
#include "LangManager.hpp"

using namespace ImGui;

bool TabsManager::tab( int i ) {
	ImRect bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + ImVec2{ GetWindowWidth( ) - 28, 32 } };
	CompBuilder::get( ).Selectable( tabs[i].label, i == next, bb, [&]( const CompBuilder::SelectableEnv& env ) {
		if ( env.pressed ) {
			next = i;
			anim_dest = 0.f;
		}

		GetWindowDrawList( )->AddRectFilled( bb.Min, bb.Max, GetColorU32( ImGuiCol_ChildBg, env.anim.selected ), GImGui->Style.FrameRounding );

		auto col = col_anim( col_anim( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_TextDisabled, 0.6f ), env.anim.hover ), GetColorU32( ImGuiCol_Text ), env.anim.selected );
		auto icon_col = col_anim( col_anim( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_TextDisabled, 0.6f ), env.anim.hover ), GetColorU32( ImGuiCol_Scheme ), env.anim.selected );

		GetWindowDrawList( )->AddText( fonts[icons].get( 14 ), 14, bb.Min + ImVec2{ 9, bb.GetHeight( ) / 2 - 7 }, icon_col, tabs[i].icon );
		GetWindowDrawList( )->AddText( bb.Min + ImVec2{ 32, bb.GetHeight( ) / 2 - GImGui->FontSize / 2 }, col, env.label, FindRenderedTextEnd( env.label ) );
	} );

	return false;
}

bool TabsManager::subtab( int i ) {
	ImRect bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + CalcTextSize( LangManager::get( ).translate( tabs[current].subtabs[i] ), 0, 1 ) };
	CompBuilder::get( ).Selectable( tabs[current].subtabs[i], i == tabs[current].next, bb, [&]( const CompBuilder::SelectableEnv& env ) {
		if ( env.pressed ) {
			tabs[current].next = i;
			subtabs_anim_dest = 0.f;
		}

		auto col = col_anim( col_anim( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_TextDisabled, 0.6f ), env.anim.hover ), GetColorU32( ImGuiCol_Text ), env.anim.selected );
		GetWindowDrawList( )->AddText( bb.Min, col, env.label, FindRenderedTextEnd( env.label ) );
	} );

	return false;
}

void TabsManager::render_tabs( float spacing, bool line ) {
	BeginGroup( );
    {
        PushStyleVar( ImGuiStyleVar_ItemSpacing, { spacing, spacing } );

        for ( int i = 0; i < tabs.size( ); ++i ) {
            tab( i );

            if ( line ) SameLine( );
        }

        PopStyleVar( );
    }
    EndGroup( );

	handle( );
}

void TabsManager::render_subtabs( float spacing, bool line ) {
	if ( tabs[current].subtabs.empty( ) )
        return;

	SetCursorPos( { 14, 14 } );
    BeginGroup( );
    {
        PushStyleVar( ImGuiStyleVar_ItemSpacing, { spacing, spacing } );
        PushStyleVar( ImGuiStyleVar_Alpha, anim * GImGui->Style.Alpha );

        for ( int i = 0; i < tabs[current].subtabs.size( ); ++i ) {
            subtab( i );

            if ( line && i < ( tabs[current].subtabs.size( ) - 1 ) ) SameLine( );
        }

        PopStyleVar( 2 );
    }
    EndGroup( );

	SetCursorPos( { 0, GetCursorPos( ).y - GImGui->Style.ItemSpacing.y } );
}

void TabsManager::handle( ) {
	anim = ImLerp( anim, anim_dest, GetIO( ).DeltaTime * 20.f );
	subtabs_anim = ImLerp( subtabs_anim, subtabs_anim_dest, GetIO( ).DeltaTime * 20.f );

	if ( anim < 0.05f ) {
		current = next;
		anim_dest = 1.f;
	}

	if ( subtabs_anim < 0.05f ) {
		tabs[current].cur = tabs[current].next;
		subtabs_anim_dest = 1.f;
	}
}

#include <string>

void TabsManager::draw_page( ImGuiWindow* window ) {
	if ( tabs[current].pages.size( ) <= tabs[current].cur )
		return;

	ImGuiWindow* content_window = GetCurrentWindow( );

	ChildManager::get( ).smoothscroll( );

	tabs[current].pages[tabs[current].cur]( );

	SetNextWindowPos( window->Pos );
	SetNextWindowSize( window->Size );
	PushStyleVar( ImGuiStyleVar_WindowPadding, { 0, 0 } );
	Begin( "glow", 0, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration );
	{
		PopStyleVar( );

		BringWindowToFocusFront( GetCurrentWindow( ) );
		BringWindowToDisplayFront( GetCurrentWindow( ) );

		static float anim1 = 0.f, anim2 = 0.f;
		anim1 = ImLerp( anim1, float( content_window->Scroll.y > 0 ), GetIO( ).DeltaTime * 24 );
		anim2 = ImLerp( anim2, float( content_window->Scroll.y < ( content_window->ScrollMax.y - 1 ) ), GetIO( ).DeltaTime * 24 );

		GetWindowDrawList( )->AddRectFilledMultiColor( content_window->Pos, content_window->Pos + ImVec2{ content_window->Size.x, GImGui->Style.WindowPadding.y }, GetColorU32( ImGuiCol_WindowBg, anim1 ), GetColorU32( ImGuiCol_WindowBg, anim1 ), GetColorU32( ImGuiCol_WindowBg, 0.f ), GetColorU32( ImGuiCol_WindowBg, 0.f ) );
		//GetWindowDrawList( )->AddRectFilledMultiColor( content_window->Pos + ImVec2{ 0, content_window->Size.y - GImGui->Style.WindowPadding.y }, content_window->Pos + content_window->Size, GetColorU32( ImGuiCol_WindowBg, 0.f ), GetColorU32( ImGuiCol_WindowBg, 0.f ), GetColorU32( ImGuiCol_WindowBg, anim2 ), GetColorU32( ImGuiCol_WindowBg, anim2 ) );
	}
	End( );
}

void TabsManager::add_page( int tab, std::function< void( ) > code ) {
	tabs[tab].pages.push_back( code );
}