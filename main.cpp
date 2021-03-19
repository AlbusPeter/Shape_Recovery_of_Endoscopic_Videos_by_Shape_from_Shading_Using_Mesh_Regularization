#include "Viewer\Viewer.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include "3Dmesh\Header.h"
#include "3Dmesh\AsRigidAsPossibleWarping.h"
#include <engine.h>

#define PI 3.14159


using namespace std;
using namespace cv;

Engine* ep;

int flag_type = 0;	//Draw Mesh Or Model
int index_num = 0;	//The current video frame
int n = 0;	//The number of quads per line.
int rowPtsnum = 0;
vector<vector<SColorPoint3D>> vP;	//The final mesh containner
vector<vector<SColorPoint3D>> vPr;	//The Row data points.

GLubyte texture[N][306][370][3];	//The containner of the texture  T[number_of_frame][Height][Width][3] three channels of color.
GLuint texName[N];	//The containner of the texture name. TN[number_of_frame]
vector<Point3f> avg;	//average adjustment of the coordinates (make the model in the center).

vector<vector<SColorPoint3D>> Read3DPoints1(vector<Mat>& img, vector<vector<Point3f>> InPts){
	vector<vector<SColorPoint3D>> pts(InPts.size());
	extern vector<Point3f> avg;
	for (int w = 0; w < InPts.size(); w++){
		Mat colormap = img[w];

		double avgx = 0;
		double avgy = 0;
		double avgz = 0;

		for (int k = 0; k < InPts[w].size(); k++){
			SColorPoint3D sp3d;
			sp3d.X = 1.0*InPts[w].at(k).x;
			sp3d.Y = -1.0*InPts[w].at(k).y;	//keep the model in the front.
			sp3d.Z = -1.0*InPts[w].at(k).z; //The depth of points coming from sfs is reversed.Thus here times -1.
			//printf("%f %f %f\n", InPts[w].at(k).x, InPts[w].at(k).y, InPts[w].at(k).z);

			avgx += sp3d.X;
			avgy += sp3d.Y;
			avgz += sp3d.Z;

			sp3d.R = colormap.at<cv::Vec3b>(int(InPts[w].at(k).y), int(InPts[w].at(k).x))[2] / 255.0;
			sp3d.G = colormap.at<cv::Vec3b>(int(InPts[w].at(k).y), int(InPts[w].at(k).x))[1] / 255.0;
			sp3d.B = colormap.at<cv::Vec3b>(int(InPts[w].at(k).y), int(InPts[w].at(k).x))[0] / 255.0;	//the color of the points.

			pts[w].push_back(sp3d);
		}

		for (int i = 0; i < 306; i++){	//i<height
			for (int j = 0; j < 370; j++){	//j<width
				texture[w][i][j][2] = GLubyte(colormap.at<cv::Vec3b>(i, j)[0]);		//bitmap RGB    Mat  BGR !!!
				texture[w][i][j][1] = GLubyte(colormap.at<cv::Vec3b>(i, j)[1]);
				texture[w][i][j][0] = GLubyte(colormap.at<cv::Vec3b>(i, j)[2]);		//input the texture from each frame.
			}
		}

		avgx /= pts[w].size();
		avgy /= pts[w].size();
		avgz /= pts[w].size();

		//printf("%f %f %f\n\n", avgx, avgy, avgz);
		avg.push_back(Point3f(avgx, avgy, avgz));

		for (int i = 0; i < pts[w].size(); i++){
			pts[w].at(i).X -= avgx;
			pts[w].at(i).Y -= avgy;
			pts[w].at(i).Z -= avgz;
			//printf("%f %f %f\n", pts[w].at(i).X, pts[w].at(i).Y, pts[w].at(i).Z);
		}
		//printf("\n");
	}
	printf("PointsCloud Loaded!\n");
	return pts;
}

