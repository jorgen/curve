#pragma once

#include <stdint.h>
#include <vector>

struct SolidTriangleVertexData
{
	float x;
	float y;
	float z;
	float u;
	float v;
};

struct SolidTriangleProgram
{
	SolidTriangleProgram();

	void setViewport(float width, float height);
	void setVertexLayoutForCurrentVaoAndArrayBuffer();

	void uploadIndicesToCurrentBuffer(const std::vector<uint16_t> indices);
	void modifyIndicesToCurrentBuffer(size_t offset, const std::vector<uint16_t> indices);

	void uploadVertexDataToCurrentBuffer(const std::vector<SolidTriangleVertexData> data);
	void modifyVertexDataToCurrentBuffer(size_t offset, const std::vector<SolidTriangleVertexData> data);

	void render(uint32_t iboSize);	
   
	uint32_t program;

	int32_t proj_loc;
	int32_t vpos_loc;
	int32_t vcol_loc;
};

