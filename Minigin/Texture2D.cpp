#include <SDL.h>
#include <SDL_image.h>
#include "Texture2D.h"
#include "Renderer.h"
#include <stdexcept>
#include <iostream>

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 dae::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

dae::Texture2D::Texture2D(const std::string &fullPath)
{
	m_texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (m_texture == nullptr)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
}

dae::Texture2D::Texture2D(SDL_Texture* texture)	: m_texture{ texture } 
{
	assert(m_texture != nullptr);
}

void dae::Texture2D::Resize(int newWidth, int newHeight)
{
    auto renderer = Renderer::GetInstance().GetSDLRenderer();

    SDL_Texture* resizedTexture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        newWidth, newHeight);

    if (resizedTexture == nullptr)
        throw std::runtime_error("Failed to create resized texture: " + std::string(SDL_GetError()));

    SDL_SetTextureBlendMode(resizedTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, resizedTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
    SDL_RenderClear(renderer);

    SDL_Rect srcRect = { 0, 0, GetSize().x, GetSize().y };
    SDL_Rect dstRect = { 0, 0, newWidth, newHeight };
    if (SDL_RenderCopy(renderer, m_texture, &srcRect, &dstRect) != 0)
    {
        throw std::runtime_error("Failed to copy texture during resize: " + std::string(SDL_GetError()));
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_DestroyTexture(m_texture);
    m_texture = resizedTexture;
}

