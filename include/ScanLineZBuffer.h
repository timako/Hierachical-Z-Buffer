#ifndef SCANLINEZBUFFER_H
#define SCANLINEZBUFFER_H

#include "framebuffer.h"
#include "model.h"
#include "objtype.h"

// with reference to ppt 11 of CG course, JieQing Feng Prof. in ZJU. 


struct Edgef
{
	Vec3f start, cur, end; //start.y < end.y
	float gradientDxDy, gradientDzDy;
	Vec3f rgbStart, rgbCur, rgbEnd;
	Vec3f gradientdRGBdy; 
	bool isPaired; 

	uint edgeId;
	uint polygonId;

	Edgef(const Vertex& v0, const Vertex& v1, uint eid, uint pid);
	void setCurPos(int current_Y);
}; 


struct Polygonf
{
	std::vector<Vertex> vertices;
	float a, b, c, d; 
	int polygonId; 
	int dy; 
};


class Renderer; 

class ScanLineZBuffer : public Framebuffer
{
public:
    ScanLineZBuffer() = delete;
	ScanLineZBuffer(int w, int h);
	~ScanLineZBuffer() = default;
	void clear();
	void buildTable(const Model& model);
	void actScan(); 

	int curFaceOffset = 0;
	int edgeIdOffset = 0;
	
	std::vector<float> zBufferLine;

	std::vector<Edgef> edgeTable;
	std::vector<Edgef> activeEdgeTable;
	std::vector<std::vector<int>> activeEdgeIdTable;   // enter by line
	std::vector<std::vector<int>> deactiveEdgeIdTable; // escape by line


    virtual void setPixel(int x, int y, const Color& color, float depth);
}; 



inline bool isEqualf(float a, float b)
{
	const float eps = 1e-6;
	if (fabs(a - b) < eps)
		return true;
	return false;
}

#endif // SCANLINEZBUFFER_H