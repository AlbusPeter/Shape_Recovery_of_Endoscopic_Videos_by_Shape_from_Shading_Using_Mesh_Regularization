#include "AsRigidAsPossibleWarping.h"

void ContentPreservingWarp::Iteration(int n){

	int rows = dataterm.size()*3+num_edge_cons;

	for (int i = 0; i < n; i++){

		double* e = new double[num_edge_cons];
		memset(e, 0, num_edge_cons*sizeof(double));
		
		//update the L matrix
		Triplets G;
		G.reserve(num_edge_cons, columns, 100000000);
		CreateEdgeConstraint(G);
		//update the e(Vm).
		matlabIterationb(G, x, e);

		//construct b(Vm)
		double* B = new double[rows];
		memset(B, 0, rows*sizeof(double));
		for (int i = 0; i < dataterm.size() * 3; i++){
			B[i] = b[i];
			//printf("%f\n", B[i]);
		}
		//printf("\n");
		for (int i = 0; i < num_edge_cons; i++){
			B[dataterm.size() * 3 + i] = e[i];
			//printf("%f\n", e[i]);
		}
		
		//calculate the new mesh vertex coordinates
		matlabSparseLinearSystem(A, B, x);

		int thirdscolumns = columns / 3;
		//printf("the coordinates are:\n");
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				cv::Point3f pt(x[i*width + j], x[thirdscolumns + i*width + j], x[thirdscolumns * 2 + i*width + j]);
				//printf("%f %f %f\n", pt.x, pt.y, pt.z);
				//this->destin->setVertex(i, j, pt);
				//update the mesh grid
				this->source->setVertex(i, j, pt);
			}
		}
		//printf("\n");
		G.clear();
		delete B;
		delete e;
	}//End iteration
}

void ContentPreservingWarp::matlabIterationb(Triplets& L, double x[], double e[]){
	extern Engine* ep;

	mxArray *inArray[4];
	inArray[0] = mxCreateDoubleMatrix(L.nnz, 1, mxREAL);
	inArray[1] = mxCreateDoubleMatrix(L.nnz, 1, mxREAL);
	inArray[2] = mxCreateDoubleMatrix(L.nnz, 1, mxREAL);
	inArray[3] = mxCreateDoubleMatrix(columns, 1, mxREAL);

	double *dArow = new double[L.nnz];
	double *dAcol = new double[L.nnz];
	double *dVal = new double[L.nnz];

	for (int i = 0; i < L.nnz; i++)
	{
		dArow[i] = double(L.ri[i] + 1);
		dAcol[i] = double(L.ci[i] + 1);
		dVal[i] = double(L.nzval[i]);
	}

	memcpy((void*)mxGetPr(inArray[0]), (void*)dArow, (L.nnz)*sizeof(double));
	memcpy((void*)mxGetPr(inArray[1]), (void*)dAcol, (L.nnz)*sizeof(double));
	memcpy((void*)mxGetPr(inArray[2]), (void*)dVal, (L.nnz)*sizeof(double));
	memcpy((void*)mxGetPr(inArray[3]), (void*)x, columns*sizeof(double));

	mxArray *outArray;
	engEvalString(ep, "clear all;");
	engPutVariable(ep, "ARows", inArray[0]);
	engPutVariable(ep, "ACols", inArray[1]);
	engPutVariable(ep, "AVals", inArray[2]);
	engPutVariable(ep, "x", inArray[3]);

	engEvalString(ep, "A = sparse(ARows, ACols, AVals);");
	engEvalString(ep, "b = A*x;");
	outArray = engGetVariable(ep, "b");
	double* X = (double*)mxGetData(outArray);

	for (int i = 0; i<L.m; i++){
		e[i] = X[i];
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