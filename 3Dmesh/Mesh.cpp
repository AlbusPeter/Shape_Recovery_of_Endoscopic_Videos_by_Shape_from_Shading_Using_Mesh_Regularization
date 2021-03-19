#include "Mesh.h"

Quad::Quad()
{
	V00.x = 0.0; V00.y = 0.0; V00.z = 0.0;
	V01.x = 0.0; V01.y = 0.0; V01.z = 0.0;
	V10.x = 0.0; V10.y = 0.0; V10.z = 0.0;
	V11.x = 0.0; V11.y = 0.0; V11.z = 0.0;
}

Quad::Quad(const Quad &inQuad)
{
	V00.x = inQuad.V00.x; V00.y = inQuad.V00.y; V00.z = inQuad.V00.z;
	V01.x = inQuad.V01.x; V01.y = inQuad.V01.y; V01.z = inQuad.V01.z;
	V10.x = inQuad.V10.x; V10.y = inQuad.V10.y; V10.z = inQuad.V10.z;
	V11.x = inQuad.V11.x; V11.y = inQuad.V11.y; V11.z = inQuad.V11.z;
}

Quad::Quad(const cv::Point3f &inV00, const cv::Point3f &inV01, const cv::Point3f &inV10, const cv::Point3f &inV11)
{
	V00.x = inV00.x; V00.y = inV00.y; V00.z = inV00.z;
	V01.x = inV01.x; V01.y = inV01.y; V01.z = inV01.z;
	V10.x = inV10.x; V10.y = inV10.y; V10.z = inV10.z;
	V11.x = inV11.x; V11.y = inV11.y; V11.z = inV11.z;
}

Quad::~Quad()
{

}

