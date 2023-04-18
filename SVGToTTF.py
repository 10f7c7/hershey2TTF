import glob
import fontforge
import re 

font = fontforge.font()

font.fontname = "NewStroke"
files = glob.glob("svg/_output/*.svg")

for file in files:
    # print(file[14:16])
    # int(result.group(1), 16)
    result = re.search('svg/_output/0x(.*).svg', file)
    print(int(result.group(1), 16))
    char = font.createChar(int(result.group(1), 16))
    char.importOutlines(file)
fontforge.activeFont().selection.all()
font.autoWidth(150,minBearing=10, maxBearing=300)

fontforge.activeFont().selection.select(" ")
font.autoWidth(400,minBearing=400, maxBearing=400)

font.generate('NewStroke.ttf')