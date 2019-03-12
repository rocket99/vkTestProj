//
//TKGeomoetryObj.cpp
//2019-1-9

#include "TKGeometryObj.h"

TKGeometryObj::TKGeometryObj(){
    m_cubePara = nullptr;
    m_spherePara = nullptr;
    m_cylinderPara = nullptr;
    m_conicalPara = nullptr;
    m_rectPara = nullptr;
    m_diskPara = nullptr;
}

TKGeometryObj::~TKGeometryObj(){
    VkDevice device = TKBaseInfo::Info()->device;
    if(m_indices != nullptr){
        free(m_indices);
    }
    if(m_points != nullptr){
        free(m_points);
    }
    if(m_pointBuffer != VK_NULL_HANDLE){
        vkDestroyBuffer(device, m_pointBuffer, nullptr);
    }
    if(m_indiceBuffer != VK_NULL_HANDLE){
        vkDestroyBuffer(device, m_indiceBuffer, nullptr);
    }
    if(m_vertexMemory != VK_NULL_HANDLE){
        vkFreeMemory(device, m_vertexMemory, nullptr);
    }
    if(m_indiceMemory != VK_NULL_HANDLE){
        vkFreeMemory(device, m_indiceMemory, nullptr);
    }
    
    if(m_cubePara != nullptr){
        delete m_cubePara;
    }
    if(m_spherePara != nullptr){
        delete m_spherePara;
    }
    if(m_cylinderPara != nullptr){
        delete m_cylinderPara;
    }
    if(m_conicalPara != nullptr){
        delete m_conicalPara;
    }
    if(m_rectPara != nullptr){
        delete m_rectPara;
    }
}

TKGeometryObj *TKGeometryObj::createCube(float width, float height, float depth){
    TKGeometryObj *obj = new TKGeometryObj;
    if(obj->initCube(width, height, depth)==false){
        delete obj;
        obj = nullptr;
    }
    return obj;
}

