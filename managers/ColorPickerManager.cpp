#define _CRT_SECURE_NO_WARNINGS
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <animations.hpp>
#include "compbuilder/CompBuilder.hpp"
#include "ColorPickerManager.hpp"
#include "WidgetsManager.hpp"
#include "FontManager.hpp"
#include "unicodes.hpp"

using namespace ImGui;

bool ColorPickerManager::draw( const char* label, float col[4] ) {
	bool value_changed = false;

	struct s {
		float h, s, v;
		bool init;
	}; auto& obj = anim_obj( label, 2323321, s{ } );

	if ( !obj.init ) {
		ColorConvertRGBtoHSV( col[0], col[1], col[2], obj.h, obj.s, obj.v );
		obj.init = true;
	}
	
	value_changed |= Square( col, obj.h, obj.s, obj.v );

	SameLine( 0, 10 );

	value_changed |= HueBar( obj.h, obj.s, obj.v );

	if ( value_changed ) {
		ColorConvertHSVtoRGB( obj.h, obj.s, obj.v, col[0], col[1], col[2] );
	}

	static char buf[7];
	static char alpha_buf[7];
	ImFormatString( buf, sizeof( buf ), "%02X%02X%02X", int( col[0] * 255 ), int( col[1] * 255 ), int( col[2] * 255 ) );
	ImFormatString( alpha_buf, sizeof( alpha_buf ), "%d%%", int( col[3] * 100 ) );

	PushStyleColor( ImGuiCol_FrameBg, GetColorU32( ImGuiCol_FrameBgHovered, 0 ) );
	PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1 );
	PushStyleVar( ImGuiStyleVar_FramePadding, { 12, 10 } );
	PushItemFlag( ImGuiItemFlags_NoNav, true );
	bool hex_changed = WidgetsManager::get( ).TextField( "##hex_input", buf, sizeof( buf ), { 188 - 90, 0 }, 0, i_hash_01 );

	SameLine( );

	bool alpha_changed = WidgetsManager::get( ).TextField( "##alpha_input", alpha_buf, sizeof( alpha_buf ), { 80, 0 }, 0, i_percent_01 );
	PopItemFlag( );

	SameLine( 0, GImGui->Style.ItemSpacing.x + 14 );

	GetWindowDrawList( )->AddCircleFilled( GetCurrentWindow( )->DC.CursorPos + ImVec2{ 0, GetFrameHeight( ) / 2 }, 8, ImColor{ col[0], col[1], col[2], GImGui->Style.Alpha }, 36 );
	Dummy( { 1, 1 } );
	PopStyleVar( 2 );
	PopStyleColor( );

	int i[4];
	sscanf( buf, "%02X%02X%02X", ( unsigned int* )&i[0], ( unsigned int* )&i[1], ( unsigned int* )&i[2] );
	sscanf( alpha_buf, "%d%%", ( unsigned int* )&i[3] );
	if ( hex_changed ) {
		col[0] = i[0] / 255.f;
		col[1] = i[1] / 255.f;
		col[2] = i[2] / 255.f;

		ColorConvertRGBtoHSV( col[0], col[1], col[2], obj.h, obj.s, obj.v );
	}
	
	if ( alpha_changed ) {
		col[3] = i[3] / 100.f;
	}

	BeginGroup( );
	{
		if ( CompBuilder::get( ).Button( "add_color", { 14, 14 }, [&]( CompBuilder::ButtonEnv env ) {
			auto col = col_anim( col_anim( GetColorU32( ImGuiCol_FrameBgHovered ), GetColorU32( ImGuiCol_FrameBgActive ), env.anim.hover ), GetColorU32( ImGuiCol_FrameBgActive, 0.6f ), env.anim.held );
			auto icon_col = col_anim( col_anim( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_Text, 0.6f ), env.anim.hover ), GetColorU32( ImGuiCol_Text ), env.anim.held );
			GetWindowDrawList( )->AddRectFilled( env.bb.Min, env.bb.Max, col, 2 );

			GetWindowDrawList( )->AddText( fonts[icons].get( 12 ), 12, env.bb.GetCenter( ) - ImVec2{ 6, 6.f }, icon_col, i_plus );
			} ) ) {
			saved_colors.push_back( ImColor{ col[0], col[1], col[2], col[3] } );
		}

		SameLine( 0, 7 );

		for ( int i = 0; i < saved_colors.size( ); ++i ) {
			if ( CompBuilder::get( ).Button( std::to_string( i ).append( "color" ).c_str( ), { 14, 14 }, [&]( CompBuilder::ButtonEnv env ) {
				ImColor col{ saved_colors[i].Value.x, saved_colors[i].Value.y, saved_colors[i].Value.z, GImGui->Style.Alpha };
				
				GetWindowDrawList( )->AddRectFilled( env.bb.Min, { env.bb.GetCenter( ).x, env.bb.Max.y }, col, 1, ImDrawFlags_RoundCornersLeft );

				GetWindowDrawList( )->PushClipRect( { env.bb.GetCenter( ).x, env.bb.Min.y }, { env.bb.Max.x, env.bb.GetCenter( ).y } );
				GetWindowDrawList( )->AddRectFilled( env.bb.Min, env.bb.Max, ImColor{ 0.6f, 0.6f, 0.6f, GImGui->Style.Alpha }, 1 );
				GetWindowDrawList( )->PopClipRect( );
				GetWindowDrawList( )->PushClipRect( env.bb.GetCenter( ), env.bb.Max );
				GetWindowDrawList( )->AddRectFilled( env.bb.Min, env.bb.Max, ImColor{ 0.9f, 0.9f, 0.9f, GImGui->Style.Alpha }, 1 );
				GetWindowDrawList( )->PopClipRect( );

				col.Value.w *= saved_colors[i].Value.w;
				GetWindowDrawList( )->AddRectFilled( { env.bb.GetCenter( ).x, env.bb.Min.y }, env.bb.Max, col, 1, ImDrawFlags_RoundCornersRight );
				} ) ) {
				col[0] = saved_colors[i].Value.x;
				col[1] = saved_colors[i].Value.y;
				col[2] = saved_colors[i].Value.z;
				col[3] = saved_colors[i].Value.w;

				ColorConvertRGBtoHSV( col[0], col[1], col[2], obj.h, obj.s, obj.v );
			}

			if ( i < 11 ) {
				if ( ( i + 1 ) % 10 != 0 )
					SameLine( 0, 7 );
			} else { 
				if ( ( i - 9 ) % 11 != 0 )
					SameLine( 0, 7 );
			}	
		}
	}
	EndGroup( );

	return value_changed;
}

