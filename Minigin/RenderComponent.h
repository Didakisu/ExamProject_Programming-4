#pragma once
#include "Component.h"
#include <string>
#include "Texture2D.h"

namespace dae
{
    class RenderComponent final : public Component
    {
    public:
        explicit RenderComponent(GameObject* owner, const std::string& textureFile); 
        void Update() override;
        void Render() const override;

        void SetTexture(const std::string& filename);

    private:
        std::shared_ptr<Texture2D> m_texture; 
    };
}


