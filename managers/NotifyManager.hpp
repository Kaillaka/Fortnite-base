#pragma once

class NotifyManager {
public:
	enum notify_status {
		notify_success,
		notify_error,
		notify_info,
	};

	struct c_notify {
		std::string message;
		notify_status status;
		float duration = 3.f;
		float time = 0.f;
		ImVec2 pos{ 0, 0 };
		float fade_time = 0.25f;
	};

	std::vector< c_notify > notifications;

	void erase_notifies( ) {
		notifications.erase(
			std::remove_if( notifications.begin( ), notifications.end( ), []( const c_notify& n ) {
				return n.time >= n.duration;
			} ), 
			notifications.end( )
		);
	}

	void add( const std::string& message, notify_status status, float duration = 3.f ) {
		notifications.emplace_back( c_notify{ message, status, duration } );
	}

	void draw( ) {
		auto draw_list = GetBackgroundDrawList( );

		float offset = 0.f;
		for ( int i = 0; i < notifications.size( ); ++i ) {
			auto& n = notifications[i];
			float alpha = n.time <= n.fade_time ? n.time / n.fade_time : n.time >= n.duration - n.fade_time ? ( n.duration - n.time ) / n.fade_time : 1.f;

			const char* titles[] {
				"SUCCESS",
				"ERROR",
				"INFO",
			};

			const char* n_icons[] = {
				i_check_circle,
				i_alert_circle,
				i_info_circle,
			};

			ImColor colors[3];

			colors[0] = GetColorU32( GetStyleColorVec4( ImGuiCol_Scheme ) );
			colors[0].Value.w = alpha;

			float h, s, v;
			ColorConvertRGBtoHSV( GetStyleColorVec4( ImGuiCol_Scheme ).x, GetStyleColorVec4( ImGuiCol_Scheme ).y, GetStyleColorVec4( ImGuiCol_Scheme ).z, h, s, v );
			ColorConvertHSVtoRGB( 0.f, s, v, colors[1].Value.x, colors[1].Value.y, colors[1].Value.z );
			colors[1].Value.w = alpha;

			ColorConvertRGBtoHSV( GetStyleColorVec4( ImGuiCol_Scheme ).x, GetStyleColorVec4( ImGuiCol_Scheme ).y, GetStyleColorVec4( ImGuiCol_Scheme ).z, h, s, v );
			ColorConvertHSVtoRGB( 0.63f, s, v, colors[2].Value.x, colors[2].Value.y, colors[2].Value.z );
			colors[2].Value.w = alpha;

			ImVec2 size{ ImMax( CalcTextSize( LangManager::get( ).translate( n.message.c_str( ) ) ).x, CalcTextSize( LangManager::get( ).translate( titles[n.status] ) ).x + 24 ) + 28, GImGui->FontSize * 2 + 38 };

			if ( n.pos.x == 0 ) n.pos = GetIO( ).DisplaySize - ImVec2{ 0, 20 + offset + size.y };

			n.pos.x = ImLerp( n.pos.x, GetIO( ).DisplaySize.x - 20 - size.x, GetIO( ).DeltaTime * 14 );
			n.pos.y = ImLerp( n.pos.y, GetIO( ).DisplaySize.y - 20 - offset - size.y, GetIO( ).DeltaTime * 14 );

			draw_list->AddRectFilled( n.pos, n.pos + size, GetColorU32( ImGuiCol_WindowBg, alpha ), 3 );

			draw_list->AddText( fonts[icons].get( 14 ), 14, n.pos + ImVec2{ 14, 14 }, colors[n.status], n_icons[n.status] );
			draw_list->AddText( n.pos + ImVec2{ 38, 14 }, GetColorU32( ImGuiCol_Text, alpha ), LangManager::get( ).translate( titles[n.status] ) );
			draw_list->AddText( n.pos + ImVec2{ 14, 24 + GImGui->FontSize }, GetColorU32( ImGuiCol_TextDisabled, alpha ), LangManager::get( ).translate( n.message.c_str( ) ) );

			n.time += 1.f / GetIO( ).Framerate;
			offset += size.y + 12;
		}

		erase_notifies( );
	}

	static NotifyManager& get( ) {
		static NotifyManager s{ };
		return s;
	}
};