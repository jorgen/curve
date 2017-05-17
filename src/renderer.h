#pragma once

#include <stdint.h>
#include <vector>

#include "memberlinkedlist.h"
#include "solidtriangleprogram.h"
#include "solidtrianglerenderer.h"

#include "bitmask.h"
//#include <glad/glad.h>

struct RenderManager;
struct Item
{
	virtual void beforeRendering(RenderManager &renderer) = 0;
	virtual void reserveRenderMemory(RenderManager &renderer) = 0;
	virtual void uploadRenderMemory(RenderManager &renderer) = 0;

	MemberListNode<Item> renderManagerNode;
	float depth_from;
	float depth_to;
};

struct RenderManager
{
	enum class DirtyState : uint8_t
	{
		Clean		= 0 << 0,
		Update		= 1 << 0,
		Reupload	= 1 << 1
	};

	RenderManager();

	void setViewport(float width, float height);
	void addItem(Item *item);
	void removeItem(Item *item);
	void stack(Item *before, Item *after);
	void setDirtyStateForItem(DirtyState state);

	void prepareRender();
	void render();

	SolidTriangleRenderer &staticSolidTriangleRenderer() { return solidTriangleRenderer; }
private:
	SolidTriangleProgram solidTriangleProgram;
	SolidTriangleRenderer solidTriangleRenderer;
	using DirtyStates = bit_mask<DirtyState>;
	DirtyStates dirtyState;
	float depthEnd;
	MemberList<Item, &Item::renderManagerNode> items;
	float viewport_width;
	float viewport_height;
};