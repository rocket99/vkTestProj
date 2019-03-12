//
//
//
#ifndef __TKMatrix_h__
#define __TKMatrix_h__

#include "TKVector.h"
#include "TKDataType.h"

class TKMatrix {
public:
    TKMatrix();
    ~TKMatrix();
    TKMatrix(uint row, uint col);
    TKMatrix(const TKMatrix &mat);
    std::string desc();
    std::string desc() const;
    TKMatrix copy();

    void mapToBuffer(float *buf);
    void mapToBuffer(float *buf) const;
    
    static TKMatrix identity(int rank);
    uint row() const;
    uint column() const;
    
    bool operator != (const TKMatrix &mat);
    bool operator == (const TKMatrix &mat);
    void operator =  (const TKMatrix &mat);
    
    //TKMatrix mat; mat[N][M] is the value of row M in column N;
    TKVector& operator [] (uint idx); //return the column at idx;
    TKVector  operator [] (uint idx) const;

    void operator += (const TKMatrix &mat);
    void operator -= (const TKMatrix &mat);
    void operator *= (const TKMatrix &mat);
    
    void exchangeRow(uint row0, uint row1);
    void exchangeColumn(uint col0, uint col1);

    TKMatrix cross(const TKMatrix &mat) throw (std::exception);

    static TKMatrix transpose(const TKMatrix &mat);
    TKMatrix transposed();
    
    friend TKMatrix operator + (const TKMatrix &mat0, const TKMatrix &mat1);
    friend TKMatrix operator - (const TKMatrix &mat0, const TKMatrix &mat1);
    friend TKMatrix operator * (const TKMatrix &mat0, const TKMatrix &mat1);
    
    TKVector rowVec(int idx);
    TKVector columnVec(int idx);

    TKMatrix adjunct(int row, int col);
    TKMatrix adjunct(int row, int col) const;
    
    static float det(const TKMatrix &mat);

    static TKMatrix perspect(float fovy, float aspect, float zNear, float zFar);
    static TKMatrix lookAt(const Float3 &center, const Float3 &pos, const Float3 &up);
    static TKMatrix rotation(const Float3 &axis, float angle);

protected:
    uint m_row, m_col;
    std::vector<TKVector *> m_vecArr;
};


#endif




