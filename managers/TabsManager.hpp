#pragma once

class TabsManager {
	float anim = 1.f, subtabs_anim = 1.f;
	float anim_dest = 1.f, subtabs_anim_dest = 1.f;

	struct Tab {
		const char* icon;
		const char* label;

		std::vector< const char* > subtabs;

		std::vector< std::function< void( ) > > pages{ };

		int next = 0;
		int cur = 0;
	};

	std::vector< Tab > tabs {
		{ i_target_01, "AimBot" },
		{ i_eye, "Visual" },
		{ i_dots_grid, "Miscellaneous" },
		{ i_folder, "Configs" },
	};
public:
	int current = 0, next = 0;

	bool tab( int i );
	bool subtab( int i );

	void render_tabs( float spacing, bool line = false );
	void render_subtabs( float spacing, bool line = true );
	void handle( );
	
	void add_page( int tab, std::function< void( ) > code );
	void draw_page( ImGuiWindow* window );

	float& get_subtabs_anim( ) {
		return subtabs_anim;
	}

	float& get_tabs_anim( ) {
		return anim;
	}

	float get_anim( ) {
		return subtabs_anim * anim;
	}

	Tab& get_tab( ) {
		return tabs[current];
	}

	Tab& get_tab( int i ) {
		return tabs[i];
	}

	static TabsManager& get( ) {
		static TabsManager s{ };
		return s;
	}
};