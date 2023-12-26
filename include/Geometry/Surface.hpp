/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <array>
#include <cstddef>

#include "Mesh.hpp"

namespace gk::geometry {

class Surface {
   public:
    virtual const Mesh& mesh() const = 0;
};

template <size_t M, size_t N>
class BezierSurface : public Surface {
   public:
    BezierSurface(const std::array<glm::vec3, M * N>&& ctrlGrid, size_t edges);
    virtual const Mesh& mesh() const override;
    void setMeshEdges(size_t edges);

   private:
    void evaluate();
    void calculateIndices();
    void calculateNormals();

    std::array<glm::vec3, M * N> m_ctrlGrid;
    Mesh m_mesh;
    size_t m_meshEdges;
};

}  // namespace gk::geometry

#include "BezierSurface_impl.hpp"
