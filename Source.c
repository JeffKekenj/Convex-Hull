#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>

/*
Jeff Kekenj
4759171
COSC 3P98
Assignment 2 Part 1
*/

//the pixel structure
typedef struct {
	GLubyte r, g, b;
} pixel;

//the global structure
typedef struct {
	pixel *data;
	int w, h;
} glob;
glob global;

//Point
typedef struct Node {
   int x;
   int y;
   int inHull;//0 not in hull, else in hull
} Point;

typedef struct Segment {
   int x;
   int y;
} segment;

Point allPoints[1000];

segment segments[1000];

int segInt = 0;



void display_image(void)
{
	glDrawPixels(global.w, global.h, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*)global.data);	
	glFlush();
}//display_image()

void create_vertex(){	
	int i = 0;
	while (i < 100){
		Point p = {rand()%400,rand()%400}; //checkforunique
		p.inHull = 0;
		/*Point p;
		int thingy = 398;
		int placex = rand()%400;
		int placey = rand()%400;
		if (1 == rand()%4){
			placey = rand()%300;
		} else{			
			placey = rand()%399;
		}

		p.x = placex;
		p.y = placey;*/

		glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
		glColor3f(1.0,0.0,0.0);	
		glPointSize(6.0);
		glBegin(GL_POINTS);
		glVertex2i(p.x,p.y);		
		glEnd();
		glFlush();
		allPoints[i] = p;
		//allPoints[i].x = p.x;
		//allPoints[i].y = p.y;
		i++;
	}
}

void printarr(){
	int i = 0;
	for (i = 0; i<100;i++){
		printf("Here is stuff x %d\n",allPoints[i].x);
		//printf("Here is stuff y %d\n",allPoints[i].y);
	}
}

int cross(Point a, Point b, Point c){
     return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x));
}


void quickHull_Top_Peel(Point p1, Point p2){	
	//P1 min x,y
	//P2 max x,y
	int i;
	int areaTemp = 0;
	int max = 0;
	int maxPoint = 0;
	for (i=0;i<100;i++){	
		//IF not on hull
		//if (allPoints[i].x>p1.x && allPoints[i].y>p1.x){
		if (allPoints[i].x>p1.x){
		//if (cross(p1,p2, allPoints[i])>=0){
		if (allPoints[i].inHull == 0){
			areaTemp = area(p1,allPoints[i],p2);
			//printf("hey AREA %d\n",areaTemp);
			printf("");
			if (areaTemp < max){
				max = areaTemp;
				maxPoint = i;
			}
		}
	}
	}
	//create line between p1 and p2, then recurse
	//recusive stuff
	//if no midpoint, then move on p2 max quickhull
	if (maxPoint<1){
		if (p1.inHull == 0 && p2.inHull == 0){ //points not in hull
		//printf("Segment Int: %d\n",segInt);
		segments[segInt].x = p1.x;
		segments[segInt].y = p1.y;
		segInt++;
		//printf("Segment Int: %d\n",segInt);
		segments[segInt].x = p2.x;
		segments[segInt].y = p2.y;
		segInt++;
		//printf("Segment Int: %d\n",segInt);

		glBegin(GL_LINES);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p1.x,p1.y);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p2.x,p2.y);
		glEnd();
		glFlush();
	}
	} 
	else 
		quickHull_Top_Peel(p1, allPoints[maxPoint]);
	if (areaTemp != 0){
		quickHull_Top_Peel(p2, allPoints[findMaxPeel()]);
	}
}

