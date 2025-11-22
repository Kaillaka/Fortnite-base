#pragma once

using namespace ImGui;

struct SearchItem {
	const char* label;
	int tab;
	int subtab;
	std::string child;
	std::function< void( ) > code;
};

class SearchManager {
	float anim = 0.f, anim_dest = 1.f;
	std::vector< SearchItem > items;
public:
	char search_buf[16];

	void additem( const char* label, std::function< void( ) > code ) {
		if ( std::find_if( items.begin( ), items.end( ), [&]( const SearchItem& it ) { return it.label == label; } ) != items.end( ) || ( GImGui->CurrentItemFlags & ImGuiItemFlags_NoNav ) )
			return;

		items.push_back( SearchItem{ label, TabsManager::get( ).current, TabsManager::get( ).get_tab( ).cur, ChildManager::get( ).get_current( ), code } );
	}

	std::vector< SearchItem >& get_items( ) {
		return items;
	}

	std::string to_lower( const char* str ) {
		std::string result;

		for ( int i = 0; str[i]; ++i ) {
			result += std::tolower( str[i] );
		}

		return result;
	}

	bool compare( const char* str, const char* substr ) {
		return strstr( to_lower( str ).c_str( ), to_lower( substr ).c_str( ) );
	}

	void update( ) {
		anim_dest = 0.f;
	}

	float& get_anim( ) {
		return anim;
	}

	void draw( ) {
		std::vector< SearchItem* > res;

		static std::string buf;

		for ( auto& item : items ) {
			if ( compare( LangManager::get( ).translate( item.label ), buf.c_str( ) ) ) {
				res.push_back( &item );
			}
		}

		anim = ImLerp( anim, anim_dest, GetIO( ).DeltaTime * 28 );

		if ( anim < 0.05f ) {
			if ( strlen( search_buf ) > 0 ) anim_dest = 1.f;
			buf = search_buf;
		}

		TabsManager::get( ).get_tabs_anim( ) = 0.f;

		PushStyleVar( ImGuiStyleVar_Alpha, anim * GImGui->Style.Alpha );
		BeginGroup( );
		{
			PushItemWidth( GetWindowWidth( ) - 28 );

			for ( int i = 0; i < res.size( ); ++i ) {
				auto& item = *res[i];

				TextDisabled( LangManager::get( ).translate( TabsManager::get( ).get_tab( item.tab ).label ) );
				SameLine( 0, 10 );
				PushFont( fonts[icons].get( 14 ) );
				TextDisabled( i_chevron_right );
				PopFont( );
				SameLine( 0, 24 );
				if ( TabsManager::get( ).get_tab( item.tab ).subtabs.size( ) > 0 ) {
					TextDisabled( TabsManager::get( ).get_tab( item.tab ).subtabs[item.subtab] );
					SameLine( 0, 10 );
					PushFont( fonts[icons].get( 14 ) );
					TextDisabled( i_chevron_right );
					PopFont( );
					SameLine( 0, 24 );
				}
				TextEx( item.child.c_str( ), FindRenderedTextEnd( item.child.c_str( ) ) );

				item.code( );

				if ( i < res.size( ) - 1 )
					WidgetsManager::get( ).Separator( );
			}

			PopItemWidth( );
		}
		EndGroup( );
		PopStyleVar( );
	}

	static SearchManager& get( ) {
		static SearchManager s;
		return s;
	}
};