#include "bvh.h"

#include <iostream>

#include "mesh.h"

void BVH::build(const Mesh* pMesh, int targetCellSize, int maxDepth)
{
    // store a pointer to the mesh
    m_pMesh = pMesh;
    // allocate the root node
    m_nodes.resize(1);

    if (m_pMesh->nbFaces() <= targetCellSize) { // only one node
        m_nodes[0].box = pMesh->AABB();
        m_nodes[0].first_face_id = 0;
        m_nodes[0].is_leaf = true;
        m_nodes[0].nb_faces = m_pMesh->nbFaces();
        m_faces.resize(m_pMesh->nbFaces());
        for (int i = 0; i < m_pMesh->nbFaces(); ++i)
            m_faces[i] = i;
    } else {
        // reserve space for other nodes to avoid multiple memory reallocations
        m_nodes.reserve(std::min<int>(2 << maxDepth, std::log(m_pMesh->nbFaces() / targetCellSize)));

        // compute centroids and initialize the face list
        m_centroids.resize(m_pMesh->nbFaces());
        m_faces.resize(m_pMesh->nbFaces());
        for (int i = 0; i < m_pMesh->nbFaces(); ++i) {
            m_centroids[i] = (m_pMesh->vertexOfFace(i, 0).position + m_pMesh->vertexOfFace(i, 1).position + m_pMesh->vertexOfFace(i, 2).position) / 3.f;
            m_faces[i] = i;
        }

        // recursively build the BVH, starting from the root node and the entire list of faces
        buildNode(0, 0, m_pMesh->nbFaces(), 0, targetCellSize, maxDepth);
    }
}

void BVH::buildNode(int nodeId, int start, int end, int level, int targetCellSize, int maxDepth)
{
    printf("\n[DEBUG] Building node %d…\n", nodeId);
    printf("[DEBUG] start = %d, end = %d, level = %d.\n", start, end, level);

    // Retrieve the node.
    Node& node = m_nodes[nodeId];

    // Set the number of faces.
    node.nb_faces = end - start;
    printf("[DEBUG] node.nb_faces = %d.\n", node.nb_faces);

    /**
     * étape 1 : calculer la boite englobante des faces indexées de m_faces[start] à m_faces[end]
     * (Utiliser la fonction extend de Eigen::AlignedBox3f et la fonction mpMesh->vertexOfFace(int) pour obtenir les coordonnées des sommets des faces)
     */

    // Compute the box.
    Mesh::Vertex A, B, C;
    for (int face_id = start; face_id < end; face_id++) {
        A = m_pMesh->vertexOfFace(face_id, 0), B = m_pMesh->vertexOfFace(face_id, 1), C = m_pMesh->vertexOfFace(face_id, 2);
        node.box.extend(A.position), node.box.extend(B.position), node.box.extend(C.position);
    }

    // Retrieve the box sizes.
    float sizes[3] = { node.box.sizes().x(), node.box.sizes().y(), node.box.sizes().z() };
    printf("[DEBUG] node.box.sizes() = (%f, %f, %f).\n", sizes[0], sizes[1], sizes[2]);

    /**
     * étape 2 : déterminer si il s'agit d'une feuille (appliquer les critères d'arrêts)
     * Si c'est une feuille, finaliser le noeud et quitter la fonction
     */

    // Check if it's going to be a leaf.
    if (node.nb_faces <= targetCellSize || level >= maxDepth) {
        // Set the node as leaf.
        node.is_leaf = true;
        printf("[DEBUG] node.is_leaf = true.\n");

        // Set the first face id.
        node.first_face_id = start;
        printf("[DEBUG] node.first_face_id = %d.\n", node.first_face_id);

        return;
    }

    /**
     * étape 3 : calculer l'index de la dimension (x=0, y=1, ou z=2) et la valeur du plan de coupe
     * (on découpe au milieu de la boite selon la plus grande dimension)
     */

    // Compute the axis according to which to split the box, and the split value associated.
    unsigned int dim = std::distance(sizes, std::max_element(sizes, sizes + 3));
    float split_value = sizes[dim] / 2;
    printf("[DEBUG] dim = %d, split_value = %f.\n", dim, split_value);

    /**
     * étape 4 : appeler la fonction split pour trier (partiellement) les faces et vérifier si le split a été utile
     */

    // Compute the faces middle index.
    int middle = split(start, end, dim, split_value);
    printf("[DEBUG] middle = %d.\n", middle);

    // Check if the split was useful.
    if (middle == start || middle == end) {
        // Set the node as leaf.
        node.is_leaf = true;
        printf("[DEBUG] node.is_leaf = true.\n");

        // Set the first face id.
        node.first_face_id = start;
        printf("[DEBUG] node.first_face_id = %d.\n", node.first_face_id);

        return;
    }

    /**
     * étape 5 : allouer les fils, et les construire en appelant buildNode...
     */

    node.is_leaf = false;
    printf("[DEBUG] node.is_leaf = false.\n");

    // Compute the children ids and set the first child id.
    unsigned int left_child_id = m_nodes.size(), right_child_id = left_child_id + 1;
    node.first_child_id = left_child_id;
    printf("[DEBUG] node.first_child_id = %d.\n", node.first_face_id);

    // Allocate and build the children.
    m_nodes.resize(m_nodes.size() + 2);
    buildNode(left_child_id, start, middle, level + 1, targetCellSize, maxDepth);
    buildNode(right_child_id, middle, end, level + 1, targetCellSize, maxDepth);
}

