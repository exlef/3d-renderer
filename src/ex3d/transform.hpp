#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ex
{
    class Transform
    {
    public:
        glm::vec3 pos = glm::vec3(0);
        glm::quat rot = glm::quat(1, 0, 0, 0);
        glm::vec3 scale = glm::vec3(1);

        // position
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

        //---------------------------------------
        glm::vec3 get_forward() const;
        glm::vec3 get_right() const;
        glm::vec3 get_up() const;
        glm::mat4 get_model_matrix() const;
    };

} // namespace ex

