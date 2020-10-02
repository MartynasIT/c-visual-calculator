#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif
#include <iostream>
#include <sstream>
#include <tchar.h>
#include <windows.h>
#include "menu.h"
#include <string>
#include <stdio.h>
#include <iomanip>
using namespace std;

/*  Declare Windows procedure  */
int i;
string sym;
 float result = 0; // our result float
 int k = 1;
 int j = 1;
string s [999]; // our string array for saving calculations //
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM); //LRESULT (some int/pointer-like thing) is just the return type. Functions called from "within Windows" just have to be stdcall
HWND TextBox; //A handle to the textbox window.
HWND TextBox2;
HWND TextBox3;
HWND label1, label2;
HWND history;
HANDLE file;
BOOL rez;
LPCSTR text; //An LPCSTR is a 32-bit pointer to a constant null-terminated string
DWORD b_read, b_written;
HANDLE picture;

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("SupaDupa Calc");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd; // main window
                /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON)); //load icon hicon is icon handle
    wincl.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0); //set icon
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);              /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(169,169,169)); // window background color

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("SupaDupa Calc"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           900,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           LoadMenu(NULL, MAKEINTRESOURCE(IDR_MYMENU)), //load menu
           hThisInstance,       /* Program Instance handler */
           NULL
                            /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
// LPCTSTR = Long Pointer to a Const TCHAR STRing

            bool LoadBMPIntoDC (HWND hwnd, LPCTSTR bmpfile ){ //A device context (DC) is a structure that defines a set of graphic objects and their associated attributes
    HDC hDC; // handle to device context
    hDC = GetDC(hwnd);
     if ( ( NULL == hDC  ) || ( NULL == bmpfile ) ){
          MessageBox(hwnd, "No parameters", "Error", MB_OK | MB_ICONERROR);
          return false;
     }
	HANDLE hBmp = LoadImage ( NULL, bmpfile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );


	if ( NULL == hBmp ){
         MessageBox(hwnd, "No pic", "Error", MB_OK | MB_ICONERROR); // if loaded image is null there is no pic
         return false;
    }

	HDC dcmem = CreateCompatibleDC( NULL ); // The CreateCompatibleDC function creates a memory device context (DC) compatible with the specified device.

	if ( NULL == SelectObject ( dcmem, hBmp ) ){
         MessageBox(hwnd, "Nepavyko ikrauti", "Klaida", MB_OK | MB_ICONERROR);
         DeleteDC ( dcmem );
         return false;
	}

	// and blit it to the visible dc
	if ( BitBlt ( hDC, 400, 200, 100, 200, dcmem, 0, 0, SRCCOPY ) == 0 ) //transfer of the color data corresponding to a rectangle of pixels from the specified source device context into a destination device context.
	{
		DeleteDC ( dcmem );
		return false;
	}
	DeleteDC ( dcmem );  // clear up the memory dc
	return true;
}



/*  This function is called by the Windows function DispatchMessage()  */
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM); // declaration of dialog function

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)

