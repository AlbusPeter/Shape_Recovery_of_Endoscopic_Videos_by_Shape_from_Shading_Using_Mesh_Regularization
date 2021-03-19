#include "AsRigidAsPossibleWarping.h"

ContentPreservingWarp::ContentPreservingWarp(int height, int width, double quadWidth, double quadHeight){
	this->source = new Mesh(height, width, quadWidth, quadHeight);
	this->destin = new Mesh(height, width, quadWidth, quadHeight);

	this->imgHeight = height;
	this->imgWidth = width;
	this->quadWidth = quadWidth;
	this->quadHeight = quadHeight;
	this->height = source->height;
	this->width = source->width;

	x_index = new int[this->height*this->width];
	y_index = new int[this->height*this->width];
	z_index = new int[this->height*this->width];
	memset(x_index, 0, this->height*this->width*sizeof(int));
	memset(y_index, 0, this->height*this->width*sizeof(int));
	memset(z_index, 0, this->height*this->width*sizeof(int));

	int cc = 0;
	for (int i = 0; i<this->height*this->width; i++) x_index[i] = cc++;
	for (int i = 0; i<this->height*this->width; i++) y_index[i] = cc++;
	for (int i = 0; i<this->height*this->width; i++) z_index[i] = cc++;

	num_edge_cons = ((this->height - 2)*(this->width - 2)) * 12 + (2 * (this->width + this->height) - 8) * 9 + 4 * 6;
	columns = this->height*this->width * 3;
	
	EdgeCons.reserve(num_edge_cons, columns, 10000000);
	CreateEdgeConstraint(EdgeCons);
}

ContentPreservingWarp::~ContentPreservingWarp(){
	delete this->source;
	delete this->destin;
	dataterm.clear();
	delete b;
	delete x;
}

//calculate the length ratio of the edges.
void ContentPreservingWarp::getlengthratio_1(const cv::Point3f &V1, const cv::Point3f &V2, double &u){
	double len = sqrt((V1.x - V2.x)*(V1.x - V2.x) + (V1.y - V2.y)*(V1.y - V2.y) + (V1.z - V2.z)*(V1.z - V2.z));
	u = 1.0*quadWidth / len;
}
void ContentPreservingWarp::getlengthratio_2(const cv::Point3f &V1, const cv::Point3f &V2, double &u){
	double len = sqrt((V1.x - V2.x)*(V1.x - V2.x) + (V1.y - V2.y)*(V1.y - V2.y) + (V1.z - V2.z)*(V1.z - V2.z));
	u = 1.0*quadHeight / len;
}

