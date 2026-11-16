#ifndef GMSHST
#define GMSHST

#include <vector>
#include <cmath>
#include <gmsh.h>

#include <set>
#include <algorithm>
#include <vector>

// getClosestPoint()
//getClosestEntities()
//getBoundary()
//getAdjacenties()


namespace gm = gmsh::model;

struct SurfaceMatch {
    int tag;
    double dot;        // signed dot(normal, targetDir), unit-normalized
    double planeCoord;  // coordinate of centroid along targetDir axis (only meaningful if targetDir is axis-aligned)
};

struct FacePair {
    int tagLo;
    int tagHi;
    double residual; // mean closest-point distance after translation, should be ~0
};

struct EntityPair
{
    int tagLo;
    int tagHi;
    double geomPrecision=std::numeric_limits<double>::max(); // To check form matching
    double distPrecision=std::numeric_limits<double>::max(); // To check position precision
};

/* Evaluate the surface normal at several sample points in parameter
 space and return the average. Also reports the max deviation between
 samples so non-planar surfaces can be rejected.*/

static bool getAverageNormal(int surfTag, std::vector<double>& avgNormal, double& maxDeviation);



/* Returns surfaces whose (planar) normal is parallel (either sign) to
 targetDir AND whose centroid lies on the overall bounding box in that
 direction (i.e. it's an outer/bounding face, not an internal one).*/
 
std::vector<SurfaceMatch> selectBoundingSurfacesByNormal(
    const std::vector<double>& targetDirIn,
    double angleTolDeg = 2.0,     // normal alignment tolerance
    double planarityTolDeg = 1.0, // max allowed normal variation across the face
    double bboxTolFrac = 1e-4) ;   // fraction of model size used as bbox coincidence tolerance




// Sample a uv grid on a surface and return the XYZ points.

static std::vector<double> samplePoints(int surfTag, int nSamples) ;



/* Translate sampled points of surfTag by (dx,dy,dz), then for each translated
 point find the closest point actually ON candidateTag (via OCC's closest-
 point projection) and return the mean residual distance. A true periodic
 pair gives ~0 regardless of curvature, since we're checking coincidence,
 not orientation.*/
 
static double translationResidual(int surfTag, int candidateTag,
                                   double dx, double dy, double dz, int nSamples) ;


 /*Coarse pre-filter: split all boundary surfaces into a "low" and "high"
 group by projecting each surface's own bounding box center onto the
 translation direction. This is only a speed optimization (avoids running
 the O(n^2) translation-residual check across every surface pair) — the
 actual pairing decision below is entirely translation-based. */

static void splitCandidatesByProjection(double tx, double ty, double tz,
                                        std::vector<int>& loGroup, std::vector<int>& hiGroup);



 /*Finds surface pairs related by translationVec. For each surface in the
 "low" group, tries every surface in the "high" group and keeps the best
 (lowest-residual) match if it's below tolerance. Curvature never enters
 the decision — a cylindrical strut wall pairs correctly with its
 translated twin exactly like a flat face would.*/

std::vector<FacePair> findPeriodicFacePairs(const std::vector<double>& translationVec,
                                            double tolerance = 1e-4,
                                            int nSamples = 5) ;


double computeDistance(const std::vector<double>& p1, const std::vector<double>& p2);

int findEdgeBySamplePoints(const int& tg1, const std::vector<int>& amongEdges, double dx, double y, double dz, int nSamples =5, double tolerance = 1e-6);
//bool findPeriodicEntityPair(const std::vector<int>& entSet1, std::vector<int>& entSet2, double x, double y, double z );

#endif
