
#include "glad.h"
#include "glfw3.h"
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

const char* vertexShaderSource = R"(
#version 330 core
layout(location=0) in vec2 aPos;
void main(){ gl_Position=vec4(aPos,0.0,1.0); }
)";

const char* fragmentShaderSource = R"(
#version 330 core
uniform vec3 objectColor;
out vec4 FragColor;
void main(){ FragColor=vec4(objectColor,1.0); }
)";

unsigned int shaderProgram, VAO, VBO;
int colorLocation;

void drawRect(float x,float y,float w,float h,
              float r,float g,float b)
{
    float v[]={x,y,x+w,y,x+w,y+h,x,y,x+w,y+h,x,y+h};
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(v),v);
    glUniform3f(colorLocation,r,g,b);
    glDrawArrays(GL_TRIANGLES,0,6);
}

struct CarRect { float x,y,w,h; };

bool checkCollision(CarRect a,CarRect b)
{
    return a.x<b.x+b.w && a.x+a.w>b.x &&
           a.y<b.y+b.h && a.y+a.h>b.y;
}

bool intersectionOccupied(float cr,float cl,float cu,float cd)
{
    CarRect center={-0.22f,-0.22f,0.44f,0.44f};
    CarRect cars[]={
        {cr,-0.15f,0.11f,0.05f},
        {cl,0.10f,0.11f,0.05f},
        {0.10f,cu,0.05f,0.11f},
        {-0.15f,cd,0.05f,0.11f}
    };

    for(auto car:cars)
        if(checkCollision(center,car)) return true;

    return false;
}

void drawTrafficLight(float x,float y,bool green)
{
    drawRect(x,y,0.055f,0.17f,0.08f,0.08f,0.08f);
    drawRect(x+0.012f,y+0.12f,0.031f,0.025f,
             green?0.30f:1.0f,0.05f,0.05f);
    drawRect(x+0.012f,y+0.075f,0.031f,0.025f,
             0.30f,0.25f,0.02f);
    drawRect(x+0.012f,y+0.03f,0.031f,0.025f,
             green?0.05f:0.02f,green?1.0f:0.25f,
             green?0.15f:0.04f);
}

void drawCar(float x,float y,bool horizontal,
             float r,float g,float b)
{
    if(horizontal)
    {
        drawRect(x,y,0.11f,0.05f,r,g,b);
        drawRect(x+0.035f,y+0.01f,0.035f,0.03f,
                 0.65f,0.85f,1.0f);
    }
    else
    {
        drawRect(x,y,0.05f,0.11f,r,g,b);
        drawRect(x+0.01f,y+0.04f,0.03f,0.035f,
                 0.65f,0.85f,1.0f);
    }
}

void drawPedestrian(float x,float y)
{
    drawRect(x+0.01f,y+0.035f,0.02f,0.02f,1,0.75f,0.5f);
    drawRect(x+0.015f,y+0.005f,0.01f,0.03f,0,0,1);
    drawRect(x,y-0.015f,0.01f,0.02f,0,0,0);
    drawRect(x+0.025f,y-0.015f,0.01f,0.02f,0,0,0);
}

// Simple 5x7 font for the Game Over screen and score.
const char** getGlyph(char c)
{
    static const char* A[]={"01110","10001","10001","11111","10001","10001","10001"};
    static const char* C[]={"11111","10000","10000","10000","10000","10000","11111"};
    static const char* E[]={"11111","10000","10000","11110","10000","10000","11111"};
    static const char* G[]={"01111","10000","10000","10111","10001","10001","01110"};
    static const char* M[]={"10001","11011","10101","10101","10001","10001","10001"};
    static const char* O[]={"01110","10001","10001","10001","10001","10001","01110"};
    static const char* P[]={"11110","10001","10001","11110","10000","10000","10000"};
    static const char* R[]={"11110","10001","10001","11110","10100","10010","10001"};
    static const char* S[]={"01111","10000","10000","01110","00001","00001","11110"};
    static const char* V[]={"10001","10001","10001","10001","10001","01010","00100"};

    static const char* N0[]={"01110","10001","10011","10101","11001","10001","01110"};
    static const char* N1[]={"00100","01100","00100","00100","00100","00100","01110"};
    static const char* N2[]={"01110","10001","00001","00010","00100","01000","11111"};
    static const char* N3[]={"11110","00001","00001","01110","00001","00001","11110"};
    static const char* N4[]={"00010","00110","01010","10010","11111","00010","00010"};
    static const char* N5[]={"11111","10000","10000","11110","00001","00001","11110"};
    static const char* N6[]={"01111","10000","10000","11110","10001","10001","01110"};
    static const char* N7[]={"11111","00001","00010","00100","01000","01000","01000"};
    static const char* N8[]={"01110","10001","10001","01110","10001","10001","01110"};
    static const char* N9[]={"01110","10001","10001","01111","00001","00001","11110"};
    static const char* DASH[]={"00000","00000","00000","11111","00000","00000","00000"};

    switch(c)
    {
        case 'A': return A;
        case 'C': return C;
        case 'E': return E;
        case 'G': return G;
        case 'M': return M;
        case 'O': return O;
        case 'P': return P;
        case 'R': return R;
        case 'S': return S;
        case 'V': return V;
        case '0': return N0;
        case '1': return N1;
        case '2': return N2;
        case '3': return N3;
        case '4': return N4;
        case '5': return N5;
        case '6': return N6;
        case '7': return N7;
        case '8': return N8;
        case '9': return N9;
        case '-': return DASH;
        default: return nullptr;
    }
}

