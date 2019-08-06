
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#include "PolyScene.h"

PolyScene::PolyScene(){
	m_pipeline = nullptr;
}

PolyScene::~PolyScene(){
	if (m_pipeline != nullptr){
		delete m_pipeline;
	}
}

PolyScene *PolyScene::create(){
	PolyScene *scene = new PolyScene();
	if(scene->init()==false){
		delete scene;
		scene = nullptr;
	}
	return scene;
}

bool PolyScene::init(){
	if(this->TKScene::init(SCREEN_WIDTH, SCREEN_HEIGHT)==false){
		return false;
	}
	m_pipeline = TKJsonPipeline::createFromJson("basic_pipeline_0.json");
	this->initData();
	this->initIndexBuffer();
	this->initVertexBuffer();

	return true;
}

void PolyScene::drawObjects(){
/*
	VkDevice device = VK_INFO->device;
	void *pData;
	VkDeviceSize commitBytes;

	vkGetDeviceMemoryCommitment(device, m_vertexMemory, &commitBytes);
	vkMapMemory(device, m_vertexMemory, 0, commitBytes, 0, &pData);
	memcpy(pData, m_points, sizeof(float)*99);
	vkUnmapMemory(device, m_vertexMemory);

	vkGetDeviceMemoryCommitment(device, m_indexMemory, &commitBytes);
	vkMapMemory(device, m_indexMemory, 0, commitBytes, 0, &pData);
	memcpy(pData, m_points, sizeof(uint32_t)*96);
	vkUnmapMemory(device, m_indexMemory);
*/	
	VkDeviceSize offsets[] = {0};
	VkCommandBuffer cmdBuf = VK_INFO->commandBuffers[m_currentIdx];
	vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->pipeline());
	vkCmdBindVertexBuffers(cmdBuf, 0, 1, &m_vertexBuffer, offsets);
	vkCmdBindIndexBuffer(cmdBuf, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	vkCmdDrawIndexed(cmdBuf, 12, 1, 0, 0, 0);
}

void PolyScene::initData(){
	m_points = (float*)malloc(sizeof(float)*3*33);
	m_points[0] = m_points[1] = m_points[2] = 0.0;
	for(uint32_t i=1; i<33; ++i){
		float angle = (float)(i-1)/16.0*M_PI;
		m_points[3*i+0] = 0.6*cos(angle);
		m_points[3*i+1] = 0.6*sin(angle);
		m_points[3*i+2] = 0.0;
	}

	m_indexes = (uint32_t *)malloc(sizeof(uint32_t)*3*32);
	for (uint32_t i = 0; i < 32; ++i) {
		m_indexes[3*i] = 0;
		m_indexes[3*i+1] = i+1;
		m_indexes[3*i+2] = i+2 > 32 ? 1 : i+2;
	}
	
/*
	for(int i=0; i<33; ++i){
		printf("[%d]:(%f, %f, %f)\n", i, m_points[3*i], m_points[3*i+1], m_points[3*i+2]);
	}
	for (uint32_t i = 0; i < 32; ++i) {
		printf("[%d, %d, %d]\n", m_indexes[3*i], m_indexes[3*i+1], m_indexes[3*i+2]);
	}	
*/

}

