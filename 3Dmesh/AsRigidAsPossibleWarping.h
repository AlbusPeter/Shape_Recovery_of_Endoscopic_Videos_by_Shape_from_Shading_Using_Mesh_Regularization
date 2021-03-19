#include "defines.h"
#include "Mesh.h"
#include "Header.h"
#include <engine.h>

#ifndef __TRIPLETS__
#define __TRIPLETS__
class Triplets {
public:
	int nmax; int m; int n; int* ri; int* ci;
	double* nzval; int nnz;
public:
	Triplets() :
		nmax(0), m(-1), n(-1), ri(0), ci(0), nzval(0), nnz(0) {
	}
	~Triplets() {
		clear();
	}
	void clear() {
		if (nzval)
			delete[] nzval;
		if (ri)
			delete[] ri;
		if (ci)
			delete[] ci;

		m = n = -1;
		nnz = 0;
		nzval = 0;
		ri = ci = 0;
		nmax = 0;
	}
	void reserve(int rows, int cols, int num) {
		clear();
		m = rows;
		n = cols;
		nmax = num;
		nzval = new double[num];
		ri = new int[num];
		ci = new int[num];
	}
	void add(int r, int c, double val) {
		if (nnz >= nmax) {
			printf("Triplets::add - the maximun number of elements reached!");
			return;
		}
		ri[nnz] = r;
		ci[nnz] = c;
		nzval[nnz] = val;
		nnz++;
	}
public:
	//debug
	void print() {
		for (int i = 0; i < nnz; i++) {
			cout << "[" << i << "]: " << ri[i] << " " << ci[i] << " " << nzval[i] << std::endl;
		}
	}
};
#endif

#ifndef __ASRIGIDASPOSSIBLE__
#define __ASRIGIDASPOSSIBLE__

class ContentPreservingWarp{
private:
	//W:the bilinear coefficients Matrix
	Triplets EdgeCons, A, W;

	int columns;
	int num_edge_cons;
	int* x_index;
	int* y_index;
	int* z_index;

	//record the current mesh vertice coordinates.
	double* x;
	//the Vp x,y,z coordinates
	double* b;

	struct dataterm_element{
		int i;  //height
		int j;  //width
		cv::Point3f orgPt; //coordinate of orginal point
		cv::Point3f desPt; //coordinate of destination point
		double dataWeight;
		double v00;    //bilinear interpolation weight
		double v01;    //bilinear interpolation weight
		double v10;    //bilinear interpolation weight
		double v11;    //bilinear interpolation weight
	};

	vector<dataterm_element> dataterm;

	Mesh* source;   //source mesh
	Mesh* destin;   //destin mesh

	vector<cv::Point3f> sourcePts;
	vector<cv::Point3f> targetPts;

	int height, width;
	int quadWidth, quadHeight;
	int imgHeight, imgWidth;

private:
	void getlengthratio_1(const cv::Point3f &V1, const cv::Point3f &V2, double &u);
	void getlengthratio_2(const cv::Point3f &V1, const cv::Point3f &V2, double &u);
	void addCoefficient_1(int i, int j, int &rowCount, int* x_index, int* y_index, int* z_index, Triplets& EdgeCons);
	void addCoefficient_2(int i, int j, int &rowCount, int* x_index, int* y_index, int* z_index, Triplets& EdgeCons);
	void addCoefficient_3(int i, int j, int &rowCount, int* x_index, int* y_index, int* z_index, Triplets& EdgeCons);
	void addCoefficient_4(int i, int j, int &rowCount, int* x_index, int* y_index, int* z_index, Triplets& EdgeCons);
	void matlabSparseLinearSystem(Triplets& A, double b[], double x[]);
	void matlabIterationb(Triplets& L, double x[], double b[]);
	void CreateEdgeConstraint(Triplets& EdgeCons);

public:
	ContentPreservingWarp(int height, int width, double quadWidth, double quadHeight);
	~ContentPreservingWarp();

	void SetPoints(vector<Point3f> InPts);

	void SetControlPts(const vector<cv::Point3f> &inputsPts, const vector<cv::Point3f> &outputsPts);
	void Solve();
	void Iteration(int n);

	Mesh* GetDestinMesh(){ return destin; }
	Mesh* GetSourceMesh(){ return source; }
	vector<cv::Point3f> GetSourcePoints(){ return this->sourcePts; }
	vector<cv::Point3f> GetTargetPoints(){ return this->targetPts; }
};
#endif