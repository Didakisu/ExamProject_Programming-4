#include "GameObject.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include <memory>
#include <iostream>


void dae::GameObject::Update()
{
    for (auto& component : m_components)
    {
        component->Update();
    }
}

void dae::GameObject::FixedUpdate()     //added
{
    for (auto& pComponent : m_components)
    {
        pComponent->FixedUpdate();
    }
}

void dae::GameObject::Render() const
{
    auto* renderComponent = GetComponent<dae::RenderComponent>();
    auto* textComponent = GetComponent<dae::TextComponent>();

    if (renderComponent)
    {
        renderComponent->Render(); 
    }

    if (textComponent)
    {
        textComponent->Render();
    }
}


dae::Transform* dae::GameObject::GetTransform()
{
    return GetComponent<Transform>(); 
}

glm::vec3 dae::GameObject::GetPosition() const
{
    auto* transform = GetComponent<Transform>();
    return transform ? transform->GetPosition() : glm::vec3{ 0, 0, 0 };
}

void dae::GameObject::SetPosition(float x, float y)
{
    auto* transform = GetComponent<dae::Transform>(); 
    if (!transform)
    {
        transform = AddComponent<dae::Transform>().get(); 
    }
    transform->SetPosition(x, y, 0.0f); 
}

void dae::GameObject::SetTexture(const std::string& filename)
{
    if (!HasComponent<RenderComponent>())
    {
        AddComponent<RenderComponent>(filename);
    }
    else
    {
        auto* renderComponent = GetComponent<RenderComponent>();
        if (renderComponent)
        {
            renderComponent->SetTexture(filename);
        }
    }
}