void drawText(string text,float x,float y,float size)
{
    for(char c:text)
    {
        const char** p=getGlyph(c);

        if(p)
        {
            for(int row=0;row<7;row++)
                for(int col=0;col<5;col++)
                    if(p[row][col]=='1')
                        drawRect(x+col*size,y-row*size-size,
                                 size*0.8f,size*0.8f,1,1,1);
        }

        x+=size*6;
    }
}

void drawScorePanel(int score)
{
    drawRect(-0.96f,0.72f,0.48f,0.25f,0.05f,0.05f,0.05f);
    drawText("SCORE",-0.91f,0.92f,0.012f);
    drawText(to_string(score),-0.91f,0.82f,0.014f);
}

void restartGame(float &cr,float &cl,float &cu,float &cd,
                 float &pr,float &pl,float &pu,float &pd,
                 bool &rp,bool &lp,bool &up,bool &dp,
                 float &px,bool &requested,bool &active,
                 bool &horizontal,int &score,int &collisions,
                 bool &collisionActive,float &jam,float &penalty)
{
    cr=cu=-1.1f;
    cl=cd=1.1f;
    pr=cr; pl=cl; pu=cu; pd=cd;

    rp=lp=up=dp=false;
    px=-0.45f;
    requested=active=false;
    horizontal=true;

    score=collisions=0;
    collisionActive=false;
    jam=penalty=0;
}

