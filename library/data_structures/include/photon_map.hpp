#ifndef RCL_PHOTON_MAP
#define RCL_PHOTON_MAP

#include "hittable_list.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "vector.hpp"

#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <queue>
#include <mutex>

namespace rcl
{
   
struct Photon
{
    vec3 position;
    vec3 power;
    vec3 direction;
    bool isCaustic;
    int sourceMaterial;    // 0=unknown, 1=diffuse, 2=metal, 3=glass
   
    Photon() {}
    Photon(const vec3& pos, const vec3& pow, const vec3& dir, bool caustic = false, int source = 0)
    : position(pos), power(pow), direction(dir), isCaustic(caustic), sourceMaterial(source) {}
};

struct KDTreeNode
{
    Photon photon;
    int axis;
    std::unique_ptr<KDTreeNode> left;
    std::unique_ptr<KDTreeNode> right;
   
    KDTreeNode(const Photon& p, int ax) : photon(p), axis(ax), left(nullptr), right(nullptr) {}
};

class PhotonMap
{
public:
    PhotonMap();
    ~PhotonMap() = default;
   
    void Build(std::vector<Photon>& photons);
   
    void FindNearestPhotons(const vec3& position, double radius,
                           std::vector<const Photon*>& result, bool causticsOnly = false) const;
   
    void FindKNearestPhotons(const vec3& position, int k,
                            std::vector<const Photon*>& result, bool causticsOnly = false) const;
   
    void Clear();

private:
    std::unique_ptr<KDTreeNode> root;
    mutable std::mutex mapMutex;
   
    std::unique_ptr<KDTreeNode> BuildTree(std::vector<Photon>& photons, int start, int end, int depth);
    std::unique_ptr<KDTreeNode> BuildBalancedTree(std::vector<Photon>& photons, int start, int end, int depth);
   
    void FindNearestPhotonsHelper(const KDTreeNode* node, const vec3& position,
                                 double squaredRadius, std::vector<const Photon*>& result,
                                 bool causticsOnly, int materialFilter = 0) const;
   
    void FindKNearestPhotonsHelper(const KDTreeNode* node, const vec3& position,
                                  std::priority_queue<std::pair<double, const Photon*>>& result,
                                  int k, bool causticsOnly) const;
   
    static constexpr int MIN_PHOTONS_FOR_PARALLEL = 1000;
    static constexpr int MIN_DEPTH_FOR_PARALLEL = 3;
};

} // namespace rcl

#endif