#include "mesh.h"
#include <halfedge.h>
#include <la.h>
//Mesh::Mesh()
//{

//}
Mesh::Mesh(GLWidget277 *context) : Drawable(context)
{}

void Mesh::Triangular(){
    for(int i = 0; i < Faces.size(); i++){
        HalfEdge *HE0 = Faces[i]->start_edge;
        int count = 1;
        int id = HE0->id;
        HE0 = HE0->next;
        while(HE0->id != id){
            count++;
            HE0 = HE0->next;
        }
        if(count == 4){
            //quadrangle:
            //Step1: Create two new halfedges HEA and HEB
            HalfEdge *HEA = new HalfEdge;
            HalfEdge *HEB = new HalfEdge;
            HEA->id = HalfEdges[HalfEdges.size()-1]->id + 1;
            HEB->id = HalfEdges[HalfEdges.size()-1]->id + 2;
            HEA->vert = HE0->vert;
            HEB->vert = HE0->next->next->vert;
            HEA->sym = HEB;
            HEB->sym = HEA;
            //Step2: Create a second face Face2
            Face *FACE2 = new Face;
            FACE2->color = Faces[i]->color;
            FACE2->id = Faces[Faces.size()-1]->id + 1;
            HEA->face = FACE2;
            HE0->next->face = FACE2;
            HE0->next->next->face = FACE2;
            HEB->face = Faces[i];
            FACE2->start_edge = HEA;
            //Step3: Fix up the next pointers for our half-edges
            HEB->next = HE0->next->next->next;
            HE0->next->next->next = HEA;
            HEA->next = HE0->next;
            HE0->next = HEB;
            //Step4: push back the edges and face.(Face should be inserted before the start(),
            //                                      because after sepratation, the remain face may still have more than 3 edges)
            HalfEdges.push_back(HEA);
            HalfEdges.push_back(HEB);
            Faces.push_back(FACE2);
        }
    }
}

void Mesh::create(){
    GLuint mesh_idx[3*Faces.size()];
    glm::vec4 mesh_face_col[Vertices.size()];
    glm::vec4 mesh_vert_pos[Vertices.size()];
    glm::vec4 mesh_vert_nor[Vertices.size()];
   // glm::vec4 mesh_vert_nor[Vertices.size()];

    //Set Vertex position and normal:
    for(int i = 0; i < Vertices.size(); i++){
        mesh_vert_pos[i] = glm::vec4(Vertices[i]->pos[0],Vertices[i]->pos[1],Vertices[i]->pos[2],1);
        mesh_vert_nor[i] = glm::vec4(Vertices[i]->nor[0],Vertices[i]->nor[1],Vertices[i]->nor[2],1);
        Vertices[i]->id = i;
    }

    //Set Indices and Colors:
    for(int i = 0; i < Faces.size(); i++){
        HalfEdge *edge = Faces[i]->start_edge;
        mesh_idx[3*i] = edge->vert->id;
        mesh_face_col[edge->vert->id] = glm::vec4(Faces[i]->color[0],Faces[i]->color[1],Faces[i]->color[2],1);
        edge = edge->next;
        mesh_idx[3*i+1] = edge->vert->id;
        mesh_face_col[edge->vert->id] = glm::vec4(Faces[i]->color[0],Faces[i]->color[1],Faces[i]->color[2],1);
        edge = edge->next;
        mesh_idx[3*i+2] = edge->vert->id;
        mesh_face_col[edge->vert->id] = glm::vec4(Faces[i]->color[0],Faces[i]->color[1],Faces[i]->color[2],1);
    }


    count = 3 * Faces.size();

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * Faces.size() * sizeof(GLuint), mesh_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec4), mesh_vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec4), mesh_vert_nor, GL_STATIC_DRAW);

    //Create a VBO of color
    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec4), mesh_face_col, GL_STATIC_DRAW);
}

