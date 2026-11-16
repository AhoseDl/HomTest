#include<gmshtools.h>



static bool getAverageNormal(int surfTag, std::vector<double>& avgNormal, double& maxDeviation)
{
// Evaluate the surface normal at several sample points in parameter
// space and return the average. Also reports the max deviation between
// samples so non-planar surfaces can be rejected.

    std::vector<double> pmin, pmax;
    gm::getParametrizationBounds(2, surfTag, pmin, pmax);

    // Sample a small interior grid (avoid exact parametric edges/seams).
    const int nSamples = 3;
    std::vector<double> uvCoords;
    for (int i = 1; i <= nSamples; ++i) {
        for (int j = 1; j <= nSamples; ++j) {
            double fu = static_cast<double>(i) / (nSamples + 1);
            double fv = static_cast<double>(j) / (nSamples + 1);
            uvCoords.push_back(pmin[0] + fu * (pmax[0] - pmin[0]));
            uvCoords.push_back(pmin[1] + fv * (pmax[1] - pmin[1]));
        }
    }

    std::vector<double> normals;
    gm::getNormal(surfTag, uvCoords, normals);
    if (normals.empty()) return false;

    avgNormal = {0.0, 0.0, 0.0};
    size_t nPts = normals.size() / 3;
    for (size_t k = 0; k < nPts; ++k) {
        avgNormal[0] += normals[3*k + 0];
        avgNormal[1] += normals[3*k + 1];
        avgNormal[2] += normals[3*k + 2];
    }
    double norm = std::sqrt(avgNormal[0]*avgNormal[0] + avgNormal[1]*avgNormal[1] + avgNormal[2]*avgNormal[2]);
    if (norm < 1e-12) return false;
    for (double& c : avgNormal) c /= norm;

    // Check consistency (planarity): deviation of each sample normal from the average.
    maxDeviation = 0.0;
    for (size_t k = 0; k < nPts; ++k) {
        double dx = normals[3*k+0], dy = normals[3*k+1], dz = normals[3*k+2];
        double n = std::sqrt(dx*dx + dy*dy + dz*dz);
        if (n < 1e-12) continue;
        dx /= n; dy /= n; dz /= n;
        double dot = dx*avgNormal[0] + dy*avgNormal[1] + dz*avgNormal[2];
        double dev = std::acos(std::max(-1.0, std::min(1.0, dot)));
        maxDeviation = std::max(maxDeviation, dev);
    }
    return true;
}



std::vector<SurfaceMatch> selectBoundingSurfacesByNormal(


// Returns surfaces whose (planar) normal is parallel (either sign) to
// targetDir AND whose centroid lies on the overall bounding box in that
// direction (i.e. it's an outer/bounding face, not an internal one).


    const std::vector<double>& targetDirIn,
    double angleTolDeg ,     // normal alignment tolerance
    double planarityTolDeg , // max allowed normal variation across the face
    double bboxTolFrac)    // fraction of model size used as bbox coincidence tolerance
{
    double tx = targetDirIn[0], ty = targetDirIn[1], tz = targetDirIn[2];
    double tn = std::sqrt(tx*tx + ty*ty + tz*tz);
    tx /= tn; ty /= tn; tz /= tn;

    // Overall model bounding box.
    double xmin, ymin, zmin, xmax, ymax, zmax;
    gm::getBoundingBox(-1, -1, xmin, ymin, zmin, xmax, ymax, zmax);
    double diag = std::sqrt((xmax-xmin)*(xmax-xmin) + (ymax-ymin)*(ymax-ymin) + (zmax-zmin)*(zmax-zmin));
    double bboxTol = bboxTolFrac * diag;

    double cosAngleTol = std::cos(angleTolDeg * M_PI / 180.0);
    double planarityTolRad = planarityTolDeg * M_PI / 180.0;

    std::vector<std::pair<int,int>> allSurfaces;
    gm::getEntities(allSurfaces, 2);

    std::vector<SurfaceMatch> matches;
    for (auto& dimTag : allSurfaces) {
        int tag = dimTag.second;

        std::vector<double> normal;
        double maxDeviation;
        if (!getAverageNormal(tag, normal, maxDeviation)) continue;
        if (maxDeviation > planarityTolRad) continue; // not (sufficiently) planar

        double dot = normal[0]*tx + normal[1]*ty + normal[2]*tz;
        if (std::fabs(dot) < cosAngleTol) continue; // not aligned with target direction

        // Centroid (mass center) of the face.
        std::vector<double> com{3};
        gm::occ::getCenterOfMass(2, tag, com[0], com[1], com[2]);
        if (com.empty()) continue;
        double centroidCoord = com[0]*tx + com[1]*ty + com[2]*tz;

        // Project bbox corners onto targetDir to get the extreme bounding coordinate.
        double bboxProj[8];
        double xs[2] = {xmin, xmax}, ys[2] = {ymin, ymax}, zs[2] = {zmin, zmax};
        int idx = 0;
        for (double x : xs) for (double y : ys) for (double z : zs)
            bboxProj[idx++] = x*tx + y*ty + z*tz;
        double projMin = bboxProj[0], projMax = bboxProj[0];
        for (double p : bboxProj) { projMin = std::min(projMin, p); projMax = std::max(projMax, p); }

        bool onBoundingPlane = (std::fabs(centroidCoord - projMin) < bboxTol) ||
                               (std::fabs(centroidCoord - projMax) < bboxTol);
        if (!onBoundingPlane) continue; // internal face (e.g. hole wall) — skip

        matches.push_back({tag, dot, centroidCoord});
    }
    return matches;
}