void quickHull_Bottom_Peel(Point p1, Point p2){	
	//P1 min x,y
	//P2 max x,y
	int i;
	int areaTemp = 0;
	int max = 0;
	int minPoint = 0;
	//int highNum = 100000;
	int highNum = 0;
	for (i=0;i<100;i++){	
		if (allPoints[i].x>p1.x){
		//if ((cross(p1,allPoints[i],p2)) != 0){
			//if (cross(p1,p2, allPoints[i])){
			if (allPoints[i].inHull == 0){
		//printf("");
		areaTemp = area(p1,allPoints[i],p2);
		//printf("hey AREA %d\n",areaTemp);
		if (areaTemp > highNum){
			highNum = areaTemp;
			minPoint = i;
			}
			}
		}
	}
	//create line between p1 and p2, then recurse
	//recusive stuff
	//if no midpoint, then move on p2 max quickhull
	if (minPoint<1){
		if (p1.inHull == 0 && p2.inHull == 0){
		segments[segInt].x = p1.x;
		segments[segInt].y = p1.y;
		segInt++;
		segments[segInt].x = p2.x;
		segments[segInt].y = p2.y;
		segInt++;

		glBegin(GL_LINES);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p1.x,p1.y);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p2.x,p2.y);
		glEnd();
		glFlush();
		}
	} 
	else
		quickHull_Bottom_Peel(p1, allPoints[minPoint]);
	if (areaTemp != 0){
		quickHull_Bottom_Peel(p2, allPoints[findMaxPeel()]);
	}
}

int findMinPeel(){
	int i;
	int minIndex = 0;
	int highNum = allPoints[0].x;
	for (i=1;i<100;i++){
		if (allPoints[i].x < highNum && allPoints[i].inHull == 0){
			highNum = allPoints[i].x;
			minIndex = i;
		}
	}
	//printf("Here is the minIndex %d",minIndex);
	//printf("Here is the minimum %d",highNum);
	return minIndex;
}

int findMaxPeel(){
	int i;
	int maxIndex = 0;
	int lowNum = 0;
	for (i=0;i<100;i++){
		if (allPoints[i].x > lowNum && allPoints[i].inHull == 0){
			lowNum = allPoints[i].x;
			maxIndex = i;
		}
	}
	//printf("Here is the minIndex %d",maxIndex);
	//printf("Here is the minimum %d",lowNum);
	return maxIndex;
}


//cross
//cross(p1,allPoints[i],p2)

void quickHull_Top(Point p1, Point p2){	
	//P1 min x,y
	//P2 max x,y
	int i;
	int areaTemp = 0;
	int max = 0;
	int maxPoint = 0;
	for (i=0;i<100;i++){
		//if (allPoints[i].x>p1.x && allPoints[i].y>p1.x){
			areaTemp = area(p1,allPoints[i],p2);
			//printf("hey AREA %d\n",areaTemp);
			//printf("");
			if (areaTemp < max){
				max = areaTemp;
				maxPoint = i;
			//}
		}
	}
	//create line between p1 and p2, then recurse
	//recusive stuff
	//if no midpoint, then move on p2 max quickhull

	if (maxPoint<1){
		segments[segInt].x = p1.x;
		segments[segInt].y = p1.y;
		segInt++;
		segments[segInt].x = p2.x;
		segments[segInt].y = p2.y;
		segInt++;

		glBegin(GL_LINES);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p1.x,p1.y);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p2.x,p2.y);
		//glVertex2i(allPoints[maxPoint].x,allPoints[maxPoint].y);
		glEnd();
		glFlush();
	} 
	else 
		quickHull_Top(p1, allPoints[maxPoint]);
	if (areaTemp != 0){
	//if (areaTemp > 0){
		quickHull_Top(p2, allPoints[findMax()]);
	}
}

/*void quickHull_Top(Point p1, Point p2){	
	//P1 min x,y
	//P2 max x,y
	int i;
	int areaTemp = 0;
	int max = 0;
	int maxPoint = 0;
	for (i=0;i<100;i++){		
		if (!(allPoints[i].y<p2.y && allPoints[i].y<p1.y)){
			areaTemp = area(p1,allPoints[i],p2);
			printf("hey AREA %d\n",areaTemp);
		}
		if (areaTemp < max){
			max = areaTemp;
			maxPoint = i;
		}
	}
	//create line between p1 and p2, then recurse
	//recusive stuff
	//if no midpoint, then move on p2 max quickhull
	if (maxPoint<1){
		glBegin(GL_LINES);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p1.x,p1.y);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p2.x,p2.y);
		glEnd();
		glFlush();
	} 
	else 
		quickHull_Top(p1, allPoints[maxPoint]);
		quickHull_Top(p2, allPoints[findMax()]);
}*/

