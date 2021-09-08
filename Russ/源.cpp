#include   <conio.h>
#include   <stdio.h>
#include  <stdlib.h>
#include    <time.h>
#include <Windows.h>
#pragma warning(disable:4996)

/**********初始化参数************/
int i, j, N, T, F, J, X, Y, dx, dy, KEY_V, Cache1, Cache2, NU, NI, RU, RI, P_X, P_Y, POS_Y_MAX, LEVEL = 1, SCORE = 0, P[4], POINT_V[12][22], MARK[21], FLAG[5] = { 0,0,0,1,0 };
int TGM[7][4] = { {0x159D,0x89AB,0x159D,0x89AB},{0x126A,0x4856,0x159A,0x4526},{0x926A,0x456A,0x1592,0x0456},{0x4859,0x4859,0x4859,0x4859},{0x5926,0x0156,0x5926,0x0156},{0x4159,0x4596,0x1596,0x4156},{0x156A,0x4152,0x156A,0x4152} };
int SRS[7][4] = { {0x159D,0x89AB,0x26AE,0x4567},{0x0159,0x4856,0x159A,0x4526},{0x8159,0x456A,0x1592,0x0456},{0x4859,0x4859,0x4859,0x4859},{0x4815,0x459A,0x5926,0x0156},{0x4159,0x4596,0x1596,0x4156},{0x0459,0x8596,0x156A,0x4152} };

/**********光标位置函数**********/
void Pos(int x, int y)
{
    COORD pos;
    HANDLE hOutput;
    pos.X = 2 * x;
    pos.Y = y;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput, pos);
}

