#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Transform3D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include <iostream>
#include <windows.h>
#include <math.h>
#include <mmsystem.h>



#define widthCam 16
#define heigthCam 8
#define PI 3.14159265358979323846
#define inf 1000000
#define gridLength 50
//dimensiunea gridului
#define range 40
//aria de constructie

using namespace std;

bool edit = true;
Visual2D *v2d1;
int moveX = 0, moveY = 0;
vector<pair<Object3D *,int>> cubes;
vector<pair<int, int>> cubePoint;
//vector<int>  cubesRotate;
Color darkGrey = Color(0.4, 0.4, 0.4);
Color red = Color(1, 0, 0) ,ligthGrey = Color (0.6,0.6,0.6), darkRed = Color ( 0.5,0,0), black = Color(0,0,0), blue = Color(0.2, 0.2, 1), green = Color(0, 1 * 0.5, 0), darkBrown = Color(0.35, 0.175, 0), ligthBrown = Color(0.7, 0.35, 0);

Object3D * X, *aux;
Object3D  * grid[gridLength][gridLength];
//gridul cu fill
Object3D  * gridFrame[gridLength][gridLength];
//cel fara
bool clearLand[gridLength][gridLength];
//daca este gol
bool isLigth[gridLength][gridLength];
//daca pamantul este in raza pietei
float MoveCamX = -gridLength, MoveCamY = gridLength;
//deplasarea camerei
int save[gridLength][gridLength];
//matricea corespondenta 

