#include "ObjLoader.h"
#include <iostream>
#include <fstream>

using namespace std;
//模型路径
string filePath = "objmodel/rose+vase.obj";

ObjLoader objModel = ObjLoader(filePath);
//实现移动鼠标观察模型所需变量
static float c = 3.1415926 / 180.0f;
static float r = 120.0f;//相机距离
static int degree = 90;
static int degree2 = 90;
static int oldPosY = -1;
static int oldPosX = -1;
int dis_z = 0;
bool mouseLeftDown;
bool mouseRightDown;
//安置光源
void setLightRes() {
	GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light0_diffuse[] = { 1, 0.2, 0.2, 1.0 };//红色点光源
	GLfloat light0_specular[] = { 0.5, 0.5, 0, 1.0 };
	GLfloat light0_position[] = { 0, 0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glEnable(GL_LIGHTING); //启用光源
	glEnable(GL_LIGHT0);   //使用指定灯光
}

GLfloat ctrlpoints[5][5][3] = {
{{-50,0,4},{-50,10,-4},{-50,20,-1},{-50,30,0},{-50,40,-2}},
{{-40,0,-2},{-40,10,0},{-40,20,-6},{-40,30,-6},{-40,40,2}},
{{-30,0,9},{-30,10,-6},{-30,20,-5},{-30,30,2},{-30,40,8}},
{{-20,0,8},{-20,10,-8},{-20,20,4},{-20,30,0},{-20,40,-5}},
{{-10,0,0},{-10,10,-8},{-10,20,4},{-10,30,-7},{-10,40,-9}},
};

GLfloat texpts[2][2][2] = { {{0.0, 0.0}, {0.0, 1.0}},
	{{1.0, 0.0}, {1.0, 1.0}} };

//初始化
void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("刘京宗-3019213043");
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	setLightRes();
	glEnable(GL_DEPTH_TEST);

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 5, 0, 1, 15, 5, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3); //激活该曲面函数
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
}


int WriteFile(string filepath, const string& Init)
{
	//定义文件输出流 
	ofstream outfile;

	//根据参数路径创建要输出的文件 
	outfile.open(filepath, ios::out | ios::trunc);
	if (!outfile)
		return 1;
	outfile << Init << endl;

	outfile.close();
	return 0;
}
string Read_Str(string filepath)
{
	ifstream infile;
	infile.open(filepath);
	//打开失败，路径不正确
	if (!infile)
		cout << "Open File Fail!" << endl;
	//读取文本内容到字符串
	string readStr((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

	return readStr;
}


//纹理数据
#define imageWidth 64
#define imageHeight 64
GLubyte image[3 * imageWidth * imageHeight];
void makeImage(void)
{
	string pixels = Read_Str("pic.txt");
	int p = 0;
	int start = 0;
	for (int k = 0; k < pixels.size(); k++) {
		if (pixels[k] == ',') {
			image[p] = stoi(pixels.substr(start, k));
			p++;
			start = k + 1;
		}
	}
	for (int i = 0; i < 3 * imageWidth * imageHeight / 2; i++) {
		swap(image[i], image[3 * imageWidth * imageHeight - 1 - i]);
	}
}

int p_num = 100;

void beizerdraw() {
	//int i, j;
	//for (j = 0; j < p_num; j++)
	//{
	//	glBegin(GL_LINE_STRIP);
	//	for (i = 0; i < p_num; i++)
	//		glEvalCoord2f((GLfloat)i / p_num, (GLfloat)j / p_num); //调用求值器
	//	glEnd();
	//	glBegin(GL_LINE_STRIP);
	//	for (i = 0; i < p_num; i++)
	//		glEvalCoord2f((GLfloat)j / p_num, (GLfloat)i / p_num); //调用求值器
	//	glEnd();
	//}
	makeImage();
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);//均匀产生坐标
	glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glEnable(GL_TEXTURE_2D);
}

void display()
{
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setLightRes();
	glPushMatrix();

	gluLookAt(r * cos(c * degree), r * cos(c * degree2), dis_z + r * sin(c * degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);//视图矩阵：
	//初始坐标为（0，0，r）
	glDisable(GL_TEXTURE_2D);

	objModel.Draw();//绘制obj模型
	glEnable(GL_TEXTURE_2D);
	beizerdraw();

	glPopMatrix();
	glutSwapBuffers();
}


void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
}

//移动鼠标360观察模型
void moseMove(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}
void changeViewPoint(int x, int y)
{
	if (mouseRightDown) {
		int temp = x - oldPosX;
		degree += temp;
		int temp2 = y - oldPosY;
		degree2 += temp2;
		oldPosX = x;
		oldPosY = y;
	}
	if (mouseLeftDown) {
		int temp3 = x - oldPosX;
		oldPosX = x;
		dis_z += temp3 * 0.5;
	}
}

void key_board(unsigned char key, int x, int y)
{
	glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);
	if (key == 'u')//逆时针旋转
		glRotatef(10, 0, 0, 1);
	if (key == 'o')//顺时针旋转
		glRotatef(10, 0, 0, -1);
	if (key == 'd')//右
		glTranslatef(10, 0, 0);
	if (key == 'a')//左
		glTranslatef(-10, 0, 0);
	if (key == 'w')//上
		glTranslatef(0, 10, 0);
	if (key == 's')//下
		glTranslatef(0, -10, 0);
	if (key == 'q')//近
		glTranslatef(0, 0, 10);
	if (key == 'e')//远
		glTranslatef(0, 0, -10);

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(moseMove);
	glutKeyboardFunc(key_board);
	glutMotionFunc(changeViewPoint);
	glutIdleFunc(glutPostRedisplay);
	glutMainLoop();
	return 0;
}