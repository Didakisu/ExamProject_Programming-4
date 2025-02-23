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


// week 02
bool dae::GameObject::IsChild(GameObject* parent) const
{
    for (GameObject* child : m_children)
    {
        if (child == parent)
        {
            return true;
        }
    }
    return false;
}

dae::GameObject* dae::GameObject::GetParent() const
{
    return m_parent;  
}

void dae::GameObject::RemoveChild(GameObject* child) {
    if (!child) return;

    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        m_children.erase(it);
        child->m_parent = nullptr;
    }
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
    if (IsChild(parent) || parent == this || m_parent == parent)
        return;

    glm::vec3 originalWorldPosition{};
    if (keepWorldPosition)
        originalWorldPosition = GetTransform()->GetWorldPosition();

    if (m_parent)
    {
        m_parent->RemoveChild(this);  
    }

    m_parent = parent;

    if (parent)
    {
        parent->m_children.push_back(this); 
    }

    if (parent && keepWorldPosition)
    {
        glm::vec3 parentWorldPosition = parent->GetTransform()->GetWorldPosition();
        glm::vec3 newLocalPosition = originalWorldPosition - parentWorldPosition;

        GetTransform()->SetLocalPosition(newLocalPosition.x , newLocalPosition.y , newLocalPosition.z);
    }
    else
    {
        GetTransform()->SetPositionDirty();
    }
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
