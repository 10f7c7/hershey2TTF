#include <bits/stdc++.h>

using namespace std;

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
        // }
        cout << endl;
        }
    }
}

bool writeCoordsToFile(int** arr, int r)  {
    int rows = 1;
    int ptAmt = r;
    cout << endl << ptAmt << endl;
    ofstream CoordH("newstroke_coords.h");
    CoordH << ("const int newstroke_coords[%d][650] = {",rows) << endl;
    CoordH << '{';
    for (int i = 0; i<ptAmt; i++)  {
        if (i == ptAmt - 1)  {
            CoordH << arr[i][0] << ", ";
            CoordH << arr[i][1];
            break;
        }
        CoordH << arr[i][0] << ", ";
        CoordH << arr[i][1] << ", ";
    }
    CoordH << '}' << endl;
    CoordH << "};";
    CoordH.close();
}


int main()  {
    const string txt = "I\\WMW^V`UaSbPbNa RWZU[Q[OZNYMWMQNOONQMUMWN";
    const int left = getCoordPoint(txt[0]);
    const int right = getCoordPoint(txt[1]);
    const bool HumanCoord = false;
    int pairNum = 0;

    int** coordPairs;
    int row = (txt.length()/2) - 1;
    int col = 2;
    coordPairs = new int*[row];
    for(int i=0; i<row; i++){
        coordPairs[i] = new int[col];
    }

    for (int i = 2; i < txt.length(); i++)  {
        if (txt[i] == ' ' & txt[i + 1] == 'R')  {
            i += 2;
            coordPairs[pairNum][0] = 'n';
            coordPairs[pairNum++][1] = 'l';
        }

        coordPairs[pairNum][0] = getCoordPoint(txt[i++]) + right;
        coordPairs[pairNum++][1] = -(getCoordPoint(txt[i])) + 9 + 7;

    }

    display(coordPairs, row, col, HumanCoord);

    if (!HumanCoord)  {
        writeCoordsToFile(coordPairs, row);
    }

}