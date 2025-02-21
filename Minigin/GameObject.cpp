#include "GameObject.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include <memory>
#include <iostream>


dae::GameObject::~GameObject()
{
    for (auto* component : m_components)
    {
        delete component;
    }
    m_components.clear();
}

void dae::GameObject::Update()
{
    for (auto& component : m_components)
    {
        component->Update();
    }
}

void dae::GameObject::FixedUpdate()
{
    for (auto& pComponent : m_components)
    {
        pComponent->FixedUpdate();
    }
}

void dae::GameObject::Render() const
{
    for (const auto& component : m_components)
    {
        component->Render();
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
        transform = AddComponent<dae::Transform>();  
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

bool dae::GameObject::IsChild(GameObject* parent) const 
{
    for (int i = 0; i <= m_children.size(); i++)
    {
        if (&m_children[i] == &parent)
        {
            return true;
        }
    }
    return false;
}

void dae::GameObject::RemoveChild(GameObject* child) {
    if (child) {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end()) {
            m_children.erase(it);  
        }
    }
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition = false) 
{
    if (IsChild(parent) || parent == this || m_parent == parent)
        return; 

    if (m_parent) 
    {
        m_parent->RemoveChild(this);  

        if (keepWorldPosition)
            SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
        SetPositionDirty();
    }

    m_parent = parent;

    if (m_parent) {
        m_parent->m_children.push_back(this); 
    }
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
    if (m_isPosDirty)
        UpdateWorldPosition();
    return m_worldPos;
}

void dae::GameObject::UpdateWorldPosition()
{
    if (m_isPosDirty)
    {
        if (m_parent == nullptr)
            m_worldPos = m_localPos;
        else
            m_worldPos = m_parent->GetWorldPosition() + m_localPos;
    }
    m_isPosDirty = false;
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
    m_localPos = pos;
    SetPositionDirty();
}

void dae::GameObject::SetPositionDirty()
{
    m_isPosDirty = true;
}






































//void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
//{
//    m_localPosition = pos;
//    SetPositionDirty();
//}
//
//const glm::vec3& dae::GameObject::GetWorldPosition()
//{
//    if (m_positionIsDirty)
//        UpdateWorldPosition();
//    return m_worldPosition;
//}
//void dae::GameObject::UpdateWorldPosition()
//{
//    if (m_positionIsDirty)
//    {
//        if (m_parent == nullptr)
//            m_worldPosition = m_localPosition;
//        else
//            m_worldPosition = m_parent->GetWorldPosition() + m_localPosition;
//    }
//    m_positionIsDirty = false;
//}
