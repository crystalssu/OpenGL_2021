#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include<math.h>
#include <stdlib.h>
#include <time.h>

GLfloat Delta = 0.0;
float rad;  
float hit_x, hit_y;   
float DeltaX, DeltaY;     
float yellow_x, yellow_y; 
float yel_moveX, yel_moveY;  
int bump; 
int cou_hit, cou_yel; 
float cou_reduce;    

void initValue() {      
    rad = 20;       

    while (sqrt(pow(hit_x - yellow_x, 2) + pow(hit_y - yellow_y, 2)) < 500) {
        hit_x = ((rand() % 600) - 300);
        hit_y = -1 * (rand() % 400);

        yellow_x = ((rand() % 600) - 300);
        yellow_y = ((rand() % 400));
    }

    
    if (hit_x > yellow_x) {
        DeltaX = -1 * fabs(hit_x - yellow_x) / 20;       
    }
    else {
        DeltaX = fabs(hit_x - yellow_x) / 20;
    }
    DeltaY = fabs(hit_y - yellow_y) / 20;

    yel_moveX = 0;      
    yel_moveY = 0;

    bump = 0;

    cou_hit = 0;
    cou_yel = 0;
    cou_reduce = 1.2;     
}

void MyTimer(int Value) {
    glutPostRedisplay();
    glutTimerFunc(40, MyTimer, 1);      
}

void DrawCircle(float cx, float cy, float r)      
{
    float x1, y1, x2, y2;
    float angle;
    double radius = r;


    x1 = cx;
    y1 = cy;
    glColor3f(1.0, 0.0, 0.0);       

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);

    for (angle = 0.01f; angle <= 2 * 3.14f + 0.2; angle += 0.2)
    {
        x2 = x1 + sin(angle) * radius;
        y2 = y1 + cos(angle) * radius;
        glVertex2f(x2, y2);
    }
    glEnd();
}
void DrawCircle2(float cx, float cy, float r) {       
    float x1, y1, x2, y2;
    float angle;
    double radius = r;


    x1 = cx;
    y1 = cy;
    glColor3f(1.0, 1.0, 0.0);       

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);

    for (angle = 0.01f; angle <= 2 * 3.14f + 0.2; angle += 0.2)
    {
        x2 = x1 + sin(angle) * radius;
        y2 = y1 + cos(angle) * radius;
        glVertex2f(x2, y2);
    }
    glEnd();
}

void MyDisplay() {     
    glViewport(0, 0, 600, 800);
    glClear(GL_COLOR_BUFFER_BIT);

    DrawCircle(hit_x + DeltaX, hit_y + DeltaY, rad);     
    DrawCircle2(yellow_x + yel_moveX, yellow_y + yel_moveY, rad);

    if (sqrt(pow(hit_x - yellow_x, 2) + pow(hit_y - yellow_y, 2)) <= 2 * rad) {     
        if (bump < 1) {       // 두 공이 한번도 부딪혔던 적이 없다면
            DeltaX = (0.5 * hit_x - (-1) * (sqrt(3) / 2) * hit_y) / 20;      // 캐롬 당구의 분리각 법칙에 의한 "부딪힌 후"의 각 당구공의 이동값 업데이트.
            DeltaY = ((-1) * (sqrt(3) / 2) * hit_x + 0.5 * hit_y) / 20;

            yel_moveX = ((sqrt(3) / 2) * yellow_x - 0.5 * yellow_y) / 20;
            yel_moveY = (0.5 * yellow_x + (sqrt(3) / 2) * yellow_y) / 20;
            bump++;     // 부딪힌 횟수 + 1
        }
        else {       // 두 공이 한번이상 부딪힌 경우
            DeltaX = 0;      // 더 이상 이동하지 않고 공이 멈추도록 이동값을 0으로 설정.
            DeltaY = 0;

            yel_moveX = 0;
            yel_moveY = 0;
        }
    }
    hit_x += DeltaX;     // 현재 정의 돼있는 이동값 만큼 수구, 적구를 이동시킴
    hit_y += DeltaY;

    yellow_x += yel_moveX;
    yellow_y += yel_moveY;

    if ((DeltaX < 0 && hit_x <= -300) || (DeltaX > 0 && hit_x >= 300)) {        // 수구가 당구대의 양 옆면에 부딪혔을 경우 ( 쿠션 )
        DeltaX *= -1;    // 기존 이동값에 -1을 곱해줌

        DeltaX /= cou_reduce;
        DeltaY /= cou_reduce;  // 쿠션시 감속
        cou_hit++;
    }
    if ((DeltaY < 0 && hit_y <= -400) || (DeltaY > 0 && hit_y >= 400)) {        // 수구가 당구대의 위, 아랫면에 부딪혔을 경우
        DeltaY *= -1;

        DeltaX /= cou_reduce;
        DeltaY /= cou_reduce;  // 쿠션시 감속
        cou_hit++;
    }
    if ((yel_moveX < 0 && yellow_x <= -300) || (yel_moveX > 0 && yellow_x >= 300)) {        // 적구가 당구대의 양 옆면에 부딪혔을 경우
        yel_moveX *= -1;

        yel_moveX /= cou_reduce;
        yel_moveY /= cou_reduce;  // 쿠션시 감속
        cou_yel++;
    }
    if ((yel_moveY < 0 && yellow_y <= -400) || (yel_moveY > 0 && yellow_y >= 400)) {        // 적구가 당구대의 위, 아랫면에 부딪혔을 경우
        yel_moveY *= -1;

        yel_moveX /= cou_reduce;
        yel_moveY /= cou_reduce;  // 쿠션시 감속
        cou_yel++;
    }

    if (cou_hit >= 2) {       // 수구 2쿠션시 서서히 정지
        DeltaX /= 1.04;
        DeltaY /= 1.04;
    }
    if (cou_yel >= 3) {       // 적구 3쿠션시 서서히 정지
        yel_moveX /= 1.04;
        yel_moveY /= 1.04;
    }

    glutSwapBuffers();      // 두 버퍼를 통째로 교체. flush 쓸 필요없음.
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
    switch (KeyPressed) {
    case 'a': //왼쪽
        DeltaX -= 0.01;
        glutPostRedisplay();
        break;
    case 'd': //오른쪽
        DeltaX += 0.01;
        glutPostRedisplay();
        break;
    case 'w': //위쪽
        DeltaY += 0.01;
        glutPostRedisplay();
        break;
    case 's': //아래쪽
        DeltaY -= 0.01;
        glutPostRedisplay();
        break;
    case 'q':
        exit(0);
        glutPostRedisplay();
        break;
    }

}


int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(nullptr)));    
    initValue();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);        

    glutInitWindowSize(600,800);      
    glutInitWindowPosition(100,0);
    glutCreateWindow("OpenGL Drawing Example");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-300.0, 300.0, -400.0, 400.0, -1.0, 1.0);

    glutDisplayFunc(MyDisplay);    
    glutTimerFunc(40, MyTimer, 1); 
    glutKeyboardFunc(MyKeyboard);
    glutMainLoop();

    return 0;
}