#pragma once

class PopupManager {
	std::function< void( ) > popup;

	float anim;
	float anim_dest;
public:
	void open_popup( std::function< void( ) > code );
	void close_popup( );
	void handle( );

	static PopupManager& get( ) {
		static PopupManager s{ };
		return s;
	}
};