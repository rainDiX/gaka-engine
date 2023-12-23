/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>

template <typename Iterator>
inline glm::vec3 deCasteljau(float u, Iterator begin, Iterator end) {
    auto beta = std::vector<glm::vec3>(begin, end);
    const auto n = beta.size();

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < n - i; j++) {
            beta[j] = beta[j] * (1.0f - u) + beta[j + 1] * u;
        }
    }
    return beta[0];
}