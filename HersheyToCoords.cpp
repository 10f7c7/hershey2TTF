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
        // if (in[x] == 'R' && in[x - 1] == ' ')  {
        //     continue;
        // }
        len++;
    }
    return len;
}

void display(int** a, int r, int c, bool HmPts){
    if (!HmPts)  {
        cout << '[';
        for(int i=0; i<r; i++){
            if (a[i][0] == 'r' & a[i][1] == 'l')  {
                cout << "nL, ";
                break;
            }
            // for(int j=0; j<c; j++){
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
        // for(int j=0; j<c; j++){
            cout << '(';
            cout << a[i][0] << ", ";
            cout << a[i][1] << ")";
        // }
        cout << endl;
        }
    }
}

bool writeCoordsToFile(int** arr)  {

    int ptAmt = sizeof(arr) / sizeof(int);
    ofstream CoordH("newstroke_coords.h");
    CoordH << "const int** newstroke_coords[] = {" << endl;

    for (int i = 0; i<ptAmt; i++)  {
        CoordH << arr[i] << ',' << endl;
    }

    CoordH << "};";
    CoordH.close();
}


int main()  {
    // const string txt = "JZNFNJ RVFVJ";
    const string txt = "I[VZT[P[NZMXMPNNPMTMVNWPWRMT";
    const int left = getCoordPoint(txt[0]);
    const int right = getCoordPoint(txt[1]);
    const bool HumanCoord = true;
    // cout << left << endl;

    // const int pairCount = getLength(txt)/2;


    int pairNum = 0;
    // vector<int> coordPairs(getLength(txt)/2);

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
        // cout << txt[i];
        // if (txt[i] == 'R' && txt[i - 1] == ' ')  {
        //     i++;
        // }
        // int pair[2];

        // pair[0] = getCoordPoint(txt[i++]);
        // pair[1] = getCoordPoint(txt[i++]);

        coordPairs[pairNum][0] = getCoordPoint(txt[i++]) + right;
        coordPairs[pairNum++][1] = -(getCoordPoint(txt[i])) + 9;

    }

    display(coordPairs, row, col, HumanCoord);

    if (!HumanCoord)  {
        writeCoordsToFile(coordPairs);
    }




}