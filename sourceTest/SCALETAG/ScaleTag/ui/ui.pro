

QT += quick qml widgets quickwidgets quickcontrols2

#CONFIG(debug, debug|release) {
#    QMAKE_CXXFLAGS += -fsanitize=address -g -O0
#    QMAKE_LFLAGS += -fsanitize=address
#    LIBS += -fsanitize=address
#}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

QMAKE_RPATHDIR += /opt/OCCT-7_8_0/lin64/gcc/lib
QMAKE_RPATHDIR += /usr/local/lib

LIBS += -L/opt/GMSH/opt/GMSH/lib -lgmsh

SOURCES += \
        source/main.cpp \
        #source/visualization/visualization.cpp \
        source/window/mainwindow.cpp \
        source/window/mainTreeView.cpp \
        source/window/outputhandle.cpp \
        source/window/scaletagdialogs.cpp \
        #source/modesls/bcs.cpp

HEADERS+= source/window/mainwindow.h \
    source/window/mainTreeView.h \
    source/window/outputhandle.h \
 \   # source/visualization/visualization.h
    source/window/scaletagdialogs.h \
    #source/modesls/bcs.h
    #source/frames/heavytask.h


RESOURCES += \
    logos.qrc \
    views.qrc \
    components.qrc \
    assets.qrc \
    graphics.qrc \
    buttons.qrc

INCLUDEPATH +=/usr/local/include/vtk-9.3 \
    source/visualization \
    source \
    source/window \
    $$PWD/../stlib/source/models \
    $$PWD/../stlib/source/ \
    $$PWD/../stlib/source/visualization/ \
    /opt/GMSH/opt/GMSH/include


#LIBS += -L$$PWD/../binaries/linux/icc/x64/debug -lstlib
#LIBS += -L/home/ahose/Desktop/SCALETAG/ScaleTag/stlib/source/models/MumpsUtilities/lib/ -lfemumps -ldmumps -lmumps_common -lpord -lesmumps -lmetis -lscotch -lscotcherr -llapack  -lmpiseq -lblas -lm -lgfortran

#For PETSc
#PETSC_INCLUDES =/opt/petsc/include \
 #   /opt/petsc/arch-linux-c-debug/include \
  #  /usr/lib/x86_64-linux-gnu/include
    #/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/include

PETSC_INCLUDES =/opt/anotherpetsc/petsc-3.24.4/include \
    /opt/anotherpetsc/petsc-3.24.4/arch-linux-c-debug/include \
    /usr/lib/x86_64-linux-gnu/include
    #/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/include


INCLUDEPATH += $$PETSC_INCLUDES
#PETSC_LIBS1 =/opt/petsc/arch-linux-c-debug/lib
PETSC_LIBS1 =/opt/anotherpetsc/petsc-3.24.4/arch-linux-c-debug/lib
LIBS += -L$$PETSC_LIBS1 -lpetsc -llapack -lblas -lm -lX11 -lgfortran -lstdc++ -lquadmath
#PETSC_LIBS2 =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/lib
PETSC_LIBS2 =/usr/lib/x86_64-linux-gnu/
LIBS += -L$$PETSC_LIBS2 -lmpi

SLEPC_INCLUDES =/opt/slepc/arch-linux-c-debug/include
INCLUDEPATH += $$SLEPC_INCLUDES
SLEPC_LIBS =/opt/slepc/arch-linux-c-debug/lib
LIBS += -L$$SLEPC_LIBS -lslepc

#LIBS += /opt/slepc/arch-linux-c-debug/lib/libslepc.so.3.24
#QMAKE_LFLAGS += -Wl,-rpath,/opt/slepc/arch-linux-c-debug/lib

#CONFIG += link_pkgconfig
#PKGCONFIG += PETSc
#QMAKE_CXX =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin/mpicxx
#QMAKE_LINK =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/lib



include($$PWD/../vtklibrarynew.pri)
include($$PWD/../occtlibrary.pri)



#INCLUDEPATH +=/media/ahose/T7/SmallProjectHomo/SomeClasses



include(../qmake-target-platform.pri)
include(../qmake-destination-path.pri)
DESTDIR = $$PWD/../binaries/$$DESTINATION_PATH
OBJECTS_DIR = $$PWD/build/$$DESTINATION_PATH/.obj
MOC_DIR = $$PWD/build/$$DESTINATION_PATH/.moc
RCC_DIR = $$PWD/build/$$DESTINATION_PATH/.qrc
UI_DIR = $$PWD/build/$$DESTINATION_PATH/.ui

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH +=$$PWD \
/usr/local/lib/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=


#QMAKE_CXXFLAGS += -fopenmp
#QMAKE_LFLAGS += -fopenmp
#QMAKE_CXXFLAGS += -O -fopenmp -fPIC -DAdd_
#QMAKE_CXXFLAGS += -O -fopenmp -fPIC -DAdd_
#QMAKE_CXX =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin/mpicxx
#QMAKE_CC =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin/mpicc
QMAKE_CXX =/usr/lib/x86_64-linux-gnu/bin/mpicxx
QMAKE_CC =/usr/lib/x86_64-linux-gnu/bin/mpicc
#QMAKE_CXXFLAGS -= -wd654 -wd1572 -wd411 -wd873 -wd1125 -wd2259 -wd2261 -wd3280 -wd3373
#QMAKE_CXXFLAGS -=-wd654,1572,411,873,1125,2259,2261,3280,3373
#QMAKE_CXXFLAGS -="-wd654,1572,411,873,1125,2259,2261,3280,3373"

#LIBS += -L$$PWD/../binaries/linux/gcc/x64/debug -lstlib

COMPILER_GCC {
        LIBS += -L$$PWD/../binaries/linux/gcc/x64/debug -lstlib
}
COMPILER_ICC {
        LIBS += -L$$PWD/../binaries/linux/icc/x64/debug -lstlib
}
