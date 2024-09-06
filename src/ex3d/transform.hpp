#pragma once

#include <glm/glm.hpp>

namespace ex
{
    class Transform
    {
    private:
        /* data */
        glm::vec3 m_pos = glm::vec3(0);
        glm::vec3 m_rot = glm::vec3(0);
        glm::vec3 m_scale = glm::vec3(1);
    public:
        Transform();
        ~Transform();

        glm::vec3 pos() const { return m_pos; }
        glm::vec3 rot() const { return m_rot; }
        glm::vec3 scale() const { return m_scale; }

        bool is_dirty = true;

        void set_pos(glm::vec3 p);
        void set_pos(float x, float y, float z);
        void translate(glm::vec3 t);
        void translateX(float x);
        void translateY(float y);
        void translateZ(float z);
        void set_rot(glm::vec3 r);
        void set_rot(float x, float y, float z);
        void rotate(glm::vec3 v);
        void rotateX(float r);
        void rotateY(float y);
        void rotateZ(float z);
        void set_scale(glm::vec3 s);
        void scale(glm::vec3 s);
        void set_scale(float x, float y, float z);
        void set_scale(float s);
        void scale(float s);
        void scaleX(float x);
        void scaleY(float y);
        void scaleZ(float z);
    };

    Transform::Transform()
    {
    }

    Transform::~Transform()
    {
    }

    void Transform::set_pos(glm::vec3 p)
    {
        m_pos = p;
        is_dirty = true;
    }
    void Transform::set_pos(float x, float y, float z)
    {
        m_pos = glm::vec3(x, y, z);
        is_dirty = true;
    }
    void Transform::translate(glm::vec3 t)
    {
        m_pos += t;
        is_dirty = true;
    }
    void Transform::translateX(float x)
    {
        m_pos.x += x;
        is_dirty = true;
    }
    void Transform::translateY(float y)
    {
        m_pos.y += y;
        is_dirty = true;
    }
    void Transform::translateZ(float z)
    {
        m_pos.z += z;
        is_dirty = true;
    }
    void Transform::set_rot(glm::vec3 r)
    {
        m_rot = r;
        is_dirty = true;
    }
    void Transform::set_rot(float x, float y, float z)
    {
        m_rot = glm::vec3(x, y, z);
        is_dirty = true;
    }
    void Transform::rotate(glm::vec3 r)
    {
        m_rot += r;
        is_dirty = true;
    }
    void Transform::rotateX(float x)
    {
        m_rot.x += x;
        is_dirty = true;
    }
    void Transform::rotateY(float y)
    {
        m_rot.y += y;
        is_dirty = true;
    }
    void Transform::rotateZ(float z)
    {
        m_rot.z += z;
        is_dirty = true;
    }
    void Transform::set_scale(glm::vec3 s)
    {
        m_scale = s;
        is_dirty = true;
    }
    void Transform::set_scale(float x, float y, float z)
    {
        m_scale = glm::vec3(x, y, z);
        is_dirty = true;
    }
    void Transform::set_scale(float s)
    {
        m_scale = glm::vec3(s, s, s);
        is_dirty = true;
    }
    void Transform::scale(glm::vec3 s)
    {
        m_scale += s;
        is_dirty = true;
    }
    void Transform::scale(float s)
    {
        m_scale += glm::vec3(s, s, s);
        is_dirty = true;
    }
    void Transform::scaleX(float x)
    {
        m_scale.x += x;
        is_dirty = true;
    }
    void Transform::scaleY(float y)
    {
        m_scale.y += y;
        is_dirty = true;
    }
    void Transform::scaleZ(float z)
    {
        m_scale.z += z;
        is_dirty = true;
    }

} // namespace ex

