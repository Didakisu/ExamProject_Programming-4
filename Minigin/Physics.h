#include "GameObject.h"
#include "Scene.h"

namespace dae
{
    class Physics
    {
    public:
        static bool CanMoveTo(GameObject* movingObj, glm::vec2 newPos, Scene* scene);
    };
}
