# Hershey's font to TrueTypeFont

## Overview

Welcome to my Hershey to TTF conversion project. After many minutes searching, I found absolutely no modern program to programmatically convert Hershey's font to TTF. So I made one.

## Installation
(Beware this was only tested on Linux and I could not get it working on windows)

To install just run

    git clone https://github.com/10f7c7/hershey2ttf

and install both `fontforge` and `inkscape`.

## Usage
First you need to download a Hershey font and put it into a header file by default named `newstroke_font.h` and in an array named `const char* const newstroke_font[]`. The array must store the hershey encoding in unicode order starting at `0x20`. Along with that, you need a bufsize var that contains the length of the array in the header file called `const int newstroke_font_bufsize`.

Then to generate the svg coords run

    g++ HersheyToCoords.cpp -o HersheyToCoord 
    ./HersheyToCoords

From that to generate the svgs 

    g++ CoordsToSVG.cpp -o CoordsToSVG
    ./CoordsToSVG

Then to fix the svgs, enter the `svg` folder and run

    bash stroke2path.sh

Now you will have all the fixed svgs in the `svg/_output` folder in a format that fontforge can read. To generate the ttf, go bach to the main dir and run

    fontforge -script SVGToTTF.py

and now you should have a ttf file in the main dir!