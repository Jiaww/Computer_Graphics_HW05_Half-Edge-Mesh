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
public:
    std::vector <Face *> Faces;
    std::vector <HalfEdge *> HalfEdges;
    std::vector <Vertex *> Vertices;
    std::vector <Vertex *> Unique_Vertices;
    int num_Vert;
    int num_Face;
    int num_HalfEdges;
    //Mesh();
    Mesh(GLWidget277 *context);
    virtual void create();
    void Triangular();

    void LoadCube();
    int Test(){
        return Faces.size();
    }
    int numOfVert(){return num_Vert;}
    int numOfFace(){return num_Face;}
    int numOfHalfEdges(){return num_HalfEdges;}

    void RepositionVert(Vertex *V, glm::vec4 new_pos);

    void setFromFile(std::vector<glm::vec4> vert_pos, std::vector<glm::vec4> vert_uv,
                     std::vector<glm::vec4> vert_nor, std::vector<QStringList> faces_con);
};

#endif // MESH_H
