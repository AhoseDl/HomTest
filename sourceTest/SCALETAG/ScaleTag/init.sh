#This script initiates variables for scaletag compiling and run


#For gcc
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ahose/Desktop/SCALETAG/ScaleTag/binaries/linux/gcc/x64/debug/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/Qt/6.5.0/gcc_64/lib/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/petsc/arch-linux-c-debug/lib


# For icc

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ahose/Desktop/SCALETAG/ScaleTag/binaries/linux/icc/x64/debug/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/Qt/6.5.0/gcc_64/lib/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/petsc/arch-linux-c-debug/lib


#using the another version for gcc

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ahose/Desktop/SCALETAG/ScaleTag/binaries/linux/gcc/x64/debug/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/anotherpetsc/petsc-3.24.4

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/anotherpetsc/petsc-3.24.4/arch-linux-c-debug/lib




#some qt project conf env variables
icc /media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin
icpc /media/ahose/InternalSsd/ONEAPI/ONEAPI_BASE/mpi/2021.11/bin
