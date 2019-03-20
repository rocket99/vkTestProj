//
//TKObject.cpp
//2018-11-21

#include "TKObject.h"
#include <unistd.h>

TKObject::TKObject(){
    m_retainCount = 0;
    m_tag    = -1;
}

TKObject::~TKObject(){
    TKLog("destruct object %p\n", this);
}

bool TKObject::initObject(){
    m_retainCount = 1;
    TKObjectPool::sharedPool()->addNewobject(this);
    return true;
}

void TKObject::retain(){
    ++ m_retainCount;
}

void TKObject::release(){
    -- m_retainCount;
    assert(m_retainCount >= 0);
}

void TKObject::setTag(int tag){
    m_tag = tag;
}

uint32_t TKObject::retainCount(){
    return m_retainCount;
}

#pragma mark TKObjectPool
static TKObjectPool *inst = nullptr; 
TKObjectPool::TKObjectPool(){
    m_allObjects.clear();
    m_isEnd = false;
}

TKObjectPool::~TKObjectPool(){
    TKLog("object pool deconstruct!\n");
    for(int i=0; i<m_allObjects.size(); ++i){
        TKLog("delete object %p\n", m_allObjects[i]);
        delete m_allObjects[i];
        m_allObjects[i] = nullptr;
    }
    m_allObjects.empty();
}

TKObjectPool *TKObjectPool::sharedPool(){
    if(inst == nullptr){
        inst = new TKObjectPool();       
    }
    return inst;
}

void TKObjectPool::purge(){
    if(inst != nullptr){
        delete inst;
        inst = nullptr;
    }
}

void TKObjectPool::startWatch(){
    std::function<void()> fn = std::bind(&TKObjectPool::watchFunc, this);
    m_watchThread = std::thread(fn);
}

void TKObjectPool::stopWatch(){
    TKLog("objects pool stop watch.\n");
    m_endLock.lock();
    m_isEnd = true;
    m_endLock.unlock();
    m_watchThread.join();
}

void TKObjectPool::watchFunc(){
    TKLog("watch function!\n");
    while(m_isEnd == false){
        m_endLock.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        for(int i=0; i<m_allObjects.size(); ++i){
            if(m_allObjects[i]->retainCount() == 0){
                delete m_allObjects[i];
                m_allObjects[i] = nullptr;
            }
        }
        m_cacheLock.lock();
        for(uint32_t i=0; i<m_objCache.size(); ++i){
            bool isAdded = false;
            for(int j=0; j<m_allObjects.size(); ++j){
                if(m_allObjects[j] == m_objCache[i]){
                    isAdded = true;
                    break;
                }
            }
            if(isAdded == false){
                TKLog("add object %p into pool\n", m_objCache[i]);
                m_allObjects.push_back(m_objCache[i]);
            }
        }
        m_objCache.empty();
        m_cacheLock.unlock();
        std::vector<TKObject *>::iterator iter = m_allObjects.begin();
        while(iter != m_allObjects.end()){
            if(*iter == nullptr){
                m_allObjects.erase(iter);
                iter = m_allObjects.begin();
            }
            ++ iter;
        }
        m_endLock.lock();
    }
    TKLog("watch end!\n");
}

void TKObjectPool::addNewobject(TKObject *obj){
    m_cacheLock.lock();
    m_objCache.push_back(obj);
    m_cacheLock.unlock();
}


