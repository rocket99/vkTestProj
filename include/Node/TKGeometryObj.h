//
//TKGeomoetryObj.h
//2019-1-9

#ifndef __TKGeometryObj_h__
#define __TKGeometryObj_h__

#include "TKBaseNode.h"

class TKGeometryObj : public TKBaseNode
{
public:
    TKGeometryObj();
    ~TKGeometryObj();

    enum {
        OBJ_CUBE = 0,
        OBJ_SPHERE,
        OBJ_CYLINDER,
        OBJ_CONICAL,
        OBJ_PLANE,
        OBJ_DISK
    };

    static TKGeometryObj *createCube(float width, float height, float depth);
    bool initCube(float width, float height, float depth);

    static TKGeometryObj *createSphere(float radius);
    bool initSphere(float radius);

    static TKGeometryObj *createCylinder(float radius, float height);
    bool initCylinder(float radius, float height);

    static TKGeometryObj *createConical(float radius, float height);
    bool initConical(float radius, float height);

    static TKGeometryObj *createRectPlane(float width, float height);
    bool initRectPlane(float width, float height);

    static TKGeometryObj *createDisk(float radius);
    bool initDisk(float radius);
    
    void draw(VkCommandBuffer cmdBuf, uint32_t swapIdx);
protected:
    uint32_t   m_type;
    
    uint32_t   m_pointNum;
    TKVertice *m_points;
    VkBuffer   m_pointBuffer;
    VkDeviceMemory m_vertexMemory;

    uint32_t   m_indexNum;
    uint32_t  *m_indices;
    VkBuffer   m_indiceBuffer;
    VkDeviceMemory m_indiceMemory;
    
    virtual void setVertexBuffer();
    virtual void setIndexBuffer();

    struct cube_para {
        float width; 
        float height;
        float depth;
    };
    cube_para *m_cubePara;

    struct sphere_para {
        float radius;
    };
    sphere_para *m_spherePara;

    struct cylinder_para {
        float radius;
        float height;
    };
    cylinder_para *m_cylinderPara;

    struct conical_para {
        float radius;
        float height;
    };
    conical_para *m_conicalPara;

    struct rect_para {
        float width;
        float height;
    };
    rect_para *m_rectPara;

    struct disk_para {
        float radius;
    };
    disk_para *m_diskPara;
};


#endif
