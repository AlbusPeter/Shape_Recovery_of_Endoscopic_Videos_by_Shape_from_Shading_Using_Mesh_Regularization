#pragma once

#include <vector>
#include <stdio.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
using namespace std;


#ifndef __QUAD__
#define __QUAD__

class Quad
{
public:
	cv::Point3f V00;
	cv::Point3f V01;
	cv::Point3f V10;
	cv::Point3f V11;

	Quad();
	Quad(const Quad &inQuad);
	Quad(const cv::Point3f &inV00, const cv::Point3f &inV01, const cv::Point3f &inV10, const cv::Point3f &inV11);
	~Quad();

	bool isPointIn(const cv::Point3f &pt) const;
	bool getBilinearCoordinates(const cv::Point3f &pt, vector<double> &coefficient) const;
	bool getBilinearCoordinates(const cv::Point3f &pt, double* &coefficient) const;
	inline void printQuad(){
		printf("V00 = %f %f\n", V00);
		printf("V01 = %f %f\n", V01);
		printf("V10 = %f %f\n", V10);
		printf("V11 = %f %f\n", V11);
	}
};

#endif

#ifndef __MESH__
#define __MESH__

class Mesh
{
public:
	int imgRows;
	int imgCols;

	int width;
	int height;

	int quadWidth;
	int quadHeight;

	Mesh();
	Mesh(const Mesh &inMesh);
	Mesh(int rows, int cols);
	Mesh(int rows, int cols, double quadWidth, double quadHeight);
	~Mesh();

	cv::Point3f getVertex(int i, int j) const;
	Quad getQuad(int i, int j) const;
	void setVertex(int i, int j, const cv::Point3f &pos);

	void initialize(int w, int h);
	void buildMesh(double quadWidth, double quadHeight);

private:
	cv::Mat xMat;
	cv::Mat yMat;
	cv::Mat zMat;
};

#endif

bool isPointInTriangular(const cv::Point3f &pt, const cv::Point3f &V0, const cv::Point3f &V1, const cv::Point3f &V2);