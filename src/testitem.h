#pragma once

#include "renderer.h"
#include <glm/glm.hpp>
#include <stdint.h>
#include <vector>

struct TestItem : public Item
{
	TestItem();
	DirtyState beforeRendering(RenderManager &renderer) override;
	void updateMemory(RenderManager &renderer) override;
	void reserveMemory(RenderManager &renderer) override;
	void uploadMemory(RenderManager &renderer) override;

	bool dirty = true;
	uint16_t index;
	glm::vec2 pos[3];

	std::vector<SolidTriangleVertexData> vData;
	std::vector<uint16_t> iData;
};