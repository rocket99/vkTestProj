//
//
//

#ifndef __PolyScene_h__
#define __PolyScene_h__

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#include "../../src/TKEngine.h"

class PolyScene:public TKScene{
public:
	PolyScene();
	~PolyScene();

	static PolyScene *create();
	bool init();

protected:
	void drawObjects() override;

	void initData();
	void initVertexBuffer();
	void initIndexBuffer();
	
private:
	TKJsonPipeline *m_pipeline;

	float *m_points;
	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexMemory;
	
	uint32_t *m_indexes;
	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexMemory;
	
	uint32_t getMemoryTypeIndex(const VkMemoryRequirements &memReq,
								VkMemoryPropertyFlags flag);

	VkCommandBuffer getCommandBuffer(bool begin);
	void flushCommandBuffer(VkCommandBuffer cmdBuf);
};

#endif

