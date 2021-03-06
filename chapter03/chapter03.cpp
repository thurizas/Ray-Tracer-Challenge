// chapter03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "matrix2x2.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

#include "vector.h"

void unitTest2x2();
void unitTest3x3();
void unitTest4x4();

int main()
{
    unitTest2x2();
    unitTest3x3();
    unitTest4x4();
}
void unitTest2x2()
{
    std::cout << "construction and displaying mat1(1,3,4,5)" << std::endl;
    matrix2x2  mat1(1, 3, 4, 5);
    matrix2x2  mat2 = mat1.ident();
    std::cout << mat1;
    std::cout << "diplaying the idenity matrix" << std::endl;
    std::cout << mat2;
    std::cout << "element a00 of mat1:" << mat1.a00() << std::endl;
    std::cout << "element a01 of mat1:" << mat1.a01() << std::endl;
    std::cout << "element a10 of mat1:" << mat1.a10() << std::endl;
    std::cout << "element a11 of mat1:" << mat1.a11() << std::endl;

    std::cout << "\narithemetic tests mat3(3,2,5,6)" << std::endl;
    matrix2x2 mat3(3, 2, 5, 6);
    matrix2x2 mat4 = mat1 + mat3;
    std::cout << "mat 1 + mat3 is:\n" << std::endl;
    std::cout << mat4;
    mat4 = mat1 - mat3;
    std::cout << "mat1 - mat3 is:\n" << std::endl;
    std::cout << mat4;
    mat4 = mat1 * mat3;
    std::cout << "mat1 * mat3 is:\n" << std::endl;
    std::cout << mat4;
    std::cout << "mat1 == mat3? " << (mat1 == mat3) << std::endl;
    std::cout << "mat1 == mat1? " << (mat1 == mat1) << std::endl;

    matrix2x2 mat5(2, 2, 1, 1);
    std::cout << "determinate of mat3 is: " << mat3.det() << std::endl;;
    std::cout << "determinate of mat5(2,2,1,1) is: " << mat5.det() << std::endl;
    std::cout << "inverse of mat3 is: \n" << mat3.inverse() << std::endl;
    std::cout << "inverse of mat5 is: \n" << mat5.inverse() << std::endl;
    std::cout << "transpose of mat3 is: \n" << mat3.transpose() << std::endl;

}

void unitTest3x3()
{
    std::cout << "construction and displaying mat1(-3,5,0,1,-2,-7,0,1,1)" << std::endl;
    matrix3x3  mat1(-3, 5, 0, 1, -2, -7, 0, 1, 1);
    matrix3x3  mat2 = mat1.ident();
    std::cout << mat1;
    std::cout << "diplaying the idenity matrix" << std::endl;
    std::cout << mat2;
    std::cout << "element a00 of mat1:" << mat1.a00() << std::endl;
    std::cout << "element a01 of mat1:" << mat1.a01() << std::endl;
    std::cout << "element a02 of mat1:" << mat1.a02() << std::endl;
    std::cout << "element a10 of mat1:" << mat1.a10() << std::endl;
    std::cout << "element a11 of mat1:" << mat1.a11() << std::endl;
    std::cout << "element a12 of mat1:" << mat1.a12() << std::endl;
    std::cout << "element a20 of mat1:" << mat1.a20() << std::endl;
    std::cout << "element a21 of mat1:" << mat1.a21() << std::endl;
    std::cout << "element a22 of mat1:" << mat1.a22() << std::endl;

    std::cout << "\narithemetic tests mat3(3, 2, 5, 6,-4, 0, 1, -4, -3)" << std::endl;
    matrix3x3 mat3(3, 2, 5, 6,-4, 0, 1, -4, -3);
    matrix3x3 mat4 = mat1 + mat3;
    std::cout << "mat 1 + mat3 is:\n" << std::endl;
    std::cout << mat4;
    mat4 = mat1 - mat3;
    std::cout << "mat1 - mat3 is:\n" << std::endl;
    std::cout << mat4;
    mat4 = mat1 * mat3;
    std::cout << "mat1 * mat3 is:\n" << std::endl;
    std::cout << mat4;
    std::cout << "mat1 == mat3? " << (mat1 == mat3) << std::endl;
    std::cout << "mat1 == mat1? " << (mat1 == mat1) << std::endl;

    matrix3x3 mat5(1,5,0,-3,2,7,0,6,-3);
    std::cout << "mat5 is given by:" << std::endl;
    std::cout << mat5;

    std::cout << "minor of mat5(0,0) is:" << std::endl;
    matrix2x2 mat6 = mat5.minor(0, 0);
    std::cout << mat6;
    std::cout << "determinate of minor is: " << mat6.det() << std::endl;

    std::cout << "minor of mat5(0,1) is:" << std::endl;
    matrix2x2 mat7 = mat5.minor(0,1);
    std::cout << mat7;
    std::cout << "determinate of minor is " << mat7.det() << std::endl;

    std::cout << "minor of mat5(0,2) is:" << std::endl;
    matrix2x2 mat8 = mat5.minor(0, 2);
    std::cout << mat8;
    std::cout << "determinate of minor is " << mat8.det() << std::endl;

    std::cout << "determinate of mat5 is: " << mat5.det() << std::endl;
    matrix3x3 mat9(0, 0, 6, 0, 0, 7, 2, 2, 0);
    std::cout << "inverse of mat9 is: \n" << mat9.inverse() << std::endl;
    std::cout << "inverse of mat5 is: \n" << mat5.inverse() << std::endl;
    std::cout << "transpose of mat5 is: \n" << mat5.transpose() << std::endl;
}