void quickHull_Bottom(Point p1, Point p2){	
	//P1 min x,y
	//P2 max x,y
	int i;
	int areaTemp = 0;
	int max = 0;
	int minPoint = 0;
	//int highNum = 100000;
	int highNum = 0;
	for (i=0;i<100;i++){		
		//printf("");
		//if (allPoints[i].x>p1.x && allPoints[i].y>p1.x){
		areaTemp = area(p1,allPoints[i],p2);
		if (areaTemp > highNum){
			highNum = areaTemp;
			minPoint = i;
		//}
		}
	}
	//create line between p1 and p2, then recurse
	//recusive stuff
	//if no midpoint, then move on p2 max quickhull

	if (minPoint<1){
		segments[segInt].x = p1.x;
		segments[segInt].y = p1.y;
		segInt++;
		segments[segInt].x = p2.x;
		segments[segInt].y = p2.y;
		segInt++;

		glBegin(GL_LINES);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p1.x,p1.y);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(p2.x,p2.y);
		glEnd();
		glFlush();
	} 
	else
		quickHull_Bottom(p1, allPoints[minPoint]);
	if (areaTemp != 0){
		quickHull_Bottom(p2, allPoints[findMax()]);
	}
}

void display_hull(){
	int i;
	int k=0;
	printf("\n");
	for (i=0;i<100;i++){
		printf("Segment values x %d\n",segments[i].x);
		printf("Segment values y %d\n",segments[i].y);
	}

	
	while (k<100) {
		glBegin(GL_LINES);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(segments[k].x,segments[k].y);
		glColor3f(1.0, 0.38, 0.0);
		k++;
		glVertex2i(segments[k].x,segments[k].y);
		k++;
		glEnd();
		glFlush();
	}

}


int area(Point a, Point b, Point c){
	return ((a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y))/2);
	//return (((a.x)(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y))/2);
}

void quick_hull(){
	quickHull_Top(allPoints[findMin()], allPoints[findMax()]);
	quickHull_Bottom(allPoints[findMin()],allPoints[findMax()]);
}

void quick_hull_peel(){
	/*int i;
	int k = 1;
	quickHull_Top_Peel(allPoints[findMin()], allPoints[findMax()]);
	for (i=0;i<100;i++){
		printf("this is segment %d\n",segments[i].x);
		printf("this is segment %d\n",segments[i].y);
		printf("count: %d\n",k);
		k++;
	}*/

	int count,found;
	int i = 0;
	int k = 0;

	int temp=0;

	//quickHull_Top_Peel(allPoints[84], allPoints[83]);
	//quickHull_Top_Peel(allPoints[99], allPoints[10]);

	
	/*glColor3f(0.0,0.0,1.0);	
	glPointSize(6.0);
	glBegin(GL_POINTS);
	glVertex2i(allPoints[99].x,allPoints[99].y);		
	glVertex2i(allPoints[10].x,allPoints[10].y);	
	glEnd();
	glFlush();*/

	//quickHull_Top(allPoints[99], allPoints[10]);
	//quickHull_Top_Peel(allPoints[84], allPoints[83]);

	//quickHull_Bottom(allPoints[99], allPoints[10]);

	//quickHull_Bottom_Peel(allPoints[84], allPoints[83]);


	/*glBegin(GL_LINES);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(allPoints[99].x,allPoints[99].y);
		glColor3f(1.0, 0.38, 0.0);
		glVertex2i(allPoints[10].x,allPoints[10].y);
		glEnd();
		glFlush();*/

	while (temp < 20){

	printf("find Min Peelllll: %d\n",findMinPeel());
	printf("find Max Peelllll: %d\n",findMaxPeel());
	glColor3f(0.0,0.0,1.0);	
	glPointSize(6.0);
	glBegin(GL_POINTS);
	glVertex2i(allPoints[findMinPeel()].x,allPoints[findMinPeel()].y);		
	glVertex2i(allPoints[findMaxPeel()].x,allPoints[findMaxPeel()].y);		
	glEnd();
	glFlush();
	//printf("find Min Peel: %d\n",findMin());
	//printf("find Max Peel: %d\n",findMax());
	quickHull_Top_Peel(allPoints[findMinPeel()], allPoints[findMaxPeel()]);
	quickHull_Bottom_Peel(allPoints[findMinPeel()], allPoints[findMaxPeel()]);
	for (i=0;i<100;i++){
		for (k=0;k<1000;k++){
			if(allPoints[i].x == segments[k].x){//point in hull
				if(allPoints[i].y == segments[k].y)	{
					allPoints[i].inHull = 1;//setBool;
				}
		}
	}	
	}
	for (i=0;i<100;i++){
		if (allPoints[i].inHull != 0){
			printf("in Hull here x: %d\n",allPoints[i].x);
			printf("in Hull here y: %d\n",allPoints[i].y);

			glColor3f(0.0,1.0,0.0);	
			glPointSize(6.0);
			glBegin(GL_POINTS);
			glVertex2i(allPoints[i].x,allPoints[i].y);		
			glEnd();
			glFlush();

		}
	}
	temp++;
	}


	/*do{
		//quickHull_Top(allPoints[findMin()], allPoints[findMax()]);
		//quickHull_Bottom(allPoints[findMin()],allPoints[findMax()]);
		quickHull_Top_Peel(allPoints[findMinPeel()], allPoints[findMaxPeel()]);
		count = 0;
		for(i = 0;i<100;i++){
			found = 0;
			for(k = 0; k<1000; k++){ //all points in hull
				if(allPoints[i].x == segments[k].x){//point in hull
					if(allPoints[i].y == segments[k].y)	{
					allPoints[i].inHull = 1;//setBool;
					found = 1;
					}
				}
			}
			if(found == 0){
				count++;
			}
		}
	}while(count > 3);*/
}

