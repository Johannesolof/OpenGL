#include "ShapeFactory.hpp"

#include "mesh.hpp"

#include <execution>
#include "utils.hpp"

namespace
{
	std::vector<GLfloat> generate_normals(const std::vector<GLfloat>& vertices)
	{
		std::vector<GLfloat> normals(vertices.size(), 0.f);
		for (size_t i = 0; i < vertices.size(); i += 3)
		{
			const glm::vec3 v = normalize(glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
			normals[i] = v.x; normals[i + 1] = v.y; normals[i + 2] = v.z;
		}
		return normals;
	}

	struct flat_result {
		std::vector<GLfloat> vertices;
		std::vector<GLfloat> normals;
		std::vector<GLuint> indices;
	} toFlatShaded(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
	{
		flat_result result;
		for (GLuint i = 0; i < indices.size(); i += 3)
		{
			glm::vec3 a(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
			glm::vec3 b(vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]);
			glm::vec3 c(vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]);

			glm::vec3 n = glm::normalize(glm::cross(b - a, c - a));
			result.vertices.insert(result.vertices.end(), { a.x, a.y, a.z });
			result.vertices.insert(result.vertices.end(), { b.x, b.y, b.z });
			result.vertices.insert(result.vertices.end(), { c.x, c.y, c.z });
			result.normals.insert(result.normals.end(), { n.x, n.y, n.z });
			result.normals.insert(result.normals.end(), { n.x, n.y, n.z });
			result.normals.insert(result.normals.end(), { n.x, n.y, n.z });
		}
		result.indices.resize(result.vertices.size());
		std::iota(result.indices.begin(), result.indices.end(), 0);
		return result;
	}

	glm::vec3 midpoint(const glm::vec3& v1, const glm::vec3& v2)
	{
		return glm::normalize(v1 + (v2 - v1) * 0.5f) / 2.f;
	}
}

je::Mesh createCylinder(uint32_t segments, bool flat_shaded)
{
	glm::vec3 northPole = { 0.5f, 0, 0 };
	glm::vec3 southPole = { -0.5f, 0, 0 };

	std::vector<GLfloat> vertices = { northPole.x, northPole.y, northPole.z, southPole.x, southPole.y, southPole.z };

	const float angle = 2 * je::Pi / segments;
	for (uint32_t i = 0; i < segments; i++)
	{
		const float y = 0.5f * cos(i * angle);
		const float z = 0.5f * sin(i * angle);

		vertices.insert(vertices.end(), { 0.5f, y, z });
		vertices.insert(vertices.end(), { -0.5f, y, z });
	}

	const std::vector<GLfloat> normals = generate_normals(vertices);


	std::vector<GLuint> indices;

	// north circle
	for (uint32_t i = 0; i < segments; i++)
	{
		indices.push_back(0);
		indices.push_back(2 + i * 2);
		indices.push_back(i == segments - 1 ? 2 : 4 + i * 2);
	}

	// south circle
	for (uint32_t i = 0; i < segments; i++)
	{
		indices.push_back(1);
		indices.push_back(i == segments - 1 ? 3 : 5 + i * 2);
		indices.push_back(3 + i * 2);
	}

	// tube part
	for (uint32_t i = 0; i < segments; i++)
	{
		indices.push_back(2 + i * 2);
		indices.push_back(3 + i * 2);
		indices.push_back(i == segments - 1 ? 2 : 4 + i * 2);

		indices.push_back(i == segments - 1 ? 2 : 4 + i * 2);
		indices.push_back(3 + i * 2);
		indices.push_back(i == segments - 1 ? 3 : 5 + i * 2);
	}

	if (!flat_shaded)
		return je::Mesh(vertices, normals, {}, {}, {}, indices, GL_TRIANGLES);

	const auto result = toFlatShaded(vertices, indices);
	return je::Mesh(result.vertices, result.normals, {}, {}, {}, result.indices, GL_TRIANGLES);
}

je::Mesh createCube(bool flat_shaded)
{
	const std::vector<GLfloat> vertices = {
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
	};

	const std::vector<GLfloat> normals = generate_normals(vertices);

	const std::vector<GLuint> indices = {
		0, 5, 4,
		0, 1, 5,
		6, 0, 4,
		6, 2, 0,
		2, 1, 0,
		2, 3, 1,
		3, 5, 1,
		3, 7, 5,
		6, 3, 2,
		6, 7, 3,
		4, 7, 6,
		4, 5, 7
	};

	if (!flat_shaded)
		return je::Mesh(vertices, normals, {}, {}, {}, indices, GL_TRIANGLES);

	const auto result = toFlatShaded(vertices, indices);
	return je::Mesh(result.vertices, result.normals, {}, {}, {}, result.indices, GL_TRIANGLES);
}


je::Mesh createIcosphere(uint32_t divisions, bool flat_shaded)
{
	constexpr float inv_gr = (1 / je::GoldenRatio);
	std::vector<glm::vec3> vec3Vertices;
	vec3Vertices.emplace_back(0.f, 1.f, inv_gr);
	vec3Vertices.emplace_back(0.f, 1.f, -inv_gr);
	vec3Vertices.emplace_back(0.f, -1.f, inv_gr);
	vec3Vertices.emplace_back(0.f, -1.f, -inv_gr);
	vec3Vertices.emplace_back(1.f, -inv_gr, 0.f);
	vec3Vertices.emplace_back(1.f, inv_gr, 0.f);
	vec3Vertices.emplace_back(-1.f, -inv_gr, 0.f);
	vec3Vertices.emplace_back(-1.f, inv_gr, 0.f);
	vec3Vertices.emplace_back(-inv_gr, 0.f, -1.f);
	vec3Vertices.emplace_back(inv_gr, 0.f, -1.f);
	vec3Vertices.emplace_back(-inv_gr, 0.f, 1.f);
	vec3Vertices.emplace_back(inv_gr, 0.f, 1.f);

	std::transform(std::execution::par_unseq, vec3Vertices.begin(), vec3Vertices.end(), vec3Vertices.begin(),
		[](glm::vec3 v) -> glm::vec3 {return glm::normalize(v) / 2.f; });

	std::vector<glm::ivec3> vec3indices;

	// 5 triangles around vertex 0
	vec3indices.emplace_back(0, 11, 5);
	vec3indices.emplace_back(0, 5, 1);
	vec3indices.emplace_back(0, 1, 7);
	vec3indices.emplace_back(0, 7, 10);
	vec3indices.emplace_back(0, 10, 11);

	// 5 adjacent triangles
	vec3indices.emplace_back(11, 4, 5);
	vec3indices.emplace_back(5, 9, 1);
	vec3indices.emplace_back(1, 8, 7);
	vec3indices.emplace_back(7, 6, 10);
	vec3indices.emplace_back(10, 2, 11);

	// 5 triangles around vertex 3
	vec3indices.emplace_back(3, 6, 8);
	vec3indices.emplace_back(3, 8, 9);
	vec3indices.emplace_back(3, 9, 4);
	vec3indices.emplace_back(3, 4, 2);
	vec3indices.emplace_back(3, 2, 6);

	// 5 adjacent triangles
	vec3indices.emplace_back(6, 7, 8);
	vec3indices.emplace_back(8, 1, 9);
	vec3indices.emplace_back(9, 5, 4);
	vec3indices.emplace_back(4, 11, 2);
	vec3indices.emplace_back(2, 10, 6);

	for (uint32_t i = 0; i < divisions; i++)
	{
		std::vector<glm::ivec3> vec3indices2;
		vec3indices2.reserve(vec3indices.size() * 4);

		for (glm::ivec3 index : vec3indices)
		{
			auto v1 = vec3Vertices[index.x];
			auto v2 = vec3Vertices[index.y];
			auto v3 = vec3Vertices[index.z];
			auto v1_v2_midpoint = midpoint(v1, v2);
			auto v1_v3_midpoint = midpoint(v1, v3);
			auto v2_v3_midpoint = midpoint(v2, v3);
			vec3indices2.emplace_back(index.x, vec3Vertices.size(), vec3Vertices.size() + 1);
			vec3indices2.emplace_back(index.y, vec3Vertices.size() + 2, vec3Vertices.size());
			vec3indices2.emplace_back(index.z, vec3Vertices.size() + 1, vec3Vertices.size() + 2);
			vec3indices2.emplace_back(vec3Vertices.size(), vec3Vertices.size() + 2, vec3Vertices.size() + 1);
			vec3Vertices.push_back(v1_v2_midpoint);
			vec3Vertices.push_back(v1_v3_midpoint);
			vec3Vertices.push_back(v2_v3_midpoint);
		}
		vec3indices.swap(vec3indices2);
	}

	std::vector<GLfloat> vertices;
	vertices.reserve(vec3Vertices.size() * 3);
	for (auto& v : vec3Vertices)
	{
		vertices.insert(vertices.end(), { v.x, v.y, v.z });
	}

	std::vector<GLuint> indices;
	vertices.reserve(vec3indices.size() * 3);
	for (auto& v : vec3indices)
	{
		indices.insert(indices.end(), { static_cast<GLuint>(v.x), static_cast<GLuint>(v.y), static_cast<GLuint>(v.z) });
	}

	const auto normals = generate_normals(vertices);

	if (!flat_shaded)
		return je::Mesh(vertices, normals, {}, {}, {}, indices, GL_TRIANGLES);

	const auto result = toFlatShaded(vertices, indices);
	return je::Mesh(result.vertices, result.normals, {}, {}, {}, result.indices, GL_TRIANGLES);
}
