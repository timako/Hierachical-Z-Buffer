#ifndef SCANLINEZBUFFER_H
#define SCANLINEZBUFFER_H

#include "framebuffer.h"
#include "model.h"

// with reference to ppt 11 of CG course, JieQing Feng Prof. in ZJU. 
struct ClassifiedPolygonf
{
	float a, b, c, d; 
	int id; 
	int dy; 
	Color color; 
};

struct ClassifiedEdgef
{
	float x; 
	float dx; 
	int dy; 
	int id; 
};


struct ActivatedEdgef
{
	float xl, xr; 
	float dxl, dxr; 
	float dyl, dyr; 
	float zl; 
	float dzx; 
	float dzy; 
	int id; 
};

class ScanLineZBuffer : public Framebuffer
{
    ScanLineZBuffer() = delete;
	ScanLineZBuffer(int w, int h);
	~ScanLineZBuffer() = default;
	void clear();
	void buildTable(const Model& model);
	void actScan(); 
	std::vector<std::vector<int>> idTable;
	std::vector<float> zBufferLine;

	std::vector<ClassifiedEdgef> relatedEdge; 
	std::vector<std::vector<ClassifiedPolygonf> > classifiedPolygonTable;
	std::vector<std::vector<ClassifiedEdgef> > classifiedEdgeTable;
	std::vector<ClassifiedPolygonf> activePolygonTable;
	std::vector<ActivatedEdgef> activeEdgeTable;	
	
	void addActiveEdgeFromActivePolygon(int y, ClassifiedPolygonf& activePolygon); 
	bool findReplaceEdgeFromActivePolygon(int y, ActivatedEdgef& activeEdge);
	

    void clear(const Color& clearColor = Color(0, 0, 0));
    virtual void setPixel(int x, int y, const Color& color, float depth);
}; 

#endif // SCANLINEZBUFFER_H