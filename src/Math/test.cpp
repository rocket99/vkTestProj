//test.cpp
//
//
#include "TKMatrix.h"
#include "TKVector.h"
#include "TKDataType.h"

int main(int argc, char *argv[])
{
    
    TKVector vec0 = TKVector(5, 10.0f, 6.3f, 6.2f, 4.0f, 5.9f);
    printf("vec0:%s\n", vec0.desc().c_str());
    TKVector vec1 = TKVector(2.0, 9);
    printf("vec1:%s\n", vec1.desc().c_str());
    TKVector vec2 = vec0*2.0;
    //TKVector vec2 = TKVector(5.7, 8);
    printf("vec0+vec1:%s\n", vec2.desc().c_str());
   
    TKMatrix mat0 = TKMatrix(10, 10);
    mat0[5][2] = 4.8;
    mat0[0][5] = 2.3;
    mat0[3] = vec0;
    printf("%s\n", mat0.desc().c_str());

    TKMatrix mat1(4,4);
    mat1[0] = TKVector(4, 1.0,2.0,3.0,4.0);
    mat1[1] = TKVector(4, 2.0,3.0,1.0,4.0);
    mat1[2] = TKVector(4, 4.0,5.0,2.0,1.0);
    mat1[3] = TKVector(4, 7.0,2.0,1.0,5.0);
    TKMatrix mat2 = TKMatrix::transpose(mat1);
    printf("%s\n", mat1.desc().c_str());
    printf("det(A)= %.3f\n", TKMatrix::det(mat1));
    return 0;
}
