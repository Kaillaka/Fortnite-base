#pragma once

class CompBuilder {
public:
	struct EmptyEnv {
		ImGuiID& id;
		bool hovered;
		bool held;
		bool pressed;
		const char* label;
	};

	void Empty( const char* label, ImRect total_bb, ImRect bb, std::function< void( const EmptyEnv& ) > code );

	struct ButtonEnv {
		ImRect bb;

		bool hovered;
		bool held;
		const char* label;
		
		struct Anim {
			float hover;
			float held;
			float anim;
		} anim;
	};

	bool Button( const char* label, ImVec2 size, std::function< void( const ButtonEnv& ) > code );

	struct SliderEnv {
		bool hovered;
		bool held;
		char* buf;
		const char* label;

		struct Anim {
			float hover;
			float held;
			float anim;
			float val_anim;
		} anim;
	};

	template < typename T >
	bool Slider( const char* label, T* v, T min, T max, const char* format, ImRect total_bb, ImRect bb, std::function< void( SliderEnv ) > code );
	bool SliderInt( const char* label, int* v, int min, int max, const char* format, ImRect total_bb, ImRect bb, std::function< void( SliderEnv ) > code );
	bool SliderFloat( const char* label, float* v, float min, float max, const char* format, ImRect total_bb, ImRect bb, std::function< void( SliderEnv ) > code );

	struct CheckboxEnv {
		bool hovered;
		bool held;
		bool pressed;
		const char* label;

		struct Anim {
			float hover;
			float held;
			float enabled;
			float anim;
		} anim;
	};

	bool Checkbox( const char* label, bool* v, int* key, float* col, std::function< void( ) > options, bool warning, ImRect total_bb, ImRect bb, ImVec2 options_pos, std::function< void( CheckboxEnv ) > code );

	struct ComboEnv {
		bool hovered;
		bool held;
		bool pressed;
		bool& open;
		const char* label;

		struct Anim {
			float hover;
			float held;
			float open;
			float anim;
		} anim;
	};

	bool Combo( const char* label, ImRect total_bb, ImRect bb, std::function< void( ComboEnv ) > code );

	struct SelectableEnv {
		bool hovered;
		bool held;
		bool pressed;
		const char* label;

		struct Anim {
			float hover;
			float held;
			float selected;
			float anim;
		} anim;
	};

	bool Selectable( const char* label, bool selected, ImRect bb, std::function< void( const SelectableEnv& ) > code );

	struct ColorEditEnv {
		bool hovered;
		bool held;
		bool pressed;
		bool& open;
		const char* label;

		struct Anim {
			float hover;
			float held;
			float open;
			float anim;
		} anim;
	};

	bool ColorEdit( const char* label, ImRect total_bb, ImRect bb, float col[4], std::function< void( ColorEditEnv ) > code );

	struct BinderEnv {
		ImRect total_bb;
		ImRect bb;

		bool hovered;
		bool held;
		bool pressed;
		bool active;
		const char* label;

		std::vector< const char* > keys;

		struct Anim {
			float hover;
			float held;
			float active;
			float anim;
		} anim;
	};

	bool Binder( const char* label, int* key, std::function< void( const BinderEnv& ) > code );

	struct OpenButtonEnv {
		ImRect bb;
		bool hovered;
		bool held;
		bool pressed;
		bool& open;

		struct Anim {
			float hover;
			float held;
			float anim;
			float open;
		} anim;
	};

	bool OpenButton( const char* str_id, ImVec2 size, std::function< void( OpenButtonEnv ) > code );

	static CompBuilder& get( ) {
		static CompBuilder s{ };
		return s;
	}
};