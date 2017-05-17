#pragma once

#include "renderer.h"

struct TestItem : public Item
{
	void beforeRendering(RenderManager &renderer) override;
	void reserveRenderMemory(RenderManager &renderer) override;
	void uploadRenderMemory(RenderManager &renderer) override;

	bool first = true;
};