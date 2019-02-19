//
//TKScene.h
//
#ifndef __TK_Scene_h__
#define __TK_Scene_h__

#include "../TKPrefix.h"
#include "../Node/TKGeometryObj.h"
#include "TKBaseInfo.h"

class TKScene {
public:
    TKScene();
    virtual ~TKScene();
    static TKScene *scene(uint32_t width, uint32_t height);
    bool init(uint32_t width, uint32_t height);

    void renderDraw();
    void stopDraw();

protected:
    uint32_t m_width;
    uint32_t m_height;
    
    uint32_t m_currentIdx = 0;
    uint32_t m_frameIdx = 0;
    
    std::vector<TKBaseNode *> m_drawNodes;
    void updateDrawCommand();
    virtual void drawObjects();
};

#endif



