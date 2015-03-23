#include <Graphics/Material.hpp>
#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>

struct SdlImageHelper {
	SDL_Surface* data;

	SdlImageHelper(const std::string& file)
	{
		data = SDL_LoadBMP(file.c_str());
		if(data == NULL)
			std::cerr << "Unable to open image: " << SDL_GetError() << std::endl;
	}

	~SdlImageHelper()
	{
		SDL_FreeSurface(data);
	}
};

Material::Material() {
	glGenTextures(1, &m_id);
}

Material::~Material() {
	glDeleteTextures(1, &m_id);
}

bool Material::loadFromFile(const std::string& str) {
	SdlImageHelper image(str);
	if(!image.data)
		return false;
	GLuint format = GL_BGR;

	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.data->w, image.data->h, 0, format, GL_UNSIGNED_BYTE, image.data->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

GLuint Material::getId() const {
	return m_id;
}