void unitTest4x4()
{
    std::cout << "construction and displaying mat1(1, 2, 3, 4, 5.5, 6.5, 7.5, 8.5, 9, 10, 11, 12, 13.5, 14.5, 15.5, 16.5)" << std::endl;
     
    matrix4x4  mat1(1, 2, 3, 4, 5.5, 6.5, 7.5, 8.5, 9, 10, 11, 12, 13.5, 14.5, 15.5, 16.5);
    matrix4x4  mat2 = mat1.ident();
    std::cout << mat1;
    std::cout << "diplaying the idenity matrix" << std::endl;
    std::cout << mat2;
    std::cout << "element a00 of mat1:" << mat1.a00() << std::endl;
    std::cout << "element a01 of mat1:" << mat1.a01() << std::endl;
    std::cout << "element a02 of mat1:" << mat1.a02() << std::endl;
    std::cout << "element a03 of mat1:" << mat1.a03() << std::endl;
    std::cout << "element a10 of mat1:" << mat1.a10() << std::endl;
    std::cout << "element a11 of mat1:" << mat1.a11() << std::endl;
    std::cout << "element a12 of mat1:" << mat1.a12() << std::endl;
    std::cout << "element a13 of mat1:" << mat1.a13() << std::endl;
    std::cout << "element a20 of mat1:" << mat1.a20() << std::endl;
    std::cout << "element a21 of mat1:" << mat1.a21() << std::endl;
    std::cout << "element a22 of mat1:" << mat1.a22() << std::endl;
    std::cout << "element a23 of mat1:" << mat1.a23() << std::endl;
    std::cout << "element a30 of mat1:" << mat1.a30() << std::endl;
    std::cout << "element a31 of mat1:" << mat1.a31() << std::endl;
    std::cout << "element a32 of mat1:" << mat1.a32() << std::endl;
    std::cout << "element a33 of mat1:" << mat1.a33() << std::endl;

    std::cout << "\narithemetic tests mat3(3, 2, 5, 6,-4, 0, 1, -4, -3)" << std::endl;
    matrix4x4 mat3(1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2);
    matrix4x4 mat4(-2,1,2,3,3,2,1,-1,4,3,6,5,1,2,7,8);
    std::cout << "mat3 is:\n" << mat3 << std::endl;
    std::cout << "mat4 is:\n" << mat4 << std::endl;
    matrix4x4 mat5 = mat3 + mat4;
    std::cout << "\nmat 3 + mat4 is:\n" << std::endl;
    std::cout << mat5;
    mat5 = mat3 - mat4;
    std::cout << "\nmat3 - mat4 is:\n" << std::endl;
    std::cout << mat5;
    mat5 = mat3 * mat4;
    std::cout << "\nmat3 * mat4 is:\n" << std::endl;
    std::cout << mat5;
    std::cout << "\nmat1 == mat3? " << (mat1 == mat3) << std::endl;
    std::cout << "mat1 == mat1? " << (mat1 == mat1) << std::endl;

    matrix4x4 mat6(0,9,3,0,9,8,0,8,1,8,5,3,0,0,5,8);
    std::cout << "mat6 is:\n" << mat6 << "\nits transpose is:\n" << mat6.transpose() << std::endl;

    matrix4x4 mat7(6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6);
    std::cout << "\nThe determinate of mat7 is; " << mat7.det() << std::endl;

    matrix4x4 mat8(-4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0, 0);
    std::cout << "\nThe determinate of mat8 is; " << mat8.det() << std::endl;
    
    matrix4x4 mat9(8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4);
    std::cout << "\nThe determinate of mat9 is:" << mat9.det() << std::endl;
    std::cout << "The inverse of mat9 is:" << std::endl;
    matrix4x4 invmat9 = mat9.inverse();
    std::cout << invmat9 << std::endl; 

    std::cout << "product of matrix and its inverse:" << std::endl;
    std::cout << mat9 * invmat9 << std::endl;

    std::cout << "product of matrix mat10(1,2,3,4,2,4,4,2,8,6,4,1,0,0,0,1) with vec1(1,2,3)\n";
    matrix4x4 mat10(1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1);
    vector    vec1(1.0f, 2.0f, 3.0f);
    vector    vec2 = mat10 * vec1;
    std::cout << vec2 << std::endl;
}