static std::vector<double> samplePoints(int surfTag, int nSamples)
{

// Sample a uv grid on a surface and return the XYZ points.
    std::vector<double> pmin, pmax;
    gm::getParametrizationBounds(2, surfTag, pmin, pmax);

    std::vector<double> uvCoords;
    for (int i = 0; i <= nSamples; ++i) {
        for (int j = 0; j <= nSamples; ++j) {
            double fu = static_cast<double>(i) / nSamples;
            double fv = static_cast<double>(j) / nSamples;
            uvCoords.push_back(pmin[0] + fu * (pmax[0] - pmin[0]));
            uvCoords.push_back(pmin[1] + fv * (pmax[1] - pmin[1]));
        }
    }
    std::vector<double> xyz;
    gm::getValue(2, surfTag, uvCoords, xyz);
    return xyz;
}

static std::vector<double> sampleEdgePoints(int edgeTag, int nSamples)
{

    // Sample points referenced by curvilinear coordinate u on an edge and return the XYZ points.
    std::vector<double> pmin, pmax;
    gm::getParametrizationBounds(1, edgeTag, pmin, pmax);

    std::vector<double> uCoords;
    for (int i = 0; i <= nSamples; ++i) {
            double fu = static_cast<double>(i) / nSamples;
            uCoords.push_back(pmin[0] + fu * (pmax[0] - pmin[0]));
    }
    std::vector<double> xyz;
    gm::getValue(1, edgeTag, uCoords, xyz);
    return xyz;
}

static double translationResidual(int surfTag, int candidateTag,
                                   double dx, double dy, double dz, int nSamples)
{

// Translate sampled points of surfTag by (dx,dy,dz), then for each translated
// point find the closest point actually ON candidateTag (via OCC's closest-
// point projection) and return the mean residual distance. A true periodic
// pair gives ~0 regardless of curvature, since we're checking coincidence,
// not orientation.
    std::vector<double> xyz = samplePoints(surfTag, nSamples);
    if (xyz.empty()) return std::numeric_limits<double>::max();

    std::vector<double> translated(xyz.size());
    for (size_t k = 0; k < xyz.size() / 3; ++k) {
        translated[3*k+0] = xyz[3*k+0] + dx;
        translated[3*k+1] = xyz[3*k+1] + dy;
        translated[3*k+2] = xyz[3*k+2] + dz;
    }

    std::vector<double> closestPoints, paramCoord;
    gm::getClosestPoint(2, candidateTag, translated, closestPoints, paramCoord);
    if (closestPoints.size() != translated.size()) return std::numeric_limits<double>::max();

    double sumDist = 0.0;
    size_t nPts = translated.size() / 3;
    for (size_t k = 0; k < nPts; ++k) {
        double ex = translated[3*k+0] - closestPoints[3*k+0];
        double ey = translated[3*k+1] - closestPoints[3*k+1];
        double ez = translated[3*k+2] - closestPoints[3*k+2];
        sumDist += std::sqrt(ex*ex + ey*ey + ez*ez);
    }
    return sumDist / nPts;
}


