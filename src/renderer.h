#pragma once

#include <stdint.h>
#include <vector>

#include "memberlinkedlist.h"
#include "solidtriangleprogram.h"
#include "solidtrianglerenderer.h"

#include "bitmask.h"
//#include <glad/glad.h>

struct RenderManager;

enum class DirtyState : uint8_t
{
	Clean,
	Update,
	Upload
};

struct Item
{
	virtual DirtyState beforeRendering(RenderManager &renderer) = 0;
	virtual void updateMemory(RenderManager &renderer) = 0;
	virtual void reserveMemory(RenderManager &renderer) = 0;
	virtual void uploadMemory(RenderManager &renderer) = 0;

	MemberListNode<Item> renderManagerNode;
	float depth_from;
	float depth_to;
};

struct RenderManager
{
	RenderManager();

	void setViewport(float width, float height);
	void addItem(Item *item);
	void removeItem(Item *item);
	void stack(Item *before, Item *after);

	void prepareRender();
	void render();

	SolidTriangleRenderer &staticSolidTriangleRenderer() { return solidTriangleRenderer; }
private:
	SolidTriangleProgram solidTriangleProgram;
	SolidTriangleRenderer solidTriangleRenderer;
	float depthEnd;
	MemberList<Item, &Item::renderManagerNode> items;
	float viewport_width;
	float viewport_height;
};