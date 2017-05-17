#include "testitem.h"

std::vector<SolidTriangleVertexData> vertexdata =
{
	{ 10.f, 10.f, 0.f, 0.f, 0.0f},
	{ 160.f, 160.f, 0.f, 0.5f, 0.f },
	{ 310.f, 10.f, 0.f, 1.f, 1.f }
 }; 

std::vector<uint16_t> indices = { 0, 1, 2};
void TestItem::beforeRendering(RenderManager &renderer)
{
	if (!first)
		return;

	renderer.setDirtyStateForItem(RenderManager::DirtyState::Reupload);
}
void TestItem::reserveRenderMemory(RenderManager &renderer)
{
	renderer.staticSolidTriangleRenderer().reserveSizes(3,3);
}
void TestItem::uploadRenderMemory(RenderManager &renderer)
{
	renderer.staticSolidTriangleRenderer().appendVertexData(vertexdata);
	renderer.staticSolidTriangleRenderer().appendIndices(indices);
}