//add Edgecons
void ContentPreservingWarp::addCoefficient_1(int i, int j, int &rowCount, int* x_index, int* y_index, int* z_index, Triplets& EdgeCons){
	/*up V1-V2
	.V2
	.V1*/
	cv::Point3f V1, V2;
	double u = 0;

	V1 = source->getVertex(i, j);
	V2 = source->getVertex(i - 1, j);
	getlengthratio_2(V1, V2, u);
	
	int coordv1 = i*width + j;
	int coordv2 = (i - 1)*width + j;

	EdgeCons.add(rowCount, x_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, x_index[coordv2], -1.0*u);
	rowCount++;

	EdgeCons.add(rowCount, y_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, y_index[coordv2], -1.0*u);
	rowCount++;

	EdgeCons.add(rowCount, z_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, z_index[coordv2], -1.0*u);
	rowCount++;
}
void ContentPreservingWarp::addCoefficient_2(int i, int j, int &rowCount, int* x_index, int* y_index, int* z_index, Triplets& EdgeCons){
	/*down V1-V2
	.V1
	.V2*/
	cv::Point3f V1, V2;
	double u = 0;

	V1 = source->getVertex(i, j);
	V2 = source->getVertex(i + 1, j);
	getlengthratio_2(V1, V2, u);

	int coordv1 = i*width + j;
	int coordv2 = (i + 1)*width + j;

	EdgeCons.add(rowCount, x_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, x_index[coordv2], -1.0*u);
	rowCount++;

	EdgeCons.add(rowCount, y_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, y_index[coordv2], -1.0*u);
	rowCount++;

	EdgeCons.add(rowCount, z_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, z_index[coordv2], -1.0*u);
	rowCount++;
}
void ContentPreservingWarp::addCoefficient_3(int i, int j, int &rowCount, int* x_index, int* y_index, int* z_index, Triplets& EdgeCons){
	/*left V1-V2
	.V2  .V1*/
	cv::Point3f V1, V2;
	double u = 0;

	V1 = source->getVertex(i, j);
	V2 = source->getVertex(i, j - 1);
	getlengthratio_1(V1, V2, u);

	int coordv1 = i*width + j;
	int coordv2 = i*width + j - 1;

	EdgeCons.add(rowCount, x_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, x_index[coordv2], -1.0*u);
	rowCount++;

	EdgeCons.add(rowCount, y_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, y_index[coordv2], -1.0*u);
	rowCount++;

	EdgeCons.add(rowCount, z_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, z_index[coordv2], -1.0*u);
	rowCount++;
}
void ContentPreservingWarp::addCoefficient_4(int i, int j, int &rowCount, int* x_index, int* y_index, int* z_index, Triplets& EdgeCons){
	/*right  V1-V2
	.V1  .V2*/
	cv::Point3f V1, V2;
	double u = 0;

	V1 = source->getVertex(i, j);
	V2 = source->getVertex(i, j + 1);
	getlengthratio_1(V1, V2, u);

	int coordv1 = i*width + j;
	int coordv2 = i*width + j + 1;

	EdgeCons.add(rowCount, x_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, x_index[coordv2], -1.0*u);
	rowCount++;

	EdgeCons.add(rowCount, y_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, y_index[coordv2], -1.0*u);
	rowCount++;

	EdgeCons.add(rowCount, z_index[coordv1], 1.0*u);
	EdgeCons.add(rowCount, z_index[coordv2], -1.0*u);
	rowCount++;
}


void ContentPreservingWarp::SetPoints(vector<Point3f> InPts)
{
	for (int i = 0; i < InPts.size(); i++){
		sourcePts.push_back(Point3f(InPts[i].x, InPts[i].y, 0));
	}
	//printf("Source Points inputed!\n");
	//printf("%d\n", sourcePts.size());
	for (int i = 0; i < InPts.size(); i++){
		targetPts.push_back(Point3f(InPts[i].x, InPts[i].y, InPts[i].z));
	}
	//printf("Target Points inputed!\n");
	//printf("%d\n", targetPts.size());
	SetControlPts(sourcePts, targetPts);
	//printf("%d\n", dataterm.size());

	//initial some vital matrices
	int num_data_cons = this->dataterm.size() * 3;
	int rows = num_data_cons;
	ContentPreservingWarp::x = new double[columns];
	memset(ContentPreservingWarp::x, 0, columns*sizeof(double));
	ContentPreservingWarp::b = new double[rows];
	memset(ContentPreservingWarp::b, 0, rows*sizeof(double));
	W.reserve(rows, columns, 10000000);
	A.reserve(W.m + EdgeCons.m, columns, 10000000);
}

