#include "Viewer.h"

TrackBall tb( GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON );
bool drawAxes = true;         // Draw world coordinate frame axes iff true.
bool drawWireframe = true;     // Draw polygons in wireframe if true, otherwise polygons are filled.
bool usePhongShading = true;   // Use Phong shading shaders if true, otherwise use Gouraud shading.

int winWidth = 1024;     // Window width in pixels.
int winHeight = 746;    // Window height in pixels.
int frameIdx = 0;
vector<SColorPoint3D> vPointCloud, vPointCloudr;



static void MyMotion( int x, int y )
{
	tb.motion(x, y);
    glutPostRedisplay();
}

void DrawAxes( double length )
{
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glDisable( GL_LIGHTING );
    glLineWidth( 3.0 );
    glBegin( GL_LINES );
        // x-axis.
        glColor3f( 1.0, 0.0, 0.0 );
        glVertex3d( 0.0, 0.0, 0.0 );
        glVertex3d( length, 0.0, 0.0 );
        // y-axis.
        glColor3f( 0.0, 1.0, 0.0 );
        glVertex3d( 0.0, 0.0, 0.0 );
        glVertex3d( 0.0, -length, 0.0 );
        // z-axis.
        glColor3f( 0.0, 0.0, 1.0 );
        glVertex3d( 0.0, 0.0, 0.0 );
        glVertex3d( 0.0, 0.0, length );
    glEnd();
    glPopAttrib();
}

void Texinit(void)
{
	extern GLuint texName[N];
	extern GLubyte texture[N][306][370][3];
	GLubyte tex[306][370][3];	//temporary texture.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(N, texName);	//the number changes as the number of frames change.

	for (int i = 0; i < N; i++){
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_NEAREST);
		for (int a = 0; a < 306; a++){
			for (int b = 0; b < 370; b++){
				tex[a][b][2] = texture[i][a][b][2];
				tex[a][b][1] = texture[i][a][b][1];
				tex[a][b][0] = texture[i][a][b][0];
			}
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 370,
			306, 0, GL_RGB, GL_UNSIGNED_BYTE,
			tex);	//inside the parameters are width,height.
	}
}

void MyDisplay( void )
{
    //glClearColor (255.0,255.0,255.0,1.0);
	extern int flag_type, index_num, n;
	extern vector<vector<SColorPoint3D>> vP, vPr;
	vPointCloud = vP[index_num];
	vPointCloudr= vPr[index_num];
	extern GLuint texName[N];
	extern GLubyte texture[N][306][370][3];
	glClearColor (0.2,0.2,0.2,1.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 80.0,(double)winWidth/winHeight, OBJECT_RADIUS, 100.0 * OBJECT_RADIUS );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    // Put point light source in eye space. 
	glLightfv( GL_LIGHT0, GL_POSITION, light0Position);
    gluLookAt( 0.0, 0.0, 100,0,0,0,0,1,0);
	glPushMatrix();
		tb.applyTransform();
		//Draw axes.
		DrawAxes(2*OBJECT_RADIUS);

		if (flag_type % 3 == 0){ DrawPointClouds(); }
		else if (flag_type % 3 == 1){ DrawTriangles(n); }
		else if (flag_type % 3 == 2){ DrawLines(n); }
		else DrawPointClouds();

		//DrawCameras();
	    //DrawFeatures();                 
		//glutSolidTeapot(OBJECT_RADIUS);

	glColor3fv( objectColor );
	glPopMatrix();
	glutSwapBuffers();
}

void MyCameraDisplay(void){
    
	glClearColor (0.0,0.0,0.0,1.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 80.0,(double)winWidth/winHeight, OBJECT_RADIUS, 100.0 * OBJECT_RADIUS );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    // Put point light source in eye space. 
	glLightfv( GL_LIGHT0, GL_POSITION, light0Position);
    gluLookAt( 0.0, 0.0, 100,0,0,0,0,1,0);
	glPushMatrix();
		tb.applyTransform();
		// Draw axes.
		if ( !drawAxes ) DrawAxes( 2 * OBJECT_RADIUS );
		// Draw object.
		//glColor3fv( objectColor );
		glutSolidTeapot(OBJECT_RADIUS);
		

	glPopMatrix();
	glutSwapBuffers();
}

void MyKeyboard(unsigned char key, int x, int y )
{
	extern int flag_type;
	extern int index_num;
	switch ( key )
    {
        case 'f':
		case 'F':
			glutPostRedisplay();
            break;


        // Toggle axes.
        case 'x':
        case 'X': 
            glutPostRedisplay();
            break;

        case 's':
		case 'S':
			tb.save("openGLpose.txt");
		    break;

		case 'l':
		case 'L':
			tb.load("openGLpose.txt");
			tb.applyTransform();
			glutPostRedisplay();
			break;
		
		case 'c':
		case 'C':
			glutPostRedisplay();
            break;

		case 'g':
		case 'G':
			glutPostRedisplay();
            break;
			
		case 'q':
        case 'Q': 
			if(frameIdx>0)frameIdx--;
            glutPostRedisplay();
			break;

		case 'b':
		case 'B':
			flag_type++;
			glutPostRedisplay();
			break;

		case 'r':
		case 'R':
			if (index_num == 0) index_num = N;
			index_num--;
			printf("Curren Frame: %d\n", index_num);
			glutPostRedisplay();
			break;

		case 't':
		case 'T':
			if (index_num == N-1) index_num = -1;
			index_num++;
			printf("Curren Frame: %d\n", index_num);
			glutPostRedisplay();
			break;
		
		default:	
        break;
	}
}

void Timer(int extra){
    glutPostRedisplay();
	glutTimerFunc(100,Timer,0);
}

void MyReshape( int w, int h )
{
	tb.reshape( w, h );
    winWidth = w;
    winHeight = h;
    glViewport( 0, 0, w, h );
}

static void MyMouse( int button, int state, int x, int y )
{
	tb.mouse( button, state, x, y );
	glutPostRedisplay();
}

void MyInit( void )
{
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // Set black background color.
    glEnable( GL_DEPTH_TEST ); // Use depth-buffer for hidden surface removal.
    glShadeModel( GL_SMOOTH );

    // Set Light 0.
    glLightfv( GL_LIGHT0, GL_AMBIENT, light0Ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light0Diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light0Specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHTING );

	// Set some global light properties.
    GLfloat globalAmbient[] = { 0.1, 0.1, 0.1, 1.0 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbient );
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );

    // Set the universal material properties.
    // The diffuse and ambient components can be changed using glColor*().
    glMaterialfv( GL_FRONT, GL_SPECULAR, materialSpecular );
    glMaterialfv( GL_FRONT, GL_SHININESS, materialShininess );
    glMaterialfv( GL_FRONT, GL_EMISSION, materialEmission );
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );

    glEnable( GL_NORMALIZE ); // Let OpenGL automatically renomarlize all normal vectors.

	tb.load("openGLpose.txt");
	tb.applyTransform();
}



void View3D(int argc, char** argv, std::vector<vector<SColorPoint3D>> &vP){

	glutInit(&argc,argv);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( winWidth, winHeight );
	glutCreateWindow( "3D points" );
	MyInit(); Texinit();
    
	glutDisplayFunc( MyDisplay ); 
    glutReshapeFunc( MyReshape );
    glutKeyboardFunc( MyKeyboard );
	glutMouseFunc( MyMouse );
	glutMotionFunc( MyMotion );

	glutMainLoop();
}




