#include "Camera.h"

#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace NK {

    Camera::Camera(const glm::vec3& position,
        const glm::vec3& rotation,
        const ProjectionMode projection_mode)
        : m_Position_(position)
        , m_Rotation_(rotation)
        , m_ProjectionMode_()
    {
        UpdateViewMatrix();
        UpdateProjectionMatrix();
    }

    void Camera::UpdateViewMatrix()
    {
        const float RollInRadians  = glm::radians(m_Rotation_.x);
        const float PitchInRadians = glm::radians(m_Rotation_.y);
        const float YawInRadians   = glm::radians(m_Rotation_.z);

        const glm::mat3 RotateMatrix_X(1, 0, 0,
            0, cos(RollInRadians), sin(RollInRadians),
            0, -sin(RollInRadians), cos(RollInRadians));

        const glm::mat3 RotateMatrix_Y(cos(PitchInRadians), 0, -sin(PitchInRadians),
            0, 1, 0,
            sin(PitchInRadians), 0, cos(PitchInRadians));

        const glm::mat3 RotateMatrix_Z(cos(YawInRadians), sin(YawInRadians), 0,
            -sin(YawInRadians), cos(YawInRadians), 0,
            0, 0, 1);

        const glm::mat3 EulerRotationMatrix = RotateMatrix_Z * RotateMatrix_Y * RotateMatrix_X;
        m_Direction_ = glm::normalize(EulerRotationMatrix * s_WorldForward_);
        m_Right_ = glm::normalize(EulerRotationMatrix * s_WorldRight_);
        m_Up_ = glm::cross(m_Right_, m_Direction_);

        m_ViewMatrix_ = glm::lookAt(m_Position_, m_Position_ + m_Direction_, m_Up_);
    }

    void Camera::UpdateProjectionMatrix()
    {
        if (m_ProjectionMode_ == ProjectionMode::Perspective)
        {
            float r = 0.1f;
            float t = 0.1f;
            float f = 10;
            float n = 0.1f;
            m_ProjectionMatrix_ = glm::mat4(n / r, 0, 0, 0,
                                            0, n / t, 0, 0,
                                            0, 0, (-f - n) / (f - n), -1,
                                            0, 0, -2 * f * n / (f - n), 0);
        }
        else
        {
            float r = 2;
            float t = 2;
            float f = 100;
            float n = 0.1f;
            m_ProjectionMatrix_ = glm::mat4(1 / r, 0, 0, 0,
                                            0, 1 / t, 0, 0,
                                            0, 0, -2 / (f - n), 0,
                                            0, 0, (-f - n) / (f - n), 1);
        }
    }

    void Camera::SetPositon(const glm::vec3& position)
    {
        m_Position_ = position;
        UpdateViewMatrix();
    }

    void Camera::SetRotation(const glm::vec3& rotation)
    {
        m_Rotation_ = rotation;
        UpdateViewMatrix();
    }

    void Camera::SetPositionRotation(const glm::vec3& position, const glm::vec3& rotation)
    {
        m_Position_ = position;
        m_Rotation_ = rotation;
        UpdateViewMatrix();
    }

    void Camera::SetPtojectionMode(const ProjectionMode projection_mode)
    {
        m_ProjectionMode_ = projection_mode;
        UpdateProjectionMatrix();
    }

    void Camera::MoveForward(const float delta)
    {
        m_Position_ += m_Direction_ * delta;
        UpdateViewMatrix();
    }
    void Camera::MoveRight(const float delta)
    {
        m_Position_ += m_Right_ * delta;
        UpdateViewMatrix();
    }
    void Camera::MoveUp(const float delta)
    {
        m_Position_ += m_Up_ * delta;
        UpdateViewMatrix();
    }

    void Camera::AddMovementAndRotation(const glm::vec3& movement_delta,
                                        const glm::vec3& rotation_delta)
    {
        m_Position_ += m_Direction_ * movement_delta.x;
        m_Position_ += m_Right_     * movement_delta.y;
        m_Position_ += m_Up_        * movement_delta.z;
        m_Rotation_ += rotation_delta;
        UpdateViewMatrix();                                                                                                                
    }
}