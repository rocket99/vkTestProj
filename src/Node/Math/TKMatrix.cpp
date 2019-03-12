//
//
//
#include "TKMatrix.h"

TKMatrix::TKMatrix(){
    m_row = 0; m_col = 0;
    m_vecArr.clear();
}

TKMatrix::~TKMatrix(){
    for (int i=0; i<m_col; ++i) {
        delete m_vecArr[i];
    }
    m_vecArr.clear();
}

TKMatrix::TKMatrix(uint row, uint col) {
    m_row = row;
    m_col = col;
    for (int i=0; i<col; ++i) {
        TKVector *vec = new TKVector(0.0, row);
        m_vecArr.push_back(vec);
    }
}

TKMatrix::TKMatrix(const TKMatrix &mat){
    m_row = mat.m_row;
    m_col = mat.m_col;
    for(int i=0; i<m_col; ++i){
        TKVector *vec = new TKVector(0.0, m_row);
        mat.m_vecArr[i]->copyTo(*vec);
        m_vecArr.push_back(vec);
    }
}

TKMatrix TKMatrix::identity(int rank){
    TKMatrix ret(rank, rank);
    for(int i=0; i<rank; ++i){
        ret[i][i] = 1.0;
    }
    return ret;
}

uint TKMatrix::row() const {
    return m_row;
}

uint TKMatrix::column() const {
    return m_col;
}

TKMatrix TKMatrix::copy(){
    TKMatrix mat(m_row, m_col);
    for(int i=0; i<m_col; ++i){
        m_vecArr[i]->copyTo(mat[i]);
    }
    return mat;
}

void TKMatrix::mapToBuffer(float *buf) {
    if( buf==nullptr ){
        return;
    }
    for(int i=0; i<m_col; ++i){
        memcpy(&buf[m_row*i], m_vecArr[i]->data(), sizeof(float)*m_row);
    }
}

void TKMatrix::mapToBuffer(float *buf) const {
    if( buf==nullptr ){
        return;
    }
    for(int i=0; i<m_col; ++i){
        memcpy(&buf[m_row*i], m_vecArr[i]->data(), sizeof(float)*m_row);
    }
}

std::string TKMatrix::desc(){
    std::string str("");
    char buf[512];
    for(int i=0; i<m_row; ++i){
        for(int j=0; j<m_col; ++j){
            sprintf(buf, "%.3f, ", (*m_vecArr[j])[i]);
            str.append(buf);
        }
        str.append("\n");
    }
    return str;
}

std::string TKMatrix::desc() const{
    std::string str("");
    char buf[512];
    for(int i=0; i<m_row; ++i){
        for(int j=0; j<m_col; ++j){
            sprintf(buf, "%.3f, ", (*m_vecArr[j])[i]);
            str.append(buf);
        }
        str.append("\n");
    }
    return str;
}

void TKMatrix::operator = (const TKMatrix &mat){
    for (int i=0; i<m_col; ++i) {
        delete m_vecArr[i];
    }
    m_vecArr.clear();
    
    m_row = mat.m_row;
    m_col = mat.m_col;
    for(int i=0; i<m_col; ++i){
        TKVector *vec = new TKVector(0.0, m_row);
        mat.m_vecArr[i]->copyTo(*vec);
        m_vecArr.push_back(vec);
    }
}

bool TKMatrix::operator != (const TKMatrix &mat){
    if(m_row != mat.m_row || m_col != mat.m_col){
        return true;
    }
    bool ret = true;
    for(int i=0; i<m_col; ++i){
        if(m_vecArr[i] == mat.m_vecArr[i]){
            return false;
            break;
        }
    }
    return ret;
}

bool TKMatrix::operator == (const TKMatrix &mat){
    if(m_row != mat.m_row || m_col != mat.m_col){
        return false;
    }
    bool ret = true;
    for(int i=0; i<m_col; ++i){
        if(*(m_vecArr[i]) == *(mat.m_vecArr[i])){
           
        }else{
            return false;
            break;
        }
    }
    return ret;
}

TKVector& TKMatrix::operator [] (uint idx){
    return *m_vecArr[idx%m_col];
}

TKVector  TKMatrix::operator [] (uint idx) const {
    if(idx >= m_col){
        return TKVector(0.0f, m_row);
    }
    return m_vecArr[idx%m_col]->copy();
}

