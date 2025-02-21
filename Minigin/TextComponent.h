#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include "Transform.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
    class TextComponent final : public Component
    {
    public:
        TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font);
        void Update() override;
        void Render() const override;

        void SetText(const std::string& text);
        void SetPosition(float x, float y);

    private:
        bool m_needsUpdate{ true };
        std::string m_text;
        std::shared_ptr<Font> m_font;
        std::shared_ptr<Texture2D> m_textTexture;
        GameObject* m_owner;  // raw pointer

        Transform* m_transform; // raw pointer
    };
}



