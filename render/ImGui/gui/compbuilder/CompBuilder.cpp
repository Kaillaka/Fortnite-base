#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#define _CRT_SECURE_NO_WARNINGS
#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <string>
#include <functional>
#include <animations.hpp>
#include "compbuilder/CompBuilder.hpp"
#include <windows.h>
#include <managers/WidgetsManager.hpp>
#include <managers/FontManager.hpp>
#include <managers/PopupManager.hpp>
#include <managers/LangManager.hpp>
#include <managers/ChildManager.hpp>
#include <unicodes.hpp>
#include <managers/TabsManager.hpp>
#include <managers/SearchManager.hpp>

using namespace ImGui;

std::vector< const char* > keys {
		"NONE",
		"M1",
		"M2",
		"CN",
		"M3",
		"M4",
		"M5",
		"NONE",
		"BACK",
		"TAB",
		"NONE",
		"NONE",
		"BACK",
		"ENT",
		"NONE",
		"NONE",
		"SHIFT",
		"CTRL",
		"ALT",
		"PAUSE",
		"CAPS",
		"KAN",
		"NONE",
		"JUN",
		"FIN",
		"KAN",
		"NONE",
		"ESC",
		"CON",
		"NCO",
		"ACC",
		"MAD",
		"SPACE",
		"PGU",
		"PGD",
		"END",
		"HOME",
		"LEFT",
		"UP",
		"RIGH",
		"DOWN",
		"SEL",
		"PRINT",
		"EXE",
		"PRINT",
		"INS",
		"DEL",
		"HEL",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"...",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"WIN",
		"WIN",
		"APP",
		"NONE",
		"SLE",
		"NUM0",
		"NUM1",
		"NUM2",
		"NUM3",
		"NUM4",
		"NUM5",
		"NUM6",
		"NUM7",
		"NUM8",
		"NUM9",
		"*",
		"+",
		"|",
		"-",
		".",
		"/",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"F13",
		"F14",
		"F15",
		"F16",
		"F17",
		"F18",
		"F19",
		"F20",
		"F21",
		"F22",
		"F23",
		"F24",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NUMLOCK",
		"SCR",
		"=",
		"MAS",
		"TOY",
		"OYA",
		"OYA",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"NONE",
		"SHIFT",
		"SHIFT",
		"CTRL",
		"CTRL",
		"ALT",
		"ALT"
	};

void CompBuilder::Empty( const char* label, ImRect total_bb, ImRect bb, std::function< void( const EmptyEnv& ) > code ) {
	ImGuiWindow* window = GetCurrentWindow( );
	auto id = window->GetID( label );
	
	ItemSize( total_bb );
	ItemAdd( total_bb, id );

	bool hovered, held;
	bool pressed = ButtonBehavior( bb, id, &hovered, &held );

	code( EmptyEnv {
		id,
		hovered,
		held,
		pressed,
		LangManager::get( ).translate( label )
	} );
}

bool CompBuilder::Button( const char* label, ImVec2 size, std::function< void( const ButtonEnv& ) > code ) {
	ImRect bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + CalcItemSize( size, CalcItemWidth( ), GetFrameHeight( ) ) };
	bool pressed;

	Empty( label, bb, bb, [&]( const EmptyEnv& env ) {
		struct s {
			float anim;
			float hover;
			float held;
		}; auto& obj = anim_obj( label, 4431423, s{ } );

		obj.anim = anim( obj.anim, 0.f, 1.f, env.hovered || env.held );
		obj.hover = anim( obj.hover, 0.f, 1.f, env.hovered );
		obj.held = anim( obj.held, 0.f, 1.f, env.held );

		code( ButtonEnv {
			bb,
			env.hovered,
			env.held,
			env.label,
			ButtonEnv::Anim {
				obj.hover,
				obj.held,
				obj.anim
			}
		} );

		pressed = env.pressed;
	} );

	return pressed;
}

