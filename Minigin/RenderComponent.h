#pragma once
#include "Component.h"
#include <string>
#include "Texture2D.h"
#include <memory>
#include <SDL_rect.h>
   
namespace dae
{
    class RenderComponent final : public Component
    {
    public:
        explicit RenderComponent(GameObject* owner, const std::string& textureFile, int width, int height);
        void Update(float deltaTime) override;
        void Render() const override;

        void SetTexture(const std::string& filename);

        //
        void SetSourceRect(const SDL_Rect& rect);
        std::shared_ptr<Texture2D> GetTexture() const { return m_texture;}

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

    private:
        std::shared_ptr<Texture2D> m_texture; 
        int m_width; 
        int m_height; 

        //
        SDL_Rect m_srcRect{};
        bool m_useSrcRect{ false };
    };
}


    
