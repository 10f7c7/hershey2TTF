import re
import os
import string
import shutil
import tempfile
import fontforge
import argparse
from string import Template
from pathlib import Path
from bs4 import BeautifulSoup
from bs4.formatter import XMLFormatter


class Colors:
    OK = '\033[92m'
    INFO = '\033[94m'
    WARN = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'


class SVGProcessor:
    _path = None
    _spool = None
    _font_name = 'IconFont'
    _qml_namespace = 'IconFont'
    _qml_element_name = 'Icon'
    _copyright = '(C) 2020 GONICUS GmbH'
    _out_path = '.'
    _strip_rect = False
    _qt = False

    def __init__(self, source_path, spool):
        self._path = source_path
        self._spool = spool

    def run(self):
        objects = {}
        objects_lt = {}
        index = 61000

        for path in Path(self._path).rglob('*.svg'):
            try:
                svg = BeautifulSoup(open(path).read().encode('utf-8'), 'xml')
            except FileNotFoundError:
                print(f"{Colors.FAIL}✗{Colors.ENDC} file not found {Colors.BOLD}{path}{Colors.ENDC}")
                return

            if self._process(svg, path):
                spool_name = os.path.join(self._spool, f'{index}.svg')
                with open(spool_name, 'w') as f:
                    f.write(svg.prettify(formatter=XMLFormatter()))

                objects[index] = spool_name
                objects_lt[index] = os.path.splitext(str(path)[len(self._path) + 1:])[0]
                index += 1

        # Do font processing
        if self._make_font(objects):
            self._write_header()
            self._write_cpp(objects_lt)
            if self._qt:
                self._write_qml()

    def _write_header(self):
        font_name = self._font_name.upper()
        file_name = self._font_name + '.h'
        if self._qt:
            header = Template("""#ifndef ${FONT_NAME}_H
#define ${FONT_NAME}_H
#include <QObject>
#include <QtQml>
class ${NAME}Resolver : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ${NAME}Resolver(QObject* parent = nullptr);
    virtual ~${NAME}Resolver() {}
    Q_INVOKABLE quint16 indexOfPath(const QString& iconPath);
};
#endif
""")
        else:
            header = Template("""#ifndef ${FONT_NAME}_H
#define ${FONT_NAME}_H
#include <cstdint>
#include <string>
namespace $NAME {
uint16_t index(const std::string& path);
}
#endif
""")

        with open(os.path.join(self._out_path, file_name), 'w') as f:
            f.write(header.substitute(FONT_NAME=font_name, NAME=self._font_name))
            print(f'{Colors.OK}✓{Colors.ENDC} {f.name} has been generated')

    def _write_cpp(self, objects):
        font_name = self._font_name.upper()
        file_name = self._font_name + '.cpp'
        data = '\n'.join(f'               {{ "{name}", {index} }},' for index, name in objects.items())

        if self._qt:
            code = Template("""#include <QFontDatabase>
#include <QHash>
#include "${NAME}.h"
${NAME}Resolver::${NAME}Resolver(QObject* parent) : QObject(parent) {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        QFontDatabase::addApplicationFont(":/${NAME}.ttf");
    }
}
quint16 ${NAME}Resolver::indexOfPath(const QString& iconPath) {
    static QHash<const QString, quint16> lookup_table {
$DATA
    };
    return lookup_table.value(iconPath, 0);
}
""")
        else:
            code = Template("""#include <iostream>
#include <map>
#include "${NAME}.h"
namespace $FONT_NAME {
    uint16_t index(const std::string& path) {
        static std::map<std::string, uint16_t> lookup_table {
$DATA
        };
        auto idx = lookup_table.find(path);
        return idx == lookup_table.end() ? 0 : idx->second;
    }
}
""")

        with open(os.path.join(self._out_path, file_name), 'w') as f:
            f.write(code.substitute(NAME=self._font_name, FONT_NAME=font_name, DATA=data))
            print(f'{Colors.OK}✓{Colors.ENDC} {f.name} has been generated')

    def _write_qml(self):
        font_name = self._font_name.upper()
        file_name = self._font_name + '.qml'
        code = Template("""import QtQuick 2.15
import ${COMPONENT} 1.0 as IconFont
/// Loads and displays an icon of the icon font by giving the path to the icon svg file
Item {
    id: control
    width: icon.implicitWidth
    height: control.size
    /// Path to the icon svg file that should be loaded; empty string (default) unloads the icon
    property string iconPath
    /// Size of the icon in pixels (default: 32)
    property int size: 32
    /// Color of the icon (default: black)
    property alias color: icon.color
    IconFont.${NAME}Resolver {
        id: resolver
    }
    Text {
        id: icon
        text: String.fromCharCode(resolver.indexOfPath(control.iconPath))
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.centerIn: parent
        font.family: "${NAME}"
        font.pixelSize: control.size
    }
}
""")

        with open(os.path.join(self._out_path, self._qml_element_name + ".qml"), 'w') as f:
            f.write(code.substitute(FONT_NAME=font_name, NAME=self._font_name, COMPONENT=self._qml_namespace))
            print(f'{Colors.OK}✓{Colors.ENDC} {f.name} has been generated')

    def _process(self, svg, path):
        # Skip icons that have no square dimensions
        main = svg.find('svg')
        if 'width' in main and 'height' in main:
            if main['width'] != main['height']:
                print(f"{Colors.WARN}⚠{Colors.ENDC} {Colors.BOLD}{path}{Colors.ENDC} aspect ratio is not 1:1 - skipping")
                return False

            # Remove unit from size
            width = int(re.findall(r'\d+', main['width'])[0])
            height = int(re.findall(r'\d+', main['height'])[0])

        # Remove bounding rectangles if any
        if self._strip_rect:
            for rect in svg.find_all('rect'):
                if int(re.findall(r'\d+', rect['height'])[0]) == height and int(re.findall(r'\d+', rect['width'])[0]) == width:
                    rect.extract()

        # Find element
        element = self._findElement(svg)

        # Check if there's no element
        if len(svg.find_all(element)) == 0:
            print(f"{Colors.WARN}⚠{Colors.ENDC} file {Colors.BOLD}{path}{Colors.ENDC} has no relevant elements - skipping")
            return False

        # Check if there's more than one element
        if len(svg.find_all(element)) != 1:
            print(f"{Colors.INFO}🛈{Colors.ENDC} file {Colors.BOLD}{path}{Colors.ENDC} has no too many elements")

        # Skip icons that use a 'rotate'
        if svg.find(element, transform=re.compile('^rotate\(')):
            print(f"{Colors.WARN}⚠{Colors.ENDC} file {Colors.BOLD}{path}{Colors.ENDC} contains rotation - skipping")
            return False

        return True

    def _findElement(self, svg):
        for el in ['path', 'polygon', 'rect', 'circle']:
            if len(svg.find_all(el)) != 0:
                return el

        return None

    def _make_font(self, objects):
        first = True
        font = fontforge.font()
        font.encoding = 'UnicodeFull'
        font.fontname = self._font_name
        font.familyname = self._font_name
        font.fullname = self._font_name
        font.copyright = self._copyright

        for index, path in objects.items():
            if first:
                char = font.createChar(87)
                char.importOutlines(str(path))
                first = False

            char = font.createChar(index)

            try:
                char.importOutlines(str(path))
            except FileNotFoundError:
                print(f"{Colors.FAIL}✗{Colors.ENDC} file not found {Colors.BOLD}{path}{Colors.ENDC}")
                return False

        font.selection.all()

        path = os.path.join(self._out_path, self._font_name + ".ttf")
        font.generate(path)
        print(f'{Colors.OK}✓{Colors.ENDC} {path} has been generated')

        return True

    def __set_font_name(self, name):
        allowed = set(string.ascii_lowercase + string.ascii_uppercase + string.digits + '_')
        if set(name) <= allowed:
            self._font_name = name
        else:
            print(f"{Colors.FAIL}✗{Colors.ENDC} only uppercase/lowercase characters, digits and _ are allowed for the font name")
            exit()

    def __get_font_name(self):
        return self._font_name

    def __set_out_path(self, path):
        self._out_path = path

    def __get_out_path(self):
        return self._out_path

    def __set_copyright(self, data):
        self._copyright = data

    def __get_copyright(self):
        return self._copyright

    def __set_strip_rect(self, data):
        self._strip_rect = data

    def __get_strip_rect(self):
        return self._strip_rect

    def __set_qt(self, data):
        self._qt = data

    def __get_qt(self):
        return self._strip_rect

    def __set_qml_element(self, data):
        self._qml_element_name = data

    def __get_qml_element(self):
        return self._qml_element_name

    def __set_qml_namespace(self, data):
        self._qml_namespace = data

    def __get_qml_namespace(self):
        return self._qml_namespace

    font_name = property(__get_font_name, __set_font_name)
    out = property(__get_out_path, __set_out_path)
    copyright = property(__get_copyright, __set_copyright)
    strip_rect = property(__get_strip_rect, __set_strip_rect)
    qt = property(__get_qt, __set_qt)
    qml_namespace = property(__get_qml_namespace, __set_qml_namespace)
    qml_element = property(__get_qml_element, __set_qml_element)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('source')
    parser.add_argument('--font-name', help='name of the generated font', default='IconFont')
    parser.add_argument('--copyright', help='copyright notice placed inside the generated TTF file', default='(C) 2020 GONICUS GmbH')
    parser.add_argument('--output', help='path where generated files are placed', default='.')
    parser.add_argument('--strip-bounding-rect', action="store_true", help='path where generated files are placed')
    parser.add_argument('--qt', action="store_true", help='whether to build Qt/QML style output files')
    parser.add_argument('--qml-namespace', help='name of the QML namespace used in your .pro file', default='IconApp')
    parser.add_argument('--qml-element', help='name of the QML icon element for this font', default='Icon')
    args = parser.parse_args()

    with tempfile.TemporaryDirectory() as spool:
        processor = SVGProcessor(args.source, spool)
        processor.font_name = args.font_name
        processor.out = args.output
        processor.copyright = args.copyright
        processor.strip_rect = args.strip_bounding_rect
        processor.qt = args.qt
        processor.qml_element = args.qml_element
        processor.qml_namespace = args.qml_namespace
        processor.run()

    del processo