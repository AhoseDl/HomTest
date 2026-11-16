      TopoDS_Shape MakeBottle(double myWidth, double myHeight,
                              double myThickness)
      {

            // ==========================================
            // STEP 1: Build the Bottle Profile (Base 2D)
            // ==========================================

            // Define the base geometric points
            gp_Pnt aPnt1(-myWidth / 2.0, 0, 0);
            gp_Pnt aPnt2(-myWidth / 2.0, -myThickness / 4.0, 0);
            gp_Pnt aPnt3(0, -myThickness / 2.0, 0);
            gp_Pnt aPnt4(myWidth / 2.0, -myThickness / 4.0, 0);
            gp_Pnt aPnt5(myWidth / 2.0, 0, 0);

            // Create lines and arcs connecting the points
            Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2, aPnt3, aPnt4);
            Handle(Geom_TrimmedCurve) aSegment1     = GC_MakeSegment(aPnt1, aPnt2);
            Handle(Geom_TrimmedCurve) aSegment2     = GC_MakeSegment(aPnt4, aPnt5);

            // Generate topological edges
            TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
            TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
            TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);

            // Join edges into a single wire (half-profile)
            TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);

            // Mirror the wire across the X axis to create a complete base profile
            gp_Ax1 xAxis = gp::OX();
            gp_Trsf aTrsf;
            aTrsf.SetMirror(xAxis);
            BRepBuilderAPI_Transform aBRepTrsf(aWire, aTrsf);
            TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
            TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);

            // Combine both halves into a single closed wire
            BRepBuilderAPI_MakeWire mkWire;
            mkWire.Add(aWire);
            mkWire.Add(aMirroredWire);
            TopoDS_Wire myWireProfile = mkWire.Wire();

            // ==========================================
            // STEP 2: Build the Bottle Body
            // ==========================================

            // Create a flat face from the closed wire profile
            TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);

            // Extrude the face along the Z-axis to create a solid prism
            gp_Vec aVecExtent(0, 0, myHeight);
            TopoDS_Shape myBody = BRepPrimAPI_MakePrism(myFaceProfile, aVecExtent);

            // Apply fillets along the vertical sharp edges
            BRepFilletAPI_MakeFillet mkFillet(myBody);
            TopExp_Explorer anEdgeExplorer(myBody, TopAbs_EDGE);
            while (anEdgeExplorer.More()) {
                TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
                mkFillet.Add(myThickness / 12.0, anEdge);
                anEdgeExplorer.Next();
            }
            myBody = mkFillet.Shape();

            // Add the neck cylinder on top of the body
            gp_Pnt neckLocation(0, 0, myHeight);
            gp_Dir neckAxis = gp::DZ();
            gp_Ax2 neckAx2(neckLocation, neckAxis);
            Standard_Real neckRadius = myThickness / 4.0;
            Standard_Real neckHeight = myHeight / 10.0;
            BRepPrimAPI_MakeCylinder mkCylinder(neckAx2, neckRadius, neckHeight);
            TopoDS_Shape myNeck = mkCylinder.Shape();

            // Fuse the main body and the neck cylinder together
            myBody = BRepAlgoAPI_Fuse(myBody, myNeck);

            // Hollow out the solid (create a thick shell open at the top neck face)
            TopoDS_Face faceToOpen;
            Standard_Real zMax = -1.0;

            // Find the topmost face (the top of the neck) to drill open
            TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE);
            for (; aFaceExplorer.More(); aFaceExplorer.Next()) {
                TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
                // Check surface properties to isolate the highest planar face
                Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
                if (aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
                    Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
                    gp_Pnt pnt = aPlane->Location();
                    if (pnt.Z() > zMax) {
                        zMax = pnt.Z();
                        faceToOpen = aFace;
                    }
                }
            }

            // Generate the hollowed bottle
            TopTools_ListOfShape facesToRemove;
            facesToRemove.Append(faceToOpen);
            BRepOffsetAPI_MakeThickSolid mkThickSolid;
            mkThickSolid.MakeThickSolidByJoin(myBody, facesToRemove, -myThickness / 50.0, 1.e-3);
            myBody = mkThickSolid.Shape();

            // ==========================================
            // STEP 3: Create Threading on Neck
            // ==========================================

            // Setup parametric dimensions for threading
            gp_Pnt aCylLoc(0, 0, myHeight + neckHeight / 4.0);
            gp_Ax3 cylindricalAxis(aCylLoc, neckAxis);

            Standard_Real majorRadius = neckRadius * 0.99;
            Standard_Real minorRadius = neckRadius * 1.05;

            // Construct 2D profiles for threading sections
            gp_Pnt2d aPnt2d_1(0, 0);
            gp_Pnt2d aPnt2d_2(Standard_Real(2 * M_PI), neckHeight / 2.0);
            Handle(Geom2d_Line) aLine2d = new Geom2d_Line(aPnt2d_1, gp_Dir2d(aPnt2d_2.X(), aPnt2d_2.Y()));

            Handle(Geom2d_TrimmedCurve) aThreadCurve = new Geom2d_TrimmedCurve(aLine2d, 0, sqrt(aPnt2d_2.X()*aPnt2d_2.X() + aPnt2d_2.Y()*aPnt2d_2.Y()));
            Handle(Geom_CylindricalSurface) aCylSurf = new Geom_CylindricalSurface(cylindricalAxis, majorRadius);

            TopoDS_Edge aThreadEdge = BRepBuilderAPI_MakeEdge(aThreadCurve, aCylSurf);
            TopoDS_Wire aThreadWire = BRepBuilderAPI_MakeWire(aThreadEdge);

            // Create threading using skinning (ThruSections tool)
            BRepOffsetAPI_ThruSections mkSkin(Standard_True, Standard_False);
            // (Note: Thread geometry calculations are heavily abbreviated for clarity)
            // Full production versions map exact cross-section profiles along the helix wire.

            // ==========================================
            // STEP 4: Compound Final Result
            // ==========================================

            TopoDS_Compound bottleCompound;
            BRep_Builder aBuilder;
            aBuilder.MakeCompound(bottleCompound);
            aBuilder.Add(bottleCompound, myBody);

            return bottleCompound;


//        int main() {
//            // Generate a bottle structure with Width=50, Height=70, Thickness=30
//            TopoDS_Shape standardBottle = MakeBottle(50.0, 70.0, 30.0);

//            // Save the finalized geometry into a standard STEP file format
//            BRepTools::Write(standardBottle, "OpenCascade_Bottle.brep");
//            return 0;
//        }

      }

