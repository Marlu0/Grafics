#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"

Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixelUnsafe( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size)
		return false;

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
		return false;

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
	
	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixelUnsafe(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		perror("Failed to open file: ");
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	return true;
}

// Draw Line using DDA Algorithm
void Image::DrawLineDDA(int x0, int y0, float z0, int x1, int y1, float z1, const Color& c, FloatImage* zbuffer)
{
	// Calculate dx and dy
	int dx = x1 - x0;
	int dy = y1 - y0;

	// Compute the largest leg d
	int d = std::max(std::abs(dx), std::abs(dy));

	// Compute the direction step vector v
	double vx = static_cast<double>(dx) / d;
	double vy = static_cast<double>(dy) / d;
    double vz = (z1 - z0) / d;
	// Starting point
	double x = x0;
	double y = y0;
    double z = z0;

	// Iterate d times to paint pixels
	for (int i = 0; i <= d; ++i) {
		// Paint the current pixel (flooring x and y)
        if(zbuffer->GetPixel(std::floor(x), std::floor(y)) > z){
            SetPixel(static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y)), c);
            zbuffer->SetPixel(std::floor(x), std::floor(y), z);
        }
		// Increment x and y by the step vector
		x += vx;
		y += vy;
        z += vz;
	}
	
}

// Draw Triangle
void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table) {
	// Calculate dx and dy
	int dx = x1 - x0;
	int dy = y1 - y0;

	// Compute the largest leg d
	int d = std::max(std::abs(dx), std::abs(dy));

	// Compute the direction step vector v
	double vx = static_cast<double>(dx) / d;
	double vy = static_cast<double>(dy) / d;

	// Starting point
	double x = x0;
	double y = y0;

	// Iterate d times to paint pixels
	for (int i = 0; i <= d; ++i) {
		// Paint the current pixel (flooring x and y)
		table[y].minX = std::min(table[y].minX, static_cast<int>(x));
		table[y].maxX = std::max(table[y].maxX, static_cast<int>(x));

		// Increment x and y by the step vector
		x += vx;
		y += vy;
	}
}

void Image::DrawTriangle(const sTriangleInfo& triangle, bool isFilled, const Color& c, FloatImage* zbuffer, bool occlusions)
{
	// Find the bounds of the triangle
	int minY = std::min({ triangle.p1.y, triangle.p2.y, triangle.p3.y });
	int maxY = std::max({ triangle.p1.y, triangle.p2.y, triangle.p3.y });

	// Clamp the bounds to the image dimensions
	minY = std::max(0, minY);
	maxY = std::min(static_cast<int>(height) - 1, maxY);

	// Fill the triangle if required
	if (isFilled) {
		// Populate the AET using ScanLineDDA

		// Create and initialize the Active Edge Table (AET)
		std::vector<Cell> aet(maxY + 1);

		ScanLineDDA(triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, aet);
		ScanLineDDA(triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y, aet);
		ScanLineDDA(triangle.p3.x, triangle.p3.y, triangle.p1.x, triangle.p1.y, aet);
		for (int y = minY; y <= maxY; ++y) {
			if (aet[y].minX <= aet[y].maxX) {
                for (int x = aet[y].minX; x <= aet[y].maxX; ++x) {
                    
					float Aa = abs(((triangle.p2.x - x) * (triangle.p3.y - y)) - ((triangle.p2.y - y) * (triangle.p3.x - x))) / 2.0f;
					float Ab = abs(((triangle.p1.x - x) * (triangle.p3.y - y)) - ((triangle.p1.y - y) * (triangle.p3.x - x))) / 2.0f;
					float Ac = abs(((triangle.p1.x - x) * (triangle.p2.y - y)) - ((triangle.p1.y - y) * (triangle.p2.x - x))) / 2.0f;
					float Aabc = abs((triangle.p2.x - triangle.p1.x) * (triangle.p3.y - triangle.p1.y) - (triangle.p2.y - triangle.p1.y) * (triangle.p3.x - triangle.p1.x)) * 0.5f;

                    float alpha = Aa / Aabc;
                    float beta = Ab / Aabc;
                    float gamma = Ac / Aabc;
                    
                    
                    float z_interpolated = alpha*triangle.p1.z + beta*triangle.p2.z + gamma*triangle.p3.z;
                    
					if (occlusions) {
						if (zbuffer->GetPixel(x, y) > z_interpolated) {
							SetPixel(x, y, c);
							zbuffer->SetPixel(x, y, z_interpolated);

						}
						else continue;
					}
					else if (!occlusions){
						SetPixel(x, y, c);
						zbuffer->SetPixel(x, y, z_interpolated);
					}
                    
				}
			}
		}
	}
	// Draw the border
	DrawLineDDA(triangle.p1.x, triangle.p1.y, triangle.p1.z, triangle.p2.x, triangle.p2.y,triangle.p2.z, c , zbuffer);
	DrawLineDDA(triangle.p2.x, triangle.p2.y, triangle.p2.z, triangle.p3.x,  triangle.p3.y, triangle.p3.z, c, zbuffer);
	DrawLineDDA(triangle.p3.x, triangle.p3.y, triangle.p3.z, triangle.p1.x, triangle.p1.y,triangle.p1.z, c, zbuffer);
}