template < typename T >
bool CompBuilder::Slider( const char* label, T* v, T min, T max, const char* format, ImRect total_bb, ImRect bb, std::function< void( SliderEnv ) > code ) {
	bool result = false;

	Empty( label, total_bb, bb, [&]( const EmptyEnv& env ) {
		struct s {
			float anim;
			float hover;
			float held;
			float val_anim;
			char buf[64];
		}; auto& obj = anim_obj( label, 032, s{ } );

		obj.anim = anim( obj.anim, 0.f, 1.f, env.hovered || env.held );
		obj.hover = anim( obj.hover, 0.f, 1.f, env.hovered );
		obj.held = anim( obj.held, 0.f, 1.f, env.held );
		obj.val_anim = ImLerp( obj.val_anim, ( ImClamp( *v, min, max ) - min * 1.f ) / ( max - min ) * bb.GetWidth( ), GetIO( ).DeltaTime * 17 );

		if ( env.held ) {
			*v = ImClamp( T( min + ( GetIO( ).MousePos.x - bb.Min.x ) / bb.GetWidth( ) * ( max - min ) ), min, max );
			result = true;
		}

		ImFormatString( obj.buf, sizeof( obj.buf ), format, *v );

		code( SliderEnv {
			env.hovered,
			env.held,
			obj.buf,
			env.label,
			SliderEnv::Anim {
				obj.hover,
				obj.held,
				obj.anim,
				obj.val_anim,
			}
		} );
	} );

	return result;
}

bool CompBuilder::SliderInt( const char* label, int* v, int min, int max, const char* format, ImRect total_bb, ImRect bb, std::function< void( SliderEnv ) > code ) {
	return Slider( label, v, min, max, format, total_bb, bb, code );
}

bool CompBuilder::SliderFloat( const char* label, float* v, float min, float max, const char* format, ImRect total_bb, ImRect bb, std::function< void( SliderEnv ) > code ) {
	return Slider( label, v, min, max, format, total_bb, bb, code );
}

