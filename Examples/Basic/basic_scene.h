//
//baisc_scene.h
//2018-12-23

#ifndef __Basic_Scene_h__
#define __Basic_Scene_h__

#include "TKEngine.h"

class BasicScene:public TKScene
{
public:
    BasicScene();
    ~BasicScene();
    static BasicScene *createScene(uint32_t width, uint32_t height);
    bool initWithSize(uint32_t width, uint32_t height);
protected:
    void updateDrawCommand();
    void drawObjects();
private:
    float theta = 0.0f;
    
    TKCamera *m_camera;
    Float3 m_spaceSize;
    TKUniform *m_uniform;
};

#endif
