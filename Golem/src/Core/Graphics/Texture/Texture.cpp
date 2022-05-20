/*
 * Texture.cpp
 *
 *  Created on: 10-Oct-2020
 *      Author: anantha
 */

#include "Texture.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>
#include <string>

#include <SOIL/SOIL.h>
#include <Utils/Debug.h>

Texture::Texture()
{
    // TODO Auto-generated constructor stub
}

Texture::~Texture()
{
    // TODO Auto-generated destructor stub
}

void Texture::load(const char *path)
{
    glGenTextures(1, &m_texture);
    // glActiveTexture(m_texture);
    // glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    Debug::log(std::to_string(glGetError()));

    // glActiveTexture(m_texture);
    // glEnable(GL_TEXTURE_2D);
    Debug::log(std::to_string(glGetError()));
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // stbi_convert_iphone_png_to_rgb(1);
    // stbi__vertically_flip_on_load_global = true;
    // stbi_set_flip_vertically_on_load(1);
    //  set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true);
    // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    //  The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    // unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    unsigned char *image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        Debug::log(std::to_string(glGetError()));
        Debug::log("Created Texture ", std::to_string(m_texture).c_str());
    }
    else
    {
        Debug::log("Failed to load texture");
    }

    SOIL_free_image_data(image);
    // stbi_image_free(data);
}

void Texture::create(int width, int height)
{

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture); // same as:- this->bind();

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Texture::resize(int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, m_texture); // same as:- this->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}
