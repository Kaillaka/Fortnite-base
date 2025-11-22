#pragma once

class WidgetsManager {
public:
	bool Checkbox( const char* label, bool* v, int* key = nullptr, float* col = nullptr, std::function< void( ) > options = nullptr, bool warning = false );
	template < typename T >
	bool Slider( const char* label, T* v, T min, T max, const char* format );
	bool SliderInt( const char* label, int* v, int min, int max, const char* format = "%d" );
	bool SliderFloat( const char* label, float* v, float min, float max, const char* format = "%.1f" );
	bool ComboEx( const char* label, const char* preview_value, std::function< void( CompBuilder::ComboEnv env ) > code );
	bool Combo( const char* label, int* v, std::vector< const char* > items );
	bool MultiCombo( const char* label, bool* v, std::vector< const char* > items );
	bool Binder( const char* label, int* key );
	bool TextField( const char* label, char* buf, size_t buf_size, ImVec2 size = ImVec2{ 0, 0 }, const char* hint = 0, const char* icon = 0 );
	bool Button( const char* label, ImVec2 size = ImVec2{ 0, 0 } );
	bool ColorEdit( const char* label, float col[4] );
	bool Selectable( const char* label, bool selected, ImVec2 size = ImVec2{ 0, 0 } );
	void Separator( );

	static WidgetsManager& get( ) {
		static WidgetsManager s{ };
		return s;
	}
};