vector<vector<SColorPoint3D>> Read3DPoints2(vector<Mat>& img, vector<vector<Point3f>> InPts){
	vector<vector<SColorPoint3D>> pts(InPts.size());
	extern vector<Point3f> avg;
	for (int w = 0; w < InPts.size(); w++){
		Mat colormap = img[w];

		double avgx = 0;
		double avgy = 0;
		double avgz = 0;

		for (int k = 0; k < InPts[w].size(); k++){
			SColorPoint3D sp3d;
			sp3d.X = 1.0*InPts[w].at(k).x;
			sp3d.Y = -1.0*InPts[w].at(k).y;	//keep the model in the front.
			sp3d.Z = -1.0*InPts[w].at(k).z; //The depth of points coming from sfs is reversed.Thus here times -1.
			//printf("%f %f %f\n", InPts[w].at(k).x, InPts[w].at(k).y, InPts[w].at(k).z);

			avgx += sp3d.X;
			avgy += sp3d.Y;
			avgz += sp3d.Z;

			sp3d.R = colormap.at<cv::Vec3b>(int(InPts[w].at(k).y), int(InPts[w].at(k).x))[2] / 255.0;
			sp3d.G = colormap.at<cv::Vec3b>(int(InPts[w].at(k).y), int(InPts[w].at(k).x))[1] / 255.0;
			sp3d.B = colormap.at<cv::Vec3b>(int(InPts[w].at(k).y), int(InPts[w].at(k).x))[0] / 255.0;	//the color of the points.

			pts[w].push_back(sp3d);
		}

		avgx /= pts[w].size();
		avgy /= pts[w].size();
		avgz /= pts[w].size();

		//printf("%f %f %f\n\n", avgx, avgy, avgz);
		avg.push_back(Point3f(avgx, avgy, avgz));

		for (int i = 0; i < pts[w].size(); i++){
			pts[w].at(i).X -= avgx;
			pts[w].at(i).Y -= avgy;
			pts[w].at(i).Z -= avgz;
			//printf("%f %f %f\n", pts[w].at(i).X, pts[w].at(i).Y, pts[w].at(i).Z);
		}
		//printf("\n");
	}
	printf("PointsCloud Loaded!\n");
	return pts;
}

vector<Point3f> getMeshPoints(int n, vector<Point3f> InPts, int height, int width, int quadWidth, int quadHeight){
	if (!(ep = engOpen("\0"))){
		fprintf(stderr, "\nCan't start MATLAB engine\n");
	}
	vector<Point3f> Pts;

	ContentPreservingWarp cpw(height, width, quadWidth, quadHeight);
	cpw.SetPoints(InPts);
	cpw.Solve();
	cpw.Iteration(10); //the iteraion times can be changed here.

	int k = pow(2.0, n) + 1;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++){
			Pts.push_back(Point3f(cpw.GetSourceMesh()->getVertex(i, j).x, cpw.GetSourceMesh()->getVertex(i, j).y, cpw.GetSourceMesh()->getVertex(i, j).z));
			//printf("%f %f %f\n", cpw.GetSourceMesh()->getVertex(i, j).x, cpw.GetSourceMesh()->getVertex(i, j).y, cpw.GetSourceMesh()->getVertex(i, j).z);
		}
	}
	//printf("\n");

	return Pts;
}

vector<Mat> getFrame(){
	printf("Read Video\n");
	cv::VideoCapture capture("test.avi");
	// check if video successfully opened
	if (!capture.isOpened())
	{
		cerr << "The video can not open!";
		exit(0);
	}
	/*double m_fps = capture.get(CV_CAP_PROP_FPS);
	int m_video_height = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	int m_video_width = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);*/
	Mat frame; // current video frame
	vector<Mat> dst;
	printf("extract frames...");
	int frame_count = 0;
	while (capture.read(frame))
	{
		Mat frame_copy = frame.clone();
		dst.push_back(frame_copy);
		frame_count++;
	}
	//printf("%d\n", frame_count);
	capture.release();
	printf("Video Capture Done!\n");

	return dst;
}

