#pragma once

#include "solidtriangleprogram.h"
#include <vector>

#include "vao.h"

struct SolidTriangleProgram;

struct SolidTriangleRenderer
{
	struct OffsetData
	{
		size_t indexOffset;
		size_t vertexOffset;
	};

	SolidTriangleRenderer(SolidTriangleProgram &program);

	OffsetData reserveSizes(size_t iboSize, size_t vboSize);
	
	void reserveMemory();

	void appendIndices(const std::vector<uint16_t> indices);
	void modifyIndices(size_t offset, const std::vector<uint16_t> indices);

	void appendVertexData(const std::vector<SolidTriangleVertexData> data);
	void modifyVertexData(size_t offset, const std::vector<SolidTriangleVertexData> data);

	void render();

private:
	Vao vao;
	uint32_t ibo;
	uint32_t vbo;
	SolidTriangleProgram &program;

	size_t indices_size;
	size_t indices_offset;
	size_t vertices_size;
	size_t vertices_offset;

	std::vector<uint16_t> indices;
	std::vector<SolidTriangleVertexData> vertex_data;
};