int findMin(){
	int i;
	int minIndex = 0;
	int highNum = 100000;
	for (i=0;i<100;i++){
		if (allPoints[i].x < highNum){
			highNum = allPoints[i].x;
			minIndex = i;
		}
	}
	//printf("Here is the minIndex %d",minIndex);
	//printf("Here is the minimum %d",highNum);
	return minIndex;
}

int findMax(){
	int i;
	int maxIndex = 0;
	int lowNum = 0;
	for (i=0;i<100;i++){
		if (allPoints[i].x > lowNum){
			lowNum = allPoints[i].x;
			maxIndex = i;
		}
	}
	//printf("Here is the minIndex %d",maxIndex);
	//printf("Here is the minimum %d",lowNum);
	return maxIndex;
}


void reset(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnd();
    glFlush();
}

/*glut keyboard function*/
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 0x1B:
	case'q':
	case 'Q':
		exit(0);
		break;
	case 'n':
	case 'N':
		create_vertex();		
		break;
	case 'h':
	case 'H':
		printarr();		
		break;
	case 'c':
	case 'C':
		quick_hull();	
		break;
	case 't':
	case 'T':
		quickHull_Top(allPoints[findMin()],allPoints[findMax()]);
		break; 
	case 'b':
	case 'B':
		quickHull_Bottom(allPoints[findMin()],allPoints[findMax()]);
		break; 
	case 'p':
	case 'P':
		quick_hull_peel();
		break; 
	case 'k':
	case 'K':
		display_hull();
		break; 
	case 'r':
	case 'R':
		reset();
		break; 

	}
}//keyboard


int main(int argc, char** argv)
{

	printf("Q:quit\nN:C:Create 100 Points\nC:Convex Hull\nP:Peel\n R:Reset");


	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(global.w,global.h);
	glutCreateWindow("SIMPLE DISPLAY");
	glShadeModel(GL_SMOOTH);
	//glutDisplayFunc(square);
	glutKeyboardFunc(keyboard);
	glMatrixMode(GL_PROJECTION);
	//glutFullScreen();
	//glOrtho(0,global.w,0,global.h,0,1);
	glOrtho(0.0, 400.0, 0.0, 400.0, 0.0, 1.0); 	
	glutMainLoop();

	return 0;
}