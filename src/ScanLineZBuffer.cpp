#include "ScanLineZBuffer.h"
#include "Timer.h"
#include "model.h"
#include "renderer.h"
#include "assert.h"
#include "algorithm"

Edgef::Edgef(const Vertex& v0, const Vertex& v1, uint eid, uint pid): edgeId(eid), polygonId(pid){
	start = v0.position; 
	end = v1.position;
	gradientDxDy = (end.x - start.x) / (end.y - start.y);
	gradientDzDy = (end.z - start.z) / (end.y - start.y);
	rgbStart = v0.normal; // debug color 
	rgbEnd = v1.normal; // debug color
	gradientdRGBdy = (rgbEnd - rgbStart) / (end.y - start.y);
	cur = start;
	rgbCur = rgbStart;
	isPaired = false;
}

void Edgef::setCurPos(int current_Y){
	float dy = float(current_Y) - start.y;
	cur.x = start.x + gradientDxDy * dy;
	cur.z = start.z + gradientDzDy * dy;
	rgbCur = rgbStart + gradientdRGBdy * dy;
}

ScanLineZBuffer::ScanLineZBuffer(int w, int h)
    :Framebuffer(w, h),
    zBufferLine(w, 0.0f)
    {}

void ScanLineZBuffer::clear(){
	Framebuffer::clear();
	zBufferLine.resize(width);
	std::fill(zBufferLine.begin(), zBufferLine.end(), -std::numeric_limits<float>::infinity());
	activeEdgeIdTable.clear();
	activeEdgeIdTable.resize(height);
	deactiveEdgeIdTable.clear();
	deactiveEdgeIdTable.resize(height);
	edgeTable.clear();
	activeEdgeTable.clear();
	curFaceOffset = 0;
	edgeIdOffset = 0;

}

void ScanLineZBuffer::buildTable(const Model& model){
	Timer timer;
	timer.reset();
	timer.start();

	Mat4x4 viewMatrix;
	pRenderer->camera.getViewMatrix(viewMatrix);

	Mat4x4 projectionMatrix;
	pRenderer->camera.getProjectionMatrix(projectionMatrix);

	uint faces_size = model.faces.size();

	for (uint faceIter = 0; faceIter < faces_size; faceIter++){
		Face face = model.faces[faceIter];
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
		for(int i = 0; i < 3; i++ ){
			auto v0 = vertices[i];
			auto v1 = vertices[(i + 1) % 3];
			float y0f = v0.position.y;
			float y1f = v1.position.y;

			if(y0f > y1f){
				std::swap(v0, v1);
				y0f = v0.position.y;
				y1f = v1.position.y;

			}
			
			if(isEqualf(y0f, y1f)){

				continue;
			}
			if(std::ceil(y0f) == std::ceil(y1f)){
				continue; 
			}
			if(y0f >= float(height - 1) || y1f < 0.f){
				continue;
			}
			int y0i = std::max(0, int(std::ceil(y0f)));
			int y1i = std::min(height - 1, int(std::ceil(y1f)));

			if (y0i == y1i){
				continue;
			}

			assert(y0i < y1i);
			Edgef edge(v0, v1, edgeIdOffset, faceIter + curFaceOffset);

			edgeIdOffset++;
			edge.setCurPos(y0i);
			edgeTable.push_back(edge);

			activeEdgeIdTable[y0i].push_back(edge.edgeId);
			deactiveEdgeIdTable[y1i].push_back(edge.edgeId);
		}

  	}
	curFaceOffset += faces_size;
	timer.stop();
	std::cout << "ScanLine Table build time:" << timer.elapsed() << std::endl;
}


