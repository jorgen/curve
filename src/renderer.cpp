#include "renderer.h"

RenderManager::RenderManager()
	: solidTriangleRenderer(solidTriangleProgram)
	, depthEnd(0)
{

}
	
void RenderManager::setViewport(float width, float height)
{
	if (viewport_width == width && viewport_height == height)
		return;
	viewport_width = width;
	viewport_height = height;
	solidTriangleProgram.setViewport(width, height);
}

void RenderManager::addItem(Item *item)
{
	items.push_back(item);
	item->depth_from = depthEnd;
	item->depth_to = depthEnd + 0.09;
	depthEnd += 0.1;
}

void RenderManager::removeItem(Item *item)
{
	MemberList<Item, &Item::renderManagerNode>::iterator it(item, &items);
	items.erase(it);
}

void RenderManager::stack(Item *before, Item *after)
{
	MemberList<Item, &Item::renderManagerNode>::iterator before_it(before, &items);
	items.erase(before_it);

	MemberList<Item, &Item::renderManagerNode>::iterator after_it(after, &items);
	items.insert(after_it, before);

	depthEnd = 0;
	for (auto item : items)
	{
		item->depth_from = depthEnd;
		item->depth_to = depthEnd + 0.09;
		depthEnd += 0.1;
		
	}
}
void RenderManager::setDirtyStateForItem(DirtyState state)
{
	dirtyState |= state;
}

void RenderManager::prepareRender()
{
	for (auto item : items)
	{
		item->beforeRendering(*this);
	}

	if (dirtyState & DirtyState::Reupload)
	{
		for (auto item : items)
		{
			item->reserveRenderMemory(*this);
		}
	}

	if ((dirtyState & DirtyState::Reupload) || (dirtyState & DirtyState::Update))
	{
		for (auto item : items)
		{
			item->uploadRenderMemory(*this);
		}
	}
}

void RenderManager::render()
{
	glViewport(0, 0, viewport_width, viewport_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	solidTriangleRenderer.render();
}
