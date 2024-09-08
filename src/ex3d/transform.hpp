#pragma once

#include <glm/glm.hpp>

namespace ex
{
    class Transform
    {
    private:
        /* data */
        glm::vec3 m_pos = glm::vec3(0);
        glm::quat m_rot = glm::quat(1, 0, 0, 0);
        glm::vec3 m_scale = glm::vec3(1);

        bool m_is_dirty = true;
        glm::mat4 m_model_matrix = glm::mat4(1);
        void update_model_matrix();

    public:
        glm::vec3 get_pos() const { return m_pos; }
        glm::quat get_rot() const { return m_rot; }
        glm::vec3 get_scale() const { return m_scale; }
        bool is_dirty() const { return m_is_dirty; }


        // position
        void set_pos(glm::vec3 p);
        void set_pos(float x, float y, float z);
        void translate(glm::vec3 t);
        void translateX(float x);
        void translateY(float y);
        void translateZ(float z);
        void local_translateX(float x);
        void local_translateY(float y);
        void local_translateZ(float z);
        // rotation
        void set_rot(const glm::quat& r);
        void set_rot(float x_in_deg, float y_in_deg, float z_in_deg);
        void set_rot(glm::vec3 v_in_deg);
        void rotate(const glm::quat& r);
        void rotateX(float angle_in_degrees);
        void rotateY(float angle_in_degrees);
        void rotateZ(float angle_in_degrees);
        void local_rotateX(float angle_in_degrees);
        void local_rotateY(float angle_in_degrees);
        void local_rotateZ(float angle_in_degrees);
        // scale
        void set_scale(glm::vec3 s);
        void scale(glm::vec3 s);
        void set_scale(float x, float y, float z);
        void set_scale(float s);
        void scale(float s);
        void scaleX(float x);
        void scaleY(float y);
        void scaleZ(float z);

        //---------------------------------------
        glm::vec3 get_forward() const;
        glm::vec3 get_right() const;
        glm::vec3 get_up() const;
        glm::mat4 get_model_matrix();
    };

    // translate
    // ------------------------------------------------------------------------------------------------------
    void Transform::set_pos(glm::vec3 p)
    {
        m_pos = p;
        m_is_dirty = true;
    }
    void Transform::set_pos(float x, float y, float z)
    {
        m_pos = glm::vec3(x, y, z);
        m_is_dirty = true;
    }
    void Transform::translate(glm::vec3 t)
    {
        m_pos += t;
        m_is_dirty = true;
    }
    void Transform::translateX(float x)
    {
        m_pos.x += x;
        m_is_dirty = true;
    }
    void Transform::translateY(float y)
    {
        m_pos.y += y;
        m_is_dirty = true;
    }
    void Transform::translateZ(float z)
    {
        m_pos.z += z;
        m_is_dirty = true;
    }
    void Transform::local_translateX(float x)
    {
        m_pos += get_right() * x;
        m_is_dirty = true;
    }
    void Transform::local_translateY(float y)
    {
        m_pos += get_up() * y;
        m_is_dirty = true;
    }
    void Transform::local_translateZ(float z)
    {
        m_pos += get_forward() * z;
        m_is_dirty = true;
    }
    // rotation
    // ------------------------------------------------------------------------------------------------------
    void Transform::set_rot(const glm::quat& r)
    {
        m_rot = r;
        m_is_dirty = true;
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
        m_rot = qX * qY * qZ;
        m_is_dirty = true;
    }
    void Transform::set_rot(glm::vec3 v_in_deg)
    {
        set_rot(v_in_deg.x, v_in_deg.y, v_in_deg.z);
    }
    void Transform::rotate(const glm::quat& r)
    {
        m_rot = r * m_rot;
        m_is_dirty = true;
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
    // scale
    // ------------------------------------------------------------------------------------------------------
    void Transform::set_scale(glm::vec3 s)
    {
        m_scale = s;
        m_is_dirty = true;
    }
    void Transform::set_scale(float x, float y, float z)
    {
        m_scale = glm::vec3(x, y, z);
        m_is_dirty = true;
    }
    void Transform::set_scale(float s)
    {
        m_scale = glm::vec3(s, s, s);
        m_is_dirty = true;
    }
    void Transform::scale(glm::vec3 s)
    {
        m_scale += s;
        m_is_dirty = true;
    }
    void Transform::scale(float s)
    {
        m_scale += glm::vec3(s, s, s);
        m_is_dirty = true;
    }
    void Transform::scaleX(float x)
    {
        m_scale.x += x;
        m_is_dirty = true;
    }
    void Transform::scaleY(float y)
    {
        m_scale.y += y;
        m_is_dirty = true;
    }
    void Transform::scaleZ(float z)
    {
        m_scale.z += z;
        m_is_dirty = true;
    }

    void Transform::update_model_matrix()
    {
        if (m_is_dirty)
        {
            m_model_matrix = glm::translate(glm::mat4(1), m_pos) *
                            glm::mat4_cast(m_rot) *
                            glm::scale(glm::mat4(1), m_scale);
            m_is_dirty = false;
        }
    }

    glm::vec3 Transform::get_forward() const
    {
        return glm::normalize(m_rot * glm::vec3(0, 0, -1));
    }

    glm::vec3 Transform::get_right() const
    {
        return glm::normalize(m_rot * glm::vec3(1, 0, 0));
    }

    glm::vec3 Transform::get_up() const
    {
        return glm::normalize(m_rot * glm::vec3(0, 1, 0));
    }

    glm::mat4 Transform::get_model_matrix()
    {
        update_model_matrix();
        return m_model_matrix;
    }

} // namespace ex

