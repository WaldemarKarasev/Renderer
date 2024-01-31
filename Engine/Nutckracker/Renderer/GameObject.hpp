#pragma once

#include "Model.hpp"

// lib
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>

namespace NK {

    struct TransformComponent
    {
        glm::vec3 translation{}; // (position offset)
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};
        
        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();

        glm::mat3 normalMatrix();
    };

    struct PointLightComponent
    {
        float lightIntensity = 1.0f;
    };


    class GameObject
    {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;

        static GameObject CreateGameObject()
        {
            static id_t currentID = 0;
            return GameObject{currentID++};
        }

        static GameObject MakePointLight(
            float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject& operator=(GameObject&&) = default;

        id_t GetId() const noexcept { return m_ID_; }

        glm::vec3 m_color_{};
        TransformComponent m_transform_{};

        // Optional pointer components
        std::shared_ptr<Model> m_Model_ = nullptr;
        std::unique_ptr<PointLightComponent> m_PointLight_ = nullptr;

    private:
        GameObject(id_t ID) : m_ID_{ID} {}

        uint32_t m_ID_;
    };

}