void ContentPreservingWarp::matlabSparseLinearSystem(Triplets& F, double b[], double x[]){
	extern Engine* ep;
	//printf("%d\n", F.m);
	mxArray *inArray[4];
	inArray[0] = mxCreateDoubleMatrix(F.nnz, 1, mxREAL);
	inArray[1] = mxCreateDoubleMatrix(F.nnz, 1, mxREAL);
	inArray[2] = mxCreateDoubleMatrix(F.nnz, 1, mxREAL);
	inArray[3] = mxCreateDoubleMatrix(F.m, 1, mxREAL);

	double *dArow = new double[F.nnz];
	double *dAcol = new double[F.nnz];
	double *dVal = new double[F.nnz];

	for (int i = 0; i < F.nnz; i++)
	{
		dArow[i] = double(F.ri[i] + 1);
		dAcol[i] = double(F.ci[i] + 1);
		dVal[i] = double(F.nzval[i]);
	}

	memcpy((void*)mxGetPr(inArray[0]), (void*)dArow, (F.nnz)*sizeof(double));
	memcpy((void*)mxGetPr(inArray[1]), (void*)dAcol, (F.nnz)*sizeof(double));
	memcpy((void*)mxGetPr(inArray[2]), (void*)dVal, (F.nnz)*sizeof(double));
	memcpy((void*)mxGetPr(inArray[3]), (void*)b, (F.m)*sizeof(double));

	mxArray *outArray;
	engEvalString(ep, "clear all;");
	engPutVariable(ep, "ARows", inArray[0]);
	engPutVariable(ep, "ACols", inArray[1]);
	engPutVariable(ep, "AVals", inArray[2]);
	engPutVariable(ep, "b", inArray[3]);

	engEvalString(ep, "A = sparse(ARows, ACols, AVals);");
	engEvalString(ep, "AT = A';");
	engEvalString(ep, "ATA = AT*A;");
	engEvalString(ep, "ATb = AT*b;");
	engEvalString(ep, "X = ATA \\ ATb;");
	outArray = engGetVariable(ep, "X");
	double* X = (double*)mxGetData(outArray);

	for (int i = 0; i<F.n; i++){
		x[i] = X[i];
	}

	for (int i = 0; i < 4; i++)
	{
		mxDestroyArray(inArray[i]);
	}
	mxDestroyArray(outArray);
	delete[] dArow;
	delete[] dAcol;
	delete[] dVal;
}

void ContentPreservingWarp::SetControlPts(const vector<cv::Point3f> &inputsPts, const vector<cv::Point3f> &outputsPts){
	this->dataterm.clear();
	//printf("%d\n", inputsPts.size());
	for (int u = 1; u<height; u++)
	{
		for (int v = 1; v<width; v++)
		{
			for (int p = 0; p<inputsPts.size(); p++)
			{
				cv::Point3f pt;
				pt.x = outputsPts.at(p).x;
				pt.y = outputsPts.at(p).y;
				pt.z = outputsPts.at(p).z;
				cv::Point3f inPt;
				inPt.x = inputsPts[p].x;
				inPt.y = inputsPts[p].y;
				inPt.z = inputsPts[p].z;

				//input point in a quad and output points in the image
				if (source->getQuad(u, v).isPointIn(inPt) &&
					pt.x>0 && pt.x < imgWidth &&
					pt.y>0 && pt.y < imgHeight)
				{
					dataterm_element e;
					e.i = u;
					e.j = v;

					e.orgPt.x = inputsPts.at(p).x;
					e.orgPt.y = inputsPts.at(p).y;
					e.orgPt.z = inputsPts.at(p).z;

					e.desPt.x = outputsPts.at(p).x;
					e.desPt.y = outputsPts.at(p).y;
					e.desPt.z = outputsPts.at(p).z;

					e.dataWeight = 1;

					vector<double> coefficients;
					source->getQuad(u, v).getBilinearCoordinates(e.orgPt, coefficients);
					e.v00 = coefficients[0];
					e.v01 = coefficients[1];
					e.v10 = coefficients[2];
					e.v11 = coefficients[3];

					this->dataterm.push_back(e);
				}
			}
		}//end for
	}//end for
}