void ScanLineZBuffer::actScan(const Model& model){
	Timer timer;
	timer.reset();
	timer.start();

	for(int h_iter = 0; h_iter < height; h_iter++){
		zBufferLine.resize(width);
		std::fill(zBufferLine.begin(), zBufferLine.end(), -std::numeric_limits<float>::infinity());

		size_t activeEdgeTableSize = activeEdgeTable.size();
		size_t activeEdgeSize = activeEdgeIdTable[h_iter].size();
		size_t deactiveEdgeSize = deactiveEdgeIdTable[h_iter].size();

		for(auto edgeId : activeEdgeIdTable[h_iter]){
			Edgef& edge = edgeTable[edgeId];
			activeEdgeTable.push_back(edge);
		}

		for(auto edgeId : deactiveEdgeIdTable[h_iter]){
			auto iter = activeEdgeTable.begin();
			for(; iter != activeEdgeTable.end();){
				if(iter->edgeId == edgeId){
					iter = activeEdgeTable.erase(iter);
				}else{
					iter++;
				}
			}
			assert(iter == activeEdgeTable.end());
		}

		assert(activeEdgeTable.size() % 2 == 0);
		assert(activeEdgeTableSize + activeEdgeSize - deactiveEdgeSize == activeEdgeTable.size());

		std::sort(activeEdgeTable.begin(), activeEdgeTable.end(), [](const Edgef& a, const Edgef& b){
			return a.cur.x < b.cur.x;
		});
		bool isSorted = std::is_sorted(activeEdgeTable.begin(), activeEdgeTable.end(), [](const Edgef& a, const Edgef& b){
			return a.cur.x < b.cur.x;
		});		

		for(size_t i = 0; i < activeEdgeTable.size(); i++){
			Edgef &edge0 = activeEdgeTable[i];
			// std::cout << "edge0.cur.x:" << edge0.cur.x << std::endl;
			if(edge0.isPaired){
				continue;
			}
			edge0.isPaired = true;
			size_t pairId; 
			for(pairId = i + 1; pairId < activeEdgeTable.size(); pairId++){
				Edgef &edge1 = activeEdgeTable[pairId];
				if(edge0.polygonId == edge1.polygonId){
					edge1.isPaired = true;
					break; 
				}
			}
			
			Edgef &edge1 = activeEdgeTable[pairId];

			assert(edge0.cur.x <= edge1.cur.x);

			uint x0 = std::max(0, int(std::ceil(edge0.cur.x)));
			uint x1 = std::min(width - 1, int(std::ceil(edge1.cur.x)));
			if(x0 >= x1){
				continue;
			}
			float zStart = edge0.cur.z;
			Vec3f rgbStart = edge0.rgbCur;
			float zEnd = edge1.cur.z;
			Vec3f rgbEnd = edge1.rgbCur;
			float gradientDzDx = (zEnd - zStart) / (edge1.cur.x - edge0.cur.x);
			Vec3f gradientdRGBdx = (rgbEnd - rgbStart) / (edge1.cur.x - edge0.cur.x);
			zStart += gradientDzDx * (float(x0) - edge0.cur.x);
			rgbStart += gradientdRGBdx * (float(x0) - edge0.cur.x);
			for(uint x = x0; x <= x1; x++){
				if(zBufferLine[x] < zStart){
					zBufferLine[x] = zStart;
					Vec3f rgb = rgbStart;
					if(rgb.x < 0.0f){
						rgb.x = 0.0f;
					}
					if(rgb.y < 0.0f){
						rgb.y = 0.0f;
					}
					if(rgb.z < 0.0f){
						rgb.z = 0.0f;
					}
					if(rgb.x > 1.0f){
						rgb.x = 1.0f;
					}
					if(rgb.y > 1.0f){
						rgb.y = 1.0f;
					}
					if(rgb.z > 1.0f){
						rgb.z = 1.0f;
					}
					Color color(rgb.x * 255, rgb.y * 255, rgb.z * 255);
					setPixel(x, h_iter, color, zStart);
				}
				zStart += gradientDzDx;
				rgbStart += gradientdRGBdx;
			}
		}
		for(auto& edge : activeEdgeTable){
			edge.setCurPos(h_iter + 1);
			edge.isPaired = false;
		}
	}



	timer.stop();
	std::cout << "ScanLine Scan time:" << timer.elapsed() << std::endl;
}

void ScanLineZBuffer::setPixel(int x, int y, const Color& color, float depth){
  if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    int index = y * width + x;
    colorBuffer[index] = color;
}