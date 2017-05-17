#include "solidtrianglerenderer.h"

#include "solidtriangleprogram.h"

SolidTriangleRenderer::SolidTriangleRenderer(SolidTriangleProgram &program)
	: ibo(0)
	, vbo(0)
	, program(program)
	, indices_size(0)
	, indices_offset(0)
	, vertices_size(0)
	, vertices_offset(0)
{
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	Vao::Binder binder(vao);
	program.setVertexLayoutForCurrentVaoAndArrayBuffer();
}

SolidTriangleRenderer::OffsetData SolidTriangleRenderer::reserveSizes(size_t iboSize, size_t vboSize)
{
	SolidTriangleRenderer::OffsetData ret = { indices_offset, vertices_offset };
	indices_offset += iboSize;
	vertices_offset += vboSize;
	return ret;
}

void SolidTriangleRenderer::reserveMemory()
{
	indices.reserve(indices_offset);
	vertex_data.reserve(vertices_offset);
}

void SolidTriangleRenderer::appendIndices(const std::vector<uint16_t> indices)
{
	this->indices.insert(this->indices.end(), indices.begin(), indices.end());
}

void SolidTriangleRenderer::modifyIndices(size_t offset, const std::vector<uint16_t> indices)
{
	if (indices.empty())
		return;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	program.modifyIndicesToCurrentBuffer(offset, indices);
}

void SolidTriangleRenderer::appendVertexData(const std::vector<SolidTriangleVertexData> data)
{
	vertex_data.insert(vertex_data.end(), data.begin(), data.end());
}

void SolidTriangleRenderer::modifyVertexData(size_t offset, const std::vector<SolidTriangleVertexData> data)
{
	if (vertex_data.empty())
		return;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	program.modifyVertexDataToCurrentBuffer(offset, data);
}

void SolidTriangleRenderer::render()
{
	Vao::Binder binder(vao);
	if (indices.size()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		program.uploadIndicesToCurrentBuffer(indices);
		indices_size = indices.size();
		indices.clear();
	}
	if (vertex_data.size()) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		program.uploadVertexDataToCurrentBuffer(vertex_data);
		vertices_size = vertex_data.size();
		vertex_data.clear();
	}
	program.render(indices_size);
}