void ContentPreservingWarp::Solve(){
	
	//add dataterm
	int rowCount = 0;
	for (int p = 0; p<dataterm.size(); p++){
		dataterm_element e = dataterm.at(p);
		int i = e.i;
		int j = e.j;

		W.add(rowCount, x_index[(i - 1)*width + j - 1], e.v00*e.dataWeight);
		W.add(rowCount, x_index[(i - 1)*width + j], e.v01*e.dataWeight);
		W.add(rowCount, x_index[i*width + j - 1], e.v10*e.dataWeight);
		W.add(rowCount, x_index[i*width + j], e.v11*e.dataWeight);
		b[rowCount] = e.desPt.x*e.dataWeight;
		rowCount++;

		W.add(rowCount, y_index[(i - 1)*width + j - 1], e.v00*e.dataWeight);
		W.add(rowCount, y_index[(i - 1)*width + j], e.v01*e.dataWeight);
		W.add(rowCount, y_index[i*width + j - 1], e.v10*e.dataWeight);
		W.add(rowCount, y_index[i*width + j], e.v11*e.dataWeight);
		b[rowCount] = e.desPt.y*e.dataWeight;
		rowCount++;

		W.add(rowCount, z_index[(i - 1)*width + j - 1], e.v00*e.dataWeight);
		W.add(rowCount, z_index[(i - 1)*width + j], e.v01*e.dataWeight);
		W.add(rowCount, z_index[i*width + j - 1], e.v10*e.dataWeight);
		W.add(rowCount, z_index[i*width + j], e.v11*e.dataWeight);
		b[rowCount] = e.desPt.z*e.dataWeight;
		rowCount++;
	}

	//set the initial vertex points
	int k = 0;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			x[k] = source->getVertex(i, j).x;
			k++;
		}
	}
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			x[k] = source->getVertex(i, j).y;
			k++;
		}
	}
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			x[k] = source->getVertex(i, j).z;
			k++;
		}
	}
	
	//construct A
	for (int i = 0; i < W.nnz; i++) A.add(W.ri[i], W.ci[i], W.nzval[i]);
	for (int i = 0; i < EdgeCons.nnz; i++) A.add(EdgeCons.ri[i] + W.m, EdgeCons.ci[i], EdgeCons.nzval[i]);

	//printf("%d\n", A.m);
	
	int thirdscolumns = columns / 3;
	//printf("the coordinates are:\n");
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			cv::Point3f pt(x[i*width + j], x[thirdscolumns + i*width + j], x[thirdscolumns * 2 + i*width + j]);
			//printf("%f %f %f\n", pt.x, pt.y, pt.z);
			//update the mesh grid.
			this->source->setVertex(i, j, pt);
		}
	}
	//printf("\n");
	//for (int i = 0; i < height; i++){
	//	for (int j = 0; j < width; j++){
	//		cv::Point3f pt;
	//		pt = source->getVertex(i, j);
	//		printf("%f %f %f\n", pt.x, pt.y, pt.z);
	//	}
	//}
	//printf("\n");
}

void ContentPreservingWarp::CreateEdgeConstraint(Triplets& EdgeCons){
	int rowCount = 0;
	//add smooth term
	int i = 0, j = 0;
	addCoefficient_2(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
	addCoefficient_4(i, j, rowCount, x_index, y_index, z_index, EdgeCons);


	i = 0, j = width - 1;
	addCoefficient_2(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
	addCoefficient_3(i, j, rowCount, x_index, y_index, z_index, EdgeCons);


	i = height - 1, j = 0;
	addCoefficient_1(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
	addCoefficient_4(i, j, rowCount, x_index, y_index, z_index, EdgeCons);


	i = height - 1, j = width - 1;
	addCoefficient_1(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
	addCoefficient_3(i, j, rowCount, x_index, y_index, z_index, EdgeCons);

	i = 0;
	for (j = 1; j<width - 1; j++){
		addCoefficient_2(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
		addCoefficient_3(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
		addCoefficient_4(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
	}

	i = height - 1;
	for (int j = 1; j<width - 1; j++){
		addCoefficient_1(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
		addCoefficient_3(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
		addCoefficient_4(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
	}

	j = 0;
	for (int i = 1; i<height - 1; i++){
		addCoefficient_1(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
		addCoefficient_2(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
		addCoefficient_4(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
	}


	j = width - 1;
	for (int i = 1; i<height - 1; i++){
		addCoefficient_1(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
		addCoefficient_2(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
		addCoefficient_3(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
	}

	for (int i = 1; i<height - 1; i++){
		for (int j = 1; j<width - 1; j++){
			addCoefficient_1(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
			addCoefficient_2(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
			addCoefficient_3(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
			addCoefficient_4(i, j, rowCount, x_index, y_index, z_index, EdgeCons);
		}
	}
}