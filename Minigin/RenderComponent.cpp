#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"

namespace dae
{
    RenderComponent::RenderComponent(std::shared_ptr<GameObject> owner, const std::string& textureFile)
        : Component(owner)
    {
        SetTexture(textureFile);
    }

    void RenderComponent::Update()
    {
       
    }

    void RenderComponent::Render() const
    {
        if (m_texture)
        {
            auto* transform = m_pOwner->GetComponent<Transform>();
            if (transform)
            {
                const auto& pos = transform->GetPosition();
                Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
            }
        }
    }

    void RenderComponent::SetTexture(const std::string& filename)
    {
        m_texture = ResourceManager::GetInstance().LoadTexture(filename);
    }
}

