#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <string>
#include <functional>
#include <animations.hpp>

#include <compbuilder/CompBuilder.hpp>
#include "WidgetsManager.hpp"
#include "ColorPickerManager.hpp"
#include "FontManager.hpp"
#include <unicodes.hpp>
#include "ChildManager.hpp"
#include "TabsManager.hpp"
#include "LangManager.hpp"
#include "SearchManager.hpp"

using namespace ImGui;

bool WidgetsManager::Checkbox( const char* label, bool* v, int* key, float* col, std::function< void( ) > options, bool warning ) {
	SearchManager::get( ).additem( label, [=]( ) { Checkbox( label, v, key, col, options, warning ); } );

	float square_sz = 16;

	ImRect total_bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + ImVec2{ CalcTextSize( label, 0, 1 ).x + GImGui->Style.ItemSpacing.x + square_sz, square_sz } };

	if ( warning ) {
		total_bb.Min.x += 24;
		total_bb.Max.x += 24;
	}

	ImRect bb{ total_bb.Min, total_bb.Min + ImVec2{ square_sz, square_sz } };
	ImVec2 options_pos{ bb.Min.x + CalcItemWidth( ) - 24.f * warning, bb.Min.y };

	return CompBuilder::get( ).Checkbox( label, v, key, col, options, warning, total_bb, bb, options_pos, [&]( CompBuilder::CheckboxEnv env ) {
		ImColor col = col_anim( col_anim( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_TextDisabled, 0.6f ), env.anim.hover ), GetColorU32( ImGuiCol_Text ), env.anim.enabled );

		GetWindowDrawList( )->AddRect( bb.Min, bb.Max, GetColorU32( ImGuiCol_Border ), 2 );
		GetWindowDrawList( )->AddRectFilled( bb.Min, bb.Max, GetColorU32( ImGuiCol_Scheme, env.anim.enabled ), 2 );
		RenderCheckMark( GetWindowDrawList( ), bb.GetCenter( ) - ImVec2{ 4, 4 }, GetColorU32( ImGuiCol_ChildBg, env.anim.enabled ), 8 );

		GetWindowDrawList( )->AddText( { total_bb.Min.x + GImGui->Style.ItemInnerSpacing.x + square_sz, total_bb.GetCenter( ).y - GImGui->FontSize / 2 - 0.5f }, col, env.label, FindRenderedTextEnd( env.label ) );
	} );
}

template < typename T >
bool WidgetsManager::Slider( const char* label, T* v, T min, T max, const char* format ) {
	SearchManager::get( ).additem( label, [=]( ) { Slider( label, v, min, max, format ); } );

	ImRect total_bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + ImVec2{ CalcItemWidth( ), GImGui->FontSize + GImGui->Style.ItemInnerSpacing.y + 5 } };
	ImRect bb{ total_bb.Max - ImVec2{ total_bb.GetWidth( ), 5 }, total_bb.Max };
	return CompBuilder::get( ).Slider( label, v, min, max, format, 
		total_bb,	
		bb,
		[&]( const CompBuilder::SliderEnv& env ) {
			ImColor col = col_anim( col_anim( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_TextDisabled, 0.6f ), env.anim.hover ), GetColorU32( ImGuiCol_Text ), env.anim.held );

			GetWindowDrawList( )->AddRectFilled( bb.Min, bb.Max, GetColorU32( ImGuiCol_FrameBg ), 2 );
			GetWindowDrawList( )->AddRectFilled( bb.Min, bb.Min + ImVec2{ env.anim.val_anim, bb.GetHeight( ) }, GetColorU32( ImGuiCol_Scheme ), 2 );
			GetWindowDrawList( )->AddCircleFilled( { bb.Min.x + env.anim.val_anim, bb.GetCenter( ).y }, 5.5f + env.anim.anim - 2.f * env.anim.held, GetColorU32( ImGuiCol_Text ), 36 );

			GetWindowDrawList( )->AddText( total_bb.Min, GetColorU32( ImGuiCol_Text ), env.label, FindRenderedTextEnd( env.label ) );
			GetWindowDrawList( )->AddText( { total_bb.Max.x - CalcTextSize( env.buf ).x, total_bb.Min.y }, col, env.buf );
		} );
}

bool WidgetsManager::SliderInt( const char* label, int* v, int min, int max, const char* format ) {
	return Slider( label, v, min, max, format );
}

bool WidgetsManager::SliderFloat( const char* label, float* v, float min, float max, const char* format ) {
	return Slider( label, v, min, max, format );
}

