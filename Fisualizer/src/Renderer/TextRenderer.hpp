#pragma once

#include <string>
#include <gtc/matrix_transform.hpp>

#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.hpp"

#include "RenderingDefinitions.hpp"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer {
public:
	TextRenderer() = delete;
	
	TextRenderer(const std::string& fontPath, const Shader& shader);

	void changeFont(const std::string& fontPath);
	

	~TextRenderer();

	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);


private:
	FT_Library  library;
	FT_Face     face;

	std::string m_fontPath;
	std::unordered_map<GLchar, Character> Characters;

	Shader m_shader;
};




