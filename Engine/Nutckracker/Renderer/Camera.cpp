#include "Camera.hpp"

#include <glm/trigonometric.hpp>
//#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NK {

    Camera::Camera(const glm::vec3& position,
               const glm::vec3& rotation,
               const ProjectionMode projection_mode)
    : m_Position_(position)
    , m_Rotation_(rotation)
    , m_ProjectionMode_(projection_mode)

    {
        m_Position_.z = -5.0f;
        //UpdateViewMatrix();
        UpdateProjectionMatrix();
        SetCameraViewMatrix(m_Position_, m_Rotation_);
        //SetPerspectiveProjection(glm::radians(50.f), 2, 0.1f, 100.f);
    }

    void Camera::SetOrthographicProjection(
    float left, float right, float top, float bottom, float near, float far) 
    {
        m_ProjectionMatrix_ = glm::mat4{1.0f};
        m_ProjectionMatrix_[0][0] = 2.f / (right - left);
        m_ProjectionMatrix_[1][1] = 2.f / (bottom - top);
        m_ProjectionMatrix_[2][2] = 1.f / (far - near);
        m_ProjectionMatrix_[3][0] = -(right + left) / (right - left);
        m_ProjectionMatrix_[3][1] = -(bottom + top) / (bottom - top);
        m_ProjectionMatrix_[3][2] = -near / (far - near);
    }

    void Camera::SetPerspectiveProjection(float fovy, float aspect, float near, float far) 
    {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovy = tan(fovy / 2.f);
        m_ProjectionMatrix_ = glm::mat4{0.0f};
        m_ProjectionMatrix_[0][0] = 1.f / (aspect * tanHalfFovy);
        m_ProjectionMatrix_[1][1] = 1.f / (tanHalfFovy);
        m_ProjectionMatrix_[2][2] = far / (far - near);
        m_ProjectionMatrix_[2][3] = 1.f;
        m_ProjectionMatrix_[3][2] = -(far * near) / (far - near);
    }

    void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) 
    {
        const glm::vec3 w{glm::normalize(direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, up))};
        const glm::vec3 v{glm::cross(w, u)};

        m_ViewMatrix_ = glm::mat4{1.f};
        m_ViewMatrix_[0][0] = u.x;
        m_ViewMatrix_[1][0] = u.y;
        m_ViewMatrix_[2][0] = u.z;
        m_ViewMatrix_[0][1] = v.x;
        m_ViewMatrix_[1][1] = v.y;
        m_ViewMatrix_[2][1] = v.z;
        m_ViewMatrix_[0][2] = w.x;
        m_ViewMatrix_[1][2] = w.y;
        m_ViewMatrix_[2][2] = w.z;
        m_ViewMatrix_[3][0] = -glm::dot(u, position);
        m_ViewMatrix_[3][1] = -glm::dot(v, position);
        m_ViewMatrix_[3][2] = -glm::dot(w, position);

        m_InverseViewMatrix_ = glm::mat4{1.f};
        m_InverseViewMatrix_[0][0] = u.x;
        m_InverseViewMatrix_[0][1] = u.y;
        m_InverseViewMatrix_[0][2] = u.z;
        m_InverseViewMatrix_[1][0] = v.x;
        m_InverseViewMatrix_[1][1] = v.y;
        m_InverseViewMatrix_[1][2] = v.z;
        m_InverseViewMatrix_[2][0] = w.x;
        m_InverseViewMatrix_[2][1] = w.y;
        m_InverseViewMatrix_[2][2] = w.z;
        m_InverseViewMatrix_[3][0] = position.x;
        m_InverseViewMatrix_[3][1] = position.y;
        m_InverseViewMatrix_[3][2] = position.z;
    }
    
    void Camera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) 
    {
        SetViewDirection(position, target - position, up);
    }

    void Camera::SetCameraViewMatrix(glm::vec3 position, glm::vec3 rotation) 
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
        const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
        const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
        m_ViewMatrix_ = glm::mat4{1.f};
        m_ViewMatrix_[0][0] = u.x;
        m_ViewMatrix_[1][0] = u.y;
        m_ViewMatrix_[2][0] = u.z;
        m_ViewMatrix_[0][1] = v.x;
        m_ViewMatrix_[1][1] = v.y;
        m_ViewMatrix_[2][1] = v.z;
        m_ViewMatrix_[0][2] = w.x;
        m_ViewMatrix_[1][2] = w.y;
        m_ViewMatrix_[2][2] = w.z;
        m_ViewMatrix_[3][0] = -glm::dot(u, position);
        m_ViewMatrix_[3][1] = -glm::dot(v, position);
        m_ViewMatrix_[3][2] = -glm::dot(w, position);

        m_InverseViewMatrix_ = glm::mat4{1.f};
        m_InverseViewMatrix_[0][0] = u.x;
        m_InverseViewMatrix_[0][1] = u.y;
        m_InverseViewMatrix_[0][2] = u.z;
        m_InverseViewMatrix_[1][0] = v.x;
        m_InverseViewMatrix_[1][1] = v.y;
        m_InverseViewMatrix_[1][2] = v.z;
        m_InverseViewMatrix_[2][0] = w.x;
        m_InverseViewMatrix_[2][1] = w.y;
        m_InverseViewMatrix_[2][2] = w.z;
        m_InverseViewMatrix_[3][0] = position.x;
        m_InverseViewMatrix_[3][1] = position.y;
        m_InverseViewMatrix_[3][2] = position.z;
    }

    void Camera::SetAspectRatio(float aspectRatio)
    {
        m_Aspect_ = aspectRatio;
        m_UpdateProjectionMatrix_ = true;
    }


    void Camera::MoveCameraInPlaneXZ(glm::vec3& rotate, glm::vec3& direction)
    {
        //glm::vec3 rotate{0};

        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
        {
            //m_transform_.rotation += lookSpeed * dt * glm::normalize(rotate);
            m_Rotation_ += lookSpeed * glm::normalize(rotate);
        }
        
        // limits pitch values between about +/- 85ish degrees
        m_Rotation_.x = glm::clamp(m_Rotation_.x, -1.5f, 1.5f);
        m_Rotation_.y = glm::mod(m_Rotation_.y, glm::two_pi<float>());

        float yaw = m_Rotation_.y;

        const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
        const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
        const glm::vec3 upDir{0.f, -1.f, 0.f};
       
        glm::vec3 moveDir{0.f};
        moveDir += forwardDir * direction.x;
        moveDir += rightDir   * direction.y;
        moveDir += upDir      * direction.z;

        if(glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
        {
            //m_transform_.translation += moveSpeed * dt * glm::normalize(moveDir);
            m_Position_ += moveSpeed * glm::normalize(moveDir);
        }   
        m_UpdateViewMatrix_ = true;
    }

    const glm::mat4& Camera::GetProjectionMatrix() 
    { 
        if (m_UpdateProjectionMatrix_)
        {
            UpdateProjectionMatrix();
        }

        return m_ProjectionMatrix_; 
    }
    const glm::mat4& Camera::GetViewMatrix()  
    { 
        if (m_UpdateViewMatrix_)
        {
            UpdateViewMatrix();
        }

        return m_ViewMatrix_; 
    }

    void Camera::UpdateProjectionMatrix() 
    {
        if (m_ProjectionMode_ == ProjectionMode::Perspective)
        {
            SetPerspectiveProjection(m_FOV_, m_Aspect_, m_NearclipPlane_, m_FarClipPlane_);
            //m_ProjectionMatrix_ = glm::perspective(m_FOV_, m_Aspect_, m_NearclipPlane_, m_FarClipPlane_);
        }
        else
        {
            //SetOrthographicProjection(2, 2, 2, 2, 1, 10);
            float r = 2;
            float t = 2;
            float f = 100;
            float n = 0.1f;
            m_ProjectionMatrix_ = glm::mat4(1 / r, 0, 0, 0,
                                            0, 1 / t, 0, 0,
                                            0, 0, -2 / (f - n), 0,
                                            0, 0, (-f - n) / (f - n), 1);
        }

        m_UpdateProjectionMatrix_ = false;
    }

    void Camera::UpdateViewMatrix() 
    {
        SetCameraViewMatrix(m_Position_, m_Rotation_);
        m_UpdateViewMatrix_ = false;
    }

}