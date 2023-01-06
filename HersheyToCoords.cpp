#include <bits/stdc++.h>
#include "./newstroke_font.h"

using namespace std;

int pointsLeft = newstroke_font_bufsize;


int getCoordPoint(char in)  {
    return int(in) - 'R';
}

int getLength(string in)  {
    int len = 0;
    for (int x = 0; x < in.length(); x++) {
        if (in[x] == ' ' & in[x + 1] == 'R')  {
            len -= 2;
        }
        len++;
    }
    return len;
}

void display(int** a, int r, int c, bool HmPts){
    if (!HmPts)  {
        cout << '[';
        for(int i=0; i<r; i++){
            cout << '[';
            cout << a[i][0] << ", ";
            cout << a[i][1] << "]";
            if (i != r-1)  {
                cout << ", ";
            }

            // }
            // cout << endl;
        }
        cout << ']';
    }
    if (HmPts) {
        for(int i=0; i<r; i++){
            cout << '(';
            cout << a[i][0] << ", ";
            cout << a[i][1] << ")";
        cout << endl;
        }
    }
}

// bool writeCoordsToFile(int*** arr, int r)  {
//     int rows = 1;
//     // int ptAmt = sizeof(arr)/sizeof(arr[0]);
//     // cout << endl << ptAmt << endl;
//     int he = 101;
//     int* heh = &he;
//     int** hehe = &heh;
//     ofstream CoordH("newstroke_coords.h");
//     CoordH << "const int newstroke_coords[" << r << "][650] = {" << endl;
//     // CoordH << '{';
//     for (int j = 0; j<r; j++)  {
//         // if (arr[j] == hehe)  {
//         //     CoordH << "{{5, 7}, {5, 28}, {19, 28}, {19, 7}, {5, 7}}, ";
//         //     break;
//         // }
//         CoordH << '{';
//         // int ptAmt = sizeof(arr[j])/sizeof(int);
//         for (int i = 0; i<650; i++)  {
//             // if (i == ptAmt - 1)  {
//             //     CoordH << arr[j][i][0] << ", ";
//             //     CoordH << arr[j][i][1];
//             //     break;
//             // }
//             if (arr[j][i][0] == 0)  {
//                 break;
//             }
//             CoordH << arr[j][i][0] << ", ";
//             CoordH << arr[j][i][1] << ", ";
//         }
//         CoordH << "}, "  << endl;
//         // if (j != r-1)  {
//         //     CoordH << ',';
//         // }
//         // CoordH << endl;
//     }
//     // CoordH << '}' << endl;
//     CoordH << "};";
//     CoordH.close();
// }


bool addCoordsToFile(ofstream& CoordH, int** arr, int pt)  {
    if (pt == 0)  {
        CoordH << "const int newstroke_coords[" << newstroke_font_bufsize << "][300][2] = {" << endl;
    }
    CoordH << '{';
    for (int i = 0; i < 300; i++)  {
        CoordH << "{" << int(arr[i][0]) << ", " << int(arr[i][1]) << "},";
    }
    CoordH << "}," << endl;
    if (pt == newstroke_font_bufsize-1)  {
        CoordH << "};" << endl << endl << "const int newstroke_coords_bufsize = sizeof(newstroke_coords)/sizeof(newstroke_coords[0]);";
        return true;
    }
    return false;
}


int **doTheThing(string txt) {
    // cout << txt << endl;
    const int left = getCoordPoint(txt[0]);
    const int right = getCoordPoint(txt[1]);
    const bool HumanCoord = false;
    int pairNum = 0;

    int** coordPairs;
    int row = 300;
    int col = 2;
    coordPairs = new int*[row];
    for(int i=0; i<row; i++){
        coordPairs[i] = new int[col];
    }
    for (int i = 2; i < 300; i+=2)  {
        if (i >= txt.length())  {
            coordPairs[pairNum][0] = 0;
            coordPairs[pairNum++][1] = 0;
        } else if (i < txt.length())  {

            if (txt[i] == ' ' & txt[i + 1] == 'R')  {
                coordPairs[pairNum][0] = 'n';
                coordPairs[pairNum++][1] = 'l';
                continue;
            }
            // if (i == 0)  {
            //     coordPairs[pairNum][0] = getCoordPoint(txt[i]) + right;
            //     coordPairs[pairNum++][1] = getCoordPoint(txt[i+1]) + right;
            //     continue;
            // }
            coordPairs[pairNum][0] = getCoordPoint(txt[i]) + right;
            coordPairs[pairNum++][1] = -(getCoordPoint(txt[i+1])) + 9 + 7;
        }
    }
    return coordPairs;

}


int main()  {
    ofstream CoordH("newstroke_coords.h");

    for (int i = 0; i < newstroke_font_bufsize; i++)  {

        bool prcs = addCoordsToFile(CoordH, doTheThing(newstroke_font[i]), i);
        if (prcs)  {
            CoordH.close();
            cout << "done";
        }
    }
    return 0;
}