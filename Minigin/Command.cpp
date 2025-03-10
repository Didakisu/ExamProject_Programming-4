#include "Command.h"
#include "GameObject.h"
#include <iostream>

using namespace dae;

void MoveUpCommand::Execute(float deltaTime)
{
    if (m_pGameObject && m_pGameObject->GetTransform())
    {
        auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();
        currentPos.y -= m_Speed * deltaTime;
        m_pGameObject->GetTransform()->SetLocalPosition(currentPos.x , currentPos.y , currentPos.z);
    }
}

void MoveDownCommand::Execute(float deltaTime)
{
    if (m_pGameObject && m_pGameObject->GetTransform())
    {
        auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();
        currentPos.y += m_Speed * deltaTime;
        m_pGameObject->GetTransform()->SetLocalPosition(currentPos.x, currentPos.y, currentPos.z);
    }
}

void MoveLeftCommand::Execute(float deltaTime)
{
    if (m_pGameObject && m_pGameObject->GetTransform())
    {
        auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();
        currentPos.x -= m_Speed * deltaTime;
        m_pGameObject->GetTransform()->SetLocalPosition(currentPos.x, currentPos.y, currentPos.z);
    }
}

void MoveRightCommand::Execute(float deltaTime)
{
    if (m_pGameObject && m_pGameObject->GetTransform())
    {
        auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();
        currentPos.x += m_Speed * deltaTime;
        m_pGameObject->GetTransform()->SetLocalPosition(currentPos.x, currentPos.y, currentPos.z);
    }
}