vector<vector<Point3f>> sfscal(bool flag, vector<Mat>& img){
	vector<vector<Point3f>> dstPts(img.size());
	for (int w = 0; w < img.size(); w++){
		Mat Pic1 = img[w].clone();
		Mat Pic2;
		Pic1.copyTo(Pic2);
		cvtColor(Pic2, Pic2, CV_BGR2GRAY);
		int m, n, r;
		m = Pic2.size().width;
		n = Pic2.size().height;
		r = Pic2.channels();

		Mat image = Mat::zeros(m*n, 1, CV_8U);
		int pos = 0;
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				image.at<uchar>(pos) = Pic2.at<uchar>(j, i);
				pos++;
			}
		}

		//define the number of iteration
		int iter = 2;
		//define the light source direction
		float sl = 52.46;  //slant(in degree)
		float tl = 11.73;  //tilt(in degree)
		float slp = sl * 2 * PI / 360;  // slant
		float tlp = tl * 2 * PI / 360;  // tilt
		float Ps = cos(tlp)*sin(slp) / cos(slp);
		float Qs = sin(tlp)*sin(slp) / cos(slp);

		Mat Zn = Mat::zeros(n, m, CV_32F);
		Mat Zn1 = Mat::zeros(n, m, CV_32F);
		Mat Si1 = Mat::zeros(n, m, CV_32F);
		Mat Si = Mat::zeros(n, m, CV_32F);

		//assume the initial estimate zero at time n-1
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				Zn1.at<float>(j, i) = 0.0;
				Si1.at<float>(j, i) = 0.01;
				//cout << i << " " << j << " " << endl;
			}
		}

		float Wn = 0.0001*0.0001;
		float p, q, pq, PQs, Eij, fZ, dfZ;
		float Y, K;
		Mat depth;
		for (int I = 0; I < iter; I++)
		{
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)    // calculate - f(Zij) & df(Zij)
				{
					if ((j <= 0) || (i <= 0))
					{
						p = 0.0;
						q = 0.0;
					}
					else
					{
						p = Zn1.at<float>(j, i) - Zn1.at<float>(j - 1, i);
						q = Zn1.at<float>(j, i) - Zn1.at<float>(j, i - 1);
					}
					pq = 1.0 + p*q + q*q;
					PQs = 1.0 + Ps*Ps + Qs*Qs;
					Eij = image.at<uchar>(i*n + j) / 255.0;
					fZ = -1.0*(Eij - max(float(0.0), (1 + p*Ps + q*Qs) / (sqrt(pq)*sqrt(PQs))));
					dfZ = -1.0*((Ps + Qs) / (sqrt(pq)*sqrt(PQs)) - (p + q)*(1.0 + p*Ps + q*Qs) / (sqrt(pq*pq*pq)*sqrt(PQs)));
					Y = fZ + dfZ*Zn1.at<float>(j, i);
					K = Si1.at<float>(j, i)*dfZ / (Wn + dfZ*Si1.at<float>(j, i)*dfZ);
					Si.at<float>(j, i) = (1.0 - K*dfZ)*Si1.at<float>(j, i);
					Zn.at<float>(j, i) = Zn1.at<float>(j, i) + K*(Y - dfZ*Zn1.at<float>(j, i));
				}
			}
			Zn.copyTo(depth);
			Zn.copyTo(Zn1);
			Si.copyTo(Si1);
		}

		//draw 3d model
		Mat depth_norm;
		depth_norm.create(depth.size(), CV_8U);
		depth_norm = -depth * 255;
		depth_norm.convertTo(depth_norm, CV_8U);
		//cout << depth_norm << " " << endl;

		if (flag){
			//中值滤波&采样（筛点）
			Mat dst;
			dst.create(int(depth.size().height / 10), int(depth.size().width / 10), CV_8U);
			for (int i = 0; i < depth.size().width; i++)
			{
				for (int j = 0; j < depth.size().height; j++){
					if (int(depth_norm.at<uchar>(j, i)) < 20)
						depth_norm.at<uchar>(j, i) = 19;
					if (i % 10 == 0 && j % 10 == 0){
						int a = i / 10; int b = j / 10;
						//outfile << i << " " << j << " " << int(depth_norm.at<uchar>(j, i)) << endl;
						dst.at<uchar>(b, a) = int(depth_norm.at<uchar>(j, i));
					}
				}
			}
			medianBlur(dst, dst, 5);
			//printf("%d %d\n", dst.size().width, dst.size().height);
			for (int i = 0; i < dst.size().width; i++)
			{
				for (int j = 0; j < dst.size().height; j++){
					if (int(dst.at<uchar>(j, i)) < 20) continue;
					else {
						dstPts[w].push_back(Point3f(i * 10.0, j * 10.0, 1.5*int(dst.at<uchar>(j, i))));//add a 1.5 coefficient to enhance the result. 
						//printf("%f %f %f\n", i * 10.0, j * 10.0, 1.5*int(dst.at<uchar>(j, i)));
					}
				}
			}
			//printf("\n");
		}
		else {
			//Row Points
			for (int i = 0; i < depth.size().width; i++)
			{
				for (int j = 0; j < depth.size().height; j++){
					//if (int(depth_norm.at<uchar>(j, i) < 20)) continue;
					//outfile << i << " " << j << " " << int(depth_norm.at<uchar>(j, i)) << endl;
					dstPts[w].push_back(Point3f(i, j, int(depth_norm.at<uchar>(j, i))));
				}
			}
			rowPtsnum = depth.size().width*depth.size().height;
			//printf("%d\n", rowPtsnum);
		}
	}
	printf("sfscal done.\n");
	return dstPts;
}