bool ColorPickerManager::HueBar( float& h, float s, float v ) {
	float h_values[] {
		0.f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.f
	};
	int h_size = IM_ARRAYSIZE( h_values );

	ImVec2 size{ 28, 188 };
	ImRect bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + size };

	for ( int i = 0; i < h_size - 1; ++i ) {
		ImColor col1, col2;

		ColorConvertHSVtoRGB( h_values[i], ImClamp( s, 0.6f, 1.f ), ImClamp( v, 0.6f, 1.f ), col1.Value.x, col1.Value.y, col1.Value.z );
		ColorConvertHSVtoRGB( h_values[i + 1], ImClamp( s, 0.6f, 1.f ), ImClamp( v, 0.6f, 1.f ), col2.Value.x, col2.Value.y, col2.Value.z );
		col1.Value.w = col2.Value.w = GImGui->Style.Alpha;

		GetWindowDrawList( )->AddRectFilledMultiColor( { bb.Min.x, bb.Min.y + ( size.y / ( h_size - 1 ) ) * i }, { bb.Max.x, bb.Min.y + ( size.y / ( h_size - 1 ) ) * ( i + 1 ) }, col1, col1, col2, col2 );
	}

	GetWindowDrawList( )->AddRect( { bb.Min.x, bb.Min.y + size.y * h - 1.5f }, { bb.Max.x, bb.Min.y + size.y * h + 1.5f }, ImColor{ 1.f, 1.f, 1.f, GImGui->Style.Alpha }, 1 );

	InvisibleButton( "hue", size );
	if ( IsItemActive( ) ) {
		h = ImSaturate( ( GetIO( ).MousePos.y - bb.Min.y ) / size.y );

		return true;
	}

	return false;
}

bool ColorPickerManager::AlphaBar( float& a ) {
	return false;
}

bool ColorPickerManager::Square( float col[4], float h, float& s, float& v ) {
	ImVec2 size{ 188, 188 };
	ImRect bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + size };

	ImColor col_white{ 1.f, 1.f, 1.f, GImGui->Style.Alpha };
	ImColor col_black{ 0.f, 0.f, 0.f, GImGui->Style.Alpha };
	ImColor col_hue;

	ColorConvertHSVtoRGB( h, 1, 1, col_hue.Value.x, col_hue.Value.y, col_hue.Value.z );
	col_hue.Value.w = GImGui->Style.Alpha;

	GetWindowDrawList( )->AddRectFilledMultiColor( bb.Min, bb.Max, col_white, col_hue, col_hue, col_white );
    GetWindowDrawList( )->AddRectFilledMultiColor( bb.Min, bb.Max, 0, 0, col_black, col_black );

	GetWindowDrawList( )->AddCircle( bb.Min + size * ImVec2{ s, 1.f - v }, 3, col_white, 36 );

	InvisibleButton( "sv", size );
    if ( IsItemActive( ) )
    {
        s = ImSaturate( ( GetIO( ).MousePos.x - bb.Min.x ) / size.x );
        v = 1.f - ImSaturate( ( GetIO( ).MousePos.y - bb.Min.y ) / size.y );

        return true;
    }

	return false;
}