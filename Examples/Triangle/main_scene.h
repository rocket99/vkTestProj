//
//main_scene.h
//

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#include "../../src/TKEngine.h"

class MainScene: public TKScene
{
public:
	MainScene();
	~MainScene();
	static MainScene *create();
	bool initScene();

protected:
	void drawObjects();
private:
	TKJsonPipeline *m_pipeline;

};


