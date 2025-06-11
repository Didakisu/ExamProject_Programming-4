#include "VersusEnemyComponent.h"
#include <RenderComponent.h>


namespace dae
{

	VersusEnemyComponent::VersusEnemyComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap):
		Component(owner), m_Scene(scene), m_pTileMap(tileMap)
	{

	}

	VersusEnemyComponent::~VersusEnemyComponent()
	{

	}

	void VersusEnemyComponent::Initialize(const glm::vec3& startPosition)
	{
        m_SpawnPosition = startPosition;

		m_pTransform = GetOwner()->GetComponent<Transform>();
		m_pTransform->SetLocalPosition(startPosition.x, startPosition.y, startPosition.z);

		m_pDirectionComponent = GetOwner()->AddComponent<DirectionComponent>();
		GetOwner()->AddComponent<RenderComponent>("enemy_sprites.png", 32, 32);

		m_pAnimationComponent = GetOwner()->AddComponent<AnimationComponent>();
        m_pAnimationComponent->AddAnimation("Walk", "enemy_sprites.png", 64/4, 18, 4, 0.15f);
        m_pAnimationComponent->AddAnimation("Hit", "Explosion.png", 24 / 3, 8, 3, 0.25f , false , false);
        m_pAnimationComponent->AddAnimation("Enraged", "EnragedEnemies.png", 64 / 4, 15, 4, 0.15f);
        m_pAnimationComponent->PlayAnimation("Walk");
		m_pCollisionComponent = GetOwner()->AddComponent<CollisionComponent>(32.f, 32.f, &m_Scene);
	}

    void VersusEnemyComponent::Update(float deltaTime)
    {
        if (m_IsDead)
        {
            m_RespawnTimer += deltaTime;
            if (m_RespawnTimer >= m_DeathDuration)
            {
                Respawn();
            }
            return;
        }

        m_EnragedTimer += deltaTime;

        if (m_IsEnraged)
        {
            if (m_EnragedTimer >= m_EnrageDuration)
            {
                SetEnraged(false);
                m_EnragedTimer = 0.f;
            }
        }
        else
        {
            if (m_EnragedTimer >= m_TimeBetweenEnrages)
            {
                SetEnraged(true);
                m_EnragedTimer = 0.f;
            }
        }
    }

    void VersusEnemyComponent::Die()
    {
        if (m_IsDead) return;

        UnbindInput();
        m_pAnimationComponent->PlayAnimation("Hit");
        m_IsDead = true;
        m_ShouldRespawn = true;
        m_RespawnTimer = 0.f;
    }

    void VersusEnemyComponent::BindInput(const InputProfile& profile)
    {
        m_InputProfile = profile;
        m_InputsBound = true;

        auto& input = InputManager::GetInstance();

        {
            auto cmd = std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ 0, -1 }, m_pTileMap, m_Scene);
            cmd->SetRestrictMovement(!m_IsEnraged);
            input.BindKeyboardCommand(m_InputProfile.up, InputState::Pressed, std::move(cmd));
        }

        {
            auto cmd = std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ 0, 1 }, m_pTileMap, m_Scene);
            cmd->SetRestrictMovement(!m_IsEnraged);
            input.BindKeyboardCommand(m_InputProfile.down, InputState::Pressed, std::move(cmd));
        }

        {
            auto cmd = std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ -1, 0 }, m_pTileMap, m_Scene);
            cmd->SetRestrictMovement(!m_IsEnraged);
            input.BindKeyboardCommand(m_InputProfile.left, InputState::Pressed, std::move(cmd));
        }

        {
            auto cmd = std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ 1, 0 }, m_pTileMap, m_Scene);
            cmd->SetRestrictMovement(!m_IsEnraged);
            input.BindKeyboardCommand(m_InputProfile.right, InputState::Pressed, std::move(cmd));
        }
    }

    void VersusEnemyComponent::UnbindInput()
    {
        if (!m_InputsBound)
            return;

        auto& input = InputManager::GetInstance();

        input.UnbindKeyboardCommand(m_InputProfile.up);
        input.UnbindKeyboardCommand(m_InputProfile.down);
        input.UnbindKeyboardCommand(m_InputProfile.left);
        input.UnbindKeyboardCommand(m_InputProfile.right);

        m_InputsBound = false;
    }

    void VersusEnemyComponent::Respawn()
    {
        m_IsDead = false;
        m_RespawnTimer = 0.f;

        if (m_pTransform)
        {
            m_pTransform->SetLocalPosition(m_SpawnPosition.x , m_SpawnPosition.y , m_SpawnPosition.z);
        }

        m_pAnimationComponent->PlayAnimation("Walk");

        BindInput(m_InputProfile);

        m_ShouldRespawn = false;
    }

    void VersusEnemyComponent::SetEnraged(bool isEnraged)
    {
        m_IsEnraged = isEnraged;

        UnbindInput();
        BindInput(m_InputProfile);

        if (m_pAnimationComponent)
        {
            m_pAnimationComponent->PlayAnimation(isEnraged ? "Enraged" : "Walk");
        }
    }
}
