#include<windows.h>
#include<GL/glut.h>
#include<bits/stdc++.h>
using namespace std;

int window=500;
int row=25, col=25;
int cell=window/row;
int speed=180;
int dx[]={cell, -cell, 0, 0};
int dy[]={0, 0, cell, -cell};
int dir=0; //0=right, 1=left, 2=top, 3=bottom
int gameOver=0;
int changeDir;
int score=0;

struct point{
     int x, y;
     point(){x=0, y=0;}
     point(int _x, int _y){ x=_x, y=_y; }
};

point food=point(-1, -1);

struct color{
     float r, g, b;
     color(){r=0, g=0, b=0;}
     color(float _r, float _g, float _b){r=_r, g=_g, b=_b;}
};

vector< point > body, boundary, grid;

void drawPoint(point p, color c, int pointSz){
     glPointSize(pointSz);
     glColor3f(c.r, c.g, c.b);
     glBegin(GL_POINTS);
          glVertex2i(p.x, p.y);
     glEnd();
}

bool checkPoint(point p){ // 1 means occupied, 0 means empty
     for(int i=0; i<(int)boundary.size(); ++i)
          if(boundary[i].x==p.x && boundary[i].y==p.y) return 1;
     for(int i=0; i<body.size(); ++i)
          if(body[i].x==p.x && body[i].y==p.y) return 1;
     return 0;
}

void setGrid(){
     int sz=window/2-cell/2;
     for(int i=sz; i>-window/2; i-=cell){
          boundary.push_back(point(-sz, i));
          boundary.push_back(point(i, -sz));
          boundary.push_back(point(sz, i));
          boundary.push_back(point(i, sz));
          for(int j=sz; j>-window/2; j-=cell){
               grid.push_back(point(i, j));
          }
     }
}

void drawGrid(){
     glColor3f(0, 0, 0);
     int x=-window/2, sz=window/2;
     for(int i=1; i<=row; ++i, x+=cell){
          glBegin(GL_LINES);
               glVertex2i(-sz, x);
               glVertex2i(sz, x);
               glVertex2i(x, -sz);
               glVertex2i(x, sz);
          glEnd();
     }
}

void drawBorder(){
     for(int i=0; i<boundary.size(); ++i){
          drawPoint(boundary[i], color(0, .2, 0), cell);
     }
}

void drawBody(){
     if(body.size()==0){
          body.push_back(point(cell, 0));
          body.push_back(point(0, 0));
          body.push_back(point(-cell, 0));
     }
     point next=point(body[0].x+dx[dir], body[0].y+dy[dir]);
     if(checkPoint(next)) gameOver=1;
     body.insert(body.begin(), next);
     if(body[0].x==food.x && body[0].y==food.y) food=point(-1, -1), score++;
     else body.pop_back();
     for(int i=1; i<body.size(); ++i){  //Body
          drawPoint(body[i], color(.8, .3, 0), cell);
          drawPoint(body[i], color(.8, .6, 0), cell-2);
          drawPoint(body[i], color(0, .6, 0), 2);
     }
     drawPoint(body[0], color(.8, .3, .0), cell);
     drawPoint(body[0], color(.4, .3, 0), cell-4);
     if(dir==0 || dir==1){
          drawPoint(point(body[0].x, body[0].y-5), color(.6, .6, 0), 5);
          drawPoint(point(body[0].x, body[0].y+5), color(.6, .6, 0), 5);
     }
     else{
          drawPoint(point(body[0].x-5, body[0].y), color(.6, .6, 0), 4);
          drawPoint(point(body[0].x+5, body[0].y), color(.6, .6, 0), 4);
     }
}

void key(unsigned char key, int x, int y) {
     switch(key){
          case 'q':
               exit(0);
               break;
          case 'd': //right
               if(changeDir!=1) dir=0;
               break;
          case 'a': //left
               if(changeDir!=0) dir=1;
               break;
          case 'w': //up
               if(changeDir!=3) dir=2;
               break;
          case 's': //down
               if(changeDir!=2) dir=3;
               break;
     }
}

void specialKey(int key, int x, int y) {
     switch(key){
          case GLUT_KEY_RIGHT:
               if(changeDir!=1) dir=0;
               break;
          case GLUT_KEY_LEFT:
               if(changeDir!=0) dir=1;
               break;
          case GLUT_KEY_UP:
               if(changeDir!=3) dir=2;
               break;
          case GLUT_KEY_DOWN:
               if(changeDir!=2) dir=3;
               break;
     }
}

point findFood(int cnt=0){
     srand(time(NULL));
     int x=rand()%grid.size();
     if(cnt>100){
          for(int i=x; i<grid.size(); ++i) if(!checkPoint(grid[i])) return grid[i];
          for(int i=0; i<x; ++i) if(!checkPoint(grid[i])) return grid[i];
     }
     if(!checkPoint(grid[x])) return grid[x];
     else return findFood(cnt+1);
}

int cnt=0;
void drawFood(){
     if(food.x==-1) food=findFood();
     if(cnt%2) drawPoint(food, color(.5, .1, .2), cell);
     else drawPoint(food, color(.2, .1, .1), cell);
     cnt++;
}

string toStr(int n){
     string tmp="";
     tmp+='0'+n/100;
     n=n%100;
     tmp+='0'+n/10;
     tmp+='0'+n%10;
     return tmp;
}

void showScore(){
     glColor3f(1, 1, 1);
     glRasterPos3f(-3*cell, window/2-cell+3, 0);
     string tmp="Your score: "+toStr(score);
     for(int i=0; i<tmp.length(); ++i){
          glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, tmp[i]);
     }
}

void display(){
     changeDir=dir;
     drawGrid();
     drawBorder();
     drawBody();
     drawFood();
     showScore();
     if(gameOver==1){
          gameOver=2;
          glColor3f(1, .2, 0);
          glRasterPos3f(-3*cell, -8, 0);
          string tmp="GAME OVER";
          for(int i=0; i<tmp.length(); ++i){
               glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, tmp[i]);
          }
     }
     glutSwapBuffers();
     glClear(GL_COLOR_BUFFER_BIT);
}

void timer(int x){
     if(gameOver<=1) glutPostRedisplay();
     glutTimerFunc(speed, timer, 0);
}

void init(){
     glClearColor(0.4, 0.8, 0.4, 0);
     glClear(GL_COLOR_BUFFER_BIT);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D(-window/2, window/2, -window/2, window/2);
}

int main(int argc, char** argv){
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
     glutInitWindowSize(window, window);
     glutInitWindowPosition(600, 100);
     glutCreateWindow("Snake Game");
     init();
     setGrid();
     glutDisplayFunc(display);
     glutKeyboardFunc(key);
     glutSpecialFunc(specialKey);
     timer(0);
     glutMainLoop();
     return 0;
}
