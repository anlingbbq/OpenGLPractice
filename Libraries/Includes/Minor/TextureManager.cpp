//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#include <iostream>
#include "TextureManager.h"

TextureManager* TextureManager::m_inst(0);

unsigned int TextureManager::_autoTexId = 0;

TextureManager* TextureManager::Inst()
{
	if(!m_inst)
		m_inst = new TextureManager();

	return m_inst;
}

TextureManager::TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	//#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
	//#endif
}

//these should never be called
//TextureManager::TextureManager(const TextureManager& tm){}
//TextureManager& TextureManager::operator=(const TextureManager& tm){}
	
TextureManager::~TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	//#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
	//#endif

	UnloadAllTextures();
	m_inst = 0;
}

FIBITMAP* TextureManager::LoadFileBitMap(const char* filename)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return nullptr;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return nullptr;

	return dib;
}

int TextureManager::LoadTexture2D(std::string filename, GLint level, GLint border)
{
	FIBITMAP* dib = LoadFileBitMap(filename.c_str());
	if (dib == nullptr)
		return -1;

	//retrieve the image data
	BYTE* bits = FreeImage_GetBits(dib);
	//get the image width and height
	unsigned int width = FreeImage_GetWidth(dib);
	unsigned int height = FreeImage_GetHeight(dib);

	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return -1;

	GLint image_format;
	GLint internal_format;
	unsigned int bpp = FreeImage_GetBPP(dib);
	if (bpp == 32)
	{
		image_format = GL_BGRA;
		internal_format = GL_RGBA;
	}
	else if (bpp == 24)
	{
		image_format = GL_BGR;
		internal_format = GL_RGB;
	}
	else
	{
		printf("loadtexture error: unkonw bpp : %d\n", bpp);
		return -1;
	}

	//if this texture ID is in use, unload the current texture
	if (m_texID.find(++_autoTexId) != m_texID.end())
		glDeleteTextures(1, &(m_texID[_autoTexId]));

	//generate an OpenGL texture ID for this texture
	GLuint gl_texID;
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
	m_texID[_autoTexId] = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	// opengl texture setting
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, image_format == GL_BGRA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, image_format == GL_BGRA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//return success
	return _autoTexId;
}

int TextureManager::LoadCubemap(std::vector<std::string> faces)
{
	GLuint gl_texID;
	glGenTextures(1, &gl_texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, gl_texID);
	
	if (m_texID.find(++_autoTexId) != m_texID.end())
		glDeleteTextures(1, &(m_texID[_autoTexId]));

	m_texID[_autoTexId] = gl_texID;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		FIBITMAP* dib = LoadFileBitMap(faces[i].c_str());
		//retrieve the image data
		BYTE* bits = FreeImage_GetBits(dib);
		//get the image width and height
		unsigned int width = FreeImage_GetWidth(dib);
		unsigned int height = FreeImage_GetHeight(dib);
		if (dib != nullptr)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
				0, GL_BGR, GL_UNSIGNED_BYTE, bits);
		else
			printf("load texture error: failed to load at path : %s\n", faces[i].c_str());
		FreeImage_Unload(dib);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	return _autoTexId;
}

bool TextureManager::LoadTexture(const char* filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	FIBITMAP* dib = LoadFileBitMap(filename);
	if (dib == nullptr)
		return false;

	//retrieve the image data
	BYTE* bits = FreeImage_GetBits(dib);
	//get the image width and height
	unsigned int width = FreeImage_GetWidth(dib);
	unsigned int height = FreeImage_GetHeight(dib);

	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;
	
	//if this texture ID is in use, unload the current texture
	if(m_texID.find(texID) != m_texID.end())
		glDeleteTextures(1, &(m_texID[texID]));

	//generate an OpenGL texture ID for this texture
	GLuint gl_texID;
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
	m_texID[texID] = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	//return success
	return true;
}

bool TextureManager::UnloadTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if(m_texID.find(texID) != m_texID.end())
	{
		glDeleteTextures(1, &(m_texID[texID]));
		m_texID.erase(texID);
	}
	//otherwise, unload failed
	else
	{
		result = false;
	}

	return result;
}

bool TextureManager::BindTexture(const unsigned int texID)
{
	//printf("texID : %d/n", texID);
	bool result(true);
	//if this texture ID mapped, bind it's texture as current
	if(m_texID.find(texID) != m_texID.end())
		glBindTexture(GL_TEXTURE_2D, m_texID[texID]);
	//otherwise, binding failed

	return result;
}

void TextureManager::UnloadAllTextures()
{
	//start at the begginning of the texture map
	std::map<unsigned int, GLuint>::iterator i = m_texID.begin();

	//Unload the textures untill the end of the texture map is found
	while (i != m_texID.end()) {
			glDeleteTextures(1, &i->second);
			i = m_texID.erase(i);
	}

	//clear the texture map
	m_texID.clear();
}