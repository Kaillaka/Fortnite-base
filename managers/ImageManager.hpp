#pragma once

class ImageManager {
	ID3D11ShaderResourceView* image = nullptr;
public:
	void load( unsigned char* data, size_t data_size, ID3D11Device* device ) {
		if ( image )
			return;

		D3DX11CreateShaderResourceViewFromMemory( device, data, data_size, 0, 0, &image, 0 );
	}

	ID3D11ShaderResourceView*& img( ) {
		return image;
	}

	static ImageManager& get( ) {
		static ImageManager s{ };
		return s;
	}
};