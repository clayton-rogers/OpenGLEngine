#pragma once

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <SOIL.h>


struct FontCharacter {
	int id;
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
	int page;
	int chnl;
};

struct CornerUV {
	glm::vec2 lowerLeft;
	glm::vec2 upperRight;
};

class Font {

	unsigned char* mImageData = nullptr;
	int mImageSize;
	glm::ivec2 mImageDimentions;
	int invalidCharIndex;
	std::vector<FontCharacter> mCharacters;

	static const int INVALID_CHAR_ID;

public:

	Font(std::string texturePath, std::string fontmapPath) {
		// *** Load the texture *** //
		mImageData = SOIL_load_image(texturePath.c_str(), &mImageDimentions.x, &mImageDimentions.y, 0, SOIL_LOAD_RGBA);
		mImageSize = mImageDimentions.x * mImageDimentions.y * 4; // RGBA byte compoments = 4
		if (!mImageData || *mImageData == '\0') {
			std::cout << "Failed to load font texture: " << texturePath << std::endl;
		}

		// *** Load the font map *** //
		std::ifstream fontmapFile;
		try {
			fontmapFile.open(fontmapPath);
			if (fontmapFile.fail()) {
				std::cout << "Failed to open font map file: " << fontmapPath << std::endl;
			}

			while (!(fontmapFile.eof() || fontmapFile.fail())) {
				std::string line;
				std::getline(fontmapFile, line);
				FontCharacter c;

				int scanRet = sscanf_s(line.c_str(),
					"char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d",
					&c.id, &c.x, &c.y, &c.width, &c.height, &c.xoffset, &c.yoffset, &c.xadvance, &c.page, &c.chnl);
				if (scanRet != 10) {
					// 10 elements are read from the line. If less than that are read, then there was some
					// kind of failure, so discard the whole line.
					if (scanRet != 0) {
						std::cout << "Failure reading font map line. Return from sscanf: " << scanRet << std::endl;
						std::cout << "Discarding line: " << line << std::endl;
					}
				} else {
					mCharacters.push_back(c);
					if (c.id == INVALID_CHAR_ID) {
						invalidCharIndex = mCharacters.size() - 1;
					}
				}
			}

		} catch (std::ifstream::failure e) {
			std::cout << "Font map file failure: " << e.what() << std::endl;
		}
	}

	~Font()	{
		SOIL_free_image_data(mImageData);
	}

	void uploadTextureToGPU(GLuint& textureIndex) {

		glGenTextures(1, &textureIndex);
		glBindTexture(GL_TEXTURE_2D, textureIndex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mImageDimentions.x, mImageDimentions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	CornerUV getUVs(unsigned char inputChar) {

		unsigned int index = invalidCharIndex;

		for (unsigned int i = 0; i < mCharacters.size(); i++) {
			if (mCharacters[i].id == inputChar) {
				index = i;
				break;
			}
		}

		CornerUV uv;
		uv.lowerLeft.x = float(mCharacters[index].x) / mImageDimentions.x;
		uv.lowerLeft.y = float(mCharacters[index].y + mCharacters[index].height) / mImageDimentions.y;

		uv.upperRight.x = float(mCharacters[index].x + mCharacters[index].width ) / mImageDimentions.x;
		uv.upperRight.y = float(mCharacters[index].y) / mImageDimentions.y;

		return uv;
	}

	FontCharacter getFontCharacter(unsigned char inputChar) {
		unsigned int index = invalidCharIndex;

		for (unsigned int i = 0; i < mCharacters.size(); i++) {
			if (mCharacters[i].id == inputChar) {
				index = i;
				break;
			}
		}

		return mCharacters[index];
	}

	unsigned char* getTexture() {
		return mImageData;
	}

	int getImageSize() {
		return mImageSize;
	}

};


const int Font::INVALID_CHAR_ID = 127; // For VCR-OSD font map this gives a box with X