#ifndef HALFEDGE_H
#define HALFEDGE_H
#include <la.h>

class HalfEdge;
class Vertex{
public:
    int id;
    int nor_id;
    glm::vec3 pos;
    glm::vec3 nor;
    glm::vec3 col;
    HalfEdge *edge;
    Vertex();
    Vertex(int id);
    Vertex(glm::vec3 pos, HalfEdge *edge, int id);
};

class Face{
public:
    int id;
    HalfEdge *start_edge;
    glm::vec3 color;
    Face();
    Face(int id);
    Face(HalfEdge *start_edge, int id);
    Face(HalfEdge *start_edge, glm::vec3 color, int id);
};

class HalfEdge
{
public:
    int id;
    Face *face;
    Vertex *vert;
    HalfEdge *next;
    HalfEdge *sym;
    HalfEdge();
    HalfEdge(int id);
    HalfEdge(Face *face, Vertex *vert, HalfEdge *next, HalfEdge *sym, int id);
};

#endif // HALFEDGE_H