void WritePoints1(vector<vector<Point3f>> InPts){
	ofstream outfile("testpoints.txt");
	if (!outfile){
		cout << "Unable to open otfile";
		exit(1); // terminate with error
	}

	for (int i = 0; i < InPts.size(); i++){
		for (int j = 0; j < InPts[i].size(); j++){
			outfile << InPts[i].at(j).x << " " << InPts[i].at(j).y << " " << InPts[i].at(j).z << endl;
		}
	}

	outfile.close();
}

void WritePoints2(vector<vector<Point3f>> InPts){
	ofstream outfile("rowpoints.txt");
	if (!outfile){
		cout << "Unable to open otfile";
		exit(1); // terminate with error
	}

	for (int i = 0; i < InPts.size(); i++){
		for (int j = 0; j < InPts[i].size(); j++){
			outfile << InPts[i].at(j).x << " " << InPts[i].at(j).y << " " << InPts[i].at(j).z << endl;
		}
	}

	outfile.close();
}

vector<vector<Point3f>> ReadPoints1(int n, vector<Mat> source){
	ifstream outfile("testpoints.txt");
	if (!outfile){
		cout << "Unable to open otfile";
		exit(1); // terminate with error
	}
	char line[50];
	char x[10], y[10], z[10];

	vector<vector<Point3f>> Pts(source.size());
	//printf("%d\n", Pts.size());
	int m = 0;
	int counter = 0;
	while (outfile.getline(line, sizeof(line))){

		stringstream word(line);
		word >> x;
		word >> y;
		word >> z;
		Pts[m].push_back(Point3f(atof(x), atof(y), atof(z)));
		counter++;

		if (counter == pow(pow(2.0, n) + 1, 2.0)){
			counter = 0;
			m++;
		}
	}
	outfile.close();
	return Pts;
}

