#include "Camera.h"

#include <glm/trigonometric.hpp>

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
        float rotate_in_radians_x = glm::radians(-m_Rotation_.x);
        glm::mat4 rotate_matrix_x(1, 0, 0, 0,
            0, cos(rotate_in_radians_x), sin(rotate_in_radians_x), 0,
            0, -sin(rotate_in_radians_x), cos(rotate_in_radians_x), 0,
            0, 0, 0, 1);

        float rotate_in_radians_y = glm::radians(-m_Rotation_.y);
        glm::mat4 rotate_matrix_y(cos(rotate_in_radians_y), 0, -sin(rotate_in_radians_y), 0,
            0, 1, 0, 0,
            sin(rotate_in_radians_y), 0, cos(rotate_in_radians_y), 0,
            0, 0, 0, 1);

        float rotate_in_radians_z = glm::radians(-m_Rotation_.z);
        glm::mat4 rotate_matrix(cos(rotate_in_radians_z), sin(rotate_in_radians_z), 0, 0,
            -sin(rotate_in_radians_z), cos(rotate_in_radians_z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        glm::mat4 translate_matrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -m_Position_[0], -m_Position_[1], -m_Position_[2], 1);

        m_ViewMatrix_ = rotate_matrix_y * rotate_matrix_x * translate_matrix;
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
}