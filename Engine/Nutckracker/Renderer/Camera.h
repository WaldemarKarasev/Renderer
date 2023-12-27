#pragma once

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

        Camera(const glm::vec3& position = { 0, 0, 0 },
               const glm::vec3& rotation = { 0, 0, 0 },
               const ProjectionMode projection_mode = ProjectionMode::Perspective);

        void SetPositon(const glm::vec3& position);
        void SetRotation(const glm::vec3& rotation);
        void SetPositionRotation(const glm::vec3& position, const glm::vec3& rotation);
        void SetPtojectionMode(const ProjectionMode projection_mode);
        
        glm::mat4 GetViewMatrix() const { return m_ViewMatrix_; }
        glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix_; }

        void MoveForward(const float delta);
        void MoveRight(const float delta);
        void MoveUp(const float delta);

        const glm::vec3& GetCameraPosition() const { return m_Position_; }
        const glm::vec3& GetCameraRotation() const { return m_Rotation_; }

        // movement_delta.x - forward, movement_delta.y - right, movement_delta.z - up
        // rotation_delta.x - roll, rotation_delta.y - pitch, rotation_delta.z - yaw
        void AddMovementAndRotation(const glm::vec3& movement_delta,
                                         const glm::vec3& rotation_delta);

    private:
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();

        glm::vec3 m_Position_;
        glm::vec3 m_Rotation_; // X - Roll, Y - Pitch, Z - Yaw
        ProjectionMode m_ProjectionMode_;

        glm::vec3 m_Direction_;
        glm::vec3 m_Right_;
        glm::vec3 m_Up_;

        static constexpr glm::vec3 s_WorldUp_{ 0.f, 0.f, 1.f };
        static constexpr glm::vec3 s_WorldRight_{ 0.f, -1.f, 0.f };
        static constexpr glm::vec3 s_WorldForward_{ 1.f, 0.f, 0.f };

        glm::mat4 m_ViewMatrix_;
        glm::mat4 m_ProjectionMatrix_;

        bool m_UpdateViewMatrix_ = false;
    };

}
