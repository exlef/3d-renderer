#pragma once

#include "glm/glm.hpp"

namespace ex 
{
    constexpr bool SHOW_SKYBOX = true;
    constexpr bool APPLY_PP = true;
    constexpr bool APPLY_GAMMA_CRRC = (APPLY_PP && true);
    constexpr float GAMMA = 2.2f;
    constexpr glm::vec4 CLEAR_COLOR = glm::vec4(0.2f);
    constexpr bool HIDE_CURSOR = true;
}