static void splitCandidatesByProjection(double tx, double ty, double tz,
                                        std::vector<int>& loGroup, std::vector<int>& hiGroup)
{

// Coarse pre-filter: split all boundary surfaces into a "low" and "high"
// group by projecting each surface's own bounding box center onto the
// translation direction. This is only a speed optimization (avoids running
// the O(n^2) translation-residual check across every surface pair) — the
// actual pairing decision below is entirely translation-based.

    std::vector<std::pair<int,int>> allSurfaces;
    gm::getEntities(allSurfaces, 2);

    std::vector<std::pair<int,double>> projected;
    for (auto& dimTag : allSurfaces) {
        double xmin, ymin, zmin, xmax, ymax, zmax;
        gm::getBoundingBox(2, dimTag.second, xmin, ymin, zmin, xmax, ymax, zmax);
        double cx = 0.5*(xmin+xmax), cy = 0.5*(ymin+ymax), cz = 0.5*(zmin+zmax);
        projected.push_back({dimTag.second, cx*tx + cy*ty + cz*tz});
    }
    if (projected.empty()) return;

    double lo = projected[0].second, hi = projected[0].second;
    for (auto& p : projected) { lo = std::min(lo, p.second); hi = std::max(hi, p.second); }
    double mid = 0.5*(lo+hi);

    for (auto& p : projected) {
        if (p.second < mid) loGroup.push_back(p.first);
        else hiGroup.push_back(p.first);
    }
}

std::vector<FacePair> findPeriodicFacePairs(const std::vector<double>& translationVec,
                                            double tolerance,
                                            int nSamples)
{

// Finds surface pairs related by translationVec. For each surface in the
// "low" group, tries every surface in the "high" group and keeps the best
// (lowest-residual) match if it's below tolerance. Curvature never enters
// the decision — a cylindrical strut wall pairs correctly with its
// translated twin exactly like a flat face would.

    double dx = translationVec[0], dy = translationVec[1], dz = translationVec[2];
    double len = std::sqrt(dx*dx + dy*dy + dz*dz);

    std::vector<int> loGroup, hiGroup;
    splitCandidatesByProjection(dx/len, dy/len, dz/len, loGroup, hiGroup);

    std::vector<FacePair> pairs;
    for (int lo : loGroup) {
        int bestHi = -1;
        double bestResidual = std::numeric_limits<double>::max();
        for (int hi : hiGroup) {
            double residual = translationResidual(lo, hi, dx, dy, dz, nSamples);
            if (residual < bestResidual) { bestResidual = residual; bestHi = hi; }
        }
        if (bestHi != -1 && bestResidual < tolerance) {
            pairs.push_back({lo, bestHi, bestResidual});
        }
    }
    return pairs;
}
/******************************************* For edges mapping *********************************/

// Fonction pour calculer la distance Euclidienne entre deux points 3D
double computeDistance(const std::vector<double>& p1, const std::vector<double>& p2) {
    return std::sqrt(std::pow(p1[0] - p2[0], 2) +
                     std::pow(p1[1] - p2[1], 2) +
                     std::pow(p1[2] - p2[2], 2));
}


int findEdgeBySamplePoints(const int& tg1, const std::vector<int>& amongEdges, double dx, double dy, double dz, int nSamples, double tolerance){

    //This function returns the tag of the edge at (-dx, -dy, -dz) of tg1. The research is done among amongEdges

    std::vector<double> samplePoints = sampleEdgePoints(tg1, nSamples); //sample nSamples points on tg1

    for(int ipt=0; ipt<nSamples; ipt++){
        samplePoints[3*ipt] += dx;
        samplePoints[3*ipt+1] += dy;
        samplePoints[3*ipt+2] += dz;
    }

    for (const auto& edge : amongEdges) {
        bool all_points_match = true;

        for (size_t ipt = 0; ipt < nSamples; ipt++) {
            std::vector<double> query_pt = {samplePoints[3*ipt], samplePoints[3*ipt+1], samplePoints[3*ipt+2]};
            std::vector<double> closest_pt;
            std::vector<double> parametric_coords;

            // Projette orthogonalement query_pt sur l'arête courante
            gmsh::model::getClosestPoint(1, edge, query_pt, closest_pt, parametric_coords);

            // Vérifie la distance entre le point d'origine et sa projection
            double dist = computeDistance(query_pt, closest_pt);

            if (dist > tolerance) {
                // Si un seul point est trop éloigné, cette arête n'est pas la bonne
                all_points_match = false;
                break;
            }
        }

    // Si tous les points échantillonnés sont validés sur cette courbe, on a trouvé le bon tag !
    if (all_points_match) {
        return edge;
    }}
    return -1; // Aucun tag trouvé correspondant à cet échantillonnage
}


//bool findPeriodicEntityPair(const std::vector<int>& entSet1, std::vector<int>& entSet2, double x, double y, double z, double eps=std::numeric_limits<double>::epsilon()*10. ){

//    /* entSet2 is intended to containt an entity at {x, y, z} from entSet1*/
//}


