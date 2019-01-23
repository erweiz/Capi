//----------------------------
//       Console API v0.1
//----------------------------
//by erweiz
//The project follows the GNU GPLv3 licence.

#include <windows.h>
#include <bits/stdc++.h>
#include <conio.h>

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

#define PERR(bSuccess, api){if(!(bSuccess)) printf("%s:Error %d from %s on line %d\n", __FILE__,GetLastError(), api, __LINE__);}
using namespace std;
COORD ConsoleCursorPosMessage(HANDLE hIn,HANDLE hOut);
void gotoxy(int x,int y);
void init();
HWND GetConsole();
bool MoveConsole(int x,int y,int width,int height,bool isRepaint);
bool CursorVisible(HANDLE hOut,bool isVisible);
bool erase(int x,int y,int width,int height);
void MyCls(HANDLE hConsole);
inline void clrscr();
void start(HANDLE hOut);
void quit();
bool password(HANDLE hOut);
/*   func     */
class T_CON
{
    public:
        string title;
        int x,y,width,height;
};

class T_CON_LABEL:public T_CON
{
    public:
        int push()
        {
            if(this->title.length()<=this->width*2*this->height)
            {
                gotoxy(this->x*2,this->y);
                for(int i=0; i<=this->height-1; i++)
                    for(int j=0; j<=this->width*2-1; j++)
                    {
                        gotoxy(this->x*2+j,this->y+i);
                        if(i*this->width*2+j<=this->title.length())
                            cout<<this->title[i*this->width*2+j];
                        else cout<<" ";
                    }
            }
        }
        int del()
        {
            for(int i=0; i<this->height; i++)
                for(int j=0; j<=this->width*2-1; j++)
                {
                    gotoxy(this->x*2+j,this->y+i);
                    cout<<" ";
                }
        }
        bool isBeClicked(int CurX,int CurY)
        {
            if(CurX>=this->x*2&&CurX<this->x*2+this->width*2&&CurY>=this->y&&CurY<this->y+this->height)
                return true;
            else return false;
        }
} ;
class T_CON_BUTTON:public T_CON
{
    public:
        int push()
        {
            gotoxy(this->x*2,this->y);
            printf("%s","╔");
            for(int i=0; i<this->width-2; i++) printf("%s","══");
            printf("%s","╗");
            for(int i=this->y+1; i<=this->height+this->y-2; i++)
            {
                gotoxy(this->x*2,i);
                printf("║");
                gotoxy(this->x*2+this->width*2-2,i);
                printf("║");
            }
            gotoxy(this->x*2,this->y+this->height-1);
            printf("%s","╚");
            for(int i=0; i<this->width-2; i++) printf("%s","══");
            printf("%s","╝");
            this->push_title();
        }
        int push_title()
        {
            if(title.length()<=this->width*2-4)
            {
                gotoxy(this->x*2+2,this->y+1);
                cout<<this->title;
            }
        }
        int del()
        {
            for(int i=0; i<this->height; i++)
            {
                gotoxy(this->x*2,this->y+i);
                for(int j=0; j<this->width*2-1; j++)
                    printf(" ");
            }
        }
        bool isBeClicked(int CurX,int CurY)
        {
            if(CurX>=this->x*2&&CurX<this->x*2+this->width*2&&CurY>=this->y&&CurY<this->y+this->height)
                return true;
            else return false;
        }
} ;
class T_CON_GROUP:public T_CON
{
    public:
        int push()
        {
            gotoxy(this->x*2,this->y);
            printf("%s","┌");
            for(int i=0; i<this->width-2; i++) printf("%s","──");
            printf("%s","┐");
            for(int i=this->y+1; i<=this->height+this->y-2; i++)
            {
                gotoxy(this->x*2,i);
                printf("│");
                gotoxy(this->x*2+this->width*2-2,i);
                printf("│");
            }
            gotoxy(this->x*2,this->y+this->height-1);
            printf("%s","└");
            for(int i=0; i<this->width-2; i++) printf("%s","──");
            printf("%s","┘");
        }
        int del()
        {
            gotoxy(this->x*2,this->y);
            printf("%s"," ");
            for(int i=0; i<this->width-2; i++) printf("%s"," ");
            printf("%s"," ");
            for(int i=this->y+1; i<=this->height+this->y-2; i++)
            {
                gotoxy(this->x*2,i);
                printf(" ");
                gotoxy(this->x*2+this->width*2-2,i);
                printf(" ");
            }
            gotoxy(this->x*2,this->y+this->height-1);
            printf("%s"," ");
            for(int i=0; i<this->width-2; i++) printf("%s"," ");
            printf("%s"," ");
        }
} ;
class T_CON_MENU:public T_CON
{
    public:
        vector<T_CON_LABEL> Blist;
        vector<T_CON_LABEL>::iterator index;
        bool exist;
        T_CON_MENU()
        {
            this->width=10;
            this->height=1;
            this->exist=false;
        }
        void add(string name)
        {
            T_CON_LABEL newButton;
            newButton.width=8;
            newButton.height=1;
            newButton.title=name;
            Blist.push_back(newButton);
            this->height+=2;
        }
        int push()
        {
            gotoxy(this->x*2,this->y);
            printf("%s","┌");
            for(int i=0; i<this->width-2; i++) printf("%s","──");
            printf("%s","┐");
            for(int i=this->y+1; i<=this->height+this->y-2; i++)
            {
                gotoxy(this->x*2,i);
                printf("│");
                gotoxy(this->x*2+this->width*2-2,i);
                printf("│");
            }
            gotoxy(this->x*2,this->y+this->height-1);
            printf("%s","└");
            for(int i=0; i<this->width-2; i++) printf("%s","──");
            printf("%s","┘");
            this->push_button();
            this->exist=true;
        }
        void push_line(int ind)
        {
            gotoxy(this->x*2+2,this->y+ind*2);
            for(int i=1; i<=this->width-2; i++)
                cout<<"──";
        }
        void push_button()
        {
            int now=0;
            if(!Blist.empty())
            {
                index=Blist.begin();
                while(index!=Blist.end())
                {
                    index->x=this->x+1;
                    index->y=this->y+now*2+1;
                    index->push();
                    index++;
                    now++;
                    this->push_line(now);
                }
            }
        }
        int del()
        {
            for(int i=0; i<this->height; i++)
                for(int j=0; j<=this->width*2-1; j++)
                {
                    gotoxy(this->x*2+j,this->y+i);
                    cout<<" ";
                }
            this->exist=false;
        }
        int Click(int CurX,int CurY)
        {
            int click_button=1;
            index=Blist.begin();
            while(index!=Blist.end())
            {
                if(index->isBeClicked(CurX,CurY)) return click_button;
                index++;
                click_button++;
            }
            return -1;
        }
} m1,m2;
COORD ConsoleCursorPosMessage(HANDLE hIn,HANDLE hOut)
{
    INPUT_RECORD    mouseRec;
    DWORD           res;
    COORD           crPos,crHome = {0, 0};
    ReadConsoleInput(hIn, &mouseRec, 1, &res);
    if (mouseRec.EventType == MOUSE_EVENT)
    {

        if (mouseRec.Event.MouseEvent.dwButtonState==FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            if(m2.exist)
            {
                m2.del();
                switch(m2.Click(crPos.X,crPos.Y))
                {
                    case 1:
                        WinExec("notepad.exe", SW_SHOWNORMAL);
                        break;
                    case 2:
                        WinExec("calc.exe", SW_SHOWNORMAL);
                        break;
                    case 3:
                        WinExec("mspaint.exe", SW_SHOWNORMAL);
                        break;
                    case 4:
                        WinExec("write.exe", SW_SHOWNORMAL);
                        break;

                }
            }
            if(m1.exist)
            {
                m1.del();
                switch(m1.Click(crPos.X,crPos.Y))
                {
                    case 1:
                        m2.x=m1.x;
                        m2.y=m1.y;
                        m2.push();
                        break;
                    case 2:
                        quit();
                        break;
                }
            }

            if (mouseRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
            {

            }
        }
        else if (mouseRec.Event.MouseEvent.dwButtonState==FROM_LEFT_2ND_BUTTON_PRESSED)
        {
            if (mouseRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
            {

            }
        }
        else if (mouseRec.Event.MouseEvent.dwButtonState==RIGHTMOST_BUTTON_PRESSED)
        {
            if(m1.exist)m1.del();
            if(m2.exist)m2.del();
            m1.x=crPos.X/2;
            m1.y=crPos.Y;
            m1.push();
            if (mouseRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
            {

            }
        }
        SetConsoleCursorPosition(hOut,crHome);
        crPos = mouseRec.Event.MouseEvent.dwMousePosition;
    }
    return crPos;
}
void MyCls(HANDLE hConsole)
{
    COORD coordScreen= {0,0};
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    bSuccess=GetConsoleScreenBufferInfo(hConsole,&csbi);
    PERR(bSuccess,"GetConsoleScreenBufferInfo");
    dwConSize=csbi.dwSize.X * csbi.dwSize.Y;
    bSuccess=FillConsoleOutputCharacter(hConsole,(TCHAR)' ',dwConSize,coordScreen,&cCharsWritten);
    PERR(bSuccess,"FillConsoleOutputCharacter");
    bSuccess=GetConsoleScreenBufferInfo(hConsole,&csbi);
    PERR(bSuccess,"ConsoleScreenBufferInfo");
    bSuccess=FillConsoleOutputAttribute(hConsole,csbi.wAttributes,dwConSize,coordScreen,&cCharsWritten);
    PERR(bSuccess,"FillConsoleOutputAttribute");
    bSuccess=SetConsoleCursorPosition(hConsole,coordScreen);
    PERR(bSuccess,"SetConsoleCursorPosition");
    return;
}
inline void clrscr()
{
    HANDLE hStdOut=GetStdHandle(STD_OUTPUT_HANDLE);
    MyCls(hStdOut);
    return;
}
void gotoxy(int x,int y)
{
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE),c);
}
bool erase(int x,int y,int width,int height)
{
    for(int i=0; i<height; i++)
        for(int j=0; j<=width*2-1; j++)
        {
            gotoxy(x*2+j,y+i);
            cout<<" ";
        }
}
HWND GetConsole()
{
    char title[255];
    GetConsoleTitle(title, 255);
    HWND hwnd=FindWindow("ConsoleWindowClass",title);
    return hwnd;
}
bool MoveConsole(int x,int y,int width,int height,bool isRepaint=TRUE)
{
    HWND hwnd=GetConsole();
    MoveWindow(hwnd,x,y,width,height,isRepaint);
}

bool CursorVisible(HANDLE hOut,bool isVisible)
{
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(hOut, &CursorInfo);
    CursorInfo.bVisible = !isVisible;
    SetConsoleCursorInfo(hOut, &CursorInfo);
}
void init()
{
    m1.add("appmenu");
    m1.add("exit");
    m2.add("notepad");
    m2.add("calculator");
    m2.add("paintingboard");
    m2.add("writingboard");
}
void start(HANDLE hOut)
{
    gotoxy(0,0);
    cout<<"--------------\nConsoleAPI\n--------------\n";
    Sleep(300);
    cout<<"v0.1\n\n\n";
    Sleep(700);
    cout<<"加载中";
    for(int i=1; i<=7; i++)
    {
        Sleep(100);
        cout<<".";
    }
    clrscr();
    cout<<"\n\n\n\n";
    cout<<"            :::::   ::::   :::::       ::    :::::   ::::::\n";
    cout<<"           :       :    :  :    :     :  :   ::  ::    ::\n";
    cout<<"           :       :    :  :    :    ::::::  :::::     ::\n";
    cout<<"           :       :    :  :    :    ::  ::  ::        ::\n";
    cout<<"            :::::   ::::   :    :    ::  ::  ::      ::::::\n";
    Sleep(1500);
    cout<<"\n\n\n                   ";
    for(int i=1; i<=14; i++)
    {
        cout<<"█";
        Sleep(50);
    }
    clrscr();
    if(!password(hOut)) exit(0);
}
void quit()
{
    clrscr();
    gotoxy(0,0);
    cout<<"退出中";
    for(int i=1; i<=2; i++)
    {
        gotoxy(7,0);
        for(int j=1; j<=7; j++)
        {
            cout<<".";
            Sleep(100);
        }
        gotoxy(7,0);
        cout<<"        ";
    }
    exit(0);
}
bool password(HANDLE hOut)
{
    string pass;
    gotoxy(0,0);
    cout<<"请输入密码:\n";
    CursorVisible(hOut,FALSE);
    cin>>pass;
    CursorVisible(hOut,TRUE);
    if(pass=="password")
    {
        MessageBox(NULL,"登入!","提示",0);
        return true;
    }
    else
    {
        MessageBox(NULL,"密码错误!","提示",0);
        return false;
    }
    clrscr();
}
int main()
{
    COORD crd;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    CursorVisible(hOut,TRUE);
    SetConsoleTitleA("CAPI");
    init();
    start(hOut);
    clrscr();
    while(true)
        crd=ConsoleCursorPosMessage(hIn,hOut);
    CloseHandle(hOut);
    CloseHandle(hIn);
}