bool CompBuilder::Checkbox( const char* label, bool* v, int* key, float* col, std::function< void( ) > options, bool warning, ImRect total_bb, ImRect bb, ImVec2 options_pos, std::function< void( CheckboxEnv ) > code ) {
	bool pressed = false;

	Empty( label, total_bb, total_bb, [&]( const EmptyEnv& env ) {
		float h, s1, v1;
		ImColor warning_col;
		ColorConvertRGBtoHSV( GetStyleColorVec4( ImGuiCol_Scheme ).x, GetStyleColorVec4( ImGuiCol_Scheme ).y, GetStyleColorVec4( ImGuiCol_Scheme ).z, h, s1, v1 );
		ColorConvertHSVtoRGB( 0.f, s1, v1, warning_col.Value.x, warning_col.Value.y, warning_col.Value.z );
		warning_col.Value.w = GImGui->Style.Alpha;

		if ( env.pressed ) {
			if ( warning && !*v ) {
				PopupManager::get( ).open_popup( [=]( ) {
					SetCursorPos( GetWindowSize( ) / 2 - ImVec2{ 300, 150 } / 2 );
					BeginChild( "popup window", { 300, 150 } );
					{
						GetWindowDrawList( )->AddText( fonts[icons].get( 14 ), 14, GetWindowPos( ) + ImVec2{ 20, 20 }, ImColor{ warning_col.Value.x, warning_col.Value.y, warning_col.Value.z, GImGui->Style.Alpha }, i_alert_triangle );
						GetWindowDrawList( )->AddText( GetWindowPos( ) + ImVec2{ 44, 20 }, GetColorU32( ImGuiCol_Text ), LangManager::get( ).translate( "Are you sure u want to enable it?" ) );
						GetWindowDrawList( )->AddText( GetWindowPos( ) + ImVec2{ 20, 46 }, GetColorU32( ImGuiCol_TextDisabled ), LangManager::get( ).translate( "This function is " ) );
						GetWindowDrawList( )->AddText( GetWindowPos( ) + ImVec2{ 20 + CalcTextSize( LangManager::get( ).translate( "This function is " ) ).x, 46 }, GetColorU32( ImGuiCol_Text ), LangManager::get( ).translate( "dangerous!" ) );

						SetCursorPos( { 20, GetWindowHeight( ) - 20 - GetFrameHeight( ) } );
						if ( WidgetsManager::get( ).Button( "YES", { 100, GetFrameHeight( ) } ) ) {
							*v = true;
							PopupManager::get( ).close_popup( );
						}

						SameLine( 0, 10 );

						PushStyleColor( ImGuiCol_Button, GetColorU32( ImGuiCol_FrameBg ) );
						PushStyleColor( ImGuiCol_ButtonHovered, GetColorU32( ImGuiCol_FrameBgHovered ) );
						PushStyleColor( ImGuiCol_ButtonActive, GetColorU32( ImGuiCol_FrameBgActive ) );
						PushStyleColor( ImGuiCol_TextButton, GetColorU32( ImGuiCol_Text ) );
						if ( WidgetsManager::get( ).Button( "NO", { GetWindowWidth( ) - 150, GetFrameHeight( ) } ) ) {
							PopupManager::get( ).close_popup( );
						}
						PopStyleColor( 4 );
					}
					EndChild( );
				} );
			} else {
				*v = !*v;
			}
		}

		struct s {
			float anim;
			float hover;
			float held;
			float enabled;
			float key_size;
		}; auto& obj = anim_obj( label, 443143223, s{ } );

		obj.anim = anim( obj.anim, 0.f, 1.f, env.hovered || *v );
		obj.hover = anim( obj.hover, 0.f, 1.f, env.hovered );
		obj.held = anim( obj.held, 0.f, 1.f, env.held );
		obj.enabled = anim( obj.enabled, 0.f, 1.f, *v );

		code( CheckboxEnv {
			env.hovered,
			env.held,
			env.pressed,
			env.label,
			CheckboxEnv::Anim {
				obj.hover,
				obj.held,
				obj.enabled,
				obj.anim,
			}
		} );

		if ( warning ) {
			GetWindowDrawList( )->AddText( fonts[icons].get( 14 ), 14, { total_bb.Min.x - 24, total_bb.GetCenter( ).y - 7 }, warning_col, i_alert_triangle );
		}

		PushItemFlag( ImGuiItemFlags_NoNav, true );

		if ( options ) {
			GetCurrentWindow( )->DC.CursorPos = ImVec2{ options_pos.x - 14 - ( obj.key_size + 10 ) * ( bool )key - 24.f * ( bool )col, bb.GetCenter( ).y - 7 };
			char temp[64];
			ImFormatString( temp, sizeof( temp ), "##%s options", label );
			
			OpenButton( temp, { 14, 14 }, [&]( OpenButtonEnv env ) {
				ImColor color = col_anim( col_anim( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_TextHovered ), env.anim.hover ), GetColorU32( ImGuiCol_Scheme ), env.anim.open );
				GetWindowDrawList( )->AddText( fonts[icons].get( 14 ), 14, env.bb.Min, color, i_settings_01 );

				if ( env.anim.open > 0.05f ) {
					PushStyleVar( ImGuiStyleVar_Alpha, env.anim.open );
					PushStyleVar( ImGuiStyleVar_ItemSpacing, { 0, 12 } );
					PushStyleVar( ImGuiStyleVar_WindowRounding, GImGui->Style.FrameRounding );
					PushStyleVar( ImGuiStyleVar_WindowPadding, { 14, 14 } );
					PushStyleVar( ImGuiStyleVar_WindowBorderSize, 1 );
					PushStyleColor( ImGuiCol_WindowBg, GetColorU32( ImGuiCol_FrameBg ) );
					Begin( label, 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove );
					{
						SetWindowPos( { env.bb.Max.x - GetWindowWidth( ), env.bb.Max.y + 20 - int( 10 * env.anim.open ) } );
						SetWindowSize( { 200, GetCurrentWindow( )->ContentSize.y + GImGui->Style.WindowPadding.y * 2 } );

						BringWindowToDisplayFront( GetCurrentWindow( ) );
						BringWindowToFocusFront( GetCurrentWindow( ) );

						if ( !IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) && IsMouseClicked( 0 ) && !env.hovered ) {
							env.open = false;
						}

						PushStyleColor( ImGuiCol_FrameBg, GetColorU32( ImGuiCol_FrameBgHovered ) );
						PushItemWidth( GetWindowWidth( ) - GImGui->Style.WindowPadding.x * 2 );
						PushItemFlag( ImGuiItemFlags_NoNav, true );

						options( );

						PopItemFlag( );
						PopItemWidth( );
						PopStyleColor( );
					}
					End( );
					PopStyleColor( );
					PopStyleVar( 5 );
				}
			} );
		}

		if ( col ) {
			GetCurrentWindow( )->DC.CursorPos = ImVec2{ options_pos.x - 14 - ( obj.key_size + 10 ) * ( bool )key, bb.GetCenter( ).y - 7 };
			char color_label[64];
			ImFormatString( color_label, sizeof( color_label ), "##%s color", label );
			WidgetsManager::get( ).ColorEdit( color_label, col );
		}

		if ( key ) {
			obj.key_size = ImLerp( obj.key_size, fonts[font].get( 12 )->CalcTextSizeA( 12, FLT_MAX, -1, keys[*key] ).x, GetIO( ).DeltaTime * 24 );

			GetCurrentWindow( )->DC.CursorPos = ImVec2{ options_pos.x - 32 - ( int )obj.key_size, bb.GetCenter( ).y - 12 };
			char bind_label[64];
			ImFormatString( bind_label, sizeof( bind_label ), "##%s bind", label );
			WidgetsManager::get( ).Binder( bind_label, key );
		}

		PopItemFlag( );

		pressed = env.pressed;
	} );

	return pressed;
}