bool Quad::isPointIn(const cv::Point3f &pt) const
{
	bool in1 = isPointInTriangular(pt, V00, V01, V11);
	bool in2 = isPointInTriangular(pt, V00, V10, V11);
	if ((in1 || in2) && (pt.z == V00.z))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//if the quad is cubic, the coefficient is independent of z coordinate.
bool Quad::getBilinearCoordinates(const cv::Point3f &pt, vector<double> &coefficient) const
{
	double k1 = -1;
	double k2 = -1;

	double a_x = V00.x - V01.x - V10.x + V11.x;
	double b_x = -V00.x + V01.x;
	double c_x = -V00.x + V10.x;
	double d_x = V00.x - pt.x;

	double a_y = V00.y - V01.y - V10.y + V11.y;
	double b_y = -V00.y + V01.y;
	double c_y = -V00.y + V10.y;
	double d_y = V00.y - pt.y;

	double bigA = -a_y*b_x + b_y*a_x;
	double bigB = -a_y*d_x - c_y*b_x + d_y*a_x + b_y*c_x;
	double bigC = -c_y*d_x + d_y*c_x;

	double tmp1 = -1;
	double tmp2 = -1;
	double tmp3 = -1;
	double tmp4 = -1;

	if (bigB*bigB - 4 * bigA*bigC >= 0.0)
	{
		if (abs(bigA) >= 0.000001)
		{
			tmp1 = (-bigB + sqrt(bigB*bigB - 4 * bigA*bigC)) / (2 * bigA);
			tmp2 = (-bigB - sqrt(bigB*bigB - 4 * bigA*bigC)) / (2 * bigA);
		}
		else
		{
			tmp1 = -bigC / bigB;
		}

		if (tmp1 >= -0.999999 && tmp1 <= 1.000001)
		{
			// 			if (abs(a_x*tmp1 + c_x) < 0.000001)
			// 			{
			// 				tmp3 = -(b_y*tmp1 + d_y) / (a_y*tmp1 + c_y);
			// 			}
			// 			else
			// 			{
			// 				tmp3 = -(b_x*tmp1 + d_x) / (a_x*tmp1 + c_x);
			// 			}
			/*			printf("%f\n", tmp3);*/

			tmp3 = -(b_y*tmp1 + d_y) / (a_y*tmp1 + c_y);
			tmp4 = -(b_x*tmp1 + d_x) / (a_x*tmp1 + c_x);
			if (tmp3 >= -0.999999 && tmp3 <= 1.000001)
			{
				k1 = tmp1;
				k2 = tmp3;
			}
			else if (tmp4 >= -0.999999 && tmp4 <= 1.000001)
			{
				k1 = tmp1;
				k2 = tmp4;
			}
		}
		if (tmp2 >= -0.999999 && tmp2 <= 1.000001)
		{
			// 			if (abs(a_x*tmp1 + c_x) < 0.000001)
			// 			{
			// 				tmp3 = -(b_y*tmp2 + d_y) / (a_y*tmp2 + c_y);
			// 			}
			// 			else
			// 			{
			// 				tmp3 = -(b_x*tmp2 + d_x) / (a_x*tmp2 + c_x);
			// 			}
			tmp3 = -(b_y*tmp2 + d_y) / (a_y*tmp2 + c_y);
			tmp4 = -(b_x*tmp2 + d_x) / (a_x*tmp2 + c_x);
			if (tmp3 >= -0.999999 && tmp3 <= 1.000001)
			{
				k1 = tmp2;
				k2 = tmp3;
			}
			else if (tmp4 >= -0.999999 && tmp4 <= 1.000001)
			{
				k1 = tmp2;
				k2 = tmp4;
			}
		}
	}
	if (k1 >= -0.999999 && k1 <= 1.000001 && k2 >= -0.999999 && k2 <= 1.000001)
	{
		double coe1 = (1.0 - k1)*(1.0 - k2);
		double coe2 = k1*(1.0 - k2);
		double coe3 = (1.0 - k1)*k2;
		double coe4 = k1*k2;

		coefficient.push_back(coe1);
		coefficient.push_back(coe2);
		coefficient.push_back(coe3);
		coefficient.push_back(coe4);

		return true;
	}
	else
	{
		// 		printf("(%lf, %lf), (%lf, %lf), (%lf, %lf), (%lf, %lf) -> (%lf, %lf) -> (%lf, %lf)\n", V00.x, V00.y, V01.x, V01.y, V10.x, V10.y, V11.x, V11.y, pt.x, pt.y, k1, k2);
		// 		printf("(%lf, %lf, %lf, %lf) -> (%lf, %lf, %lf, %lf)\n", a_x, b_x, c_x, d_x, a_y, b_y, c_y, d_x);
		// 		printf("(%lf, %lf, %lf, %lf) -> (%lf, %lf, %lf)\n", tmp1, tmp2, tmp3, tmp4, bigA, bigB, bigC);
		return false;
	}


	// 	if (V01.x-V00.x != 0.0 && V11.x-V10.x!= 0.0)
	// 	{
	// 		double k1 = (pt.x-V00.x)/(V01.x-V00.x);
	// 		double k2 = (pt.x-V10.x)/(V11.x-V10.x);
	// 		double B0y = (1-k1)*V00.y+k1*V01.y;
	// 		double B1y = (1-k2)*V10.y+k2*V11.y;
	// 		if (B1y-B0y != 0.0)
	// 		{
	// 			double k3 = (pt.y-B0y)/(B1y-B0y);
	// 			double c00 = (1-k1)*(1-k3);
	// 			double c01 = k1*(1-k3);
	// 			double c10 = (1-k2)*k3;
	// 			double c11 = k2*k3;
	// 			coefficient.push_back(c00);
	// 			coefficient.push_back(c01);
	// 			coefficient.push_back(c10);
	// 			coefficient.push_back(c11);
	// 
	// 			return true;
	// 		}
	// 		else
	// 		{
	// 			return false;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		return false;
	// 	}
}

bool Quad::getBilinearCoordinates(const cv::Point3f &pt, double* &coefficient) const{

	if (!this->isPointIn(pt)) 	return false;

	if (pt.x == 0 && pt.y == 0)      return false;

	double d1, d2, d3, d4;

	double x0, y0, x1, y1, x, y;

	x0 = this->V00.x;
	y0 = this->V00.y;
	x1 = this->V11.x;
	y1 = this->V11.y;
	x = pt.x;
	y = pt.y;

	d1 = y - y0; d1 = abs(d1);
	d2 = y - y1; d2 = abs(d2);
	d3 = x - x0; d3 = abs(d3);
	d4 = x - x1; d4 = abs(d4);

	coefficient[0] = d2*d4 / (d1*d3 + d2*d3 + d1*d4 + d2*d4);//V00
	coefficient[1] = d1*d4 / (d1*d3 + d2*d3 + d1*d4 + d2*d4);//V01
	coefficient[2] = d2*d3 / (d1*d3 + d2*d3 + d1*d4 + d2*d4);//V10
	coefficient[3] = d1*d3 / (d1*d3 + d2*d3 + d1*d4 + d2*d4);//V11

	return true;
}

Mesh::Mesh()
{
	imgRows = 0;
	imgCols = 0;
	width = 0;
	height = 0;
}

Mesh::Mesh(const Mesh &inMesh)
{
	imgRows = inMesh.imgRows;
	imgCols = inMesh.imgCols;
	width = inMesh.width;
	height = inMesh.height;
	xMat = cv::Mat::zeros(height, width, CV_64FC1);
	yMat = cv::Mat::zeros(height, width, CV_64FC1);
	zMat = cv::Mat::zeros(height, width, CV_64FC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			setVertex(i, j, inMesh.getVertex(i, j));
		}
	}
}

