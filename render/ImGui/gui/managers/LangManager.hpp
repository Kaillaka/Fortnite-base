#pragma once

class LangManager {
	int lang = 0;

	struct Lang {
        const char* label;
        int font;
        std::unordered_map< std::string, std::string > dict{ };
    };

	std::vector< Lang > languages;
public:
	void add_language( const char* label, int font, std::unordered_map< std::string, std::string > dict ) {
        languages.push_back( Lang{ label, font, dict } );
    }

    int& get_lang( ) {
        return lang;
    }

    const char* get_lang_name( ) {
        return languages[lang].label;
    }

    std::vector< Lang >& get_langs( ) {
        return languages;
    }

    int get_font( ) {
        return ( int )languages[lang].font;
    }
    
    ImFont* get_font( int sz ) {
        return fonts[ ( int )languages[lang].font ].get( sz );
    }

    void set_lang( int i ) {
        lang = i;
    }

	static LangManager& get( ) {
		static LangManager s;
		return s;
	}

    const char* translate( const char* str ) {
        if ( lang == 0 )
            return str;

        auto it = languages[lang].dict.find( str );
        if ( it == languages[lang].dict.end( ) ) return str;

        return it->second.c_str( );
    }

    void initialize( ) {
        add_language( "English", font, { } );

    }
};