#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <string>
#include <bitset>
#include <ctime>
#include <fstream>

#include "./ArrayBuffer.h"
#include "./Uint8Array.h"
#include "./Int8Array.h"
#include "./Int16Array.h"
#include "./Uint16Array.h"
#include "./Uint32Array.h"
#include "./Int32Array.h"
#include "./Float32Array.h"


namespace draco
{

template <typename T>
void showArrayView(T vs, std::string ns = "", int begin = 0, int end = 30)
{
    std::string str;
    if (end > vs.getLength())
    {
        end = vs.getLength();
    }
    for (int i = 0; i < end; ++i)
    {
        if ((i + 1) < end)
        {
            str = str + std::to_string(vs[i]) + ",";
        }
        else
        {
            str = str + std::to_string(vs[i]);
        }
    }
    std::cout << "      " << ns << "(" << vs.getLength() << "), value: " << str << std::endl;
}
template <typename T>
void showArray(T vs, Int32 length, std::string ns = "")
{
    std::string str;
    for (auto i = 0; i < length; ++i)
    {
        if ((i + 1) < length)
        {
            str = str + std::to_string(vs[i]) + ",";
        }
        else
        {
            str = str + std::to_string(vs[i]);
        }
    }
    std::cout << "      " << ns << "(" << length << "), value: " << str << std::endl;
}
//void readFile(std::string path){
//
//    FILE* raw_file_ptr = fopen(path.c_str(), "rb");
//
//    if (raw_file_ptr == nullptr)
//    {
//        //return nullptr;
//    }
//}

std::shared_ptr<Uint8Array> readFile(std::string path)
{
    //"./assets/tri01.ctm"
    std::ifstream               inFIle;
    std::shared_ptr<Uint8Array> u8Arr = nullptr;
    inFIle.open(path, std::ios::binary | std::ios::in);
    if (inFIle.is_open())
    {
        inFIle.unsetf(std::ios::skipws);
        std::streampos size;
        inFIle.seekg(0, std::ios::end);
        size = inFIle.tellg();
        inFIle.seekg(0, std::ios::beg);

        std::cout << "open file " << size << " bytes." << std::endl;
        if (size > 0)
        {
            //auto memblock = new char[size]{};
            ////inFIle.seekg(0, std::ios::beg);

            u8Arr         = std::make_shared<Uint8Array>(size);
            auto memblock = reinterpret_cast<char*>(u8Arr->getBytes());

            inFIle.read(memblock, size);
            std::cout << "loaded " << size << " bytes file, url: " << path << std::endl;
        }
        inFIle.close();
    }
    else
    {
        std::cout << "load file io error, path: " << path << std::endl;
    }
    return u8Arr;
}
void testBase()
{
    ///*
    std::cout << "------- ctm testBase begin --------------" << std::endl;

    

    char* chars = new char[16]{};
    std::shared_ptr<ArrayBuffer> buf01( new ArrayBuffer(chars, 16) );    
    Uint8Array u8Arr01(buf01);
    showArrayView(u8Arr01, "u8Arr01");

    std::vector<char> vct_chars;
    vct_chars.resize(32);
    vct_chars[8] = 8;
    std::shared_ptr<ArrayBuffer> buf02(new ArrayBuffer(vct_chars.data(), vct_chars.size()));
    Uint8Array u8Arr02(buf02);
    showArrayView(u8Arr02, "u8Arr02");



    Uint8Array u8Arr(22);
    u8Arr[11] += 12;
    Uint32 value = u8Arr[11];
    std::cout << "      u8Arr[11], value: " << value << std::endl;
    showArrayView(u8Arr, "u8Arr");
    for (auto i = 0; i < u8Arr.getLength(); ++i) u8Arr[i] = i + 190;
    showArrayView(u8Arr, "u8Arr");

    Uint8Array u8SubArr(u8Arr.getBuffer(), 3, 4);
    showArrayView(u8SubArr, "u8SubArr");

    Uint16Array u16FromU8SubA(u8Arr.getBuffer(), 2, 3);
    showArrayView(u16FromU8SubA, "u16FromU8SubA");

    Uint32Array u32FromU8SubA(u8Arr.getBuffer(), 4, 3);
    showArrayView(u32FromU8SubA, "u32FromU8SubA");

    Int32Array i32FromU8SubA(u8Arr.getBuffer(), 4, 3);
    showArrayView(i32FromU8SubA, "i32FromU8SubA");

    Float32Array f32FromU8SubA(u8Arr.getBuffer(), 4, 3);
    showArrayView(f32FromU8SubA, "f32FromU8SubA");


    Float32Array f32Arr(10);
    for (auto i = 0; i < f32Arr.getLength(); ++i) f32Arr[i] = 10.0f + static_cast<Float32>(i) / 10.0f;
    showArrayView(f32Arr, "f32Arr");

    std::cout << "------- ctm testBase end --------------" << std::endl;
}

void test()
{
    testBase();
}

} // namespace draco