void Mesh::LoadCube(){
    //Eight Vertices:
    Vertex *V0 = new Vertex(0), *V1 = new Vertex(1),
            *V2 = new Vertex(2), *V3 = new Vertex(3),
            *V4 = new Vertex(4), *V5 = new Vertex(5),
            *V6 = new Vertex(6), *V7 = new Vertex(7),
            *V01 = new Vertex(8), *V11 = new Vertex(9),
            *V21 = new Vertex(10), *V31 = new Vertex(11),
            *V41 = new Vertex(12), *V51 = new Vertex(13),
            *V61 = new Vertex(14), *V71 = new Vertex(15),
            *V02 = new Vertex(16), *V12 = new Vertex(17),
            *V22 = new Vertex(18), *V32 = new Vertex(19),
            *V42 = new Vertex(20), *V52 = new Vertex(21),
            *V62 = new Vertex(22), *V72 = new Vertex(23);
    //set pos
    V0->pos = glm::vec3(-0.5, -0.5,  0.5);
    V1->pos = glm::vec3(-0.5, -0.5, -0.5);
    V2->pos = glm::vec3( 0.5, -0.5, -0.5);
    V3->pos = glm::vec3( 0.5, -0.5,  0.5);
    V4->pos = glm::vec3(-0.5,  0.5,  0.5);
    V5->pos = glm::vec3( 0.5,  0.5,  0.5);
    V6->pos = glm::vec3( 0.5,  0.5, -0.5);
    V7->pos = glm::vec3(-0.5,  0.5, -0.5);
    V01->pos = glm::vec3(-0.5, -0.5,  0.5);
    V11->pos = glm::vec3(-0.5, -0.5, -0.5);
    V21->pos = glm::vec3( 0.5, -0.5, -0.5);
    V31->pos = glm::vec3( 0.5, -0.5,  0.5);
    V41->pos = glm::vec3(-0.5,  0.5,  0.5);
    V51->pos = glm::vec3( 0.5,  0.5,  0.5);
    V61->pos = glm::vec3( 0.5,  0.5, -0.5);
    V71->pos = glm::vec3(-0.5,  0.5, -0.5);
    V02->pos = glm::vec3(-0.5, -0.5,  0.5);
    V12->pos = glm::vec3(-0.5, -0.5, -0.5);
    V22->pos = glm::vec3( 0.5, -0.5, -0.5);
    V32->pos = glm::vec3( 0.5, -0.5,  0.5);
    V42->pos = glm::vec3(-0.5,  0.5,  0.5);
    V52->pos = glm::vec3( 0.5,  0.5,  0.5);
    V62->pos = glm::vec3( 0.5,  0.5, -0.5);
    V72->pos = glm::vec3(-0.5,  0.5, -0.5);

    //bottom face:
    Face *F0 = new Face(0);
    F0->color = glm::vec3(1,0,1);
    HalfEdge *HE0 = new HalfEdge(0), *HE1 = new HalfEdge(1), *HE2 = new HalfEdge(2), *HE3 = new HalfEdge(3);
    F0->start_edge = HE0;
    //set next
    HE0->next = HE1;
    HE1->next = HE2;
    HE2->next = HE3;
    HE3->next = HE0;
    //set face
    HE0->face = F0;
    HE1->face = F0;
    HE2->face = F0;
    HE3->face = F0;
    //set Vertex
    HE0->vert = V0;
    HE1->vert = V1;
    HE2->vert = V2;
    HE3->vert = V3;
    V0->nor = glm::vec3(0,-1,0);
    V1->nor = glm::vec3(0,-1,0);
    V2->nor = glm::vec3(0,-1,0);
    V3->nor = glm::vec3(0,-1,0);

    //front face:
    Face *F1 = new Face(1);
    F1->color = glm::vec3(1,0,0);
    HalfEdge *HE4 = new HalfEdge(4), *HE5 = new HalfEdge(5), *HE6 = new HalfEdge(6), *HE7 = new HalfEdge(7);
    F1->start_edge = HE4;
    //set next
    HE4->next = HE5;
    HE5->next = HE6;
    HE6->next = HE7;
    HE7->next = HE4;
    //set face
    HE4->face = F1;
    HE5->face = F1;
    HE6->face = F1;
    HE7->face = F1;
    //set Vertex
    HE4->vert = V31;
    HE5->vert = V5;
    HE6->vert = V4;
    HE7->vert = V01;
    V31->nor = glm::vec3(0,0,1);
    V5->nor = glm::vec3(0,0,1);
    V4->nor = glm::vec3(0,0,1);
    V01->nor = glm::vec3(0,0,1);

    //right face:
    Face *F2 = new Face(2);
    HalfEdge *HE8 = new HalfEdge(8), *HE9 = new HalfEdge(9), *HE10 = new HalfEdge(10), *HE11 = new HalfEdge(11);
    F2->color = glm::vec3(0,0,1);
    F2->start_edge = HE9;
    //set next
    HE8->next = HE9;
    HE9->next = HE10;
    HE10->next = HE11;
    HE11->next = HE8;
    //set face
    HE8->face = F2;
    HE9->face = F2;
    HE10->face = F2;
    HE11->face = F2;
    //set Vertex
    HE8->vert = V21;
    HE9->vert = V6;
    HE10->vert = V51;
    HE11->vert = V32;
    V21->nor = glm::vec3(1,0,0);
    V6->nor = glm::vec3(1,0,0);
    V51->nor = glm::vec3(1,0,0);
    V32->nor = glm::vec3(1,0,0);

    //top face:
    Face *F3 = new Face(3);
    HalfEdge *HE12 = new HalfEdge(12), *HE13 = new HalfEdge(13), *HE14 = new HalfEdge(14), *HE15 = new HalfEdge(15);
    F3->color = glm::vec3(1,1,1);
    F3->start_edge = HE12;
    //set next
    HE12->next = HE13;
    HE13->next = HE14;
    HE14->next = HE15;
    HE15->next = HE12;
    //set face
    HE12->face = F3;
    HE13->face = F3;
    HE14->face = F3;
    HE15->face = F3;
    //set Vertex
    HE12->vert = V52;
    HE13->vert = V61;
    HE14->vert = V7;
    HE15->vert = V41;
    V52->nor = glm::vec3(0,1,0);
    V61->nor = glm::vec3(0,1,0);
    V7->nor = glm::vec3(0,1,0);
    V41->nor = glm::vec3(0,1,0);

    //back face:
    Face *F4 = new Face(4);
    HalfEdge *HE16 = new HalfEdge(16), *HE17 = new HalfEdge(17), *HE18 = new HalfEdge(18), *HE19 = new HalfEdge(19);
    F4->color = glm::vec3(0,1,1);
    F4->start_edge = HE16;
    //set next
    HE16->next = HE17;
    HE17->next = HE18;
    HE18->next = HE19;
    HE19->next = HE16;
    //set face
    HE16->face = F4;
    HE17->face = F4;
    HE18->face = F4;
    HE19->face = F4;
    //set Vertex
    HE16->vert = V11;
    HE17->vert = V71;
    HE18->vert = V62;
    HE19->vert = V22;
    V11->nor = glm::vec3(0,0,-1);
    V71->nor = glm::vec3(0,0,-1);
    V62->nor = glm::vec3(0,0,-1);
    V22->nor = glm::vec3(0,0,-1);

    //left face:
    Face *F5 = new Face(5);
    HalfEdge *HE20 = new HalfEdge(20), *HE21 = new HalfEdge(21), *HE22 = new HalfEdge(22), *HE23 = new HalfEdge(23);
    F5->color = glm::vec3(1,1,0);
    F5->start_edge = HE20;
    //set next
    HE20->next = HE21;
    HE21->next = HE22;
    HE22->next = HE23;
    HE23->next = HE20;
    //set face
    HE20->face = F5;
    HE21->face = F5;
    HE22->face = F5;
    HE23->face = F5;
    //set Vertex
    HE20->vert = V02;
    HE21->vert = V42;
    HE22->vert = V72;
    HE23->vert = V12;
    V02->nor = glm::vec3(-1,0,0);
    V42->nor = glm::vec3(-1,0,0);
    V72->nor = glm::vec3(-1,0,0);
    V12->nor = glm::vec3(-1,0,0);

    //set SYM:
    HE0->sym = HE4;
    HE1->sym = HE23;
    HE2->sym = HE16;
    HE3->sym = HE8;
    HE4->sym = HE0;
    HE5->sym = HE11;
    HE6->sym = HE12;
    HE7->sym = HE20;
    HE8->sym = HE3;
    HE9->sym = HE19;
    HE10->sym = HE13;
    HE11->sym = HE5;
    HE12->sym = HE6;
    HE13->sym = HE10;
    HE14->sym = HE18;
    HE15->sym = HE21;
    HE16->sym = HE2;
    HE17->sym = HE22;
    HE18->sym = HE14;
    HE19->sym = HE9;
    HE20->sym = HE7;
    HE21->sym = HE15;
    HE22->sym = HE17;
    HE23->sym = HE1;

    //push_back:
    //Faces
    Faces.push_back(F0);
    Faces.push_back(F1);
    Faces.push_back(F2);
    Faces.push_back(F3);
    Faces.push_back(F4);
    Faces.push_back(F5);
    //Vertices
    Vertices.push_back(V0);
    Vertices.push_back(V1);
    Vertices.push_back(V2);
    Vertices.push_back(V3);
    Vertices.push_back(V4);
    Vertices.push_back(V5);
    Vertices.push_back(V6);
    Vertices.push_back(V7);
    Vertices.push_back(V01);
    Vertices.push_back(V11);
    Vertices.push_back(V21);
    Vertices.push_back(V31);
    Vertices.push_back(V41);
    Vertices.push_back(V51);
    Vertices.push_back(V61);
    Vertices.push_back(V71);
    Vertices.push_back(V02);
    Vertices.push_back(V12);
    Vertices.push_back(V22);
    Vertices.push_back(V32);
    Vertices.push_back(V42);
    Vertices.push_back(V52);
    Vertices.push_back(V62);
    Vertices.push_back(V72);
    //HalfEdges
    HalfEdges.push_back(HE0);
    HalfEdges.push_back(HE1);
    HalfEdges.push_back(HE2);
    HalfEdges.push_back(HE3);
    HalfEdges.push_back(HE4);
    HalfEdges.push_back(HE5);
    HalfEdges.push_back(HE6);
    HalfEdges.push_back(HE7);
    HalfEdges.push_back(HE8);
    HalfEdges.push_back(HE9);
    HalfEdges.push_back(HE10);
    HalfEdges.push_back(HE11);
    HalfEdges.push_back(HE12);
    HalfEdges.push_back(HE13);
    HalfEdges.push_back(HE14);
    HalfEdges.push_back(HE15);
    HalfEdges.push_back(HE16);
    HalfEdges.push_back(HE17);
    HalfEdges.push_back(HE18);
    HalfEdges.push_back(HE19);
    HalfEdges.push_back(HE20);
    HalfEdges.push_back(HE21);
    HalfEdges.push_back(HE22);
    HalfEdges.push_back(HE23);
}
