#include <bits/stdc++.h>
#include "newstroke_coords.h"

using namespace std;

int writeToSVG(int** coords, int nameInt)  {

    nameInt += 32;
    ostringstream ss;

    ss << "0x" << hex << nameInt;
    string name = ss.str();

    ofstream SVG("svg/" + name + ".svg");
    SVG << "<svg viewBox='0 0 30 30' xmlns='http://www.w3.org/2000/svg'>" << endl;
    for (int i = 0; i < newstroke_coords_bufsize - 1; i++)  {
        if (coords[i][0] == 110 && coords[i][1] == 108)  {
            i++;
        }
        int c = i+1;
        if (c > newstroke_coords_bufsize)  {
            c = 0;
        }
        if (coords[c][0] == 110 && coords[c][1] == 108)  {
            i++;
        }
        SVG << "<circle cx='" << coords[i][0] << "' cy='" << coords[i][1] << "' r='1.26' />";
        SVG << "<line x1='" << coords[i][0] << "' y1='" << coords[i][1] << "' x2='" << coords[c][0] << "' y2='" << coords[c][1] << "' stroke='black' stroke-width='2.52'/>" << endl;
    }
    SVG << "</svg>";
    SVG.close();
}


int main()  {
    // const int coords[][2] = {{17, 17},{16, 18},{14, 19},{12, 19},{10, 18},{9, 17},{8, 15},{8, 13},{9, 11},{10, 10},{12, 9},{14, 9},{16, 10},{17, 11},{110, 108},{17, 19},{17, 11},{18, 10},{19, 10},{21, 11},{22, 13},{22, 18},{20, 21},{17, 23},{13, 24},{9, 23},{6, 21},{4, 18},{3, 14},{4, 10},{6, 7},{9, 5},{13, 4},{17, 5},{20, 7}};
    newstroke_coords[1];
    // const int newstroke_coords_bufsizes = sizeof(newstroke_coords)/sizeof(newstroke_coords[0]);
// for (int j = 0; j < sizeof(newstroke_coords)/sizeof(newstroke_coords[0]); j++){
for (int j = 0; j < 100; j++){
    // int j = 33;
    const int coordss = sizeof(newstroke_coords[j])/sizeof(newstroke_coords[j][0]);
    ostringstream ss;
    // int usingg = 65 - 32;
    int minn = 0;
    int maxx = 0;
    for (int i = 0; i < coordss; i++)  {
        if (newstroke_coords[j][i][0] == 110 && newstroke_coords[j][i][1] == 108)  {
            continue;
        }
        if (newstroke_coords[j][i][0] == 0 && newstroke_coords[j][i][1] == 0)  {
            continue;
        }
        if (newstroke_coords[j][i][0] < minn)  {
            minn = newstroke_coords[j][i][0];
        }
        if (newstroke_coords[j][i][0] > maxx)  {
            maxx = newstroke_coords[j][i][0];
        }
    }
    int offset = 14 - ((minn + maxx)/2);
    // offset = 14 - offset;
    int usingg = j;
    ss << "0x" << hex << j + 32;
    string name = ss.str();
    ofstream SVG("svg/"+name+".svg");
    cout << name << endl;
    SVG << "<svg viewBox='0 0 30 35' xmlns='http://www.w3.org/2000/svg'>" << endl;
    bool done = false;
    for (int i = 0; i < coordss - 1; i++)  {
        if (done) continue;
        if(newstroke_coords[usingg][i][0] != 0 && newstroke_coords[usingg][i][1] != 0) {SVG << "<circle cx='" << newstroke_coords[usingg][i][0] + offset << "' cy='" << newstroke_coords[usingg][i][1] << "' r='1.26' />";}
        if(newstroke_coords[usingg][i+1][0] == 0 && newstroke_coords[usingg][i+1][1] == 0)  {
            done = true;
            continue;
            // return 0;
        }
        // cout << i << ' ' << newstroke_coords[usingg][i+1][0] << ' ' << newstroke_coords[usingg][i+1][1] << ' ' << newstroke_coords[usingg][i][0] << ' ' << newstroke_coords[usingg][i][1] << endl;
        if (newstroke_coords[usingg][i][0] == 110 && newstroke_coords[usingg][i][1] == 108)  {
            i++;
        }
        int c = i+1;
        if (c > coordss)  {
            c = 0;
        }
        if (newstroke_coords[usingg][c][0] == 110 && newstroke_coords[usingg][c][1] == 108)  {
            i++;
        }
        SVG << "<line x1='" << newstroke_coords[usingg][i][0] + offset << "' y1='" << newstroke_coords[usingg][i][1] << "' x2='" << newstroke_coords[usingg][c][0] + offset << "' y2='" << newstroke_coords[usingg][c][1] << "' stroke='black' stroke-width='2.52'/>" << endl;
    }
            SVG << "</svg>";
            SVG.close();
    // SVG.close();
}



    // for (int i = 0; i < newstroke_coords_bufsize; i++)  {
        // writeToSVG(newstroke_coords[1], 1);
    // }



}