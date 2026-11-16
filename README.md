# Some notes on scaletag

## Introduction

The main idea is to design a robust and high-performance softwares which computes effective (homogenized) properties of a periodic domain.

### What is specifically added

1. Unlike most commercial and non commercial softwares which are based on a third-party component(script, python), to achieve this computing, SCALETAG covers every step and the user does not need any other script or software to perform the computing.

2. SCALETAG algorithm can be used on any kind of CAD geometry(curved surface and straight surfaces) provided periodicity properties are met.

3. Mapping corner nodes and surface nodes is quite straightforward.
The mapping of edge nodes can be tricky for many algorithms but SCALETAG provides an original solution (algorithm) to deal automatically with the edge nodes. This has helped to build the platform in such a way that it can solve every step of the computing of the CAD geomery to the effective properties even for curved surface geometries.

### Tools used in the project

- For meshing : `GMSH`.
- Solvers : `PETSC` and `MUMPS`
- CAD : `Opencascade`
- Visualization : `VTK`
- General environment : `c++`, `QT`, etc.

![mesh](./scaletag1.png)

<!--For VTK, the reader  may refer to [VTK  website](https://vtk.org/doc/nightly/release/9.7/html/index.html)-->

For typical visualization of displacement field on the Kelvin cell :

![Kelvin](./kelvin.png)


Another field visualization on the Kelvin cell :


![Kelvin2](./kelvin2.png)
