// chaper02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "canvas.h"
#include "color.h"

void colorUnitTest();
void canvasUnitTest();

int main()
{
    colorUnitTest();
    canvasUnitTest();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

void colorUnitTest()
{
    std::cout << "constuction test.  c1 is defalut, c2 is (-0.5,0.4,1.7)" << std::endl;
    color c1;
    color c2(-0.5f, 0.4f, 1.7f);

    std::cout << "c1 red is   :" << c1.r() << std::endl;
    std::cout << "c1 green is :" << c1.g() << std::endl;
    std::cout << "c1 blue is  :" << c1.b() << std::endl;
    std::cout << "c2 red is   :" << c2.r() << std::endl;
    std::cout << "c2 green is :" << c2.g() << std::endl;
    std::cout << "c2 blue is  :" << c2.b() << std::endl;

    std::cout << "\naddition test. c3(0.9, 0.6, 0.75) c4(0.7,0.1, 0.25)" << std::endl;
    color c3(0.9f, 0.6f, 0.75f);
    color c4(0.7f, 0.1f, 0.25f);
    color c5 = c3 + c4;
    std::cout << "c5 red is   :" << c5.r() << std::endl;
    std::cout << "c5 green is :" << c5.g() << std::endl;
    std::cout << "c5 blue is  :" << c5.b() << std::endl;

    std::cout << "\nsubtraction test. c3(0.9, 0.6, 0.75) c4(0.7,0.1, 0.25)" << std::endl;
    c5 = c3 - c4;
    std::cout << "c5 red is   :" << c5.r() << std::endl;
    std::cout << "c5 green is :" << c5.g() << std::endl;
    std::cout << "c5 blue is  :" << c5.b() << std::endl;

    std::cout << "\nmultiplication test. c6(0.2,0.3,0.4) * 2"<< std::endl;
    color c6(0.2f, 0.3f, 0.4f);
    c5 = c6 * 2;
    std::cout << "c5 red is   :" << c5.r() << std::endl;
    std::cout << "c5 green is :" << c5.g() << std::endl;
    std::cout << "c5 blue is  :" << c5.b() << std::endl;
    std::cout << "multilication test. 2*c6" << std::endl;
    c5 = 2 * c6;
    std::cout << "c5 red is   :" << c5.r() << std::endl;
    std::cout << "c5 green is :" << c5.g() << std::endl;
    std::cout << "c5 blue is  :" << c5.b() << std::endl;

    std::cout << "\nHadamard product, c7(1, 0.2, 0.4)*c8(0.9, 1, 0.1)" << std::endl;
    color c7(1.0f, 0.2f, 0.4f);
    color c8(0.9f, 1.0f, 0.1f);
    color c9 = c7 * c8;
    std::cout << "c9 red is   :" << c9.r() << std::endl;
    std::cout << "c9 green is :" << c9.g() << std::endl;
    std::cout << "c9 blue is  :" << c9.b() << std::endl;

    std::cout << "\nTesting output of c9 to std::cout (overloading stream insertion operator" << std::endl;
    std::cout << c9 << std::endl;
}


void canvasUnitTest()
{
    std::cout << "\n\nconstruction tests cnvs1 10x20, all black; cnvs2 5x5 all (0.1f, 0.2f, 0.3f)" << std::endl;
    canvas cnvs1(10, 20);
    canvas cnvs2(5, 5, color(0.1f, 0.2f, 0.3f));
    std::cout << "canvas 1" << std::endl;
    cnvs1.print();
    std::cout << "canvas 2" << std::endl;
    cnvs2.print();

    std::cout << "\nsetting a pixel, setColor(2,3, color(0.0f, 1.0f, 0.0f)" << std::endl;
    cnvs1.setColor(2, 3, color(0.0f, 1.0f, 0.0f));
    cnvs1.print();
    cnvs1.setColor(10, 19, color(0.0f, 1.0f, 0.0f));     //should fail
    cnvs1.setColor(9, 20, color(0.0f, 1.0f, 0.0f));      //should fail

    std::cout << "\nwritting a PPM file" << std::endl;
    canvas cnvs3(5, 3);
    color c1(1.5, 0, 0);
    color c2(0, 0.5, 0);
    color c3(-0.5, 0, 1);
    cnvs3.setColor(0, 0, c1);
    cnvs3.setColor(2, 1, c2);
    cnvs3.setColor(4, 2, c3);
    std::cout << "writing to a file" << std::endl;
    cnvs3.writePPM("./test.ppm");

    std::cout << "\nwritting long lines to PPM file" << std::endl;
    color c4(1.0f, 0.8f, 0.6f);
    canvas cnvs4(10, 2, c4);
    cnvs4.writePPM("./test1.ppm");

}



