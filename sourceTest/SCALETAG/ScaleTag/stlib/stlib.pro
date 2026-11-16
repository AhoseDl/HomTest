QT -= gui
QT += quick \
    widgets \
    quickwidgets \
    qml

TEMPLATE = lib
DEFINES += STLIB_LIBRARY

CONFIG += c++17
QMAKE_CXXFLAGS += -Wall -Wextra -Wno-unused-parameter
QMAKE_CFLAGS += -Wall -Wextra -Wno-unused-parameter
#CONFIG += warn_on

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#CONFIG(debug, debug|release) {
#    QMAKE_CXXFLAGS += -fsanitize=address -g -O0
#    QMAKE_LFLAGS += -fsanitize=address
#    LIBS += -fsanitize=address
#}

RESOURCES +=$$PWD/../ui/views.qrc
SOURCES += \
    source/controllers/mainwindowcontroller.cpp \
    source/controllers/solaris.cpp \
    source/frames/iobjectFactory.cpp \
    source/frames/model.cpp \
    source/frames/objectFactory.cpp \
    source/frames/heavytask.cc \
    source/frames/part.cpp \
    source/frames/project.cpp \
    source/models/pbcutilities.cpp \
    source/models/physics.cc \
    source/models/stlib.cpp \
    source/models/element.cpp \
    source/models/elementtype.cc \
    source/models/node.cpp \
    source/models/MeshPbc.cpp \
    source/models/fem.cpp \
    source/models/bcs.cpp \
    source/models/pbcanalysis.cpp \
    source/models/pbcFunctions.cpp \
#    source/models/petscutilities.cpp \
#    source/models/petscutilities2.cpp \
    source/visualization/cad.cpp \
    source/visualization/visualization.cpp \
    source/visualization/scaletagviewport.cpp \
    source/visualization/generatedataforvis.cpp \
    source/frames/workdirectory.cpp \
    source/models/taoanalysis.cpp \
#    source/models/MumpsUtilities/source/SolverAssembledNonSym.c \
#    source/models/MumpsUtilities/source/SolverAssembledSym.c \
#    source/models/MumpsUtilities/source/SolverElementalNonSym.c \
#    source/models/MumpsUtilities/source/SolverElementalSym.c
   # $$PWD/../ui/source/visualization/visualization.cpp
     source/models/pbcmeshtools.cc \
     source/models/gmshtools.cc \
     source/models/masterpbc.cc

HEADERS += \
    source/controllers/mainwindowcontroller.h \
    source/controllers/solaris.h \
    source/frames/iobjectFactory.h \
    source/frames/model.h \
    source/frames/objectFactory.h \
    source/frames/heavytask.h \
    source/frames/part.h \
    source/frames/project.h \
    source/models/pbcutilities.h \
    source/models/physics.h \
    source/models/stlib_global.h \
    source/models/stlib.h \
    source/models/element.h \
    source/models/elementtype.h \
    source/models/node.h \
    source/models/MeshPbc.h \
    source/models/fem.h \
    source/models/femPbc.h \
    source/models/bcs.h \
    source/models/pbcanalysis.h \
    source/models/pbcanalysis.tpp \
    source/models/matrix.h \
    source/models/pbcFunctions.h \
 #   source/models/petscutilities.h \
#    source/models/petscutilities2.h \
    source/visualization/cad.h \
    source/visualization/visualization.h \
    source/visualization/scaletagviewport.h \
    source/visualization/generatedataforvis.h \
    source/frames/workdirectory.h \
    source/models/taoanalysis.h \
#    source/models/solver.h \
#    source/models/MumpsUtilities/includes/FEMumps.h
    source/models/pbcmeshtools.h \
    source/models/gmshtools.h \
    source/models/masterpbc.h


INCLUDEPATH += source \
                /usr/local/include/vtk-9.3 \
                 source/models \
                 $$PWD/../ui/source \
                source/models/MumpsUtilities/includes \
                source/visualization/ \
                /opt/GMSH/opt/GMSH/include


#LIBS += -L/home/ahose/Desktop/SCALETAG/ScaleTag/stlib/source/models/MumpsUtilities/lib/ -lfemumps -ldmumps -lmumps_common -lpord -lesmumps -lmetis -lscotch -lscotcherr -llapack  -lmpiseq -lblas -lm -lgfortran

#For PETSc
#PETSC_INCLUDES =/opt/petsc/include \
#    /opt/petsc/arch-linux-c-debug/include \
#    /usr/lib/x86_64-linux-gnu/include
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

LIBS += -L$$PETSC_LIBS2 -lmpi -lgomp#-lmpicxx
LIBS += -L/opt/GMSH/opt/GMSH/lib -lgmsh


SLEPC_INCLUDES =/opt/slepc/arch-linux-c-debug/include
SLEPC_LIBS=/opt/slepc/arch-linux-c-debug/lib
INCLUDEPATH += $$SLEPC_INCLUDES
LIBS += -L$$SLEPC_LIBS -lslepc

#CONFIG += link_pkgconfig
#PKGCONFIG += PETSc
#QMAKE_CXX =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin/mpicxx
#QMAKE_LINK =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/lib

#QMAKE_CXX =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin/mpicxx
#QMAKE_CC =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin/mpicc

QMAKE_CXX =/usr/lib/x86_64-linux-gnu/bin/mpicxx
QMAKE_CC =/usr/lib/x86_64-linux-gnu/bin/mpicc
#QMAKE_CXXFLAGS -= -wd654 -wd1572 -wd411 -wd873 -wd1125 -wd2259 -wd2261 -wd3280 -wd3373
#QMAKE_CXXFLAGS -=-wd654,1572,411,873,1125,2259,2261,3280,3373
#QMAKE_CXXFLAGS -="-wd654,1572,411,873,1125,2259,2261,3280,3373"

include($$PWD/../vtklibrarynew.pri)
include($$PWD/../occtlibrary.pri)
#include($$PWD/../solver.pri)


include(../qmake-target-platform.pri)
include(../qmake-destination-path.pri)
DESTDIR = $$PWD/../binaries/$$DESTINATION_PATH
OBJECTS_DIR = $$PWD/build/$$DESTINATION_PATH/.obj
MOC_DIR = $$PWD/build/$$DESTINATION_PATH/.moc
RCC_DIR = $$PWD/build/$$DESTINATION_PATH/.qrc
UI_DIR = $$PWD/build/$$DESTINATION_PATH/.ui



QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

QMAKE_CXXFLAGS += -O -fopenmp -fPIC -DAdd_
QMAKE_CXXFLAGS += -O -fopenmp -fPIC -DAdd_

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
