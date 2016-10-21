#ifndef MESH_H
#define MESH_H

#include <drawable.h>
#include <halfedge.h>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>


class Mesh : public Drawable
{
private:
    std::vector <Face *> Faces;
    std::vector <HalfEdge *> HalfEdges;
    std::vector <Vertex *> Vertices;
public:
    //Mesh();
    Mesh(GLWidget277 *context);
    virtual void create();
    void Triangular();

    void LoadCube();
    int Test(){
        return Faces.size();
    }
};

#endif // MESH_H