bool WidgetsManager::ComboEx( const char* label, const char* preview_value, std::function< void( CompBuilder::ComboEnv env ) > code ) {
	ImRect total_bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + ImVec2{ CalcItemWidth( ), GetFrameHeight( ) + GImGui->FontSize + GImGui->Style.ItemInnerSpacing.y } };
	ImRect bb{ total_bb.Max - ImVec2{ CalcItemWidth( ), GetFrameHeight( ) }, total_bb.Max };

	CompBuilder::get( ).Combo( label, total_bb, bb, [&]( const CompBuilder::ComboEnv& env ) {
		ImColor col = col_anim( col_anim( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_TextDisabled, 0.6f ), env.anim.hover ), GetColorU32( ImGuiCol_Scheme ), env.anim.open );

		GetWindowDrawList( )->AddText( total_bb.Min, GetColorU32( ImGuiCol_Text ), env.label, FindRenderedTextEnd( env.label ) );

		GetWindowDrawList( )->AddRectFilled( bb.Min, bb.Max, GetColorU32( ImGuiCol_FrameBg ), GImGui->Style.FrameRounding, env.open ? ImDrawFlags_RoundCornersTop : ImDrawFlags_RoundCornersAll );
		GetWindowDrawList( )->AddText( bb.Min + GImGui->Style.FramePadding, GetColorU32( ImGuiCol_Text ), LangManager::get( ).translate( preview_value ), FindRenderedTextEnd( LangManager::get( ).translate( preview_value ) ) );
		GetWindowDrawList( )->AddText( fonts[icons].get( 14 ), 14, { bb.Max.x - GImGui->Style.FramePadding.x - 14, bb.GetCenter( ).y - 7.5f }, col, i_chevron_selector_vertical );

		if ( env.anim.open > 0.05f ) {
			SetNextWindowPos( { bb.Min.x, bb.Max.y } );
			PushStyleVar( ImGuiStyleVar_Alpha, env.anim.open );
			PushStyleVar( ImGuiStyleVar_ItemSpacing, { 0, 0 } );
			PushStyleVar( ImGuiStyleVar_WindowRounding, GImGui->Style.FrameRounding );
			PushStyleVar( ImGuiStyleVar_WindowPadding, { 0, 0 } );
			PushStyleColor( ImGuiCol_WindowBg, GetColorU32( ImGuiCol_FrameBg ) );
			Begin( label, 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground );
			{
				SetWindowSize( { bb.GetWidth( ), ( GetCurrentWindow( )->ContentSize.y + GImGui->Style.WindowRounding ) * env.anim.open } );

				BringWindowToDisplayFront( GetCurrentWindow( ) );
				BringWindowToFocusFront( GetCurrentWindow( ) );

				GetWindowDrawList( )->AddRectFilled( GetWindowPos( ), GetWindowPos( ) + GetWindowSize( ), GetColorU32( ImGuiCol_WindowBg ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );
				GetWindowDrawList( )->AddRect( GetWindowPos( ), GetWindowPos( ) + GetWindowSize( ), GetColorU32( ImGuiCol_Border ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );

				if ( !IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) && IsMouseClicked( 0 ) && !env.hovered ) {
					env.open = false;
				}

				SetCursorPosY( GImGui->Style.WindowRounding );
				PushStyleColor( ImGuiCol_FrameBg, GetColorU32( ImGuiCol_FrameBgHovered ) );

				code( env );

				PopStyleColor( );
			}
			End( );
			PopStyleColor( );
			PopStyleVar( 4 );
		}
	} );

	return false;
}

bool WidgetsManager::Combo( const char* label, int* v, std::vector< const char* > items ) {
	SearchManager::get( ).additem( label, [=]( ) { Combo( label, v, items ); } );

	ComboEx( label, items[*v], [&]( CompBuilder::ComboEnv env ) {
		for ( int i = 0; i < items.size( ); ++i ) {
			if ( Selectable( items[i], *v == i ) ) {
				*v = i;
				env.open = !env.open;
			}
		}
	} );

	return false;
}

