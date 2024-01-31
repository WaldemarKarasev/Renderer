#pragma once

#include "GameObject.hpp"

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace NK {
    
    class Camera
    {
    public:
        enum class ProjectionMode
        {
            Perspective,
            Orthographic
        };

        public:
        Camera()
        {
            m_transform_.translation.z = -2.5f;
        }

        void SetOrthographicProjection(
            float left, float right, float top, float bottom, float near, float far);
        void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

        void SetViewDirection(
            glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
        void SetViewTarget(
            glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
        void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);
        

        const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix_; }
        const glm::mat4& GetViewMatrix() const { return m_viewMatrix_; }
        const glm::mat4& GetInverseViewMatrix() const { return m_inverseViewMatrix_; }
        const glm::vec3  GetPosition() const { return glm::vec3(m_inverseViewMatrix_[3]); }


        private:
        glm::mat4 m_projectionMatrix_{1.f};
        glm::mat4 m_viewMatrix_{1.f};
        glm::mat4 m_inverseViewMatrix_{1.f};

        // This part is for moving camera with buttons in Sandbox Layer
        public: 
        void MoveInPlaneXZ(glm::vec3& rotate, glm::vec3& moveDir);
        glm::vec3 GetCameraTranslation() { return m_transform_.translation; }
        glm::vec3 GetCameraRotation()    { return m_transform_.rotation; }

        private:
        TransformComponent m_transform_;
        float moveSpeed = 1.5f;
        float lookSpeed = 1.5f;
    };


}
