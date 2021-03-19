#include "AsRigidAsPossibleWarping.h"
#include <engine.h>
#include "Header.h"

using namespace std;
using namespace cv;

Engine* ep;

void main()
{
	if (!(ep = engOpen("\0"))){
		fprintf(stderr, "\nCan't start MATLAB engine\n");
		return;
	}

	int n;
	printf("Input the number of quad per row.\n");
	scanf_s("%d", &n);

	cv::Mat source = cv::imread("s1.jpg");

	int height = source.rows;
	int width = source.cols;
	int quadWidth = 1.0*width / pow(2.0, n);
	int quadHeight = 1.0*height / pow(2.0, n);

	ContentPreservingWarp cpw(height, width, quadWidth, quadHeight);
	cpw.SetPoints();
	cpw.Solve();
	cpw.Iteration(20);

	//construct the ply file.
	ofstream outfile("OutputPts.txt");
	if (!outfile){
		cout << "Unable to open otfile";
		exit(1); // terminate with error
	}

	outfile << "ply" << endl << "format ascii 1.0" << endl << "comment made by Peter" << endl;
	outfile << "element vertex " << pow((pow(2.0, n) + 1), 2.0) << endl;
	outfile << "property float32 x" << endl << "property float32 y" << endl << "property float32 z" << endl;
	outfile << "element face " << 8 * pow(4.0, n - 1) << endl;
	outfile << "property list uint8 int32 vertex_index" << endl << "end_header" << endl;
	
	int k = pow(2.0, n) + 1;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++){
			outfile << cpw.GetSourceMesh()->getVertex(i, j).x << " " << cpw.GetSourceMesh()->getVertex(i, j).y << " " << cpw.GetSourceMesh()->getVertex(i, j).z << endl;
		}
	}
	outfile << endl;
	for (int i = 0; i < pow(2.0, n); i = i + 2){
		for (int j = 0; j < pow(2.0, n); j = j + 2){
			outfile << "3 " << i*k + j << " " << (i + 1)*k + j << " " << (i + 1)*k + j + 1 << endl;
			outfile << "3 " << i*k + j << " " << i*k + j + 1 << " " << (i + 1)*k + j + 1 << endl;
			outfile << "3 " << i*k + j + 2 << " " << i*k + j + 1 << " " << (i + 1)*k + j + 1 << endl;
			outfile << "3 " << i*k + j + 2 << " " << (i + 1)*k + j + 1 << " " << (i + 1)*k + j + 2 << endl;
			outfile << "3 " << (i + 1)*k + j << " " << (i + 1)*k + j + 1 << " " << (i + 2)*k + j << endl;
			outfile << "3 " << (i + 1)*k + j + 1 << " " << (i + 2)*k + j << " " << (i + 2)*k + j + 1 << endl;
			outfile << "3 " << (i + 1)*k + j + 1 << " " << (i + 2)*k + j + 1 << " " << (i + 2)*k + j + 2 << endl;
			outfile << "3 " << (i + 1)*k + j + 1 << " " << (i + 1)*k + j + 2 << " " << (i + 2)*k + j + 2 << endl;
		}
	}
	outfile.close();


	printf("Done.");
	while (1);
}