int main()
{
    if(!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window=glfwCreateWindow(
        900,700,"Signal Master - Smart Traffic Control",NULL,NULL);

    if(!window){ glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout<<"GLAD loading failed!"<<endl;
        glfwTerminate();
        return -1;
    }

    unsigned int vs=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vertexShaderSource,NULL);
    glCompileShader(vs);

    unsigned int fs=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fragmentShaderSource,NULL);
    glCompileShader(fs);

    shaderProgram=glCreateProgram();
    glAttachShader(shaderProgram,vs);
    glAttachShader(shaderProgram,fs);
    glLinkProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,12*sizeof(float),NULL,GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    colorLocation=glGetUniformLocation(shaderProgram,"objectColor");

    float cr=-1.1f,cl=1.1f,cu=-1.1f,cd=1.1f;
    float pr=cr,pl=cl,pu=cu,pd=cd;

    bool rp=false,lp=false,up=false,dp=false;
    float px=-0.45f;
    bool requested=false,active=false;
    bool horizontal=true;

    bool key1Was=false,key2Was=false,keyPWas=false,keyRWas=false;
    bool gameOver=false,collisionActive=false;

    int score=0,collisions=0;
    float jam=0,penalty=0;
    float lastTime=(float)glfwGetTime();

    const float speed=0.28f,pedSpeed=0.20f;
    const float rightStop=-0.365f,leftStop=0.255f;
    const float upStop=-0.365f,downStop=0.255f;

    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
            glfwSetWindowShouldClose(window,true);

        float now=(float)glfwGetTime();
        float dt=min(now-lastTime,0.05f);
        lastTime=now;

        bool k1=glfwGetKey(window,GLFW_KEY_1)==GLFW_PRESS;
        bool k2=glfwGetKey(window,GLFW_KEY_2)==GLFW_PRESS;
        bool kp=glfwGetKey(window,GLFW_KEY_P)==GLFW_PRESS;
        bool kr=glfwGetKey(window,GLFW_KEY_R)==GLFW_PRESS;

        if(kr && !keyRWas && gameOver)
        {
            restartGame(cr,cl,cu,cd,pr,pl,pu,pd,
                        rp,lp,up,dp,px,requested,active,
                        horizontal,score,collisions,collisionActive,
                        jam,penalty);
            gameOver=false;
            lastTime=(float)glfwGetTime();
        }
        keyRWas=kr;

        if(!gameOver)
        {
            if(k1 && !key1Was) horizontal=true;
            if(k2 && !key2Was) horizontal=false;
            if(kp && !keyPWas) requested=true;
        }

        key1Was=k1;
        key2Was=k2;
        keyPWas=kp;

        bool vertical=!horizontal;

        if(!gameOver)
        {
            if(requested && !active &&
               !intersectionOccupied(cr,cl,cu,cd))
            {
                active=true;
                requested=false;
                px=-0.45f;
            }

           
float d = speed * dt;

if(!active)
{
    // Right-moving car
    if(horizontal || rp)
    {
        cr += d;
        if(cr >= rightStop) rp = true;
    }
    else
        cr = min(cr + d, rightStop);

    // Left-moving car
    if(horizontal || lp)
    {
        cl -= d;
        if(cl <= leftStop) lp = true;
    }
    else
        cl = max(cl - d, leftStop);

    // Up-moving car
    if(vertical || up)
    {
        cu += d;
        if(cu >= upStop) up = true;
    }
    else
        cu = min(cu + d, upStop);

    // Down-moving car
    if(vertical || dp)
    {
        cd -= d;
        if(cd <= downStop) dp = true;
    }
    else
        cd = max(cd - d, downStop);
}

            if(active)
            {
                px+=pedSpeed*dt;
                if(px>0.45f)
                {
                    px=-0.45f;
                    active=false;
                }
            }

            CarRect cars[]={
                {cr,-0.15f,0.11f,0.05f},
                {cl,0.10f,0.11f,0.05f},
                {0.10f,cu,0.05f,0.11f},
                {-0.15f,cd,0.05f,0.11f}
            };

            bool hit=false;
            for(int i=0;i<4;i++)
                for(int j=i+1;j<4;j++)
                    if(checkCollision(cars[i],cars[j])) hit=true;

            if(hit && !collisionActive)
            {
                score-=25;
                collisions++;
            }
            collisionActive=hit;

            if(pr<=0.22f && cr>0.22f) score+=10;
            if(pl>=-0.33f && cl< -0.33f) score+=10;
            if(pu<=0.22f && cu>0.22f) score+=10;
            if(pd>=-0.33f && cd< -0.33f) score+=10;

            pr=cr; pl=cl; pu=cu; pd=cd;

            if(intersectionOccupied(cr,cl,cu,cd))
            {
                jam+=dt;
                if(jam>3.0f)
                {
                    penalty+=dt;
                    if(penalty>=1.0f)
                    {
                        score--;
                        penalty-=1.0f;
                    }
                }
            }
            else jam=penalty=0;

            if(cr>1.1f){ cr=-1.1f; rp=false; pr=cr; }
            if(cl< -1.1f){ cl=1.1f; lp=false; pl=cl; }
            if(cu>1.1f){ cu=-1.1f; up=false; pu=cu; }
            if(cd< -1.1f){ cd=1.1f; dp=false; pd=cd; }

            if(score<=-100 || collisions>=3)
                gameOver=true;
        }

        glClearColor(0.20f,0.48f,0.20f,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        drawScorePanel(score);

        // Roads
        drawRect(-1,-0.22f,2,0.44f,0.20f,0.21f,0.23f);
        drawRect(-0.22f,-1,0.44f,2,0.20f,0.21f,0.23f);

        // Road markings
        for(float x=-0.95f;x<0.95f;x+=0.15f)
            if(x< -0.28f || x>0.28f)
                drawRect(x,-0.008f,0.07f,0.016f,0.95f,0.95f,0.85f);

        for(float y=-0.95f;y<0.95f;y+=0.15f)
            if(y< -0.28f || y>0.28f)
                drawRect(-0.008f,y,0.016f,0.07f,0.95f,0.95f,0.85f);

        // Stop lines
        drawRect(-0.22f,0.29f,0.44f,0.018f,1,1,1);
        drawRect(-0.22f,-0.31f,0.44f,0.018f,1,1,1);
        drawRect(0.29f,-0.22f,0.018f,0.44f,1,1,1);
        drawRect(-0.31f,-0.22f,0.018f,0.44f,1,1,1);

        // Traffic lights
        drawTrafficLight(-0.34f,0.30f,horizontal && !active);
        drawTrafficLight(0.28f,0.30f,vertical && !active);
        drawTrafficLight(-0.34f,-0.47f,vertical && !active);
        drawTrafficLight(0.28f,-0.47f,horizontal && !active);

        // Cars
        drawCar(cr,-0.15f,true,0.95f,0.15f,0.12f);
        drawCar(cl,0.10f,true,0.15f,0.35f,1.0f);
        drawCar(0.10f,cu,false,1.0f,0.8f,0.1f);
        drawCar(-0.15f,cd,false,0.7f,0.2f,0.9f);

        if(requested || active) drawPedestrian(px,0.25f);

        // Game Over screen
        if(gameOver)
        {
            drawRect(-0.55f,-0.50f,1.10f,1.00f,0.035f,0.035f,0.045f);
            drawRect(-0.55f,0.43f,1.10f,0.07f,0.90f,0.05f,0.05f);
            drawRect(-0.35f,0.27f,0.70f,0.008f,0.70f,0.10f,0.10f);

            drawText("GAME OVER",-0.378f,0.20f,0.014f);

            drawRect(-0.30f,-0.10f,0.60f,0.25f,0.12f,0.12f,0.14f);
            drawText("SCORE",-0.21f,0.07f,0.014f);

            string result=to_string(score);
            float sx=-result.length()*0.042f;
            drawText(result,sx,-0.01f,0.014f);

            drawRect(-0.30f,-0.34f,0.60f,0.13f,0.55f,0.06f,0.06f);
            drawText("PRESS R",-0.294f,-0.25f,0.014f);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
