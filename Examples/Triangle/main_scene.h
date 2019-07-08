//
//main_scene.h
//


#include "TKEngine.h"

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


