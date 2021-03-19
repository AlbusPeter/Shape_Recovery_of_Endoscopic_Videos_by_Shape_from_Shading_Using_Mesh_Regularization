#include "Draw.h"

void DrawPointClouds(){
	glPointSize(2);
	glBegin(GL_POINTS);	
	for(int i=0;i<vPointCloudr.size();i++){	
		SColorPoint3D p = vPointCloudr[i];
		glColor3f(p.R,p.G,p.B);
		glVertex3f(p.X,p.Y,p.Z);
	}
    glEnd();
}

void DrawTriangles1(int n){
	int k = pow(2.0, n) + 1;
	for (int i = 0; i < pow(2.0, n); i = i + 2){
		for (int j = 0; j < pow(2.0, n); j = j + 2){
			//1
			glBegin(GL_TRIANGLES);
			glVertex3f(vPointCloud[i*k + j].X, vPointCloud[i*k + j].Y, vPointCloud[i*k + j].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j].X, vPointCloud[(i + 1)*k + j].Y, vPointCloud[(i + 1)*k + j].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();
			//2
			glBegin(GL_TRIANGLES);
			glVertex3f(vPointCloud[i*k + j].X, vPointCloud[i*k + j].Y, vPointCloud[i*k + j].Z);
			glVertex3f(vPointCloud[i*k + j + 1].X, vPointCloud[i*k + j + 1].Y, vPointCloud[i*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();
			//3
			glBegin(GL_TRIANGLES);
			glVertex3f(vPointCloud[i*k + j + 2].X, vPointCloud[i*k + j + 2].Y, vPointCloud[i*k + j + 2].Z);
			glVertex3f(vPointCloud[i*k + j + 1].X, vPointCloud[i*k + j + 1].Y, vPointCloud[i*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();
			//4
			glBegin(GL_TRIANGLES);
			glVertex3f(vPointCloud[i*k + j + 2].X, vPointCloud[i*k + j + 2].Y, vPointCloud[i*k + j + 2].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 2].X, vPointCloud[(i + 1)*k + j + 2].Y, vPointCloud[(i + 1)*k + j + 2].Z);
			glEnd();
			//5
			glBegin(GL_TRIANGLES);
			glVertex3f(vPointCloud[(i + 1)*k + j].X, vPointCloud[(i + 1)*k + j].Y, vPointCloud[(i + 1)*k + j].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j].X, vPointCloud[(i + 2)*k + j].Y, vPointCloud[(i + 2)*k + j].Z);
			glEnd();
			//6
			glBegin(GL_TRIANGLES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j].X, vPointCloud[(i + 2)*k + j].Y, vPointCloud[(i + 2)*k + j].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 1].X, vPointCloud[(i + 2)*k + j + 1].Y, vPointCloud[(i + 2)*k + j + 1].Z);
			glEnd();
			//7
			glBegin(GL_TRIANGLES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 1].X, vPointCloud[(i + 2)*k + j + 1].Y, vPointCloud[(i + 2)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 2].X, vPointCloud[(i + 2)*k + j + 2].Y, vPointCloud[(i + 2)*k + j + 2].Z);
			glEnd();
			//8
			glBegin(GL_TRIANGLES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 2].X, vPointCloud[(i + 1)*k + j + 2].Y, vPointCloud[(i + 1)*k + j + 2].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 2].X, vPointCloud[(i + 2)*k + j + 2].Y, vPointCloud[(i + 2)*k + j + 2].Z);
			glEnd();
		}
	}
}

