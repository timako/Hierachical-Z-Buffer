#include "ScanLineZBuffer.h"
#include "Timer.h"
#include "model.h"
#include "renderer.h"

ScanLineZBuffer::ScanLineZBuffer(int w, int h)
    :Framebuffer(w, h),
    zBufferLine(w, 0.0f),
    idTable(h, std::vector<int>(w, -1))
    {}

void ScanLineZBuffer::clear(){
	Framebuffer::clear();
	zBufferLine.resize(width);
	std::fill(zBufferLine.begin(), zBufferLine.end(), 0.0f);
	idTable.resize(height);
	for (int i = 0; i < height; i++){
    	idTable[i].resize(width);
    	std::fill(idTable[i].begin(), idTable[i].end(), -1);
	}	 
}

void ScanLineZBuffer::buildTable(const Model& model){
	Timer timer;
	timer.start();

	classifiedPolygonTable.clear();
	classifiedPolygonTable.resize(height);
	classifiedEdgeTable.clear();
	classifiedEdgeTable.resize(height);

	Mat4x4 viewMatrix;
	pRenderer->camera.getViewMatrix(viewMatrix);

	Mat4x4 projectionMatrix;
	pRenderer->camera.getProjectionMatrix(projectionMatrix);

	uint faces_size = model.faces.size();

	for (uint j = 0; j < faces_size; j++){
		Face face = model.faces[j];
		std::vector<Vertex> vertices; 
        vertices.resize(3);
        for (int i = 0; i < 3; ++i) {
            const Face::VertexIndices& idx = face.vertices[i];
            vertices[i].position = model.vertices[idx.v];
            vertices[i].normal = model.vNormals[idx.v];
            vertices[i].texcoord = model.texcoords.empty() ? Vec2f() : model.texcoords[idx.vt];
        }
		for (int i = 0; i < 3; ++i) {
            Vec4f pos(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z, 1.0f);
            pos = viewMatrix * pos; 
            pos = projectionMatrix * pos; 
            if (pos.w != 0.0f) {
                vertices[i].position = Vec3f(pos.x / pos.w, pos.y / pos.w, pos.z / pos.w);
            } else {
                std::cerr << "Warning: pos.w is 0.0f when transforming." << std::endl;;
            }

			vertices[i].position.x = (vertices[i].position.x + 1.0f) * 0.5f * width;
			vertices[i].position.y = (vertices[i].position.y + 1.0f) * 0.5f * height;
    	}
            
		float maxy, miny; 
		for(int i = 0; i < 3; i++){
			Vec3f p1 = vertices[i].position;
			Vec3f p2 = vertices[(i + 1) % 3].position;

			if (p1.y < p2.y){
				Vec3f tmp = p1;
				p1 = p2;
				p2 = tmp;
			}

			maxy = std::max(-std::numeric_limits<float>::infinity(), p1.y);
    		miny = std::min(std::numeric_limits<float>::infinity(), p2.y);
			ClassifiedEdgef edge;
			edge.x = floor(p1.x);
			edge.id = i;
			edge.dy = floor(p1.y) - floor(p2.y);
			if (isEqualf(edge.dy,0)) continue;
			edge.dx = -(floor(p1.x) - floor(p2.x)) / edge.dy;
			classifiedEdgeTable[floor(p1.y)].push_back(edge);
		}

		ClassifiedPolygonf polygon;
		polygon.id = j;
		polygon.dy = floor(maxy) - floor(miny);
		Vec3f pos = model.vertices[face.vertices[0].v];
		polygon.a = model.fNormals[face.vertices[0].v].x;
		polygon.b = model.fNormals[face.vertices[0].v].y;
		polygon.c = model.fNormals[face.vertices[0].v].z;
		polygon.d = -(polygon.a * pos.x + polygon.b * pos.y + polygon.c * pos.z);
		Color RandColor(
			static_cast<uint8_t>(rand() % 255),
			static_cast<uint8_t>(rand() % 255),
			static_cast<uint8_t>(rand() % 255)
		);
		polygon.color = RandColor;
		classifiedPolygonTable[floor(maxy)].push_back(polygon);

  	}

	timer.stop();
	std::cout << "ScanLine Table build time:" << timer.elapsed() << std::endl;
  }

void ScanLineZBuffer::setPixel(int x, int y, const Color& color, float depth){
  ;
}