#include "transform.hpp"

namespace ex 
{
    // translate
    // ------------------------------------------------------------------------------------------------------
    void Transform::local_translateX(float x)
    {
        pos += get_right() * x;
    }
    void Transform::local_translateY(float y)
    {
        pos += get_up() * y;
    }
    void Transform::local_translateZ(float z)
    {
        pos += get_forward() * z;
    }
    // rotation
    // ------------------------------------------------------------------------------------------------------
    void Transform::set_rot(const glm::quat& r)
    {
        rot = r;
    }
    void Transform::set_rot(float x_in_deg, float y_in_deg, float z_in_deg)
    {
        // Convert degrees to radians
        float pitch = glm::radians(x_in_deg); // Rotation around X-axis
        float yaw = glm::radians(y_in_deg);   // Rotation around Y-axis
        float roll = glm::radians(z_in_deg);  // Rotation around Z-axis

        // Create quaternions for each rotation
        glm::quat qX = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
        glm::quat qY = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
        glm::quat qZ = glm::angleAxis(roll, glm::vec3(0, 0, 1));

        // Combine the rotations (note the order)
        // m_rot = qY * qX * qZ; // Yaw -> Pitch -> Roll
        rot = qX * qY * qZ;
    }
    void Transform::set_rot(glm::vec3 v_in_deg)
    {
        set_rot(v_in_deg.x, v_in_deg.y, v_in_deg.z);
    }
    void Transform::rotate(const glm::quat& r)
    {
        rot = r * rot;
    }
    void Transform::rotateX(float angle_in_degrees)
    {
        float angle = glm::radians(angle_in_degrees);
        glm::quat rotationX = glm::angleAxis(angle, glm::vec3(1, 0, 0));
        rotate(rotationX);
    }
    void Transform::rotateY(float angle_in_degrees)
    {
        float angle = glm::radians(angle_in_degrees);
        glm::quat rotationY = glm::angleAxis(angle, glm::vec3(0, 1, 0));
        rotate(rotationY);
    }
    void Transform::rotateZ(float angle_in_degrees)
    {
        float angle = glm::radians(angle_in_degrees);
        glm::quat rotationZ = glm::angleAxis(angle, glm::vec3(0, 0, 1));
        rotate(rotationZ);
    }
    void Transform::local_rotateX(float angle_in_degrees)
    {
        float angle = glm::radians(angle_in_degrees);
        glm::quat rotationX = glm::angleAxis(angle, get_right());
        rotate(rotationX);
    }
    void Transform::local_rotateY(float angle_in_degrees)
    {
        float angle = glm::radians(angle_in_degrees);
        glm::quat rotationY = glm::angleAxis(angle, get_up());
        rotate(rotationY);
    }
    void Transform::local_rotateZ(float angle_in_degrees)
    {
        float angle = glm::radians(angle_in_degrees);
        glm::quat rotationZ = glm::angleAxis(angle, get_forward());
        rotate(rotationZ);
    }

    glm::vec3 Transform::get_forward() const
    {
        return glm::normalize(rot * glm::vec3(0, 0, -1));
    }

    glm::vec3 Transform::get_right() const
    {
        return glm::normalize(rot * glm::vec3(1, 0, 0));
    }

    glm::vec3 Transform::get_up() const
    {
        return glm::normalize(rot * glm::vec3(0, 1, 0));
    }

    glm::mat4 Transform::get_model_matrix() const
    {
        return glm::translate(glm::mat4(1), pos) *
               glm::mat4_cast(rot) *
               glm::scale(glm::mat4(1), scale);
    }
}