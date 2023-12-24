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

    private:
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();

        glm::vec3 m_Position_;
        glm::vec3 m_Rotation_;
        ProjectionMode m_ProjectionMode_;
        glm::mat4 m_ViewMatrix_;
        glm::mat4 m_ProjectionMatrix_;

    };

}
