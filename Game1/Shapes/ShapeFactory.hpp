#pragma once
#include <cstdint>

// Forward declaration
namespace je
{
	class Mesh;
}

je::Mesh createCylinder(uint32_t segments, bool flat_shaded = false);

je::Mesh createCube(bool flat_shaded = false);

je::Mesh createIcosphere(uint32_t divisions, bool flat_shaded = false);