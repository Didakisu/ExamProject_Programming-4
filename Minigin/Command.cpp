#include "Command.h"
#include "GameObject.h"
#include <iostream>

void dae::MoveCommand::Execute(float deltaTime)
{
	if (!m_pGameObject || !m_pGameObject->GetTransform()) return;

	auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();

	currentPos.x += m_Direction.x * m_Speed * deltaTime;
	currentPos.y += m_Direction.y * m_Speed * deltaTime;

	m_pGameObject->GetTransform()->SetLocalPosition(currentPos.x, currentPos.y, currentPos.z);
}