void Image::DrawTriangleInterpolated(const sTriangleInfo& triangle, FloatImage* zbuffer, Image* texture, bool occlusions){
	// Find the bounds of the triangle
	int minY = std::min({ triangle.p1.y, triangle.p2.y, triangle.p3.y });
	int maxY = std::max({ triangle.p1.y, triangle.p2.y, triangle.p3.y });

	// Clamp the bounds to the image dimensions
	minY = std::max(0, minY);
	maxY = std::min(static_cast<int>(height) - 1, maxY);

	std::vector<Cell> aet(maxY + 1);

	ScanLineDDA(triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, aet);
	ScanLineDDA(triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y, aet);
	ScanLineDDA(triangle.p3.x, triangle.p3.y, triangle.p1.x, triangle.p1.y, aet);
	for (int y = minY; y <= maxY; ++y) {
		if (aet[y].minX <= aet[y].maxX) {
			for (int x = aet[y].minX; x <= aet[y].maxX; ++x)
			{
                
				float Aa = abs(((triangle.p2.x - x) * (triangle.p3.y - y)) - ((triangle.p2.y - y) * (triangle.p3.x - x))) / 2.0f;
				float Ab = abs(((triangle.p1.x - x) * (triangle.p3.y - y)) - ((triangle.p1.y - y) * (triangle.p3.x - x))) / 2.0f;
				float Ac = abs(((triangle.p1.x - x) * (triangle.p2.y - y)) - ((triangle.p1.y - y) * (triangle.p2.x - x))) / 2.0f;
				float Aabc = abs((triangle.p2.x - triangle.p1.x) * (triangle.p3.y - triangle.p1.y) - (triangle.p2.y - triangle.p1.y) * (triangle.p3.x - triangle.p1.x)) * 0.5f;
				
				float alpha = abs(Aa / Aabc);
				float beta = abs(Ab / Aabc);
				float gamma = abs(Ac / Aabc);
                
                float z_interpolated = beta*triangle.p1.z + alpha*triangle.p2.z + gamma*triangle.p3.z;
                
				if (occlusions) {
					if (zbuffer->GetPixel(x, y) > z_interpolated) {

						if (texture == nullptr) {
							Color finalColor = (triangle.c1 * alpha) + (triangle.c2 * beta) + (triangle.c3 * gamma);

							SetPixel(x, y, finalColor);

							zbuffer->SetPixel(x, y, z_interpolated);
						}
						else {

							float u = alpha * triangle.uv1.x + beta * triangle.uv2.x + gamma * triangle.uv3.x;
							float v = alpha * triangle.uv1.y + beta * triangle.uv2.y + gamma * triangle.uv3.y;

							Vector2 uv(u, v);
							float text_x = u * texture->width - 1;
							float text_y = v * texture->height - 1;

							Color tex_color = texture->GetPixelSafe(text_x, text_y);

							SetPixel(x, y, tex_color);

							zbuffer->SetPixel(x, y, z_interpolated);

						}
					}
				}
				else if (!occlusions) {
					if (texture == nullptr) {
						Color finalColor = (triangle.c1 * alpha) + (triangle.c2 * beta) + (triangle.c3 * gamma);

						SetPixel(x, y, finalColor);

						zbuffer->SetPixel(x, y, z_interpolated);
					}
					else {

						float u = alpha * triangle.uv1.x + beta * triangle.uv2.x + gamma * triangle.uv3.x;
						float v = alpha * triangle.uv1.y + beta * triangle.uv2.y + gamma * triangle.uv3.y;

						Vector2 uv(u, v);
						float text_x = u * texture->width - 1;
						float text_y = v * texture->height - 1;

						Color tex_color = texture->GetPixelSafe(text_x, text_y);

						SetPixel(x, y, tex_color);

						zbuffer->SetPixel(x, y, z_interpolated);

					}
				}
			}
		}
	}
}

#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}