void HideCursor()
{
    CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

/**********初始化界面************/
void CreatUI()
{
    int i, j, BOUNDARY;
    printf                          ("┏━━━━━━━━━━━━━━━━━━━━━┓\n");
    for (j = 1; j <= 20; j++) {
                if (j == 3) { printf("┃　　　　　　　　　 　┃ LEVEL：1\n"); }
           else if (j == 5) { printf("┃　　　　　　　　　 　┃ SCORE：0\n"); }
           else if (j == 7) { printf("┃　　　　　　　　　 　┃ NEXT   \n"); }
                       else { printf("┃　　　　　　　　　 　┃ \n"); }
    }
                              printf("┗━━━━━━━━━━━━━━━━━━━━━┛\n");
    printf(" 睿智的唯物主义者\n");
    for (j = 1; j <= 21; j++) {
        for (i = 0; i <= 11; i++) {
            BOUNDARY = i * (i - 11) * (j - 21);
            if (BOUNDARY == 0) {
                POINT_V[i][j] = 1;
            }
            else {
                POINT_V[i][j] = 0;
            }
        }
    }
}

/**********按键获取**************/
int Getkey(int N, int T)
{
    int start = clock();
    if (KEY_V == 115) { return 115; }
    do {
        if (kbhit()) {
            KEY_V = (int)(getch());
            if (KEY_V < 97) { KEY_V += 32; }
            return KEY_V;
        }
        for (i = 0; i <= N; i++);
    } while ((clock() - start) < T);
    dy = 1;
    return -1;
}

/***********块体转置*************/
int Rote(int S, int I)
{
    return (F == 0) ? TGM[S][(I + 4) % 4] : SRS[S][(I + 4) % 4];
}

/***********擦除显示*************/
int Display(int x, int y, int CAC, int Mode)
{
    for (j = 0; j <= 3; j++) {
        P[j] = CAC & 0xF, CAC >>= 4;
        if (Mode == 1) { Pos((P[j] >> 2) + x, (P[j] & 0x3) + y); printf("■"); }
        else if (Mode == 0) { Pos((P[j] >> 2) + x, (P[j] & 0x3) + y); printf("　"); }
    }
    return 0;
}

/***********固化块体*************/
int DoBlocks()
{
    //~~~游戏结束
    if (Y < 2) {
        Pos(1, 22); printf("GAME OVER！");
        exit(0);
    }
    //~~~固化块体
    POS_Y_MAX = 0, FLAG[3] = 1;
    for (j = 0; j <= 3; j++) {
        P_X = (P[j] >> 2) + X, P_Y = (P[j] & 0x3) + Y;
        if (POS_Y_MAX < P_Y) { POS_Y_MAX = P_Y; }
        POINT_V[P_X][P_Y] = 1;
    }
    //~~~关卡得分
    for (j = Y; j <= POS_Y_MAX; j++) {
        FLAG[2] = 1;
        for (i = 1; i <= 10; i++) {
            if (POINT_V[i][j] == 0) { FLAG[2] = 0; }
        }
        if (FLAG[2]) {
            SCORE += 10, MARK[j] = 1;
            if (SCORE == 400) {
                SCORE = 0, LEVEL += 1, T -= 100;
                FLAG[4] = 1;
            }
        }
    }
    //~~~极品消行
    for (j = 20; j >= 5; j--) {
        if (FLAG[4]) {
            for (i = 1; i <= 10; i++) {
                POINT_V[i][j] = 0;
                Pos(i, j); printf("　");
            }
        }
        else if (MARK[j])
        {
            MARK[j] = 0, J = j - 1;
            for (N = 1; N <= 3; N++) {
                if (MARK[J]) { J--; }
            }
            MARK[J] = 1;
            for (i = 1; i <= 10; i++) {
                Pos(i, j);
                if (POINT_V[i][j] = POINT_V[i][J]) {
                    printf("■");
                }
                else {
                    printf("　");
                }
            }
        }
    }
    FLAG[4] = 0;
    return 0;
}

/***********碰撞检测*************/
int CheckCollision()
{
    for (j = 0; j <= 3; j++) {
        P_X = (P[j] >> 2) + X + dx, P_Y = (P[j] & 0x3) + Y + dy;
        if (POINT_V[P_X][P_Y]) {
            if (dx != 0) { return 1; }
            if (dy) {
                DoBlocks();
                Pos(12, 3); printf("LEVEL：%-3d", LEVEL);
                Pos(12, 5); printf("SCORE：%-3d", SCORE);
                return 2;
            }
            if (KEY_V == 119) { FLAG[0] = 1; }
        }
    }
    return 0;
}

/***********循环核心*************/
int GameCycle(int N, int T, int F)
{
    srand((unsigned)time(NULL)); RU = rand() % 7, RI = (rand() % 4);
    while (1) {
        if (FLAG[3]) {
            Display(12, 8, Rote(RU, RI), 0);
            X = 4, Y = 1, NU = RU, NI = RI, RU = rand() % 7, RI = (rand() % 4), FLAG[3] = 0, KEY_V = 0;
            Display(12, 8, Rote(RU, RI), 1);
            Display(X, Y, Rote(NU, NI), 1);
        }
        dx = 0, dy = 0;
        KEY_V = Getkey(N, T);
        if (KEY_V == 119) {
            NI++;
            Display(X, Y, Rote(NU, NI), 2);
        }//旋W
        else if (KEY_V == 115) { dy = 1; }//下S
        else if (KEY_V == 97) { dx = -1; }//左A
        else if (KEY_V == 100) { dx = 1; }//右D
        else if (KEY_V == 112) { getch(); }//暂停P
        else if (KEY_V == 113) { return 0; }//退出Q
        if (dx != 0 || dy != 0 || KEY_V == 119) {
            if (!CheckCollision()) {
                if (FLAG[0]) {
                    NI--, FLAG[0] = 0;
                    Display(X, Y, Rote(NU, NI), 0);
                }
                else if (KEY_V == 119) {
                    Display(X, Y, Rote(NU, NI - 1), 0);
                }
                else {
                    Display(X, Y, Rote(NU, NI), 0);
                }
                Display(X + dx, Y + dy, Rote(NU, NI), 1);
                X += dx, Y += dy;
            }
        }
    }
    return 0;
}

/**********Main主函数***********/
int main()
{
    system("color F0&mode con cols=35 lines=25");
    HideCursor();
    CreatUI();
    GameCycle(10, 800, 1);
    return 0;
   
}