bool CompBuilder::Combo( const char* label, ImRect total_bb, ImRect bb, std::function< void( ComboEnv ) > code ) {
	Empty( label, total_bb, bb, [&]( const EmptyEnv& env ) {
		struct s {
			float anim;
			float hover;
			float held;
			float open_anim;
			bool open;
		}; auto& obj = anim_obj( label, 21231, s{ } );

		obj.anim = anim( obj.anim, 0.f, 1.f, env.hovered || obj.open );
		obj.hover = anim( obj.hover, 0.f, 1.f, env.hovered );
		obj.held = anim( obj.held, 0.f, 1.f, env.held );
		obj.open_anim = anim( obj.open_anim, 0.f, 1.f, obj.open );

		if ( env.pressed ) {
			obj.open = !obj.open;
		}

		code( ComboEnv {
			env.hovered,
			env.held,
			env.pressed,
			obj.open,
			env.label,
			ComboEnv::Anim {
				obj.hover,
				obj.held,
				obj.open_anim,
				obj.anim
			}
		} );
	} );

	return false;
}

bool CompBuilder::Selectable( const char* label, bool selected, ImRect bb, std::function< void( const SelectableEnv& ) > code ) {
	bool pressed = false;

	Empty( label, bb, bb, [&]( const EmptyEnv& env ) {
		struct s {
			float anim;
			float hover;
			float held;
			float selected;
		}; auto& obj = anim_obj( label, 231, s{ } );

		obj.anim = anim( obj.anim, 0.f, 1.f, env.hovered || selected );
		obj.hover = anim( obj.hover, 0.f, 1.f, env.hovered );
		obj.held = anim( obj.held, 0.f, 1.f, env.held );
		obj.selected = anim( obj.selected, 0.f, 1.f, selected );

		code( SelectableEnv {
			env.hovered,
			env.held,
			env.pressed,
			env.label,
			SelectableEnv::Anim {
				obj.hover,
				obj.held,
				obj.selected,
				obj.anim
			}
		} );

		pressed = env.pressed;
	} );

	return pressed;
}

bool CompBuilder::ColorEdit( const char* label, ImRect total_bb, ImRect bb, float col[4], std::function< void( ColorEditEnv ) > code ) {
	Empty( label, total_bb, bb, [&]( const EmptyEnv& env ) {
		struct s {
			float anim;
			float hover;
			float held;
			float open_anim;
			bool open;
		}; auto& obj = anim_obj( label, 23321, s{ } );

		obj.anim = anim( obj.anim, 0.f, 1.f, env.hovered || obj.open );
		obj.hover = anim( obj.hover, 0.f, 1.f, env.hovered );
		obj.held = anim( obj.held, 0.f, 1.f, env.held );
		obj.open_anim = anim( obj.open_anim, 0.f, 1.f, obj.open );

		if ( env.pressed ) {
			obj.open = !obj.open;
		}

		code( ColorEditEnv {
			env.hovered,
			env.held,
			env.pressed,
			obj.open,
			env.label,
			ColorEditEnv::Anim {
				obj.hover,
				obj.held,
				obj.open_anim,
				obj.anim
			}
		} );
	} );

	return false;
}

