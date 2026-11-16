TEMPLATE = subdirs

CONFIG += c++17

SUBDIRS += \
    ui \
    stlib
#    stlib \
#    ui

SLEPC_INCLUDES =/opt/slepc/arch-linux-c-debug/include
INCLUDEPATH += $$SLEPC_INCLUDES
SLEPC_LIBS =/opt/slepc/arch-linux-c-debug/lib
LIBS += -L$$SLEPC_LIBS -lslepc

#QMAKE_CXXFLAGS += -fopenmp
#QMAKE_LFLAGS += -fopenmp

#QMAKE_CXX =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin/mpicxx
#QMAKE_CC =/media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin/mpicc
#QMAKE_CXXFLAGS -=-wd654,1572,411,873,1125,2259,2261,3280,3373
#QMAKE_CXXFLAGS -= -wd654 -wd1572 -wd411 -wd873 -wd1125 -wd2259 -wd2261 -wd3280 -wd3373
#QMAKE_CXXFLAGS -="-wd654,1572,411,873,1125,2259,2261,3280,3373"

## Remove any MSVC-only flags if building with g++
#contains(QMAKE_CXX, g++) {
#    QMAKE_CXXFLAGS = $$replace(QMAKE_CXXFLAGS, "-wd[0-9,]*", "")
#}