void PolyScene::initVertexBuffer(){
	VkBuffer stageBuffer = VK_NULL_HANDLE;
	VkDeviceMemory stageMemory = VK_NULL_HANDLE;
	
	VkDevice device = VK_INFO->device;
	VkBufferCreateInfo bufferInfo;
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.flags = 0;
	bufferInfo.pNext = nullptr;
	bufferInfo.size = 33*3*4;
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	std::vector<uint32_t> queueFamilies = {
		VK_INFO->graphicsQueueFamily
	};
	bufferInfo.queueFamilyIndexCount = queueFamilies.size();
	bufferInfo.pQueueFamilyIndices = queueFamilies.data();
	TK_CHECK(vkCreateBuffer(device, &bufferInfo, nullptr, &stageBuffer));

	VkMemoryRequirements memReqInfo;
	vkGetBufferMemoryRequirements(device, stageBuffer, &memReqInfo);
	VkMemoryAllocateInfo memAllocInfo;
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.pNext = nullptr;
	memAllocInfo.allocationSize = memReqInfo.size;
	VkMemoryPropertyFlags flag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	memAllocInfo.memoryTypeIndex = this->getMemoryTypeIndex(memReqInfo, flag);
	TK_CHECK(vkAllocateMemory(device, &memAllocInfo, nullptr, &stageMemory));
	
	void *pData;
	TK_CHECK(vkMapMemory(device, stageMemory, 0, memAllocInfo.allocationSize, 0, &pData));
	memcpy(pData, m_points, bufferInfo.size);
	vkUnmapMemory(device, stageMemory);
	TK_CHECK(vkBindBufferMemory(device, stageBuffer, stageMemory, 0));

	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	TK_CHECK(vkCreateBuffer(VK_INFO->device, &bufferInfo, nullptr, &m_vertexBuffer));

	vkGetBufferMemoryRequirements(VK_INFO->device, m_vertexBuffer, &memReqInfo);
	memAllocInfo.allocationSize = memReqInfo.size;
	memAllocInfo.memoryTypeIndex = this->getMemoryTypeIndex(memReqInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vkAllocateMemory(VK_INFO->device, &memAllocInfo, nullptr, &m_vertexMemory);
	vkBindBufferMemory(VK_INFO->device, m_vertexBuffer, m_vertexMemory, 0);
	VkCommandBuffer cmdBuf = this->getCommandBuffer(true);
	VkBufferCopy copyRegion = {};
	copyRegion.size = sizeof(float)*99;
	vkCmdCopyBuffer(cmdBuf, stageBuffer, m_vertexBuffer, 1, &copyRegion);
	this->flushCommandBuffer(cmdBuf);

	vkDestroyBuffer(VK_INFO->device, stageBuffer, nullptr);
	vkFreeMemory(VK_INFO->device, stageMemory, nullptr);
}

void PolyScene::initIndexBuffer(){
	VkBuffer stageBuffer;
	VkDeviceMemory stageMemory;
	VkDevice device = VK_INFO->device;
	VkBufferCreateInfo bufferInfo;
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.flags = 0;
	bufferInfo.pNext = nullptr;
	bufferInfo.size = 96*4;
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	std::vector<uint32_t> queueFamilies = {
		VK_INFO->graphicsQueueFamily
	};
	bufferInfo.queueFamilyIndexCount = queueFamilies.size();
	bufferInfo.pQueueFamilyIndices = queueFamilies.data();
	TK_CHECK(vkCreateBuffer(device, &bufferInfo, nullptr, &stageBuffer));

	VkMemoryRequirements memReqInfo;
	vkGetBufferMemoryRequirements(device, stageBuffer, &memReqInfo);
	VkMemoryAllocateInfo memAllocInfo;
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.pNext = nullptr;
	memAllocInfo.allocationSize = memReqInfo.size;
	VkMemoryPropertyFlags flag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	memAllocInfo.memoryTypeIndex = this->getMemoryTypeIndex(memReqInfo, flag);
	vkAllocateMemory(device, &memAllocInfo, nullptr, &stageMemory);
   
	void *pData;
	vkMapMemory(device, stageMemory, 0, 96*4, 0, &pData);
	memcpy(pData, m_points, sizeof(uint32_t)*96);
	vkUnmapMemory(device, stageMemory);
	TK_CHECK(vkBindBufferMemory(device, stageBuffer, stageMemory, 0));


	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	TK_CHECK(vkCreateBuffer(VK_INFO->device, &bufferInfo, nullptr, &m_indexBuffer));

	vkGetBufferMemoryRequirements(VK_INFO->device, m_indexBuffer, &memReqInfo);
	memAllocInfo.allocationSize = memReqInfo.size;
	memAllocInfo.memoryTypeIndex = this->getMemoryTypeIndex(memReqInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vkAllocateMemory(VK_INFO->device, &memAllocInfo, nullptr, &m_indexMemory);
	vkBindBufferMemory(VK_INFO->device, m_indexBuffer, m_indexMemory, 0);
	VkCommandBuffer cmdBuf = this->getCommandBuffer(true);
	VkBufferCopy copyRegion = {};
	copyRegion.size = sizeof(float)*99;
	vkCmdCopyBuffer(cmdBuf, stageBuffer, m_indexBuffer, 1, &copyRegion);
	this->flushCommandBuffer(cmdBuf);

	vkDestroyBuffer(VK_INFO->device, stageBuffer, nullptr);
	vkFreeMemory(VK_INFO->device, stageMemory, nullptr);	
}

uint32_t PolyScene::getMemoryTypeIndex(const VkMemoryRequirements &memReq, VkMemoryPropertyFlags flag){
	VkPhysicalDeviceMemoryProperties memoryInfo = VK_INFO->physicalMemoryInfo;
	
	uint32_t memoryTypeIdx = UINT32_MAX;
	
    for (uint32_t i=0; i<VK_INFO->physicalMemoryInfo.memoryTypeCount; ++i){
		VkMemoryPropertyFlags tmpPropertyFlag = memoryInfo.memoryTypes[i].propertyFlags;
		if ((memReq.memoryTypeBits & 1) == 1){
			if((tmpPropertyFlag & flag) == flag){
				memoryTypeIdx = i;
				break;
			}
		}
    }
	printf("memory type index %d\n", memoryTypeIdx);
    return memoryTypeIdx;
}

VkCommandBuffer PolyScene::getCommandBuffer(bool begin){
	VkCommandBuffer cmdBuf;
	VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
	cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufAllocateInfo.commandPool = VK_INFO->commandPool;
	cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufAllocateInfo.commandBufferCount = 1;
	vkAllocateCommandBuffers(VK_INFO->device, &cmdBufAllocateInfo, &cmdBuf);
	if (begin){
		VkCommandBufferBeginInfo cmdBufInfo;
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(cmdBuf, &cmdBufInfo);
	}
	return cmdBuf;
}

void PolyScene::flushCommandBuffer(VkCommandBuffer cmdBuf){
	vkEndCommandBuffer(cmdBuf);
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuf;
	VkFenceCreateInfo fenceInfo;
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = 0;
	VkFence fence;
	vkCreateFence(VK_INFO->device, &fenceInfo, nullptr, &fence);
	vkQueueSubmit(VK_INFO->graphicsQueue, 1, &submitInfo, fence);
	vkWaitForFences(VK_INFO->device, 1, &fence, VK_TRUE, INT_MAX);
	vkDestroyFence(VK_INFO->device, fence, nullptr);
	vkFreeCommandBuffers(VK_INFO->device, VK_INFO->commandPool, 1, &cmdBuf);
}