float n = 2;
//dimensiunea unei celule
void connectThis(int x, int y);
bool checkGrid(int x, int y, int val){
	//verificam daca e val
	if ((x < 0) | (x > gridLength))
		return false;
	if ((y < 0) | (y > gridLength))
		return false;
	if (save[y][x] == val)
		return true;
	return false;
}
void connectAround(int x, int y){
	//conecteaza tot
	connectThis(x - 1, y);
	connectThis(x, y - 1);
	connectThis(x + 1, y);
	connectThis(x, y + 1);
}
bool checkAround(int x, int y, int val){
	//Verificam daca gasim val pe langa
	if (checkGrid(x - 1, y, val))
		return true;
	if (checkGrid(x, y - 1, val))
		return true;
	if (checkGrid(x + 1, y, val))
		return true;
	if (checkGrid(x, y + 1, val))
		return true;
	return false;
}
void connectThis(int x, int y){
	//conecteaza elementul curent
	if ((x < 0) | (x > gridLength))
		return;
	if ((y < 0) | (y > gridLength))
		return;
	if (save[y][x] == 6){
		//daca drumul nu este conectat il facem conectat
		grid[y][x]->color = ligthGrey;
		save[y][x] = 7;
		connectAround(x, y);
	}
	if (save[y][x] == 4){
		//daca este casa neconectata o conectam
		save[y][x] = 5;
		for (int i = 0; i < cubePoint.size(); i++){
			if (cubePoint[i].first == x){
				if (cubePoint[i].second == y){
					cubes[i].first->color = darkRed;
					return;
				}
			}
		}
	}
}
void newGrid(float x, float y, float z, int row, int column){
	//creaza un patrat
	vector <Point3D*> vertices;
	vector <Face*> faces;
	Point3D po, inte;
	vertices.push_back(new Point3D(x, y, z));
	vertices.push_back(new Point3D(x + n, y, z));
	vertices.push_back(new Point3D(x + n, y, z + n));
	vertices.push_back(new Point3D(x, y, z + n));
	//cele 6 fete
	vector <int> contour;
	//fata jos
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));
	Object3D  * gridFrameSpace = new Object3D(vertices, faces);

	Object3D  * gridSpace = new Object3D(vertices, faces, darkBrown, true);
	DrawingWindow::addObject3D_to_Visual2D(gridFrameSpace, v2d1);
	DrawingWindow::addObject3D_to_Visual2D(gridSpace, v2d1);
	grid[row][column] = gridSpace;
	gridFrame[row][column] = gridFrameSpace;
}
Object3D *  newTower(float x, float y, float z, Color color, bool fill){
	//creaaza piata
	vector <Point3D*> vertices;
	vector <Face*> faces;

	//varfurile de jos
	vertices.push_back(new Point3D(x, y, z));
	vertices.push_back(new Point3D(x + n, y, z));
	vertices.push_back(new Point3D(x + n, y, z + n));
	vertices.push_back(new Point3D(x, y, z + n));
	//varfurile de sus			  
	vertices.push_back(new Point3D(x, y + n, z));
	vertices.push_back(new Point3D(x + n, y + n, z));
	vertices.push_back(new Point3D(x + n, y + n, z + n));
	vertices.push_back(new Point3D(x, y + n, z + n));
	vertices.push_back(new Point3D(x + (n / 2), y + (2 * n), z + (n / 2)));
	vector <int> contour;
	//fata jos
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));
	////fata sus
	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));
	//fata fata
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));
	//fata dreapta
	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));
	//fata spate
	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));
	//fata stanga
	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));
	//

	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(8);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(8);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(6);
	contour.push_back(7);
	contour.push_back(8);
	faces.push_back(new Face(contour));

	contour.clear();
	contour.push_back(7);
	contour.push_back(4);
	contour.push_back(8);



	faces.push_back(new Face(contour));

	Object3D  * cube = new Object3D(vertices, faces, color, fill);
	DrawingWindow::addObject3D_to_Visual2D(cube, v2d1);
	cubes.push_back(pair<Object3D*,int>(cube,0));
	cubePoint.push_back(pair<int, int>(-1, -1));
	cube = new Object3D(vertices, faces, black, false);
	DrawingWindow::addObject3D_to_Visual2D(cube, v2d1);
	cubes.push_back(pair<Object3D*, int>(cube, 0));
	cubePoint.push_back(pair<int, int>(-1, -1));
	//punem aria de constructie
	for (int i = 0; i < gridLength; i ++ ){
		for (int j = 0; j < gridLength; j++)
		{
			if (grid[i][j]->color == darkBrown){
				if (((moveX - j) * (moveX - j) + (moveY - i) * (moveY - i)) * n <= range){
					grid[i][j]->color = ligthBrown;
					isLigth[i][j] = true;
				}
			}
		}
	}
	return cube;

}
Object3D *  newHouse(float x, float y, float z, Color color, bool fill){
	//casa noua
	vector <Point3D*> vertices;
	vector <Face*> faces;

	//varfurile de jos
	vertices.push_back(new Point3D(x, y, z));//6
	vertices.push_back(new Point3D(x + n, y, z));//1
	vertices.push_back(new Point3D(x + n, y, z + n / 2));//2
	vertices.push_back(new Point3D(x + n / 2, y, z + n / 2));//3
	vertices.push_back(new Point3D(x + n/2, y, z + n));//4
	vertices.push_back(new Point3D(x, y, z + n));//5
	//varfurile de sus			  
	vertices.push_back(new Point3D(x, y+n, z));//6
	vertices.push_back(new Point3D(x + n, y+n, z));//1
	vertices.push_back(new Point3D(x + n, y+n, z + n / 2));//2
	vertices.push_back(new Point3D(x + n / 2, y + n, z + n / 2));//3
	vertices.push_back(new Point3D(x + n / 2, y + n, z + n));//4
	vertices.push_back(new Point3D(x, y+n, z + n));//5

	//cele 6 fete
	vector <int> contour;
	//fata jos
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(5);
	faces.push_back(new Face(contour));
	////fata sus
	contour.clear();
	contour.push_back(6);
	contour.push_back(7);
	contour.push_back(8);
	contour.push_back(9);
	contour.push_back(10);
	contour.push_back(11);

	faces.push_back(new Face(contour));
	//fata fata
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));
	//fata dreapta
	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(8);
	contour.push_back(7);
	faces.push_back(new Face(contour));
	//fata spate
	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(9);
	contour.push_back(8);
	faces.push_back(new Face(contour));
	//
	contour.clear();
	contour.push_back(3);
	contour.push_back(4);
	contour.push_back(10);
	contour.push_back(9);
	faces.push_back(new Face(contour));

	//
	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(11);
	contour.push_back(10);
	faces.push_back(new Face(contour));


	//fata stanga
	contour.clear();
	contour.push_back(5);
	contour.push_back(0);
	contour.push_back(6);
	contour.push_back(11);
	faces.push_back(new Face(contour));

	Object3D  * cube = new Object3D(vertices, faces, color, fill);
	DrawingWindow::addObject3D_to_Visual2D(cube, v2d1);
	cubes.push_back(pair<Object3D*, int>(cube, 0));
	cubePoint.push_back(pair<int, int>(moveX, moveY));
	cube = new Object3D(vertices, faces, black, false);
	DrawingWindow::addObject3D_to_Visual2D(cube, v2d1);
	cubes.push_back(pair<Object3D*, int>(cube, 0));
	cubePoint.push_back(pair<int, int>(moveX, moveY));
	return cube;

}
Object3D *  newCube(float x, float y, float z, Color color, bool fill){
	//cub nou
	vector <Point3D*> vertices;
	vector <Face*> faces;

	//varfurile de jos
	vertices.push_back(new Point3D(x, y, z));
	vertices.push_back(new Point3D(x + n, y, z));
	vertices.push_back(new Point3D(x + n, y, z + n));
	vertices.push_back(new Point3D(x, y, z + n));
	//varfurile de sus			  
	vertices.push_back(new Point3D(x, y + n, z));
	vertices.push_back(new Point3D(x + n, y + n, z));
	vertices.push_back(new Point3D(x + n, y + n, z + n));
	vertices.push_back(new Point3D(x, y + n, z + n));
	//cele 6 fete
	vector <int> contour;
	//fata jos
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));
	////fata sus
	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));
	//fata fata
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));
	//fata dreapta
	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));
	//fata spate
	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));
	//fata stanga
	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));
	
	Object3D  * cube = new Object3D(vertices, faces, color, fill);
	DrawingWindow::addObject3D_to_Visual2D(cube, v2d1);
	cubes.push_back(pair<Object3D*, int>(cube, 0));
	cubePoint.push_back(pair<int, int>(-1, -1));
	return cube;

}

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	v2d1 = new Visual2D( - widthCam, -heigthCam, widthCam, heigthCam, 0, 0, DrawingWindow::width, DrawingWindow::height);
	v2d1->tipTran(true);
	addVisual2D(v2d1);
	
	X = newCube(-gridLength * n, 0, 0, red, false);

	for (int i = 0; i < gridLength; i++){
		for (int j = gridLength ; j > 0; j--){
			newGrid(-j * n, 0, -i * n, i, gridLength - j);
		}
	}
	//newGrid(0, 0, 0);
}