void DrawLines(int n){
	int k = pow(2.0, n) + 1;
	for (int i = 0; i < pow(2.0, n); i = i + 2){
		for (int j = 0; j < pow(2.0, n); j = j + 2){
			//1
			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j].X, vPointCloud[i*k + j].Y, vPointCloud[i*k + j].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j].X, vPointCloud[(i + 1)*k + j].Y, vPointCloud[(i + 1)*k + j].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j].X, vPointCloud[(i + 1)*k + j].Y, vPointCloud[(i + 1)*k + j].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j].X, vPointCloud[i*k + j].Y, vPointCloud[i*k + j].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();
			//2
			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j].X, vPointCloud[i*k + j].Y, vPointCloud[i*k + j].Z);
			glVertex3f(vPointCloud[i*k + j + 1].X, vPointCloud[i*k + j + 1].Y, vPointCloud[i*k + j + 1].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j + 1].X, vPointCloud[i*k + j + 1].Y, vPointCloud[i*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j].X, vPointCloud[i*k + j].Y, vPointCloud[i*k + j].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();
			//3
			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j + 2].X, vPointCloud[i*k + j + 2].Y, vPointCloud[i*k + j + 2].Z);
			glVertex3f(vPointCloud[i*k + j + 1].X, vPointCloud[i*k + j + 1].Y, vPointCloud[i*k + j + 1].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j + 1].X, vPointCloud[i*k + j + 1].Y, vPointCloud[i*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j + 2].X, vPointCloud[i*k + j + 2].Y, vPointCloud[i*k + j + 2].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();
			//4
			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j + 2].X, vPointCloud[i*k + j + 2].Y, vPointCloud[i*k + j + 2].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 2].X, vPointCloud[(i + 1)*k + j + 2].Y, vPointCloud[(i + 1)*k + j + 2].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[i*k + j + 2].X, vPointCloud[i*k + j + 2].Y, vPointCloud[i*k + j + 2].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 2].X, vPointCloud[(i + 1)*k + j + 2].Y, vPointCloud[(i + 1)*k + j + 2].Z);
			glEnd();
			//5
			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j].X, vPointCloud[(i + 1)*k + j].Y, vPointCloud[(i + 1)*k + j].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j].X, vPointCloud[(i + 2)*k + j].Y, vPointCloud[(i + 2)*k + j].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j].X, vPointCloud[(i + 1)*k + j].Y, vPointCloud[(i + 1)*k + j].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j].X, vPointCloud[(i + 2)*k + j].Y, vPointCloud[(i + 2)*k + j].Z);
			glEnd();
			//6
			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j].X, vPointCloud[(i + 2)*k + j].Y, vPointCloud[(i + 2)*k + j].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 2)*k + j].X, vPointCloud[(i + 2)*k + j].Y, vPointCloud[(i + 2)*k + j].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 1].X, vPointCloud[(i + 2)*k + j + 1].Y, vPointCloud[(i + 2)*k + j + 1].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 1].X, vPointCloud[(i + 2)*k + j + 1].Y, vPointCloud[(i + 2)*k + j + 1].Z);
			glEnd();
			//7
			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 1].X, vPointCloud[(i + 2)*k + j + 1].Y, vPointCloud[(i + 2)*k + j + 1].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 2)*k + j + 1].X, vPointCloud[(i + 2)*k + j + 1].Y, vPointCloud[(i + 2)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 2].X, vPointCloud[(i + 2)*k + j + 2].Y, vPointCloud[(i + 2)*k + j + 2].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 2].X, vPointCloud[(i + 2)*k + j + 2].Y, vPointCloud[(i + 2)*k + j + 2].Z);
			glEnd();
			//8
			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 1)*k + j + 2].X, vPointCloud[(i + 1)*k + j + 2].Y, vPointCloud[(i + 1)*k + j + 2].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 2].X, vPointCloud[(i + 1)*k + j + 2].Y, vPointCloud[(i + 1)*k + j + 2].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 2].X, vPointCloud[(i + 2)*k + j + 2].Y, vPointCloud[(i + 2)*k + j + 2].Z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glVertex3f(vPointCloud[(i + 2)*k + j + 2].X, vPointCloud[(i + 2)*k + j + 2].Y, vPointCloud[(i + 2)*k + j + 2].Z);
			glEnd();
		}
	}
}