bool CompBuilder::Binder( const char* label, int* key, std::function< void( const BinderEnv& ) > code ) {
	struct s {
		float anim;
		float hover;
		float held;
		float active;
		float val_anim;
	}; auto& obj = anim_obj( label, 21131, s{ } );

	obj.val_anim = ImLerp( obj.val_anim, fonts[font].get( 12 )->CalcTextSizeA( 12, FLT_MAX, -1, keys[*key] ).x, GetIO( ).DeltaTime * 24 );

	ImRect total_bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + ImVec2{ CalcTextSize( label, 0, 1 ).x > 0 ? CalcItemWidth( ) : 32.f + ( int )obj.val_anim, 24 } };
	ImRect bb{ total_bb.Max - ImVec2{ 32.f + ( int )obj.val_anim, 24 }, total_bb.Max };

	Empty( label, total_bb, bb, [&]( const EmptyEnv& env ) {
		struct s {
			float anim;
			float hover;
			float held;
			float active;
			bool is_active;
		}; auto& obj = anim_obj( label, 21131, s{ } );

		obj.anim = anim( obj.anim, 0.f, 1.f, env.hovered || IsItemActive( ) );
		obj.hover = anim( obj.hover, 0.f, 1.f, env.hovered );
		obj.held = anim( obj.held, 0.f, 1.f, env.held );
		obj.active = anim( obj.active, 0.f, 1.f, obj.is_active );

		const bool SHOULD_EDIT = env.hovered && GetIO( ).MouseClicked[0] && !obj.is_active;

		if ( SHOULD_EDIT ) {
			memset( GetIO( ).MouseDown, 0, sizeof( GetIO( ).MouseDown ) );
			*key = 58;

			obj.is_active = true;
		}

		bool value_changed = false;
		int k = *key;

		if ( obj.is_active ) {
			for ( auto i = 0; i < 5; i++ ) {
				if ( GetIO( ).MouseDown[i] ) {
					switch ( i ) {
					case 0:
						k = VK_LBUTTON;
						break;
					case 1:
						k = VK_RBUTTON;
						break;
					case 2:
						k = VK_MBUTTON;
						break;
					case 3:
						k = VK_XBUTTON1;
						break;
					case 4:
						k = VK_XBUTTON2;
					}
					value_changed = true;
					obj.is_active = false;
				}
			}

			if ( !value_changed ) {
				for ( auto i = VK_BACK; i <= VK_RMENU; i++ ) {
					if ( GetAsyncKeyState( i ) & 0x8000 ) {
						k = i;
						value_changed = true;
						obj.is_active = false;
					}
				}
			}

			*key = k;
		}

		code( BinderEnv {
			total_bb,
			bb,

			env.hovered,
			env.held,
			env.pressed,
			obj.is_active,
			env.label,

			keys,

			BinderEnv::Anim {
				obj.hover,
				obj.held,
				obj.active,
				obj.anim
			}
		} );
	} );

	return false;
}

bool CompBuilder::OpenButton( const char* str_id, ImVec2 size, std::function< void( OpenButtonEnv ) > code ) {
	ImRect bb{ GetCurrentWindow( )->DC.CursorPos, GetCurrentWindow( )->DC.CursorPos + size };
	bool pressed;

	Empty( str_id, bb, bb, [&]( const EmptyEnv& env ) {
		struct s {
			float anim;
			float hover;
			float held;
			float open_anim;
			bool open;
		}; auto& obj = anim_obj( str_id, 4431423123, s{ } );

		obj.anim = anim( obj.anim, 0.f, 1.f, env.hovered || env.held );
		obj.hover = anim( obj.hover, 0.f, 1.f, env.hovered );
		obj.held = anim( obj.held, 0.f, 1.f, env.held );
		obj.open_anim = anim( obj.open_anim, 0.f, 1.f, obj.open );

		if ( env.pressed ) {
			obj.open = !obj.open;
		}

		code( OpenButtonEnv {
			bb,
			env.hovered,
			env.held,
			env.pressed,
			obj.open,
			OpenButtonEnv::Anim {
				obj.hover,
				obj.held,
				obj.anim,
				obj.open_anim
			}
		} );

		pressed = env.pressed;
	} );

	return pressed;
}