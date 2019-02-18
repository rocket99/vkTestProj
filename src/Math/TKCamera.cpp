//
//TKCamera.cpp
//2018-12-27

#include "TKCamera.h"

TKCamera::TKCamera(){
    m_pos = Float3(0.0, 0.0, -1.0);
    m_center = Float3(0.0, 0.0, 0.0);
    m_up = Float3(0.0, 1.0, 0.0);
}

TKCamera::~TKCamera(){
}

TKCamera* TKCamera::createWithLookat(const Float3 &center, const Float3 &pos, const Float3 &up){
    TKCamera *camera = new TKCamera;
    if(camera->initWithLookAtPara(center, pos, up) == false){
        delete camera;
        camera = nullptr;
    }
    return camera;
}

bool TKCamera::initWithLookAtPara(const Float3 &center, const Float3 &pos, const Float3 &up){
    m_fovy = M_PI/3.0; m_aspect = 1.0;
    m_zNear = 0.01;    m_zFar = 100.0;
    m_pos = pos; m_center = center; m_up = up;
    
    Float3 space(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
    Float3 P = m_pos/space;
   
    this->normalized();
    m_lookatMat = TKMatrix::identity(4);
    m_lookatMat[0] = TKVector(4, nx.x, nx.y, nx.z, nx.dot(P));
    m_lookatMat[1] = TKVector(4, ny.x, ny.y, ny.z, ny.dot(P));
    m_lookatMat[2] = TKVector(4, nz.x, nz.y, nz.z, nz.dot(P));
    m_lookatMat[3] = TKVector(4,  0.0,  0.0,  0.0, 1.0);
    m_projMat = TKMatrix::perspect(m_fovy, m_aspect, m_zNear, m_zFar);
    m_viewProjMat = m_projMat.transposed().cross(m_lookatMat.transposed());
    return true;
}

void TKCamera::updateUniformData(){
    this->normalized();
  
    Float3 space(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
    Float3 P = m_pos/space;
    m_lookatMat[0] = TKVector(4, nx.x, nx.y, nx.z,-nx.dot(P));
    m_lookatMat[1] = TKVector(4, ny.x, ny.y, ny.z,-ny.dot(P));
    m_lookatMat[2] = TKVector(4,-nz.x,-nz.y,-nz.z, nz.dot(P));
    m_lookatMat[3] = TKVector(4,  0.0,  0.0,  0.0, 1.0);
    m_viewProjMat = m_projMat.transposed().cross(m_lookatMat.transposed());
}

const TKMatrix &TKCamera::viewProjMatrix() const {
    return m_viewProjMat;
}

void TKCamera::normalized(){
    ny = m_up.normal();
    nz = (m_center-m_pos).normal();
    nx = (nz.cross(ny)).normal();
    ny = (nx.cross(nz)).normal();
}

Float3 TKCamera::getPointCenter() const {
    return m_center;
}

Float3 TKCamera::getPosition() const {
    return m_pos;
}

void TKCamera::setPosition(float x, float y, float z){
    m_pos.x = x; m_pos.y = y; m_pos.z = z;
}

void TKCamera::roll(float angle){
    
}

void TKCamera::pitch(float angle){

}

void TKCamera::yaw(float angle){

}