void DrawTriangles(int n){
	extern GLuint texName[N];
	extern int index_num;
	extern vector<Point3f> avg;
	
	glEnable(GL_TEXTURE_2D);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, texName[index_num]);
	glBegin(GL_TRIANGLES);
	int k = pow(2.0, n) + 1;
	for (int i = 0; i < pow(2.0, n); i = i + 2){
		for (int j = 0; j < pow(2.0, n); j = j + 2){ //分母选择 max(width,height) 贴图坐标介于0-1. 贴图y坐标*(-1) 抵消main.cpp 中为了显示乘上的-1.
			//1
			glTexCoord2f((vPointCloud[i*k + j].X + avg[index_num].x) / 370, -(vPointCloud[i*k + j].Y + avg[index_num].y)/ 370); glVertex3f(vPointCloud[i*k + j].X, vPointCloud[i*k + j].Y, vPointCloud[i*k + j].Z);
			glTexCoord2f((vPointCloud[(i + 1)*k + j].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j].X, vPointCloud[(i + 1)*k + j].Y, vPointCloud[(i + 1)*k + j].Z);
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			//2
			glTexCoord2f((vPointCloud[i*k + j].X + avg[index_num].x) / 370, -(vPointCloud[i*k + j].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[i*k + j].X, vPointCloud[i*k + j].Y, vPointCloud[i*k + j].Z);
			glTexCoord2f((vPointCloud[i*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[i*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[i*k + j + 1].X, vPointCloud[i*k + j + 1].Y, vPointCloud[i*k + j + 1].Z);
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			
			//3
			
			glTexCoord2f((vPointCloud[i*k + j + 2].X + avg[index_num].x) / 370, -(vPointCloud[i*k + j + 2].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[i*k + j + 2].X, vPointCloud[i*k + j + 2].Y, vPointCloud[i*k + j + 2].Z);
			glTexCoord2f((vPointCloud[i*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[i*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[i*k + j + 1].X, vPointCloud[i*k + j + 1].Y, vPointCloud[i*k + j + 1].Z);
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			
			//4
			
			glTexCoord2f((vPointCloud[i*k + j + 2].X + avg[index_num].x) / 370, -(vPointCloud[i*k + j + 2].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[i*k + j + 2].X, vPointCloud[i*k + j + 2].Y, vPointCloud[i*k + j + 2].Z);
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 2].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 2].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 2].X, vPointCloud[(i + 1)*k + j + 2].Y, vPointCloud[(i + 1)*k + j + 2].Z);
			
			//5
			
			glTexCoord2f((vPointCloud[(i + 1)*k + j].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j].X, vPointCloud[(i + 1)*k + j].Y, vPointCloud[(i + 1)*k + j].Z);
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glTexCoord2f((vPointCloud[(i + 2)*k + j].X + avg[index_num].x) / 370, -(vPointCloud[(i + 2)*k + j].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 2)*k + j].X, vPointCloud[(i + 2)*k + j].Y, vPointCloud[(i + 2)*k + j].Z);
			
			//6
			
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glTexCoord2f((vPointCloud[(i + 2)*k + j].X + avg[index_num].x) / 370, -(vPointCloud[(i + 2)*k + j].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 2)*k + j].X, vPointCloud[(i + 2)*k + j].Y, vPointCloud[(i + 2)*k + j].Z);
			glTexCoord2f((vPointCloud[(i + 2)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 2)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 2)*k + j + 1].X, vPointCloud[(i + 2)*k + j + 1].Y, vPointCloud[(i + 2)*k + j + 1].Z);
			
			//7
			
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glTexCoord2f((vPointCloud[(i + 2)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 2)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 2)*k + j + 1].X, vPointCloud[(i + 2)*k + j + 1].Y, vPointCloud[(i + 2)*k + j + 1].Z);
			glTexCoord2f((vPointCloud[(i + 2)*k + j + 2].X + avg[index_num].x) / 370, -(vPointCloud[(i + 2)*k + j + 2].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 2)*k + j + 2].X, vPointCloud[(i + 2)*k + j + 2].Y, vPointCloud[(i + 2)*k + j + 2].Z);
			
			//8
			
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 1].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 1].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 1].X, vPointCloud[(i + 1)*k + j + 1].Y, vPointCloud[(i + 1)*k + j + 1].Z);
			glTexCoord2f((vPointCloud[(i + 1)*k + j + 2].X + avg[index_num].x) / 370, -(vPointCloud[(i + 1)*k + j + 2].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 1)*k + j + 2].X, vPointCloud[(i + 1)*k + j + 2].Y, vPointCloud[(i + 1)*k + j + 2].Z);
			glTexCoord2f((vPointCloud[(i + 2)*k + j + 2].X + avg[index_num].x) / 370, -(vPointCloud[(i + 2)*k + j + 2].Y + avg[index_num].y) / 370); glVertex3f(vPointCloud[(i + 2)*k + j + 2].X, vPointCloud[(i + 2)*k + j + 2].Y, vPointCloud[(i + 2)*k + j + 2].Z);

		}
	}
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
}