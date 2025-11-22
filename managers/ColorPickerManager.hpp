#pragma once

class ColorPickerManager {
	std::vector< ImColor > saved_colors;
public:
	bool HueBar( float& h, float s, float v );
	bool AlphaBar( float& a );
	bool Square( float col[4], float h, float& s, float& v );
	bool draw( const char* label, float col[4] );

	static ColorPickerManager& get( ) {
		static ColorPickerManager s{ };
		return s;
	}
};