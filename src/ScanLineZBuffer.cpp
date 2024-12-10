#include "ScanLineZBuffer.h"

ScanLineZBuffer::ScanLineZBuffer(int w, int h)
    : Framebuffer(w, h),
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
  ;
}

void ScanLineZBuffer::setPixel(int x, int y, const Color& color, float depth){
  ;
}