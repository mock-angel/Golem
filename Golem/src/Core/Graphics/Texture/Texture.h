/*
 * Texture.h
 *
 *  Created on: 10-Oct-2020
 *      Author: anantha
 */

#ifndef GRAPHICS_TEXTURES_TEXTURE_H_
#define GRAPHICS_TEXTURES_TEXTURE_H_

class Texture
{

public:
    Texture();
    virtual ~Texture();

    void load(const char *str);
    void create(int width, int height);
    void resize(int width, int height);
    inline unsigned int get()
    {
        return m_texture;
    }

    // TODO DEPRECATED:: remove this. IMplemented in Camera
    inline void setTexture(unsigned int value) { m_texture = value; }

    void bind();

private:
    unsigned int m_texture;
};

#endif /* GRAPHICS_TEXTURES_TEXTURE_H_ */