bool WidgetsManager::MultiCombo( const char* label, bool* v, std::vector< const char* > items ) {
	SearchManager::get( ).additem( label, [=]( ) { MultiCombo( label, v, items ); } );

	auto& style = GetStyle( );

    std::string buf;

    buf.clear( );
    for ( size_t i = 0; i < items.size( ); ++i ) {
        if ( v[i] ) {
            buf += LangManager::get( ).translate( items[i] );
            buf += ", ";
        }
    }

    if ( !buf.empty( ) ) {
        buf.resize( buf.size( ) - 2 );
    }

    if ( CalcTextSize( buf.c_str( ) ).x > 160 - style.FramePadding.x * 3 - 10 ) {
        for ( int i = 0; i < buf.size( ) - 1; ++i ) {
            if ( CalcTextSize( buf.substr( 0, i + 1 ).c_str( ) ).x > 160 - style.FramePadding.x - 10 ) {
                buf.resize( i );
                if ( buf[buf.size( ) - 1] == ',' ) {
                    buf.resize( buf.size( ) - 1 );
                }
                buf.append( ".." );
            }
        }
    }

	ComboEx( label, buf.c_str( ), [&]( CompBuilder::ComboEnv env ) {
		for ( int i = 0; i < items.size( ); ++i ) {
			if ( Selectable( items[i], v[i] ) ) {
				v[i] = !v[i];
			}
		}
	} );

	return false;
}

bool WidgetsManager::Binder( const char* label, int* key ) {
	SearchManager::get( ).additem( label, [=]( ) { Binder( label, key ); } );

	CompBuilder::get( ).Binder( label, key, [&]( const CompBuilder::BinderEnv& env ) {
		GetWindowDrawList( )->AddText( { env.total_bb.Min.x, env.total_bb.GetCenter( ).y - GImGui->FontSize / 2 }, GetColorU32( ImGuiCol_Text ), env.label, FindRenderedTextEnd( env.label ) );

		GetWindowDrawList( )->AddRectFilled( env.bb.Min, env.bb.Max, GetColorU32( ImGuiCol_FrameBg ), 2 );
		GetWindowDrawList( )->AddText( fonts[font].get( 12 ), 12, env.bb.Min + ImVec2{ 6, env.bb.GetHeight( ) / 2 - 6 }, GetColorU32( ImGuiCol_Text ), env.keys[*key] );
		GetWindowDrawList( )->AddText( fonts[icons].get( 12 ), 12, env.bb.Min + ImVec2{ env.bb.GetWidth( ) - 18, env.bb.GetHeight( ) / 2 - 6 }, GetColorU32( ImGuiCol_Scheme ), i_keyboard_02 );
	} );

	return false;
}

bool WidgetsManager::TextField( const char* label, char* buf, size_t buf_size, ImVec2 size, const char* hint, const char* icon ) {
	SearchManager::get( ).additem( label, [=]( ) { TextField( label, buf, buf_size, size, hint, icon ); } );

	char str_id[64];
	ImFormatString( str_id, sizeof( str_id ), "##%s", label );
	
	ImVec2 pos = GetCursorPos( );

	bool value_changed = false;

	if ( CalcTextSize( label, 0, 1 ).x > 0 ) {
		PushStyleVar( ImGuiStyleVar_ItemSpacing, { 0, GImGui->Style.ItemInnerSpacing.y } );
		TextEx( label, FindRenderedTextEnd( label ) );
		PopStyleVar( );
	}

	if ( icon ) {
		ImRect bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + CalcItemSize( size, CalcItemWidth( ), GetFrameHeight( ) ) };
		GetWindowDrawList( )->AddRectFilled( bb.Min, bb.Max, GetColorU32( ImGuiCol_FrameBg ), GImGui->Style.FrameRounding );

		if ( GImGui->Style.FrameBorderSize != 0 ) {
			GetWindowDrawList( )->AddRect( bb.Min, bb.Max, GetColorU32( ImGuiCol_Border ), GImGui->Style.FrameRounding );
		}

		GetWindowDrawList( )->AddText( FontManager::get( ).get_fonts( ).at( icons ).get( 14 ), 14, bb.Min + GImGui->Style.FramePadding - ImVec2{ 0, 1 }, GetColorU32( ImGuiCol_TextDisabled ), icon );

		PushStyleColor( ImGuiCol_FrameBg, GetColorU32( ImGuiCol_FrameBg, 0 ) );
		PushStyleColor( ImGuiCol_Border, GetColorU32( ImGuiCol_Border, 0 ) );
		SetCursorPosX( pos.x + 24 );
		SetCursorPosY( pos.y - 1 );
		value_changed = InputTextEx( str_id, LangManager::get( ).translate( hint ), buf, buf_size, size - ImVec2{ 24, 0 }, 0 );
		PopStyleColor( 2 );
	} else {
		value_changed = InputTextEx( str_id, LangManager::get( ).translate( hint ), buf, buf_size, size, 0 );
	}
	
	return value_changed;
}