//functia care permite animatia
void DrawingWindow::onIdle()
{
	Transform3D::loadIdentityModelMatrix();
	Transform3D::rotateMatrixOy(0.785);
	Transform3D::rotateMatrixOx(0.615);
	for (int i = 0; i < gridLength; i++){
		for (int j = 0; j < gridLength; j++){
			Transform3D::applyTransform(grid[i][j]);

		}
	}
	for (int i = 0; i < gridLength; i++){
		for (int j = 0; j < gridLength; j++){
			Transform3D::applyTransform(gridFrame[i][j]);

		}
	}
	
	for (int i = 0; i < cubes.size(); i++){
		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(-cubes[i].first->vertices[0]->x, -cubes[i].first->vertices[0]->y, -cubes[i].first->vertices[0]->z);
		Transform3D::rotateMatrixOy(cubes[i].second * PI / 180);
		switch (cubes[i].second)
		{
			//in fuctie de unghi mai este nevoie de o deplasare
		case 270:
			Transform3D::translateMatrix(cubes[i].first->vertices[0]->x, cubes[i].first->vertices[0]->y, cubes[i].first->vertices[0]->z+ n);
			break;
		case 90:
			Transform3D::translateMatrix(cubes[i].first->vertices[0]->x + n, cubes[i].first->vertices[0]->y, cubes[i].first->vertices[0]->z);
			break;
		case 180:
			Transform3D::translateMatrix(cubes[i].first->vertices[0]->x + n, cubes[i].first->vertices[0]->y, cubes[i].first->vertices[0]->z+ n);
			break;
		default:
			Transform3D::translateMatrix(cubes[i].first->vertices[0]->x, cubes[i].first->vertices[0]->y, cubes[i].first->vertices[0]->z);
			break;
		}
		Transform3D::rotateMatrixOy(0.785);
		Transform3D::rotateMatrixOx(0.615);
		Transform3D::applyTransform(cubes[i].first);

	}
	Transform3D::loadIdentityModelMatrix();
	Transform3D::translateMatrix(moveX * n, 0, - moveY * n);
	Transform3D::rotateMatrixOy(0.785);
	Transform3D::rotateMatrixOx(0.615);
	Transform3D::applyTransform(X);
	float movecx = MoveCamX *0.7;
	float movecy = MoveCamY * 0.4;
	v2d1->fereastra(movecx * n - widthCam, movecy* n - heigthCam, widthCam + movecx* n, heigthCam + movecy* n);
	cout << moveX << " " << moveY << " " << clearLand[moveX][moveY] << "|" << X->vertices[0][0].x + moveX + gridLength* n << " " << X->vertices[0][0].z + moveY << endl;
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{
	v2d1->poarta(0,0,width,height); 
}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{
	switch (key)
	{
	case 'v':
		//V adauga piata
		if (edit){
			break;
		}
		if (moveX < 0 || moveX >= gridLength){
			break;
		}
		if (moveY < 0 || moveY >= gridLength){
			break;
		}
		if (save[moveY][moveX] != 0 && save[moveY][moveX] != 1){
			break;
		}
		save[moveY][moveX] = 3;
		newTower(moveX * n - gridLength* n, 0, -moveY * n, darkGrey, true);
		break;
	case 'b':
		//adauga drum
		if (edit){
			break;
		}
		if (moveX < 0 || moveX >= gridLength){
			break;
		}
		if (moveY < 0 || moveY >= gridLength){
			break;
		}
		if (save[moveY][moveX] != 0 && save[moveY][moveX] != 1){
			break;
		}
		if (save[moveY][moveX] != 0 && save[moveY][moveX] != 1){
			break;
		}
		if (checkAround(moveX, moveY, 3) || checkAround(moveX, moveY, 7)){
			save[moveY][moveX] = 7;
			grid[moveY][moveX]->color = ligthGrey;
			connectAround(moveX, moveY);
		}
		else{
			save[moveY][moveX] = 6;

			grid[moveY][moveX]->color = red;
		}
		break;
	case 'c':
		//adauga casa
		if (edit){
			break;
		}
		if (moveX < 0 || moveX >= gridLength){
			break;
		}
		if (moveY < 0 || moveY >= gridLength){
			break;
		}
		if (save[moveY][moveX] != 0 && save[moveY][moveX] != 1){
			break;
		}
		if (grid[moveY][moveX]->color == ligthBrown){
			if (checkAround(moveX, moveY, 7) || checkAround(moveX, moveY, 3)){
				newHouse(moveX * n - gridLength* n, 0, -moveY * n, darkRed, true);
				save[moveY][moveX] = 5;
			}
			else{
				newHouse(moveX * n - gridLength* n, 0, -moveY * n, red, true);
				save[moveY][moveX] = 4;
			}
			grid[moveY][moveX]->color = ligthGrey;
		}
		break;
	case 'x':
		if (edit){
			break;
		}
		if (moveX < 0 || moveX >= gridLength){
			break;
		}
		if (moveY < 0 || moveY >= gridLength){
			break;
		}
		if (save[moveY][moveX] != 0 && save[moveY][moveX] != 1){
			break;
		}
		for (int i = 0; i < cubePoint.size(); i++){
			if (cubePoint[i].first == moveX){
				if (cubePoint[i].second == moveY){
					if (cubes[i].second == 270)
						cubes[i].second = 0;
					else{
						cubes[i].second += 90;
					}
				}
			}
		}
		break;
	case 'z':
		//edit mode
		edit = !edit;
		break;

	case KEY_UP:
		moveY++;
		MoveCamY++;
		MoveCamX++;
		break;
	case KEY_DOWN:
		moveY--;
		MoveCamY--;
		MoveCamX--;
		break;
	case KEY_LEFT:
		moveX--;
		MoveCamY++;
		MoveCamX--;
		break;
	case KEY_RIGHT:
		moveX++;
		MoveCamY--;
		MoveCamX++;
		break;
	case ' ':
		if (!edit){
			break;
		}
		if (moveX < 0 || moveX >= gridLength){
			break;
		}
		if (moveY < 0 || moveY >= gridLength){
			break;
		}
		if (save[moveY][moveX] != 0 && save[moveY][moveX] != 1){
			break;
		}
		if (edit){
			if (grid[moveY][moveX]->color == blue){
				if (isLigth[moveY][moveX]){
					save[moveY][moveX] = 1;
					grid[moveY][moveX]->color = ligthBrown;
				}
				else{
					save[moveY][moveX] = 0;
					grid[moveY][moveX]->color = darkBrown;
				}
				clearLand[moveY][moveX] = false;
			}
			else if ((grid[moveY][moveX]->color == ligthBrown) || (grid[moveY][moveX]->color == darkBrown)){
				clearLand[moveY][moveX] = true;
				grid[moveY][moveX]->color = blue;
				save[moveY][moveX] = 2;
		}
	}
			break;
		case 27 : exit(0);
	}
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{
	cout << x << " " << y << endl;
}


int main(int argc, char** argv)
{	
	//creare fereastra
	PlaySound(TEXT("lux"), NULL, SND_SYNC | SND_ASYNC | SND_LOOP);
	cout << "Hello world!!!" << endl;
	cout << "Space - swap water | Z - edit mode | X - turn 90 | C - place house | V place townhall" << endl;
	cout << "Grid scale" << endl;
	cin >> n;
	DrawingWindow dw(argc, argv, 1600, 700, 0, 0, "Builder");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;
}