#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"
#include <iostream>
#include "AnimationComponent.h"
#include "DirectionComponent.h"


namespace dae
{
    RenderComponent::RenderComponent(GameObject* owner, const std::string& textureFile, int width, int height)
        : Component(owner), m_width(width), m_height(height)
    {
        SetTexture(textureFile);
    }

    void RenderComponent::Update(float /*deltaTime*/)
    {
        
    }

    void RenderComponent::Render() const
    {
        if (!m_texture) return;

        auto transform = m_pOwner->GetComponent<Transform>();
        if (!transform) return;

        const auto& pos = transform->GetWorldPosition();

        SDL_Rect dstRect{};
        dstRect.x = static_cast<int>(pos.x);
        dstRect.y = static_cast<int>(pos.y);
        dstRect.w = /*m_useSrcRect ? m_srcRect.w :*/ m_width;
        dstRect.h = /*m_useSrcRect ? m_srcRect.h : */m_height;

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        double angle = 0.0;

        //?
        auto dirComp = m_pOwner->GetComponent<DirectionComponent>();
        if (dirComp)
        {
            switch (dirComp->GetDirection())
            {
            case Direction::Left:
                flip = SDL_FLIP_HORIZONTAL;
                break;
            case Direction::Up:
                angle = -90.0;
                break;
            case Direction::Down:
                angle = 90.0;
                break;
            default:
                break;
            }
        }

        SDL_RenderCopyEx(
            Renderer::GetInstance().GetSDLRenderer(),
            m_texture->GetSDLTexture(),
            m_useSrcRect ? &m_srcRect : nullptr,
            &dstRect,
            angle,
            nullptr,
            flip
        );


    }




    void RenderComponent::SetTexture(const std::string& filename)
    {
        m_texture = ResourceManager::GetInstance().LoadTexture(filename);

        if (m_texture)
        {
            //m_texture->Resize(static_cast<int>(m_width), static_cast<int>(m_height));
        }
    }

    void RenderComponent::SetSourceRect(const SDL_Rect& rect)
    {
        m_srcRect = rect;
        m_useSrcRect = true;
    }

}