Mesh::Mesh(int rows, int cols)
{
	imgRows = rows;
	imgCols = cols;
	width = 0;
	height = 0;
}

Mesh::Mesh(int rows, int cols, double quadWidth, double quadHeight)
{
	imgRows = rows;
	imgCols = cols;
	buildMesh(quadWidth, quadHeight);
	this->quadWidth = quadWidth;
	this->quadHeight = quadHeight;
}

Mesh::~Mesh()
{
}

Quad Mesh::getQuad(int i, int j) const
{
	cv::Point3f V00;
	cv::Point3f V01;
	cv::Point3f V10;
	cv::Point3f V11;

	//printf("%d %d %d\n", i, j, k);
	//printf("%d %d %d\n", this->xMat.dims, this->yMat.dims, this->zMat.dims);

	V00 = getVertex(i - 1, j - 1);
	V01 = getVertex(i - 1, j);
	V10 = getVertex(i, j - 1);
	V11 = getVertex(i, j);

	Quad qd(V00, V01, V10, V11);

	return qd;
}

cv::Point3f Mesh::getVertex(int i, int j) const
{
	double x;
	double y;
	double z;

	x = xMat.at<double>(i, j);
	y = yMat.at<double>(i, j);
	z = zMat.at<double>(i, j);

	return cv::Point3f(x, y, z);
}

void Mesh::setVertex(int i, int j, const cv::Point3f &pos)
{
	xMat.at<double>(i, j) = pos.x;
	yMat.at<double>(i, j) = pos.y;
	zMat.at<double>(i, j) = pos.z;
}

void Mesh::initialize(int w, int h)
{
	width = w;
	height = h;
	xMat = cv::Mat::zeros(height, width, CV_64FC1) - 10000;
	yMat = cv::Mat::zeros(height, width, CV_64FC1) - 10000;
	zMat = cv::Mat::zeros(height, width, CV_64FC1) - 10000;
}

void Mesh::buildMesh(double quadWidth, double quadHeight)
{
	vector<int> xSet;
	vector<int> ySet;

	for (int x = 0; imgCols - x > 0.5*quadWidth; x += quadWidth)
	{
		xSet.push_back(x);
	}
	xSet.push_back(imgCols - 1);
	for (int y = 0; imgRows - y > 0.5*quadHeight; y += quadHeight)
	{
		ySet.push_back(y);
	}
	ySet.push_back(imgRows - 1);

	width = xSet.size();
	height = ySet.size();

	xMat.create(height, width, CV_64FC1);
	yMat.create(height, width, CV_64FC1);
	zMat.create(height, width, CV_64FC1);

	//printf("%d %d %d\n", this->xMat.dims, this->yMat.dims, this->zMat.dims);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			xMat.at<double>(y, x) = xSet[x];
			yMat.at<double>(y, x) = ySet[y];
			zMat.at<double>(y, x) = 0;
		}
	}
}

bool isPointInTriangular(const cv::Point3f &pt, const cv::Point3f &V0, const cv::Point3f &V1, const cv::Point3f &V2)
{
	double lambda1 = ((V1.y - V2.y)*(pt.x - V2.x) + (V2.x - V1.x)*(pt.y - V2.y)) / ((V1.y - V2.y)*(V0.x - V2.x) + (V2.x - V1.x)*(V0.y - V2.y));
	double lambda2 = ((V2.y - V0.y)*(pt.x - V2.x) + (V0.x - V2.x)*(pt.y - V2.y)) / ((V2.y - V0.y)*(V1.x - V2.x) + (V0.x - V2.x)*(V1.y - V2.y));
	double lambda3 = 1 - lambda1 - lambda2;
	if (lambda1 >= 0.0 && lambda1 <= 1.0 && lambda2 >= 0.0 && lambda2 <= 1.0 && lambda3 >= 0.0 && lambda3 <= 1.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

