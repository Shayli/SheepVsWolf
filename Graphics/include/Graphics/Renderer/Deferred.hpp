#pragma once

#include <Graphics/Renderer/Renderer.hpp>
#include <Graphics/Framebuffer/Framebuffer.hpp>
#include <Graphics/Framebuffer/LightFramebuffer.hpp>
#include <Graphics/Framebuffer/PostFXFramebuffer.hpp>
#include <Graphics/Framebuffer/ShadowFramebuffer.hpp>

class Deferred : public Renderer
{
public:
	Deferred();
	void renderFrame();

protected:
	void onCameraChanged();

	void geometryPass();
	void lightPass();
	void shadowPass();
	void alphaPass();
	void postFXPass();

	Framebuffer frameBuffer;
	LightFramebuffer lightBuffer;
	PostFXFramebuffer postFxBuffer;
	ShadowFramebuffer shadowBuffer;
	Model3D screenQuad;
	Shader blit, compose;
	Shader directional, spot, point, sobel, shadow;
	uint32_t width, height;

};