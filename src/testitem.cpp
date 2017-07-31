#include "testitem.h"

std::vector<SolidTriangleVertexData> vertexdata =
{
	{ 10.f, 50.f, 0.f,   0.f,  0.0f },
	{ 360.f, 333.f, 0.f, 0.5f, 0.0f },
	{ 610.f, 10.f, 0.f,  1.f,  1.f  }
 }; 

std::vector<uint16_t> indices = { 0, 1, 2};

TestItem::TestItem()
{
	vData = vertexdata;
	iData = indices;
//	vData.reserve(vertexdata.size() * 2);
//	iData.reserve(indices.size() * 2);

//	float halfOneX = (vertexdata[0].x + vertexdata[1].x) / 2;
//	float halfOneY = (vertexdata[0].y + vertexdata[1].y) / 2;

//	float halfTwoX = (vertexdata[1].x + vertexdata[2].x) / 2;
//	float halfTwoY = (vertexdata[1].y + vertexdata[2].y) / 2;

//	float newMiddleVertexX = (halfOneX + halfTwoX) / 2;
//	float newMiddleVertexY = (halfOneY + halfTwoY) / 2;

//	vData.push_back(vertexdata[0]);
//	vData.push_back({ halfOneX, halfOneY, 0.f, 0.5f, 0.f });
//	vData.push_back({ newMiddleVertexX, newMiddleVertexY, 0.f, 1.f, 1.f });
//	vData.push_back({ newMiddleVertexX, newMiddleVertexY, 0.f, 0.f, 0.f });
//	vData.push_back({ halfTwoX, halfTwoY, 0.f, 0.5f, 0.f });
//	vData.push_back(vertexdata[2]);
//	iData.push_back(0);
//	iData.push_back(1);
//	iData.push_back(2);
//	iData.push_back(3);
//	iData.push_back(4);
//	iData.push_back(5);
}

DirtyState TestItem::beforeRendering(RenderManager &renderer)
{
	if (!dirty)
		return DirtyState::Clean;

	dirty = false;
	return DirtyState::Upload;
}
void TestItem::updateMemory(RenderManager &renderer)
{}

void TestItem::reserveMemory(RenderManager &renderer)
{
	renderer.staticSolidTriangleRenderer().reserveSizes(iData.size(),vData.size());
}
void TestItem::uploadMemory(RenderManager &renderer)
{
	renderer.staticSolidTriangleRenderer().appendIndices(iData);
	renderer.staticSolidTriangleRenderer().appendVertexData(vData);
}
