#include "TextComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Transform.h" 
#include <SDL_ttf.h>
#include <stdexcept>
#include <iostream>

namespace dae
{
    TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font)
        : Component(owner), m_text(text), m_font(std::move(font)), m_needsUpdate(true)
    {
        m_owner = owner;  
    }

    void TextComponent::Update(float /*deltaTime*/)
    {
        if (m_needsUpdate)
        {
            const SDL_Color color = { 255, 255, 255, 255 };
            auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);

            auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);

            SDL_FreeSurface(surf);

            m_textTexture = std::make_shared<Texture2D>(texture);

            m_needsUpdate = false;
        }
    }

    void TextComponent::Render() const
    {
        if (!m_textTexture)
        {
            return;
        }

        if (!m_owner) return; 
        auto* transform = m_owner->GetComponent<Transform>();
        if (!transform)
        {
            return;
        }

        const auto& pos = transform->GetWorldPosition();

        Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
    }

    void TextComponent::SetText(const std::string& text)
    {
        m_text = text;
        m_needsUpdate = true;
    }

    void TextComponent::SetPosition(float x, float y)
    {
        if (!m_owner) return; 
        auto* transform = m_owner->GetComponent<Transform>();
        if (transform)
        {
            transform->SetPosition(x, y, 0.0f);
        }
    }
}









