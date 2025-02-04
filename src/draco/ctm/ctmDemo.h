#pragma once

#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include "ctmDef.h"
#include <bitset>
#include <ctime>
#include <fstream>
#include <thread>

#include "draco/stream/Uint8Array.h"
#include "draco/stream/Int8Array.h"
#include "draco/stream/Int16Array.h"
#include "draco/stream/Uint16Array.h"
#include "draco/stream/Uint32Array.h"
#include "draco/stream/Int32Array.h"
#include "draco/stream/Float32Array.h"
#include "./CTMFile.h"
#include "./CTMStream.h"


namespace draco
{

using Uint8Array   = draco::Uint8Array;
using Int8Array    = draco::Int8Array;
using Int16Array   = draco::Int16Array;
using Uint16Array  = draco::Uint16Array;
using Uint32Array  = draco::Uint32Array;
using Int32Array   = draco::Int32Array;
using Float32Array = draco::Float32Array;

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
void loadAndParseCTM()
{
    std::cout << "<<<< -------------- loadAndParseCTM begin ---------------------\n"
              << std::endl;

    //std::string url("D:\\dev\\res\\assets\\box01.ctm");
    //std::string url(".\\assets\\tri01.ctm");
    //std::string url(".\\assets\\box01.ctm");
    //std::string url(".\\assets\\sh202_0.ctm");
    //std::string url(".\\assets\\sh202_20.ctm");
    //std::string url(".\\assets\\sh202_22.ctm");

    std::string url(".\\assets\\sh202_26.ctm");
    auto        dataU8Arr = readFile(url);
    if (dataU8Arr == nullptr)
    {
        return;
    }

    //std::cout << "dataU8Arr->getLength(): " << dataU8Arr->getLength() << std::endl;
    //std::clock_t begin, end;
    //begin        = std::clock();
    auto stream  = std::make_shared<CTMStream>(dataU8Arr);
    auto ctmFile = std::make_shared<CTMFile>(stream);

    //end = std::clock();
    //std::cout << "loss time: " << double(end - begin) / CLOCKS_PER_SEC << "s" << std::endl;
    ///*
    auto ctmBody = ctmFile->body;
    showArrayViewPtr(ctmBody->indices, "ctmBody->indices\n");
    showArrayViewPtr(ctmBody->normals, "ctmBody->normals\n");
    if (ctmBody->uvMaps.size() > 0)
    {
        auto uvs = ctmBody->getUVSAt(0);
        showArrayViewPtr(uvs, "ctmBody->uvs\n");
    }
    showArrayViewPtr(ctmBody->vertices, "ctmBody->vertices\n");
    //*/
    //std::cout << "\n>>>> ---------------------- loadAndParseCTM end ---------------------" << std::endl;
}

void testCTMParse()
{
    std::cout << "---------------------- testCTMParse begin ---------------------\n"
              << std::endl;

    auto flag = isLittleEndian();
    std::cout << ">>>>> isLittleEndian():  " << flag << std::endl;

    for (auto i = 0; i < 1; ++i)
    {
        std::cout << ">>>>> $$$ Step " << i << std::endl;
        loadAndParseCTM();
    }
    std::cout << "\n---------------------- testCTMParse end ---------------------" << std::endl;
}

void testCircleCTMParse()
{

    std::cout << "\n---------------------- testCircleCTMParse begin ---------------------" << std::endl;
    for (auto i = 0; i < 1; ++i)
    {
        std::cout << ">>>>> $$$ Step " << i << std::endl;
        loadAndParseCTM();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << "\n---------------------- testCircleCTMParse end ---------------------" << std::endl;
    for (auto i = 0; i < 200; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "---------------------- exec end ---------------------" << std::endl;
}
void testCTM()
{
    testCircleCTMParse();

    //testCTMParse();
    // testCTMBase()
}
void testCTMBase()
{
    //std::string url(".\\assets\\tri01.ctm");
    //std::string url("D:\\dev\\res\\assets\\box01.ctm");
    std::string url("D:\\dev\\res\\assets\\box01.ctm");
    auto        ctmDataU8Arr = readFile(url);
    if (ctmDataU8Arr == nullptr)
    {
        return;
    }
    //DELETE_OBJ_ARRAY_NAKED_PTR(fileBufData);

    //return;
    /*
    Uint32 ctmData[]{1297367887, 5, 3295053, 3, 1, 1, 0, 1, 0, 1211254605, 1018852906, 998244352, 3212836864, 0, 3212836864, 1065353216, 0, 1065353216, 4, 1, 4, 1414677846, 10, 16777309, 1845493760, 4115357200, 1191231662, 156779593, 1560281088, 65536, 2137718784, 10111507, 1145981184, 2392, 23808, 1, 1318223467, 1317062750, 189616719, 1560281088, 65536, 3194814464, 3870626691, 1409286366, 222517317, 1140850688, 1969645161, 1663067507, 1919904879, 0, 964689920, 7, 16777309, 1845493760, 14173960};
    Uint32 ctmDataLen = sizeof(ctmData) / sizeof(Uint32);

    std::cout << "      ctmDataLen: " << ctmDataLen << std::endl;

    auto u32Arr = std::make_shared<Uint32Array>(ctmDataLen);
    for (auto i = 0; i < u32Arr->getLength(); ++i) (*u32Arr)[i] = ctmData[i];
    auto ctmDataU8Arr = std::make_shared<Uint8Array>(u32Arr->getBuffer());
    //*/

    std::cout << "ctmDataU8Arr->getLength(): " << ctmDataU8Arr->getLength() << std::endl;
    auto ctmStream01 = std::make_shared<CTMStream>(ctmDataU8Arr);
    auto ctmFile01   = std::make_shared<CTMFile>(ctmStream01);
    auto ctmBody     = ctmFile01->body;
    showArrayViewPtr(ctmBody->vertices, "ctmBody->vertices\n");
    if (ctmBody->uvMaps.size() > 0)
    {
        auto uvs = ctmBody->getUVSAt(0);
        //showArrayViewPtr(uvs, "ctmBody->uvs\n");
    }
    showArrayViewPtr(ctmBody->normals, "ctmBody->normals\n");
    showArrayViewPtr(ctmBody->indices, "ctmBody->indices\n");

}
void test()
{
    for (const uint8_t i : {0, 1}) {
        std::cout << "  ## i: " << i + 0 << std::endl;
    }
    testCTM();
}

}