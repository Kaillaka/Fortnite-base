#pragma once

class GUI {
	ImVec2 size;
public:
	void initialize( ID3D11Device* device );
	void draw( );

	static GUI& get( ) {
		static GUI s{};
		return s;
	}
};