{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE: //Sent when an application requests that a window be created


            label1 = CreateWindow("static", "ST_U",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                              600, 240, 120, 40,
                              hwnd, (HMENU)(501),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL); // Hinstance is A handle to an instance. This is the base address of the module in memory.
                              SetWindowText(label1, "Draw a cool image:");

            label2 = CreateWindow("static", "ST_U",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                              10, 420, 60, 40,
                              hwnd, (HMENU)(501),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
                              SetWindowText(label2, "Answer:");


             TextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
                               WS_CHILD | WS_VISIBLE, 30, 20, 180,
                               40, hwnd, NULL, NULL, NULL);

             TextBox2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
                               WS_CHILD | WS_VISIBLE, 30, 220, 180,
                               40, hwnd, NULL, NULL, NULL);

                               TextBox3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("0"),
                               WS_CHILD | WS_VISIBLE | ES_READONLY, 80, 420, 230,
                               40, hwnd, NULL, NULL, NULL);

            history = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
                               WS_CHILD | WS_VISIBLE | ES_READONLY, 300, 20, 560,
                               90, hwnd, NULL, NULL, NULL);


            CreateWindow(
                "BUTTON",  // Predefined class; Unicode assumed
                "+",      // Button text
                WS_TABSTOP | WS_VISIBLE | WS_CHILD |  BS_DEFPUSHBUTTON,  // Styles
                10,         // x position
                80,         // y position
                100,        // Button width
                50,        // Button height
                hwnd,     // Parent window
                (HMENU) BUTTON_1,       // No menu.
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);      // Pointer not needed.

           CreateWindow(
                "BUTTON",  // Predefined class; Unicode assumed
                "-",      // Button text
                WS_TABSTOP | WS_VISIBLE | WS_CHILD |  BS_DEFPUSHBUTTON,  // Styles
                120,         // x position
                80,         // y position
                100,        // Button width
                50,        // Button height
                hwnd,     // Parent window
                (HMENU) BUTTON_2,       // No menu.
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);      // Pointer not needed.
            CreateWindow(
                "BUTTON",  // Predefined class; Unicode assumed
                "*",      // Button text
                WS_TABSTOP | WS_VISIBLE | WS_CHILD |  BS_DEFPUSHBUTTON,  // Styles
                10,         // x position
                150,         // y position
                100,        // Button width
                50,        // Button height
                hwnd,     // Parent window
                (HMENU) BUTTON_3,       // No menu.
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);      // Pointer not needed.
            CreateWindow(
                "BUTTON",  // Predefined class; Unicode assumed
                "/",      // Button text
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
                120,         // x position
                150,         // y position
                100,        // Button width
                50,        // Button height
                hwnd,     // Parent window
                (HMENU) BUTTON_4,       // No menu.
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);
                  CreateWindow(
                "BUTTON",  // Predefined class; Unicode assumed
                "=",      // Button text
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
                65,         // x position
                270,         // y position
                100,        // Button width
                50,        // Button height
                hwnd,     // Parent window
                (HMENU) BUTTON_5,       // No menu.
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);

                  CreateWindow(
                "BUTTON",  // Predefined class; Unicode assumed
                "C",      // Button text
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
                65,         // x position
                340,         // y position
                100,        // Button width
                50,        // Button height
                hwnd,     // Parent window
                (HMENU) BUTTON_6,       // No menu.
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);

                 CreateWindow(
                "BUTTON",  // Predefined class; Unicode assumed
                "Clear",      // Button text
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
                550,         // x position
                140,         // y position
                100,        // Button width
                50,        // Button height
                hwnd,     // Parent window
                (HMENU) BUTTON_7,       // No menu.
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);



          CreateWindow("BUTTON", TEXT("Pic 1"),
                    BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
                    600, 300, 100, 20,
                    hwnd, (HMENU) BUTTON_8, GetModuleHandle(NULL), NULL);

                      CreateWindow("BUTTON", TEXT("Pic 2"),
                    BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
                    600, 340, 100, 20,
                    hwnd, (HMENU) BUTTON_9, GetModuleHandle(NULL), NULL);

            break;



        case WM_COMMAND: //Sent when the user selects a command item from a menu
            switch(LOWORD(wParam)){


                case ID_FILE_SAVE: //when menu item save clicked


                     file = CreateFile("output.txt", GENERIC_WRITE,  // load file into file object
                                        0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (file != INVALID_HANDLE_VALUE){
                        // nothing for now
                    }
                    else {
                        MessageBox(hwnd, "Something is wrong", "File", MB_OK);
                    }
                    while (j != 0)
                  {

                   text =  s [j].c_str(); // The pointer returned points to the internal array currently used by the string object to store the characters that conform its value.
                    b_written = 0; // DWORD
                    rez = WriteFile(file, text, strlen(text), &b_written, NULL); // failas, pointeris i teksta, irasymo dydis, kiek baitu irasyta
                    rez = WriteFile(file, "\r\n", strlen("\r\n"), &b_written, NULL); // irasome newlina

                    j--;
                  }
                    if (rez){
                        MessageBox(hwnd, "Success", "Writing", MB_OK);
                    }
                    else{
                        MessageBox(hwnd, "Text not written or written async", "File", MB_OK);
                    }

                    CloseHandle(file);


                        break;


                 case ID_FILE_LOAD: //load saved calc data
                     {



                    file = CreateFile("output.txt", GENERIC_READ,
                                        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (file != INVALID_HANDLE_VALUE){
                        //nothing so far
                    }
                    else {
                        MessageBox(hwnd, "Something is wrong", "File", MB_OK);
                    }
                    CHAR texta[2048] = {0}; // our char space for data
                    b_read = 0;
                    rez = ReadFile(file, texta, 2048, &b_read, NULL);
                    if (rez){
                         SendMessage(history, EM_SETSEL, -1, -1);
                         SendMessage(history, EM_REPLACESEL, 0, (LPARAM)texta);
                    }
                    else{
                        MessageBox(hwnd, "Seems like there is nothing to read", "File", MB_OK);
                    }

                    CloseHandle(file);

                     }
                     break;

                     case ID_FILE_CLEAR:

                         DialogBox( NULL, MAKEINTRESOURCE(MYDIALOG), hwnd, DialogProc );

                     break;

                case BUTTON_7:
                    SetWindowText(history,"" );

                        break;

                case BUTTON_1:
                    sym = "+";


                    break;
                case BUTTON_2:
                    sym = "-";

                    break;
                case BUTTON_3:
                   sym = "*";
                    break;
                case BUTTON_4:
                    sym = "/";
                    break;


                case BUTTON_8:
                    {
                        LoadBMPIntoDC(hwnd, "C:\\Users\\Martin\\Desktop\\c++\\projektas\\cool.bmp");
                    }



                    break;

                    case BUTTON_9:


                LoadBMPIntoDC(hwnd, "C:\\Users\\Martin\\Desktop\\c++\\projektas\\verycool.bmp");



                    break;


                    case BUTTON_6: // when clear clicked

                    SetWindowText(TextBox,"" );
                    SetWindowText(TextBox2,"" );
                    SetWindowText(TextBox3,"0" );
                    result = 0;



                    break;



                     case BUTTON_5: // when = button clicked

                           int len = GetWindowTextLength(TextBox) + 1;
                            char* text = new char[len];
                            GetWindowText(TextBox, &text[0], len);
                                        //SetWindowText(TextBox,text );
                            int len2 = GetWindowTextLength(TextBox2) + 1;
                            char* text2 = new char[len2];
                            GetWindowText(TextBox2, &text2[0], len2);

                         float f1=atof(text); //Convert string to double
                            float f2=atof(text2);

                            if (sym == "*")
                           result = f1*f2;

                          else if (sym == "/")
                           result = f1/f2;

                           else if (sym == "+")
                           result = f1+f2;

                           else if (sym == "-")
                           result = f1-f2;

                           ostringstream os;
                           os <<  f1<< " " << sym <<" " << f2 << " =" << " " << result << ";  " ;

                           s[k]  = os.str();

                         SendMessage(history, EM_SETSEL, -1, -1);
                         SendMessage(history, EM_REPLACESEL, 0, (LPARAM)s[k].c_str()); // pointer in array

                              char str[20] = {0};
                              sprintf(str, "%f", result); //Write formatted data to string, we use float format
                              char *w = str;
                              LPCSTR q = w; //An LPCSTR is a 32-bit pointer to a constant null-terminated string

                            SetWindowText(TextBox3,q );
                            k++;
                            j=k;

                    break;
            }
            break;



        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }



    return 0;
}
BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) // our function with dealing with dialogs
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case DIALOG_BUTTON: // if clicked yes
                  {
                     _unlink("output.txt"); // deletes without sending to bin

                  }
                     case DIALOG_BUTTON2: // if no we just terminate dialog
                         {
                             EndDialog(hDlg, 0);
                         }
                    return TRUE;
            }

	    return TRUE;
        case WM_CLOSE:
            return TRUE;
        case WM_DESTROY:
            EndDialog(hDlg, 0);
            return TRUE;
    }
    return FALSE;
}

