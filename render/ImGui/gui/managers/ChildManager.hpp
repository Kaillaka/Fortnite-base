#pragma once

class ChildManager {
	std::string current;
public:
	void beginchild( const char* label, ImVec2 size = ImVec2{ 0, 0 } );
	void endchild( );

	void smoothscroll( );

	std::string& get_current( ) {
		return current;
	}

	static ChildManager& get( ) {
		static ChildManager s{ };
		return s;
	}
};