bool WidgetsManager::Button( const char* label, ImVec2 size ) {
	return CompBuilder::get( ).Button( label, size, [&]( const CompBuilder::ButtonEnv& env ) {
		auto col = col_anim( col_anim( GetColorU32( ImGuiCol_Button ), GetColorU32( ImGuiCol_ButtonHovered ), env.anim.hover ), GetColorU32( ImGuiCol_ButtonActive ), env.anim.held );

		GetWindowDrawList( )->AddRectFilled( env.bb.Min, env.bb.Max, col, GImGui->Style.FrameRounding );
		GetWindowDrawList( )->AddText( env.bb.GetCenter( ) - CalcTextSize( env.label, 0, 1 ) / 2, GetColorU32( ImGuiCol_TextButton ), env.label, FindRenderedTextEnd( env.label ) );
	} );
}

bool WidgetsManager::ColorEdit( const char* label, float col[4] ) {
	SearchManager::get( ).additem( label, [=]( ) { ColorEdit( label, col ); } );

	float square_sz = 14;

	ImRect total_bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + ImVec2{ CalcTextSize( label, 0, 1 ).x > 0 ? CalcItemWidth( ) : square_sz, square_sz } };
	ImRect bb{ total_bb.Max - ImVec2{ square_sz, square_sz }, total_bb.Max };

	bool value_changed = false;

	CompBuilder::get( ).ColorEdit( label, total_bb, bb, col, [&]( CompBuilder::ColorEditEnv env ) {
		GetWindowDrawList( )->AddText( { total_bb.Min.x, total_bb.GetCenter( ).y - GImGui->FontSize / 2 }, GetColorU32( ImGuiCol_Text ), env.label, FindRenderedTextEnd( env.label ) );

		GetWindowDrawList( )->AddCircleFilled( bb.GetCenter( ), square_sz / 2, ImColor{ col[0], col[1], col[2], GImGui->Style.Alpha }, 36 );

		if ( env.anim.open > 0.05f ) {
			SetNextWindowPos( { bb.Min.x, bb.Max.y + 5 } );
			PushStyleVar( ImGuiStyleVar_Alpha, env.anim.open );
			PushStyleVar( ImGuiStyleVar_ItemSpacing, { 10, 10 } );
			PushStyleVar( ImGuiStyleVar_WindowRounding, GImGui->Style.FrameRounding );
			PushStyleVar( ImGuiStyleVar_WindowPadding, { 10, 10 } );
			PushStyleColor( ImGuiCol_WindowBg, GetColorU32( ImGuiCol_FrameBg ) );
			Begin( label, 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize );
			{
				BringWindowToDisplayFront( GetCurrentWindow( ) );
				BringWindowToFocusFront( GetCurrentWindow( ) );

				if ( !IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) && IsMouseClicked( 0 ) && !env.hovered ) {
					env.open = false;
				}

				ColorPickerManager::get( ).draw( env.label, col );
			}
			End( );
			PopStyleColor( );
			PopStyleVar( 4 );
		}
	} );

	return value_changed;
}

bool WidgetsManager::Selectable( const char* label, bool selected, ImVec2 size ) {
	ImRect bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + CalcItemSize( size, GetWindowWidth( ), GetFrameHeight( ) ) };
	return CompBuilder::get( ).Selectable( label, selected, bb, [&]( const CompBuilder::SelectableEnv& env ) {
		ImColor col = col_anim( col_anim( GetColorU32( ImGuiCol_Text ), GetColorU32( ImGuiCol_Text, 0.6f ), env.anim.hover ), GetColorU32( ImGuiCol_Scheme ), env.anim.selected );

		GetWindowDrawList( )->AddRectFilled( bb.Min, bb.Max, GetColorU32( ImGuiCol_FrameBg, env.anim.selected ) );
		GetWindowDrawList( )->AddText( { bb.Min.x + GImGui->Style.FramePadding.x, bb.GetCenter( ).y - GImGui->FontSize / 2 }, col, env.label, FindRenderedTextEnd( env.label ) );
	} );
}

void WidgetsManager::Separator( ) {
	GetWindowDrawList( )->AddRectFilled( { GetWindowPos( ).x, GetCurrentWindow( )->DC.CursorPos.y }, { GetWindowPos( ).x + GetWindowWidth( ), GetCurrentWindow( )->DC.CursorPos.y + 1 }, GetColorU32( ImGuiCol_Separator ) );
	Dummy( { GetWindowWidth( ), 1 } );
}