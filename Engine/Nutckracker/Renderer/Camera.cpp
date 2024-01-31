#include "Camera.hpp"

#include <glm/trigonometric.hpp>
//#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NK {

    void Camera::SetOrthographicProjection(
    float left, float right, float top, float bottom, float near, float far) 
    {
        m_projectionMatrix_ = glm::mat4{1.0f};
        m_projectionMatrix_[0][0] = 2.f / (right - left);
        m_projectionMatrix_[1][1] = 2.f / (bottom - top);
        m_projectionMatrix_[2][2] = 1.f / (far - near);
        m_projectionMatrix_[3][0] = -(right + left) / (right - left);
        m_projectionMatrix_[3][1] = -(bottom + top) / (bottom - top);
        m_projectionMatrix_[3][2] = -near / (far - near);
    }

    void Camera::SetPerspectiveProjection(float fovy, float aspect, float near, float far) 
    {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovy = tan(fovy / 2.f);
        m_projectionMatrix_ = glm::mat4{0.0f};
        m_projectionMatrix_[0][0] = 1.f / (aspect * tanHalfFovy);
        m_projectionMatrix_[1][1] = 1.f / (tanHalfFovy);
        m_projectionMatrix_[2][2] = far / (far - near);
        m_projectionMatrix_[2][3] = 1.f;
        m_projectionMatrix_[3][2] = -(far * near) / (far - near);
    }

    void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) 
    {
        const glm::vec3 w{glm::normalize(direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, up))};
        const glm::vec3 v{glm::cross(w, u)};

        m_viewMatrix_ = glm::mat4{1.f};
        m_viewMatrix_[0][0] = u.x;
        m_viewMatrix_[1][0] = u.y;
        m_viewMatrix_[2][0] = u.z;
        m_viewMatrix_[0][1] = v.x;
        m_viewMatrix_[1][1] = v.y;
        m_viewMatrix_[2][1] = v.z;
        m_viewMatrix_[0][2] = w.x;
        m_viewMatrix_[1][2] = w.y;
        m_viewMatrix_[2][2] = w.z;
        m_viewMatrix_[3][0] = -glm::dot(u, position);
        m_viewMatrix_[3][1] = -glm::dot(v, position);
        m_viewMatrix_[3][2] = -glm::dot(w, position);

        m_inverseViewMatrix_ = glm::mat4{1.f};
        m_inverseViewMatrix_[0][0] = u.x;
        m_inverseViewMatrix_[0][1] = u.y;
        m_inverseViewMatrix_[0][2] = u.z;
        m_inverseViewMatrix_[1][0] = v.x;
        m_inverseViewMatrix_[1][1] = v.y;
        m_inverseViewMatrix_[1][2] = v.z;
        m_inverseViewMatrix_[2][0] = w.x;
        m_inverseViewMatrix_[2][1] = w.y;
        m_inverseViewMatrix_[2][2] = w.z;
        m_inverseViewMatrix_[3][0] = position.x;
        m_inverseViewMatrix_[3][1] = position.y;
        m_inverseViewMatrix_[3][2] = position.z;
    }
    
    void Camera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) 
    {
        SetViewDirection(position, target - position, up);
    }

    void Camera::SetViewYXZ(glm::vec3 position, glm::vec3 rotation) 
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
        m_viewMatrix_ = glm::mat4{1.f};
        m_viewMatrix_[0][0] = u.x;
        m_viewMatrix_[1][0] = u.y;
        m_viewMatrix_[2][0] = u.z;
        m_viewMatrix_[0][1] = v.x;
        m_viewMatrix_[1][1] = v.y;
        m_viewMatrix_[2][1] = v.z;
        m_viewMatrix_[0][2] = w.x;
        m_viewMatrix_[1][2] = w.y;
        m_viewMatrix_[2][2] = w.z;
        m_viewMatrix_[3][0] = -glm::dot(u, position);
        m_viewMatrix_[3][1] = -glm::dot(v, position);
        m_viewMatrix_[3][2] = -glm::dot(w, position);

        m_inverseViewMatrix_ = glm::mat4{1.f};
        m_inverseViewMatrix_[0][0] = u.x;
        m_inverseViewMatrix_[0][1] = u.y;
        m_inverseViewMatrix_[0][2] = u.z;
        m_inverseViewMatrix_[1][0] = v.x;
        m_inverseViewMatrix_[1][1] = v.y;
        m_inverseViewMatrix_[1][2] = v.z;
        m_inverseViewMatrix_[2][0] = w.x;
        m_inverseViewMatrix_[2][1] = w.y;
        m_inverseViewMatrix_[2][2] = w.z;
        m_inverseViewMatrix_[3][0] = position.x;
        m_inverseViewMatrix_[3][1] = position.y;
        m_inverseViewMatrix_[3][2] = position.z;
    }

    void Camera::MoveInPlaneXZ(glm::vec3& rotate, glm::vec3& moveDir)
    {
        //glm::vec3 rotate{0};

        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
        {
            //m_transform_.rotation += lookSpeed * dt * glm::normalize(rotate);
            m_transform_.rotation += lookSpeed * glm::normalize(rotate);
        }
        
        // limits pitch values between about +/- 85ish degrees
        m_transform_.rotation.x = glm::clamp(m_transform_.rotation.x, -1.5f, 1.5f);
        m_transform_.rotation.y = glm::mod(m_transform_.rotation.y, glm::two_pi<float>());

        float yaw = m_transform_.rotation.y;
        const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
        const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
        const glm::vec3 upDir{0.f, -1.f, 0.f};

        #if 0
        glm::vec3 moveDir{0.f};
        if(glfwGetKey(window, keys.moveForward)  == GLFW_PRESS) moveDir += forwardDir;
        if(glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
        if(glfwGetKey(window, keys.moveRight)    == GLFW_PRESS) moveDir += rightDir;
        if(glfwGetKey(window, keys.moveLeft)     == GLFW_PRESS) moveDir -= rightDir;
        if(glfwGetKey(window, keys.moveUp)       == GLFW_PRESS) moveDir += upDir;
        if(glfwGetKey(window, keys.moveDown)     == GLFW_PRESS) moveDir -= upDir;
        #endif

        if(glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
        {
            //m_transform_.translation += moveSpeed * dt * glm::normalize(moveDir);
            m_transform_.translation += moveSpeed * glm::normalize(moveDir);
        }   
    }


}