void TKMatrix::operator += (const TKMatrix &mat){
    int num = m_col<mat.m_col ? m_col : mat.m_col;
    for(int i=0; i<num; ++i){
        *m_vecArr[i] += *mat.m_vecArr[i];
    }
}

void TKMatrix::operator -= (const TKMatrix &mat){
    int num = m_col<mat.m_col ? m_col : mat.m_col;
    for(int i=0; i<num; ++i){
        *m_vecArr[i] -= *mat.m_vecArr[i];
    }
}

void TKMatrix::operator *= (const TKMatrix &mat){
    uint num = m_col<mat.m_col ? m_col : mat.m_col;
    for(int i=0; i<num; ++i){
        *m_vecArr[i] *= *mat.m_vecArr[i];
    }
}

void TKMatrix::exchangeRow(uint row0, uint row1){
    if (row0>=m_row || row1>=m_row) {
        return;
    }
    for (int i=0; i<m_col; ++i) {
        float temp = (*m_vecArr[i])[row0];
        (*m_vecArr[i])[row0] = (*m_vecArr[i])[row1];
        (*m_vecArr[i])[row1] = temp;
    }
}

void TKMatrix::exchangeColumn(uint col0, uint col1){
    if(col0>=m_col || col1>=m_col){
        return;
    }
    (*m_vecArr[col0]).swap(*m_vecArr[col1]);
}

TKMatrix TKMatrix::transposed() {
    TKMatrix ret(m_col, m_row);
    for(int i=0; i<m_col; ++i){
        for(int j=0; j<m_row; ++j){
            ret[j][i] = (*m_vecArr[i])[j];
        }
    }
    return ret;
}

TKMatrix TKMatrix::cross(const TKMatrix &mat) throw (std::exception){
    if (this->column() != mat.row()) {
        throw std::out_of_range("matrix cross range error!");
    }

    TKMatrix transMat = this->transposed();
    TKMatrix ret(row(), mat.column());
    for(int i=0; i<row(); ++i){
        for(int j=0; j<mat.column(); ++j){
            /*
            float sum = 0;
            for(int k=0; k<column(); ++k){
                sum += (*m_vecArr[k])[i]*mat[j][k];
            }
            ret[j][i] = sum;*/
            ret[j][i] = transMat[i].dot(mat[j]);
        }
    }
    return ret;
}

TKMatrix TKMatrix::transpose(const TKMatrix &mat){
    TKMatrix ret(mat.column(), mat.row());
    for(int i=0; i<mat.row(); ++i){
        for(int j=0; j<mat.column(); ++j){
            ret[i][j] = mat[j][i];
        }
    }
    return ret;
}

TKMatrix operator + (const TKMatrix &mat0, const TKMatrix &mat1){
    int row = mat0.m_row>mat1.m_row ? mat0.m_row : mat1.m_row;
    int col = mat0.m_col>mat1.m_col ? mat0.m_col : mat1.m_col;
    TKMatrix mat(row, col);
    for(int i=0; i<col; ++i){
        mat[i] = mat0[i] + mat1[i];
    }
    return mat;
}

TKMatrix operator - (const TKMatrix &mat0, const TKMatrix &mat1){
    int row = mat0.m_row>mat1.m_row ? mat0.m_row : mat1.m_row;
    int col = mat0.m_col>mat1.m_col ? mat0.m_col : mat1.m_col;
    TKMatrix mat(row, col);
    for(int i=0; i<col; ++i){
        mat[i] = mat0[i] - mat1[i];
    }
    return mat;
}

TKMatrix operator * (const TKMatrix &mat0, const TKMatrix &mat1){
    int row = mat0.m_row>mat1.m_row ? mat0.m_row : mat1.m_row;
    int col = mat0.m_col>mat1.m_col ? mat0.m_col : mat1.m_col;
    TKMatrix mat(row, col);
    for(int i=0; i<col; ++i){
        mat[i] = mat0[i] * mat1[i];
    }
    return mat;
}

TKVector TKMatrix::rowVec(int idx){
    TKVector vec(0.0f, m_col);
    for(int i=0; i<m_col; ++i){
        vec[i] = (*m_vecArr[i])[idx];
    }
    return vec;
}

TKVector TKMatrix::columnVec(int idx){
    TKVector vec(0.0f, m_row);
    for(int i=0; i<m_row; ++i){
        vec[i] = (*m_vecArr[idx])[i];
    }
    return vec;
}