vector<vector<Point3f>> ReadPoints2(vector<Mat> source){
	ifstream outfile("rowpoints.txt");
	if (!outfile){
		cout << "Unable to open otfile";
		exit(1); // terminate with error
	}
	char line[50];
	char x[10], y[10], z[10];

	vector<vector<Point3f>> Pts(source.size());
	//printf("%d\n", Pts.size());
	int m = 0;
	int counter = 0;
	while (outfile.getline(line, sizeof(line))){

		stringstream word(line);
		word >> x;
		word >> y;
		word >> z;
		Pts[m].push_back(Point3f(atof(x), atof(y), atof(z)));
		counter++;

		if (counter == source[0].rows*source[0].cols){
			counter = 0;
			m++;
		}
	}
	outfile.close();
	return Pts;
}

vector<vector<Point3f>> myGaussianBlur(int size, vector<vector<Point3f>> InPts){
	vector<vector<Point3f>> OutPts(InPts.size());
	int half = (size - 1) / 2;
	double sigma = sqrt(size);
	for (int i = 0; i < InPts[0].size(); i++){
		for (int w = 0; w < InPts.size(); w++){
			Point3f s(0.0, 0.0, 0.0);
			double sum = 0;
			for (int j = w - half; j <= w + half; j++){
				if (j>0 && j < InPts.size()){
					s.x += exp(-(j - w)*(j - w) / (2 * sigma*sigma))*InPts[j].at(i).x;
					s.y += exp(-(j - w)*(j - w) / (2 * sigma*sigma))*InPts[j].at(i).y;
					s.z += exp(-(j - w)*(j - w) / (2 * sigma*sigma))*InPts[j].at(i).z;
					sum += exp(-(j - w)*(j - w) / (2 * sigma*sigma));
				}
			}
			s.x /= sum; s.y /= sum; s.z /= sum;
			//printf("%f %f %f\n", s.x, s.y, s.z);
			OutPts[w].push_back(s);
		}
		//printf("\n");
	}
	return OutPts;
}

void main(char argc, char** argv){

	printf("Input the number of quad per row.\n");
	scanf_s("%d", &n);

	bool ReconFlag = false; //Set to false first to extract control and raw points. Then set to true for reconstruction.

	vector<Mat> source;
	source = getFrame();

	if (ReconFlag == true){
		vector<vector<Point3f>> OutPts, rowPts;
		OutPts = ReadPoints1(n, source);
		rowPts = ReadPoints2(source);
		//printf("%d\n", rowPts[0].size());
		//for (int w = 0; w < OutPts.size(); w++){
		//	for (int i = 0; i < OutPts[w].size(); i++){
		//		printf("%f %f %f\n", OutPts[w].at(i).x, OutPts[w].at(i).y, OutPts[w].at(i).z);
		//	}
		//	printf("\n");
		//}
		OutPts = myGaussianBlur(5, OutPts);

		vP = Read3DPoints1(source, OutPts);
		vPr = Read3DPoints2(source, rowPts);
		//for (int i = 0; i < vP.size(); i++){
		//	for (int j = 0; j < vP[i].size(); j++){
		//		printf("%f %f %f\n", vP[i].at(j).X, vP[i].at(j).Y, vP[i].at(j).Z);
		//	}
		//	printf("\n");
		//}

		//printf("%d\n", avg.size());

		View3D(argc, argv, vP);

		return;
	}
	else{
		int height = source[0].rows;
		int width = source[0].cols;
		int quadWidth = 1.0*width / pow(2.0, n);
		int quadHeight = 1.0*height / pow(2.0, n);

		vector<vector<Point3f>> ControlPts, OutPts, RowPts;
		ControlPts = sfscal(TRUE, source);
		RowPts = sfscal(FALSE, source);
		//printf("%d\n", ControlPts.size());

		for (int i = 0; i < ControlPts.size(); i++){
			//printf("%d\n", i);
			OutPts.push_back(getMeshPoints(n, ControlPts[i], height, width, quadWidth, quadHeight));
		}
		printf("MeshPoints Done!\n");

		WritePoints1(OutPts);
		WritePoints2(RowPts);

		return;
	}
}