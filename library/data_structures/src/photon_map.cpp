#include "photon_map.hpp"

#include <random>
#include <chrono>
#include <thread>
#include <future>

namespace rcl
{

PhotonMap::PhotonMap() : root(nullptr) {}

void PhotonMap::Build(std::vector<Photon>& inputPhotons)
{
    std::lock_guard<std::mutex> lock(mapMutex);
    
    if (inputPhotons.empty()) 
    {
        root = nullptr;
        return;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    if (inputPhotons.size() >= MIN_PHOTONS_FOR_PARALLEL) 
    {
        root = BuildBalancedTree(inputPhotons, 0, inputPhotons.size(), 0);
    } 
    else 
    {
        root = BuildTree(inputPhotons, 0, inputPhotons.size(), 0);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Photon map built with " << inputPhotons.size() << " photons in " 
              << duration.count() << "ms" << std::endl;
}

void PhotonMap::FindNearestPhotons(const vec3& position, double radius, 
                                       std::vector<const Photon*>& result, bool causticsOnly) const
{
    if (!root) return;
    
    double squaredRadius = radius * radius;
    result.clear();
    result.reserve(100); // Reserve some space to avoid frequent reallocations
    
    FindNearestPhotonsHelper(root.get(), position, squaredRadius, result, causticsOnly);
}

void PhotonMap::FindKNearestPhotons(const vec3& position, int k, 
                                        std::vector<const Photon*>& result, bool causticsOnly) const
{
    if (!root || k <= 0) return;
    
    std::priority_queue<std::pair<double, const Photon*>> pq;
    
    FindKNearestPhotonsHelper(root.get(), position, pq, k, causticsOnly);
    
    result.clear();
    result.reserve(pq.size());
    
    while (!pq.empty()) 
    {
        result.push_back(pq.top().second);
        pq.pop();
    }
    
    std::reverse(result.begin(), result.end());
}

void PhotonMap::Clear()
{
    std::lock_guard<std::mutex> lock(mapMutex);
    root = nullptr;
}

std::unique_ptr<KDTreeNode> PhotonMap::BuildTree(std::vector<Photon>& inputPhotons, 
                                                          int start, int end, int depth)
{
    if (start >= end) return nullptr;
    
    int axis = depth % 3;  // Cycle through x, y, z axes
    int mid = start + (end - start) / 2;
    
    std::nth_element
    (
        inputPhotons.begin() + start,
        inputPhotons.begin() + mid,
        inputPhotons.begin() + end,
        [axis](const Photon& a, const Photon& b) 
        {
            return a.position[axis] < b.position[axis];
        }
    );
    
    auto node = std::make_unique<KDTreeNode>(inputPhotons[mid], axis);
    node->left = BuildTree(inputPhotons, start, mid, depth + 1);
    node->right = BuildTree(inputPhotons, mid + 1, end, depth + 1);
    
    return node;
}

std::unique_ptr<KDTreeNode> PhotonMap::BuildBalancedTree(std::vector<Photon>& inputPhotons, 
                                                                  int start, int end, int depth)
{
    if (start >= end) return nullptr;
    
    int size = end - start;
    int axis = depth % 3;
    
    std::nth_element
    (
        inputPhotons.begin() + start,
        inputPhotons.begin() + start + size / 2,
        inputPhotons.begin() + end,
        [axis](const Photon& a, const Photon& b) 
        {
            return a.position[axis] < b.position[axis];
        }
    );
    
    int mid = start + size / 2;
    auto node = std::make_unique<KDTreeNode>(inputPhotons[mid], axis);
    
    if (size >= MIN_PHOTONS_FOR_PARALLEL && depth < MIN_DEPTH_FOR_PARALLEL) 
    {
        auto leftFuture = std::async(std::launch::async, 
            [this, &inputPhotons, start, mid, depth]() 
            {
                return BuildBalancedTree(inputPhotons, start, mid, depth + 1);
            });
        
        auto rightFuture = std::async(std::launch::async,
            [this, &inputPhotons, mid, end, depth]() 
            {
                return BuildBalancedTree(inputPhotons, mid + 1, end, depth + 1);
            });
        
        node->left = leftFuture.get();
        node->right = rightFuture.get();
    } 
    else 
    {
        node->left = BuildBalancedTree(inputPhotons, start, mid, depth + 1);
        node->right = BuildBalancedTree(inputPhotons, mid + 1, end, depth + 1);
    }
    
    return node;
}

void PhotonMap::FindNearestPhotonsHelper(const KDTreeNode* node, const vec3& position,
                                             double squaredRadius, std::vector<const Photon*>& result,
                                             bool causticsOnly, int materialFilter) const
{
    if (!node) return;
    
    const Photon& photon = node->photon;
    double squaredDistance = (position - photon.position).LengthSquared();
    
    if (squaredDistance <= squaredRadius) 
    {
        bool materialMatch = (materialFilter == 0) || (photon.sourceMaterial == materialFilter);
        if ((!causticsOnly || photon.isCaustic) && materialMatch)
            result.push_back(&photon);
    }
    
    int axis = node->axis;
    double axisDist = position[axis] - photon.position[axis];

    const KDTreeNode* first = (axisDist < 0) ? node->left.get() : node->right.get();
    const KDTreeNode* second = (axisDist < 0) ? node->right.get() : node->left.get();
    
    FindNearestPhotonsHelper(first, position, squaredRadius, result, causticsOnly, materialFilter);
    
    if (axisDist * axisDist <= squaredRadius) 
        FindNearestPhotonsHelper(second, position, squaredRadius, result, causticsOnly, materialFilter);
}

void PhotonMap::FindKNearestPhotonsHelper(const KDTreeNode* node, const vec3& position,
                                              std::priority_queue<std::pair<double, const Photon*>>& result,
                                              int k, bool causticsOnly) const
{
    if (!node) return;
    
    const Photon& photon = node->photon;
    double squaredDistance = (position - photon.position).LengthSquared();
    
    if (!causticsOnly || photon.isCaustic) 
    {
        if (result.size() < k) 
        {
            result.push(std::make_pair(squaredDistance, &photon));
        } 
        else if (squaredDistance < result.top().first) 
        {
            result.pop();
            result.push(std::make_pair(squaredDistance, &photon));
        }
    }
    
    int axis = node->axis;
    double axisDist = position[axis] - photon.position[axis];
    
    const KDTreeNode* first = (axisDist < 0) ? node->left.get() : node->right.get();
    const KDTreeNode* second = (axisDist < 0) ? node->right.get() : node->left.get();
    
    FindKNearestPhotonsHelper(first, position, result, k, causticsOnly);
    
    double maxSquaredDist = (result.size() < k) ? std::numeric_limits<double>::max() : result.top().first;
    if (axisDist * axisDist <= maxSquaredDist) 
    {
        FindKNearestPhotonsHelper(second, position, result, k, causticsOnly);
    }
}

}