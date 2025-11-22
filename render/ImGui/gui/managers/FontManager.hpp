#pragma once

struct Font {
	std::vector< ImFont* > fonts;

	void setup( unsigned char* data, size_t data_size, std::vector< float > sizes, const ImWchar* ranges ) {
		auto config = ImFontConfig( );
		config.FontDataOwnedByAtlas = false;

		for ( auto& sz : sizes ) {
			fonts.push_back( ImGui::GetIO( ).Fonts->AddFontFromMemoryTTF( data, data_size, sz, &config, ranges ) );
		}
	}

	ImFont* get( float size ) {
		auto it = std::find_if( fonts.begin( ), fonts.end( ),  [&]( const ImFont* a ) {
			return a->FontSize == size;
		} );

		if ( it == fonts.end( ) ) {
			return nullptr;
		}

		return *it;
	}
};

class FontManager {
	std::vector< Font > fonts;
public:
	static FontManager& get( ) {
		static FontManager s{ };
		return s;
	}

	auto& get_fonts( ) {
		return fonts;
	}
};

enum fonts_ {
	font,
	icons,
	fonts_size
};

static std::vector< Font >& fonts = FontManager::get( ).get_fonts( );