/**
 * @brief Sorts the faces with respect to their centroid along the dimension @a dim and spliting value @a split_value.
 * 
 * @returns the middle index
 */
int BVH::split(int start, int end, int dim, float split_value)
{
    int l(start), r(end - 1);
    while (l < r) {
        // find the first on the left
        while (l < end && m_centroids[l](dim) < split_value)
            ++l;
        while (r >= start && m_centroids[r](dim) >= split_value)
            --r;
        if (l > r)
            break;
        std::swap(m_centroids[l], m_centroids[r]);
        std::swap(m_faces[l], m_faces[r]);
        ++l;
        --r;
    }
    return m_centroids[l][dim] < split_value ? l + 1 : l;
}

bool BVH::intersect(const Ray& ray, Hit& hit) const
{
    int nodeId = 0;
    printf("\n[DEBUG] Intersecting node %d…\n", nodeId);

    // Retrieve the node.
    const Node& node = m_nodes[nodeId];

    // Check if there is at least intersection with the box of this node.
    float tMin, tMax;
    Normal3f n;
    if (!::intersect(ray, node.box, tMin, tMax, n) || tMin > hit.t() || (tMin < 0 && tMax < 0))
        return false;

    // It's necessarily an inside node, check if there is intersection with a child of the node (recursively).
    Hit left_hit, right_hit;
    if (!intersectNode(node.first_child_id, ray, left_hit) && !intersectNode(node.first_child_id + 1, ray, right_hit))
        return false;
    hit = right_hit.t() < left_hit.t() ? right_hit : left_hit;
    return true;
}

bool BVH::intersectNode(int nodeId, const Ray& ray, Hit& hit) const
{
    printf("[DEBUG] Intersecting node %d…\n", nodeId);

    // Retrieve the node.
    const Node& node = m_nodes[nodeId];

    // Check if there is at least intersection with the box of this node.
    float tMin, tMax;
    Normal3f n;
    if (!::intersect(ray, node.box, tMin, tMax, n) || tMin > hit.t() || (tMin < 0 && tMax < 0))
        return false;

    // If it's an inside node, check if there is intersection with a child of the node (recursively).
    if (!node.is_leaf) {
        Hit left_hit, right_hit;
        if (!intersectNode(node.first_child_id, ray, left_hit) && !intersectNode(node.first_child_id + 1, ray, right_hit))
            return false;
        hit = right_hit.t() < left_hit.t() ? right_hit : left_hit;
        return true;
    }

    // If it's a leaf, check if there is intersection with a face of the node.
    bool found_intersection = false;
    Hit face_hit;
    for (int face_id = node.first_face_id; face_id < node.first_face_id + node.nb_faces; face_id++)
        if (m_pMesh->intersectFace(ray, face_hit, face_id) && face_hit.t() < hit.t())
            hit = face_hit, found_intersection = true;
    if (found_intersection)
        printf("[DEBUG] Intersection found.\n");
    return found_intersection;
}