bool TKGeometryObj::initCube(float width, float height, float depth){
    if(this->initialize()==false){
        return false;
    }
    m_type = OBJ_CUBE;
    m_cubePara = new cube_para;
    m_cubePara->width  = width;
    m_cubePara->height = height;
    m_cubePara->depth  = depth;
    float w = width; float h = height; float d = depth;
    m_pointNum = 24; 
    m_points = (TKVertice *)malloc(sizeof(TKVertice)*m_pointNum);
    m_points[0] = TKVertice(Float3(-0.5*w, -0.5*h, -0.5*d), Float3(0.0, 0.0, -1.0),
                            Float4(1.0, 0.0, 0.0, 1.0), Float2(0.0, 0.0));
    m_points[1] = TKVertice(Float3(-0.5*w,  0.5*h, -0.5*d), Float3(0.0, 0.0, -1.0),
                            Float4(1.0, 0.0, 0.0, 1.0), Float2(0.0, 1.0));
    m_points[2] = TKVertice(Float3( 0.5*w,  0.5*h, -0.5*d), Float3(0.0, 0.0, -1.0),
                            Float4(1.0, 0.0, 0.0, 1.0), Float2(1.0, 1.0));
    m_points[3] = TKVertice(Float3( 0.5*w, -0.5*h, -0.5*d), Float3(0.0, 0.0, -1.0),
                            Float4(1.0, 0.0, 0.0, 1.0), Float2(1.0, 0.0));
    
    m_points[4] = TKVertice(Float3(-0.5*w, -0.5*h, 0.5*d), Float3(0.0, 0.0, 1.0),
                            Float4(1.0, 1.0, 0.0, 1.0), Float2(0.0, 0.0));
    m_points[5] = TKVertice(Float3(-0.5*w,  0.5*h, 0.5*d), Float3(0.0, 0.0, 1.0),
                            Float4(1.0, 1.0, 0.0, 1.0), Float2(0.0, 0.0));
    m_points[6] = TKVertice(Float3( 0.5*w,  0.5*h, 0.5*d), Float3(0.0, 0.0, 1.0),
                            Float4(1.0, 1.0, 0.0, 1.0), Float2(0.0, 0.0));
    m_points[7] = TKVertice(Float3( 0.5*w, -0.5*h, 0.5*d), Float3(0.0, 0.0, 1.0),
                            Float4(1.0, 1.0, 0.0, 1.0), Float2(0.0, 0.0));

    m_points[8] = TKVertice(Float3(-0.5*w, -0.5*h, -0.5*d), Float3(0.0, -1.0, 0.0),
                            Float4(1.0, 0.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_points[9] = TKVertice(Float3(-0.5*w, -0.5*h,  0.5*d), Float3(0.0, -1.0, 0.0),
                            Float4(1.0, 0.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_points[10] = TKVertice(Float3( 0.5*w, -0.5*h,  0.5*d), Float3(0.0, -1.0, 0.0),
                             Float4(1.0, 0.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_points[11] = TKVertice(Float3( 0.5*w, -0.5*h, -0.5*d), Float3(0.0, -1.0, 0.0),
                             Float4(1.0, 0.0, 1.0, 1.0), Float2(0.0, 0.0));
    
    m_points[12] = TKVertice(Float3(-0.5*w, 0.5*h, -0.5*d), Float3(0.0, 1.0, 0.0),
                             Float4(0.0, 1.0, 0.0, 1.0), Float2(0.0, 0.0));
    m_points[13] = TKVertice(Float3(-0.5*w, 0.5*h,  0.5*d), Float3(0.0, 1.0, 0.0),
                             Float4(0.0, 1.0, 0.0, 1.0), Float2(0.0, 0.0));
    m_points[14] = TKVertice(Float3( 0.5*w, 0.5*h,  0.5*d), Float3(0.0, 1.0, 0.0),
                             Float4(0.0, 1.0, 0.0, 1.0), Float2(0.0, 0.0));
    m_points[15] = TKVertice(Float3( 0.5*w, 0.5*h, -0.5*d), Float3(0.0, 1.0, 0.0),
                             Float4(0.0, 1.0, 0.0, 1.0), Float2(0.0, 0.0));

    m_points[16] = TKVertice(Float3(-0.5*w, -0.5*h, -0.5*d), Float3(-1.0, 0.0, 0.0),
                             Float4(0.0, 0.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_points[17] = TKVertice(Float3(-0.5*w, -0.5*h,  0.5*d), Float3(-1.0, 0.0, 0.0),
                             Float4(0.0, 0.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_points[18] = TKVertice(Float3(-0.5*w,  0.5*h,  0.5*d), Float3(-1.0, 0.0, 0.0),
                             Float4(0.0, 0.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_points[19] = TKVertice(Float3(-0.5*w,  0.5*h, -0.5*d), Float3(-1.0, 0.0, 0.0),
                             Float4(0.0, 0.0, 1.0, 1.0), Float2(0.0, 0.0));

    m_points[20] = TKVertice(Float3(0.5*w, -0.5*h, -0.5*d), Float3(1.0, 0.0, 0.0),
                             Float4(1.0, 1.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_points[21] = TKVertice(Float3(0.5*w, -0.5*h,  0.5*d), Float3(1.0, 0.0, 0.0),
                             Float4(1.0, 1.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_points[22] = TKVertice(Float3(0.5*w,  0.5*h,  0.5*d), Float3(1.0, 0.0, 0.0),
                             Float4(1.0, 1.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_points[23] = TKVertice(Float3(0.5*w,  0.5*h, -0.5*d), Float3(1.0, 0.0, 0.0),
                             Float4(1.0, 1.0, 1.0, 1.0), Float2(0.0, 0.0));
    m_indexNum = 36;
    m_indices = (uint32_t*)malloc(sizeof(uint32_t)*m_indexNum);
    uint32_t indices[] = { 0,   1,  2,  2,  0,  3,
                           4,   5,  6,  4,  6,  7,
                           8,   9, 10,  8, 10, 11,
                           12, 14, 13, 12, 14, 15,
                           16, 17, 18, 16, 18, 19,
                           20, 21, 22, 20, 22, 23 };
    memcpy(m_indices, indices, sizeof(uint32_t)*m_indexNum);

    this->setVertexBuffer();
    this->setIndexBuffer();

    return true;
}

TKGeometryObj *TKGeometryObj::createSphere(float radius){
    TKGeometryObj *sphere = new TKGeometryObj;
    if(sphere->initSphere(radius)==false){
        delete sphere;
        sphere = nullptr;
    }
    return sphere;
}

bool TKGeometryObj::initSphere(float radius){
    if(this->initialize()==false){
        return false;
    }
    m_type = OBJ_SPHERE;
    m_spherePara = new sphere_para;
    m_spherePara->radius = radius;
    uint32_t row = 16;
    uint32_t col = 32;
    m_pointNum = (row+1)*(col+1);
    m_points = (TKVertice*)malloc(sizeof(TKVertice)*m_pointNum);
    for(uint32_t i=0; i<=row; ++i){
        float theta = (float)i/(float)row*M_PI;
        float h = radius*cos(theta);
        for(uint32_t j=0; j<=col; ++j){
            float r = radius*sin(theta);
            float alpha = (float)j/(float)col*M_PI*2.0;
            m_points[i*(col+1)+j] = TKVertice(Float3(r*cos(alpha), r*sin(alpha), h),
                                              Float3(cos(alpha)*sin(theta),
                                                     sin(alpha)*sin(theta), cos(theta)),
                                              Float4(0.5, 0.5, 0.5, 1.0),
                                              Float2((float)j/(float)col, (float)i/(float)row));
        }
    }
    m_indexNum = 6*row*col;
    m_indices = (uint32_t*)malloc(sizeof(uint32_t)*row*col*6);
    uint32_t idx = 0;
    for(uint32_t i=0; i<row; ++i){
        for(uint32_t j=0; j<col; ++j){
            m_indices[idx++] = (col+1)*i+j;
            m_indices[idx++] = (col+1)*i+j+1;
            m_indices[idx++] = (col+1)*(i+1)+j;
            m_indices[idx++] = (col+1)*(i+1)+j;
            m_indices[idx++] = (col+1)*i+j+1;
            m_indices[idx++] = (col+1)*(i+1)+j+1;            
        }
    }
    this->setVertexBuffer();
    this->setIndexBuffer();
    return true;
}

TKGeometryObj *TKGeometryObj::createCylinder(float radius, float height){
    TKGeometryObj *obj = new TKGeometryObj;
    if(obj->initCylinder(radius, height)==false){
        delete obj;
        obj = nullptr;
    }
    return obj;
}

bool TKGeometryObj::initCylinder(float radius, float height){
    if(this->initialize()==false){
        return false;
    }
    m_cylinderPara = new cylinder_para;
    m_cylinderPara->height = height;
    m_cylinderPara->radius = radius;

    m_pointNum = 66;
    m_points = (TKVertice*)malloc(sizeof(TKVertice)*66);
    for(uint32_t i=0; i<=32; ++i){
        float a = (float)i/(float)32 * M_PI*2.0;
        m_points[2*i+0] = TKVertice(Float3(radius*cos(a), radius*sin(a), -0.5*height),
                                    Float3(cos(a), sin(a), 0.0), Float4(1.0, 0.0, 0.0, 1.0),
                                    Float2((float)i/float(32), 0.0));
        m_points[2*i+1] = TKVertice(Float3(radius*cos(a), radius*sin(a), 0.5*height),
                                    Float3(cos(a), sin(a), 0.0), Float4(1.0, 0.0, 0.0, 1.0),
                                    Float2((float)i/(float)32, 1.0));
    }
    m_indexNum = 32*6;
    m_indices = (uint32_t*)malloc(sizeof(uint32_t)*32*6);
    for(uint i=0; i<32; ++i){
        m_indices[6*i+0] = 2*i;
        m_indices[6*i+1] = 2*i+1;
        m_indices[6*i+2] = 2*(i+1);
        m_indices[6*i+3] = 2*(i+1);
        m_indices[6*i+4] = 2*i+1;
        m_indices[6*i+5] = 2*(i+1)+1;
    }
    
    this->setVertexBuffer();
    this->setIndexBuffer();
    return true;
}

TKGeometryObj *TKGeometryObj::createConical(float radius, float height){
    TKGeometryObj *obj = new TKGeometryObj;
    if(obj->initConical(radius, height) == false){
        delete obj;
        obj = nullptr;
    }
    return obj;
}

bool TKGeometryObj::initConical(float radius, float height){
    if(this->initialize()==false){
        return false;
    }
    m_conicalPara = new conical_para;
    m_conicalPara->radius = radius;
    m_conicalPara->height = height;

    uint32_t m_pointNum = 32;
    m_pointNum = m_pointNum+2;
    m_points = (TKVertice *)malloc(sizeof(TKVertice)*m_pointNum);
    m_points[0] = TKVertice(Float3(0.0, 0.0, height), Float3(0.0, 0.0, 1.0),
                            Float4(1.0, 1.0, 1.0, 1.0), Float2(0.5, 0.5));
    for(uint i=0; i<=m_pointNum; ++i){
        float angle = (float)i/(float)m_pointNum*M_PI*2.0;
        m_points[i+1] = TKVertice(Float3(radius*cos(angle), radius*sin(angle), 0.0),
                                  Float3(cos(angle), sin(angle), radius/height),
                                  Float4(1.0, 1.0, 1.0, 1.0),
                                  Float2(0.5*cos(angle), 0.5*sin(angle)));
    }
    m_indexNum = 3*(m_pointNum+1);
    m_indices = (uint32_t *)malloc(sizeof(uint32_t)*m_indexNum);
    for(int i=0; i<m_pointNum; ++i){
        m_indices[3*i] = 0;
        m_indices[3*i+1] = (i+1);
        m_indices[3*i+2] = (i+2);
    }
    this->setVertexBuffer();
    this->setIndexBuffer();
    return true;
}

TKGeometryObj *TKGeometryObj::createRectPlane(float width, float height){
    TKGeometryObj *obj = new TKGeometryObj;
    
    if(obj->initRectPlane(width, height)==false){
        delete obj;
        obj = nullptr;
    }
    return obj;
}

bool TKGeometryObj::initRectPlane(float width, float height){
    if(this->initialize()==false){
        return false;
    }
    m_rectPara = new rect_para;
    m_rectPara->width = width;
    m_rectPara->height = height;

    m_pointNum = 4;
    m_points = (TKVertice *)malloc(sizeof(TKVertice)*m_pointNum);
    m_points[0] = TKVertice(Float3(-0.5f*width, -0.5f*height, 0.0f), Float3(0.0, 0.0, 1.0),
                              Float4(0.5, 0.5, 0.5, 1.0), Float2(0.0, 0.0));
    m_points[1] = TKVertice(Float3(-0.5f*width,  0.5f*height, 0.0f), Float3(0.0, 0.0, 1.0),
                              Float4(0.5, 0.5, 0.5, 1.0), Float2(0.0, 1.0));
    m_points[2] = TKVertice(Float3( 0.5f*width,  0.5f*height, 0.0f), Float3(0.0, 0.0, 1.0),
                              Float4(0.5, 0.5, 0.5, 1.0), Float2(1.0, 1.0));
    m_points[3] = TKVertice(Float3( 0.5f*width, -0.5f*height, 0.0f),Float3(0.0, 0.0, 1.0),
                              Float4(0.5, 0.5, 0.5, 1.0), Float2(1.0, 0.0));
    m_indexNum = 6;
    m_indices = (uint32_t*)malloc(sizeof(uint32_t)*m_indexNum);
    uint32_t indices[] = {1, 0, 3, 2, 1, 3};
    memcpy(m_indices, indices, sizeof(uint32_t)*m_indexNum);
    this->setVertexBuffer();
    this->setIndexBuffer();
    return true;
}

TKGeometryObj *TKGeometryObj::createDisk(float radius){
    TKGeometryObj *disk = new TKGeometryObj;
    if(disk->initDisk(radius)==false){
        delete disk;
        disk = nullptr;
    }
    return disk;
}

bool TKGeometryObj::initDisk(float radius){
    if(this->initialize()==false){
        return false;
    }
    m_diskPara = new disk_para;
    m_diskPara->radius = radius;
    uint32_t sideNum = 32;
    m_pointNum = sideNum+2;
    m_points = (TKVertice *)malloc(sizeof(TKVertice)*m_pointNum);
    m_points[0] = TKVertice(Float3(0.0,0.0, 0.0), Float3(0.0, 0.0, 1.0),
                            Float4(1.0, 1.0, 1.0, 1.0), Float2(0.5, 0.5));
    
    for(uint i=0; i<=sideNum; ++i){
        float angle = (float)i/(float)sideNum*M_PI*2.0;
        m_points[i+1] = TKVertice(Float3(radius*cos(angle), radius*sin(angle), 0.0),
                                  Float3(0.0, 0.0, 1.0), Float4(1.0, 1.0, 1.0, 1.0),
                                  Float2(0.5*cos(angle), 0.5*sin(angle)));
    }
    m_indexNum = 3*(sideNum+1);
    m_indices = (uint32_t *)malloc(sizeof(uint32_t)*m_indexNum);
    for(int i=0; i<sideNum; ++i){
        m_indices[3*i+0] = 0;
        m_indices[3*i+1] = i+1;
        m_indices[3*i+2] = i+2;
    }
    this->setVertexBuffer();
    this->setIndexBuffer();
    
    return true;
}

void TKGeometryObj::setVertexBuffer() {
    VkDevice device = TKBaseInfo::Info()->device;
    VkBufferCreateInfo bufInfo;
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext = nullptr;
    bufInfo.flags = 0;
    bufInfo.size  = m_pointNum*sizeof(TKVertice);
    bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    std::vector<uint32_t> queueFamilies = {
        TKBaseInfo::Info()->graphicsQueueFamily
    };
    bufInfo.queueFamilyIndexCount = queueFamilies.size();
    bufInfo.pQueueFamilyIndices   = queueFamilies.data();
    TK_CHECK(vkCreateBuffer(device, &bufInfo, nullptr, &m_pointBuffer));

    VkMemoryRequirements memReqInfo;
    vkGetBufferMemoryRequirements(device, m_pointBuffer, &memReqInfo);
    VkMemoryAllocateInfo memAllocInfo;
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.pNext = nullptr;
    memAllocInfo.allocationSize = memReqInfo.size;
    memAllocInfo.memoryTypeIndex = TKBaseInfo::getMemoryTypeIndex(memReqInfo);
    vkAllocateMemory(device, &memAllocInfo, nullptr, &m_vertexMemory);
    vkBindBufferMemory(device, m_pointBuffer, m_vertexMemory, 0);
    
    void *pData;
    VkDeviceSize commitBytes;
    vkGetDeviceMemoryCommitment(device, m_vertexMemory, &commitBytes);
    vkMapMemory(device, m_vertexMemory, 0, commitBytes, 0, &pData);
    memcpy(pData, m_points, sizeof(TKVertice)*m_pointNum);
    vkUnmapMemory(device, m_vertexMemory);
}

void TKGeometryObj::setIndexBuffer() {
    VkDevice device = TKBaseInfo::Info()->device;
    VkBufferCreateInfo bufInfo;
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext = nullptr;
    bufInfo.flags = 0;
    bufInfo.size  = m_indexNum*sizeof(uint32_t);
    bufInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    std::vector<uint32_t> queueFamilies = {
        TKBaseInfo::Info()->graphicsQueueFamily
    };
    bufInfo.queueFamilyIndexCount = queueFamilies.size();
    bufInfo.pQueueFamilyIndices   = queueFamilies.data();
    TK_CHECK(vkCreateBuffer(device, &bufInfo, nullptr, &m_indiceBuffer));

    VkMemoryRequirements memReqInfo;
    vkGetBufferMemoryRequirements(device, m_indiceBuffer, &memReqInfo);
    VkMemoryAllocateInfo memAllocInfo;
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.pNext = nullptr;
    memAllocInfo.allocationSize = memReqInfo.size;
    memAllocInfo.memoryTypeIndex = TKBaseInfo::getMemoryTypeIndex(memReqInfo);
    vkAllocateMemory(device, &memAllocInfo, nullptr, &m_indiceMemory);
    vkBindBufferMemory(device, m_indiceBuffer, m_indiceMemory, 0);
    void *pData;
    VkDeviceSize commitBytes;
    vkGetDeviceMemoryCommitment(device, m_indiceMemory, &commitBytes);
    vkMapMemory(device, m_indiceMemory, 0, commitBytes, 0, &pData);
    memcpy(pData, m_indices, sizeof(uint32_t)*m_indexNum);
    vkUnmapMemory(device, m_indiceMemory);
}


void TKGeometryObj::draw(VkCommandBuffer cmdBuf, uint32_t swapIdx){
	TKLog("draw 1\n");
    if(m_pipeline != nullptr && m_pointNum>0 && m_indexNum>0){
		TKLog("draw 2 point num = %d, index num = %d\n", m_pointNum, m_indexNum);
		void *pData;
		VkDeviceSize commitBytes;
		VkDevice device = TKBaseInfo::Info()->device;
		vkGetDeviceMemoryCommitment(device, m_vertexMemory, &commitBytes);
		vkMapMemory(device, m_vertexMemory, 0, commitBytes, 0, &pData);
		memcpy(pData, m_points, sizeof(TKVertice)*m_pointNum);
		vkUnmapMemory(device, m_vertexMemory);
         
		vkGetDeviceMemoryCommitment(device, m_indiceMemory, &commitBytes);
		vkMapMemory(device, m_indiceMemory, 0, commitBytes, 0, &pData);
		memcpy(pData, m_indices, sizeof(uint32_t)*m_indexNum);
		vkUnmapMemory(device, m_indiceMemory);
		
		this->bindUniforms(swapIdx);
		VkPipelineLayout layout = m_pipeline->pipelineLayout();
		VkDescriptorSet descSet = m_pipeline->descriptorSet(m_drawIdx);
		vkUpdateDescriptorSets(device, m_writeDescSets.size(), m_writeDescSets.data(),
							   m_copyDescSets.size(), m_copyDescSets.data());
		vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS,
								layout, 0, 1, &descSet, 0, nullptr);
         
		VkDeviceSize offsets[] = {0, sizeof(float)*3, sizeof(float)*6, sizeof(float)*10};   
		vkCmdBindVertexBuffers(cmdBuf, 0, 1, &m_pointBuffer, offsets);
		vkCmdBindIndexBuffer(cmdBuf, m_indiceBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(cmdBuf, m_indexNum, 1, 0, 0, 0);
		m_writeDescSets.empty();
		TKLog("TKGeometryObj draw!\n");
    }
	
    TKNodeList *lst = m_subList->next;
    while(lst != nullptr){
        TKGeometryObj *obj = static_cast<TKGeometryObj *>(lst->node);
        if(nullptr != obj){ obj->draw(cmdBuf, swapIdx); }
        lst = lst->next;
    }    
}
