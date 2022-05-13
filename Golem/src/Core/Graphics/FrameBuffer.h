/*
 * FrameBuffer.h
 *
 *  Created on: 10-May-2022
 *      Author: anantha
 */

#ifndef CORE_GRAPHICS_FRAMEBUFFER_H_
#define CORE_GRAPHICS_FRAMEBUFFER_H_

class FrameBuffer
{
public:
	FrameBuffer();
	virtual ~FrameBuffer();

	void create(int t_width, int t_height);
	void resize(int t_width, int t_height);
	void bind();
	void unbind();
	void del();

	unsigned int getTexture() { return m_TextureObject; }
	unsigned int getWidth() { return m_Width; }
	unsigned int getHeight() { return m_Height; }

private:
	unsigned int m_FBO;
	unsigned int m_TextureObject;
	unsigned int m_RBO;
	unsigned int m_Width;
	unsigned int m_Height;
};

#endif /* CORE_GRAPHICS_FRAMEBUFFER_H_ */
