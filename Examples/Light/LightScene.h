//
//LightScene.h
//2019-1-3

#ifndef __Light_Scene_h__
#define __Light_Scene_h__

#include "../../src/TKEngine.h"


class LightScene : public TKScene
{
public:
    LightScene();
    ~LightScene();
    static LightScene *createLightScene(uint32_t lightNum);
    bool initLightScene(uint32_t lightNum);
    
protected:
    void drawObjects();
private:
    void initObjects();
    Float3 m_spaceSize;
    TKCamera *m_camera;
    TKBaseNode *m_rootNode;
    TKUniform *m_cameraUniform;    
    TKLightSource *m_lightSources;
    TKUniform *m_lightUniform;
    TKUniform *m_eyeUniform;
    float angle;

	TKJsonPipeline *m_pipeline;
};

#endif