TKMatrix TKMatrix::adjunct(int row, int col){
    if(row<0 || row>=m_row || col<0 || col>=m_col){
        return TKMatrix();
    }
    TKMatrix mat(m_row-1, m_col-1);
    int idx_x = 0;
    for(int i=0; i<m_col; ++i){
        if (i==col){
            continue;
        }
        int idx_y = 0;
        for(int j=0; j<m_row; ++j){
            if(j==row){
                continue;
            }
            mat[idx_x][idx_y] = (*m_vecArr[i])[j];
            ++ idx_y;
        }
        ++ idx_x;
    }
    return mat;
}

TKMatrix TKMatrix::adjunct(int row, int col) const {
    if(row<0 || row>=m_row || col<0 || col>=m_col){
        return TKMatrix();
    }
    TKMatrix mat(m_row-1, m_col-1);
    int idx_x = 0;
    for(int i=0; i<m_col; ++i){
        if (i==col){
            continue;
        }
        int idx_y = 0;
        for(int j=0; j<m_row; ++j){
            if(j==row){
                continue;
            }
            mat[idx_x][idx_y] = (*m_vecArr[i])[j];
            ++ idx_y;
        }
        ++ idx_x;
    }
    return mat;
}

float TKMatrix::det(const TKMatrix &mat){
    if (mat.m_row != mat.m_col){
        return 0.0f;
    }
    
    if(mat.m_row == 2){
        return mat[0][0]*mat[1][1]-mat[1][0]*mat[0][1]; 
    }

    float sum = 0.0f;
    for(int i=0; i<mat.column(); ++i){
        sum += i%2==0 ?
            mat[i][0] * TKMatrix::det(mat.adjunct(0, i)) :
            -mat[i][0] * TKMatrix::det(mat.adjunct(0, i));
    }
    return sum;
}

TKMatrix TKMatrix::lookAt(const Float3 &center, const Float3 &pos, const Float3 &up){
    Float3 nz = (center-pos).normal();
    Float3 ny = up.normal();
    Float3 nx = (nz.cross(ny)).normal();
    ny = (nx.cross(nz)).normal();
    
    TKMatrix mat(4, 4);
    mat[0] = TKVector(4, nx.x, nx.y, nx.z, nx.dot(pos));
    mat[1] = TKVector(4, ny.x, ny.y, ny.z, ny.dot(pos));
    mat[2] = TKVector(4, nz.x, nz.y, nz.z, nz.dot(pos));
    mat[3] = TKVector(4, 0.0, 0.0, 0.0, 1.0);
    return mat;
}

TKMatrix TKMatrix::perspect(float fovy, float aspect,
                            float zNear, float zFar) {
    TKMatrix mat(4, 4);
    float cotA = cos(0.5*fovy)/sin(0.5*fovy);
    mat[0] = TKVector(4, cotA/aspect, 0.0, 0.0, 0.0);
    mat[1] = TKVector(4, 0.0, cotA, 0.0, 0.0);
    mat[2] = TKVector(4, 0.0, 0.0, (zNear+zFar)/(zNear-zFar),
                      2.0*zNear*zFar/(zNear-zFar));
    mat[3] = TKVector(4, 0.0, 0.0, -1.0, 0.0);
    return mat;
}

TKMatrix TKMatrix::rotation(const Float3 &axis, float angle){
    float ca = cos(angle);
    float sa = sin(angle);
    Float3 n = axis.normal();
    TKMatrix mat(4, 4);
    mat[0] = TKVector(4, n.x*n.x*(1.0-ca)+ca, n.x*n.y*(1-ca)-n.z*sa,
                      n.x*n.z*(1-ca)+n.y*sa, 0.0);
    mat[1] = TKVector(4, n.x*n.y*(1.0-ca)+n.z*sa, n.y*n.y*(1-ca)+ca,
                      n.y*n.z*(1-ca)-n.x*sa, 0.0);
    mat[2] = TKVector(4, n.x*n.z*(1.0-ca)-n.y*sa, n.y*n.z*(1-ca)+n.x*sa,
                      n.z*n.z*(1-ca)+ca, 0.0);
    mat[3] = TKVector(4, 0.0, 0.0, 0.0, 1.0);
    return mat;
}


