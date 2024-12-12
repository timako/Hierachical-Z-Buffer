#include "framebuffer.h"
#include <fstream>
#include <iostream>
#include <cstring>

Framebuffer::Framebuffer(int w, int h)
    : width(w), height(h),
      colorBuffer(w * h, Color(0, 0, 0))
      {}

void Framebuffer::clear(const Color& clearColor) {
    std::fill(colorBuffer.begin(), colorBuffer.end(), clearColor);
}

// Simple BMP writer
void Framebuffer::saveToBMP(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // BMP Header
    uint16_t bfType = 0x4D42; // 'BM'
    uint32_t bfSize = 54 + 3 * width * height;
    uint32_t bfReserved = 0;
    uint32_t bfOffBits = 54;

    // DIB Header
    uint32_t biSize = 40;
    int32_t biWidth = width;
    int32_t biHeight = height;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 24;
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 3 * width * height;
    int32_t biXPelsPerMeter = 0;
    int32_t biYPelsPerMeter = 0;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;

    // Write BMP Header
    ofs.write(reinterpret_cast<const char*>(&bfType), sizeof(bfType));
    ofs.write(reinterpret_cast<const char*>(&bfSize), sizeof(bfSize));
    ofs.write(reinterpret_cast<const char*>(&bfReserved), sizeof(bfReserved));
    ofs.write(reinterpret_cast<const char*>(&bfOffBits), sizeof(bfOffBits));

    // Write DIB Header
    ofs.write(reinterpret_cast<const char*>(&biSize), sizeof(biSize));
    ofs.write(reinterpret_cast<const char*>(&biWidth), sizeof(biWidth));
    ofs.write(reinterpret_cast<const char*>(&biHeight), sizeof(biHeight));
    ofs.write(reinterpret_cast<const char*>(&biPlanes), sizeof(biPlanes));
    ofs.write(reinterpret_cast<const char*>(&biBitCount), sizeof(biBitCount));
    ofs.write(reinterpret_cast<const char*>(&biCompression), sizeof(biCompression));
    ofs.write(reinterpret_cast<const char*>(&biSizeImage), sizeof(biSizeImage));
    ofs.write(reinterpret_cast<const char*>(&biXPelsPerMeter), sizeof(biXPelsPerMeter));
    ofs.write(reinterpret_cast<const char*>(&biYPelsPerMeter), sizeof(biYPelsPerMeter));
    ofs.write(reinterpret_cast<const char*>(&biClrUsed), sizeof(biClrUsed));
    ofs.write(reinterpret_cast<const char*>(&biClrImportant), sizeof(biClrImportant));

    // Write Pixel Data (BMP stores pixels bottom-up)
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            const Color& color = colorBuffer[y * width + x];
            ofs.put(static_cast<char>(color.b));
            ofs.put(static_cast<char>(color.g));
            ofs.put(static_cast<char>(color.r));
        }
        // Padding for 4-byte alignment
        int padding = (4 - (width * 3) % 4) % 4;
        for (int p = 0; p < padding; ++p) {
            ofs.put(0);
        }
    }

    ofs.close();
    std::cout << "Image saved to " << filename << std::endl;
}

SimpleZbuffer::SimpleZbuffer(int w, int h)
    : Framebuffer(w, h),
      depthBuffer(w * h, -std::numeric_limits<float>::infinity()) {}

void SimpleZbuffer::clear(const Color& clearColor) {
    std::fill(colorBuffer.begin(), colorBuffer.end(), clearColor);
    std::fill(depthBuffer.begin(), depthBuffer.end(), -std::numeric_limits<float>::infinity());
}

void SimpleZbuffer::setPixel(int x, int y, const Color& color, float depth) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    int index = y * width + x;
    if (depth > depthBuffer[index]) {
        depthBuffer[index] = depth;
        colorBuffer[index] = color;
    }
    if(y == 733){
        colorBuffer[index] = Color(0, 255, 0);
    }
}
