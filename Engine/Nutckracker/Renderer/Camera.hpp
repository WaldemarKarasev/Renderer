#pragma once

#include "GameObject.hpp"

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include "KeyCodes.hpp"
#include <vector>
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
        Camera(const glm::vec3& position = { 0, 0, 0 },
               const glm::vec3& rotation = { 0, 0, 0 },
               const ProjectionMode projection_mode = ProjectionMode::Perspective);

        void SetOrthographicProjection(
            float left, float right, float top, float bottom, float near, float far);
        void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

        void SetViewDirection(
            glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
        void SetViewTarget(
            glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
        void SetCameraViewMatrix(glm::vec3 position, glm::vec3 rotation);
        void SetAspectRatio(float aspectRatio);

        const glm::mat4& GetProjectionMatrix();
        const glm::mat4& GetViewMatrix();
        const glm::mat4& GetInverseViewMatrix() { return m_InverseViewMatrix_; }
        const glm::vec3& GetCameraPosition()    { return glm::vec3(m_InverseViewMatrix_[3]); } // error
        const glm::vec3& GetCameraRotation()    { return m_Rotation_; }

        // This part is for moving camera with buttons in Sandbox Layer
        void MoveCameraInPlaneXZ(glm::vec3& rotate, glm::vec3& direction);
        
    private:
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();

    private:
        glm::mat4 m_ProjectionMatrix_{1.f};
        glm::mat4 m_ViewMatrix_{1.f};
        glm::mat4 m_InverseViewMatrix_{1.f};
        bool m_UpdateViewMatrix_ = false;
        bool m_UpdateProjectionMatrix_ = false;

        glm::vec3 m_Position_{}; // same as inverseViewMatrix.xyz
        glm::vec3 m_Rotation_{}; // X - Roll, Y - Pitch, Z - Yaw
        ProjectionMode m_ProjectionMode_ = ProjectionMode::Perspective;

        float m_FarClipPlane_{ 100.0f };
        float m_NearclipPlane_{ 0.1f };
        float m_Aspect_{1.0f};
        float m_FOV_{ 50.0f };

        float moveSpeed = 0.15f;
        float lookSpeed = 0.005f;
    };



}
