//
//TKObject.h
//2018-11-21

#ifndef __TKObject_h__
#define __TKObject_h__

#include "../TKPrefix.h"

class TKObject {
public:
    TKObject();
    virtual ~TKObject();
    void setTag(int tag);
    uint32_t retainCount();
    void retain();
    void release();
protected:
    bool initObject();
    int32_t m_tag;
    uint32_t m_retainCount;
};

class TKObjectPool {
public:
    TKObjectPool();
    ~TKObjectPool();
    static TKObjectPool *sharedPool();
    static void purge();
    void addNewobject(TKObject *obj);
    void startWatch();
    void stopWatch();
private:
    std::vector<TKObject*> m_allObjects;
    std::vector<TKObject*> m_objCache;
    bool m_isEnd;
    std::thread m_watchThread;
    std::mutex m_endLock, m_cacheLock;
    void watchFunc();
};

#endif





