/*
 * HashReverser reverses hashes
 * Copyright (C) 2017 Petr Petrovich Petrov
 *
 * This file is part of HashReverser.
 *
 * HashReverser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HashReverser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HashReverser.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <iostream>
#include <string>

#include "BitExpressions.h"
#include "Program.h"
#include "Utility.h"

void CreateMD5(BitExpressionStates& state, Program& program)
{
    auto w = state.AddArray("w", true, 16, 0);
    auto h = state.AddArray("h", false, 4, 0);
    auto a = state.AddVariable("a", true, 0);
    auto b = state.AddVariable("b", true, 0);
    auto c = state.AddVariable("c", true, 0);
    auto d = state.AddVariable("d", true, 0);
    auto i = state.AddVariable("i", true, 0);
    auto f = state.AddVariable("f", true, 0);
    auto g = state.AddVariable("g", true, 0);
    auto temp = state.AddVariable("temp", true, 0);
    auto exp0 = state.AddVariable("exp0", true, 0);
    auto exp1 = state.AddVariable("exp1", true, 0);
    auto c16 = state.AddVariable("c16", true, 16);
    auto c32 = state.AddVariable("c32", true, 32);
    auto c48 = state.AddVariable("c48", true, 48);
    auto c64 = state.AddVariable("c64", true, 64);
    auto r = state.AddArray("r", true, 64, 0);
    auto k = state.AddArray("k", true, 64, 0);

    SetConstant::Create(program, h + 0x0, 0x67452301);
    SetConstant::Create(program, h + 0x1, 0xefcdab89);
    SetConstant::Create(program, h + 0x2, 0x98badcfe);
    SetConstant::Create(program, h + 0x3, 0x10325476);

    SetConstant::Create(program, a, 0x67452301);
    SetConstant::Create(program, b, 0xefcdab89);
    SetConstant::Create(program, c, 0x98badcfe);
    SetConstant::Create(program, d, 0x10325476);

    SetConstant::Create(program, r + 0x0,  7, false);
    SetConstant::Create(program, r + 0x1, 12, false);
    SetConstant::Create(program, r + 0x2, 17, false);
    SetConstant::Create(program, r + 0x3, 22, false);
    SetConstant::Create(program, r + 0x4,  7, false);
    SetConstant::Create(program, r + 0x5, 12, false);
    SetConstant::Create(program, r + 0x6, 17, false);
    SetConstant::Create(program, r + 0x7, 22, false);
    SetConstant::Create(program, r + 0x8,  7, false);
    SetConstant::Create(program, r + 0x9, 12, false);
    SetConstant::Create(program, r + 0xA, 17, false);
    SetConstant::Create(program, r + 0xB, 22, false);
    SetConstant::Create(program, r + 0xC,  7, false);
    SetConstant::Create(program, r + 0xD, 12, false);
    SetConstant::Create(program, r + 0xE, 17, false);
    SetConstant::Create(program, r + 0xF, 22, false);

    SetConstant::Create(program, r + 0x10,  5, false);
    SetConstant::Create(program, r + 0x11,  9, false);
    SetConstant::Create(program, r + 0x12, 14, false);
    SetConstant::Create(program, r + 0x13, 20, false);
    SetConstant::Create(program, r + 0x14,  5, false);
    SetConstant::Create(program, r + 0x15,  9, false);
    SetConstant::Create(program, r + 0x16, 14, false);
    SetConstant::Create(program, r + 0x17, 20, false);
    SetConstant::Create(program, r + 0x18,  5, false);
    SetConstant::Create(program, r + 0x19,  9, false);
    SetConstant::Create(program, r + 0x1A, 14, false);
    SetConstant::Create(program, r + 0x1B, 20, false);
    SetConstant::Create(program, r + 0x1C,  5, false);
    SetConstant::Create(program, r + 0x1D,  9, false);
    SetConstant::Create(program, r + 0x1E, 14, false);
    SetConstant::Create(program, r + 0x1F, 20, false);

    SetConstant::Create(program, r + 0x20,  4, false);
    SetConstant::Create(program, r + 0x21, 11, false);
    SetConstant::Create(program, r + 0x22, 16, false);
    SetConstant::Create(program, r + 0x23, 23, false);
    SetConstant::Create(program, r + 0x24,  4, false);
    SetConstant::Create(program, r + 0x25, 11, false);
    SetConstant::Create(program, r + 0x26, 16, false);
    SetConstant::Create(program, r + 0x27, 23, false);
    SetConstant::Create(program, r + 0x28,  4, false);
    SetConstant::Create(program, r + 0x29, 11, false);
    SetConstant::Create(program, r + 0x2A, 16, false);
    SetConstant::Create(program, r + 0x2B, 23, false);
    SetConstant::Create(program, r + 0x2C,  4, false);
    SetConstant::Create(program, r + 0x2D, 11, false);
    SetConstant::Create(program, r + 0x2E, 16, false);
    SetConstant::Create(program, r + 0x2F, 23, false);

    SetConstant::Create(program, r + 0x30,  6, false);
    SetConstant::Create(program, r + 0x31, 10, false);
    SetConstant::Create(program, r + 0x32, 15, false);
    SetConstant::Create(program, r + 0x33, 21, false);
    SetConstant::Create(program, r + 0x34,  6, false);
    SetConstant::Create(program, r + 0x35, 10, false);
    SetConstant::Create(program, r + 0x36, 15, false);
    SetConstant::Create(program, r + 0x37, 21, false);
    SetConstant::Create(program, r + 0x38,  6, false);
    SetConstant::Create(program, r + 0x39, 10, false);
    SetConstant::Create(program, r + 0x3A, 15, false);
    SetConstant::Create(program, r + 0x3B, 21, false);
    SetConstant::Create(program, r + 0x3C,  6, false);
    SetConstant::Create(program, r + 0x3D, 10, false);
    SetConstant::Create(program, r + 0x3E, 15, false);
    SetConstant::Create(program, r + 0x3F, 21, false);

    SetConstant::Create(program, k + 0x00, 0xd76aa478);
    SetConstant::Create(program, k + 0x01, 0xe8c7b756);
    SetConstant::Create(program, k + 0x02, 0x242070db);
    SetConstant::Create(program, k + 0x03, 0xc1bdceee);
    SetConstant::Create(program, k + 0x04, 0xf57c0faf);
    SetConstant::Create(program, k + 0x05, 0x4787c62a);
    SetConstant::Create(program, k + 0x06, 0xa8304613);
    SetConstant::Create(program, k + 0x07, 0xfd469501);
    SetConstant::Create(program, k + 0x08, 0x698098d8);
    SetConstant::Create(program, k + 0x09, 0x8b44f7af);
    SetConstant::Create(program, k + 0x0A, 0xffff5bb1);
    SetConstant::Create(program, k + 0x0B, 0x895cd7be);
    SetConstant::Create(program, k + 0x0C, 0x6b901122);
    SetConstant::Create(program, k + 0x0D, 0xfd987193);
    SetConstant::Create(program, k + 0x0E, 0xa679438e);
    SetConstant::Create(program, k + 0x0F, 0x49b40821);

    SetConstant::Create(program, k + 0x10, 0xf61e2562);
    SetConstant::Create(program, k + 0x11, 0xc040b340);
    SetConstant::Create(program, k + 0x12, 0x265e5a51);
    SetConstant::Create(program, k + 0x13, 0xe9b6c7aa);
    SetConstant::Create(program, k + 0x14, 0xd62f105d);
    SetConstant::Create(program, k + 0x15, 0x02441453);
    SetConstant::Create(program, k + 0x16, 0xd8a1e681);
    SetConstant::Create(program, k + 0x17, 0xe7d3fbc8);
    SetConstant::Create(program, k + 0x18, 0x21e1cde6);
    SetConstant::Create(program, k + 0x19, 0xc33707d6);
    SetConstant::Create(program, k + 0x1A, 0xf4d50d87);
    SetConstant::Create(program, k + 0x1B, 0x455a14ed);
    SetConstant::Create(program, k + 0x1C, 0xa9e3e905);
    SetConstant::Create(program, k + 0x1D, 0xfcefa3f8);
    SetConstant::Create(program, k + 0x1E, 0x676f02d9);
    SetConstant::Create(program, k + 0x1F, 0x8d2a4c8a);

    SetConstant::Create(program, k + 0x20, 0xfffa3942);
    SetConstant::Create(program, k + 0x21, 0x8771f681);
    SetConstant::Create(program, k + 0x22, 0x6d9d6122);
    SetConstant::Create(program, k + 0x23, 0xfde5380c);
    SetConstant::Create(program, k + 0x24, 0xa4beea44);
    SetConstant::Create(program, k + 0x25, 0x4bdecfa9);
    SetConstant::Create(program, k + 0x26, 0xf6bb4b60);
    SetConstant::Create(program, k + 0x27, 0xbebfbc70);
    SetConstant::Create(program, k + 0x28, 0x289b7ec6);
    SetConstant::Create(program, k + 0x29, 0xeaa127fa);
    SetConstant::Create(program, k + 0x2A, 0xd4ef3085);
    SetConstant::Create(program, k + 0x2B, 0x04881d05);
    SetConstant::Create(program, k + 0x2C, 0xd9d4d039);
    SetConstant::Create(program, k + 0x2D, 0xe6db99e5);
    SetConstant::Create(program, k + 0x2E, 0x1fa27cf8);
    SetConstant::Create(program, k + 0x2F, 0xc4ac5665);

    SetConstant::Create(program, k + 0x30, 0xf4292244);
    SetConstant::Create(program, k + 0x31, 0x432aff97);
    SetConstant::Create(program, k + 0x32, 0xab9423a7);
    SetConstant::Create(program, k + 0x33, 0xfc93a039);
    SetConstant::Create(program, k + 0x34, 0x655b59c3);
    SetConstant::Create(program, k + 0x35, 0x8f0ccc92);
    SetConstant::Create(program, k + 0x36, 0xffeff47d);
    SetConstant::Create(program, k + 0x37, 0x85845dd1);
    SetConstant::Create(program, k + 0x38, 0x6fa87e4f);
    SetConstant::Create(program, k + 0x39, 0xfe2ce6e0);
    SetConstant::Create(program, k + 0x3A, 0xa3014314);
    SetConstant::Create(program, k + 0x3B, 0x4e0811a1);
    SetConstant::Create(program, k + 0x3C, 0xf7537e82);
    SetConstant::Create(program, k + 0x3D, 0xbd3af235);
    SetConstant::Create(program, k + 0x3E, 0x2ad7d2bb);
    SetConstant::Create(program, k + 0x3F, 0xeb86d391);

    SetConstant::Create(program, i, 0, false);

    auto next = IfALessBGoto::Create(program, i, c64, "next");
    auto to_end = Goto::Create(program); //end

    auto main = IfALessBGoto::Create(program, i, c16, "main"); //li16
    next->SetDestinationLine(main->GetLineNumber());

    auto if_li32 = IfALessBGoto::Create(program, i, c32); //li32
    auto if_li48 = IfALessBGoto::Create(program, i, c48); //li48

    //li64
    LetRA::Create(program, exp0, b);
    LetRA::Create(program, exp1, d);
    InverseR::Create(program, exp1);
    OrRA::Create(program, exp0, exp1);
    LetRA::Create(program, f, c);
    XorRA::Create(program, f, exp0);

    SetConstant::Create(program, exp0, 7, false);
    MulRA::Create(program, exp0, i);
    RestDivideRA::Create(program, exp0, c16);
    LetRA::Create(program, g, exp0);
    auto to_common64 = Goto::Create(program); //common

    //li16
    main->SetDestinationLine(LetRA::Create(program, exp0, b, "li16")->GetLineNumber());
    AndRA::Create(program, exp0, c);
    LetRA::Create(program, exp1, b);
    InverseR::Create(program, exp1);
    AndRA::Create(program, exp1, d);
    OrRA::Create(program, exp0, exp1);
    LetRA::Create(program, f, exp0);
    LetRA::Create(program, g, i);
    auto to_common16 = Goto::Create(program); //common

    //li32
    if_li32->SetDestinationLine(LetRA::Create(program, exp0, d, "li32")->GetLineNumber());
    AndRA::Create(program, exp0, b);
    LetRA::Create(program, exp1, d);
    InverseR::Create(program, exp1);
    AndRA::Create(program, exp1, c);
    OrRA::Create(program, exp0, exp1);
    LetRA::Create(program, f, exp0);

    SetConstant::Create(program, exp0, 5, false);
    MulRA::Create(program, exp0, i);
    SetConstant::Create(program, exp1, 1, false);
    AddRA::Create(program, exp0, exp1);
    RestDivideRA::Create(program, exp0, c16);
    LetRA::Create(program, g, exp0);
    auto to_common32 = Goto::Create(program); //common

    //li48
    if_li48->SetDestinationLine(LetRA::Create(program, exp0, b, "li48")->GetLineNumber());
    XorRA::Create(program, exp0, c);
    XorRA::Create(program, exp0, d);
    LetRA::Create(program, f, exp0);

    SetConstant::Create(program, exp0, 3, false);
    MulRA::Create(program, exp0, i);
    SetConstant::Create(program, exp1, 5, false);
    AddRA::Create(program, exp0, exp1);
    RestDivideRA::Create(program, exp0, c16);
    LetRA::Create(program, g, exp0);

    //common
    auto common = LetRA::Create(program, temp, d, "common")->GetLineNumber();
    to_common16->SetDestinationLine(common);
    to_common32->SetDestinationLine(common);
    to_common64->SetDestinationLine(common);

    LetRA::Create(program, d, c);
    LetRA::Create(program, c, b);

    LetRA::Create(program, exp1, a);
    AddRA::Create(program, exp1, f);
    LetRAI::Create(program, exp0, k, i);
    AddRA::Create(program, exp1, exp0);
    LetRAI::Create(program, exp0, w, g);
    AddRA::Create(program, exp1, exp0);
    LetRAI::Create(program, exp0, r, i);
    LcrRA::Create(program, exp1, exp0);
    AddRA::Create(program, b, exp1);
    LetRA::Create(program, a, temp);

    IncR::Create(program, i);
    Goto::Create(program)->SetDestinationLine(next->GetLineNumber());

    auto end = AddRA::Create(program, h + 0x0, a, "end")->GetLineNumber();
    to_end->SetDestinationLine(end);
    AddRA::Create(program, h + 0x1, b);
    AddRA::Create(program, h + 0x2, c);
    AddRA::Create(program, h + 0x3, d);
}

void Print(uint32_t value)
{
    uint8_t *p;
    p = (uint8_t *)&value;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
}

void Print(uint32_t value0, uint32_t value1, uint32_t value2, uint32_t value3)
{
    Print(value0);
    Print(value1);
    Print(value2);
    Print(value3);
    puts("");
}

void MD5Experiment()
{
    BitExpressionStates input;
    Program program;
    CreateMD5(input, program);

    FullState start_state;
    start_state.Copy(input);
    Print(program, start_state);
    std::cout << std::endl;

    input.SetInputVarValue(0, 0x6c6c6548);
    //input.SetInputVarConstant(0, false);
    input.SetInputBitConstant(0, false);
    input.SetInputBitConstant(1, false);
    input.SetInputVarValue(1, 0x0080216f);
    input.SetInputVarValue(14, 0x00000030);

    BitExpressionStates output;
    Execute(program, input, output);

    std::cout << "Bit expressions" << std::endl;
    PrintCurrentVar(output, 16);
    PrintCurrentVar(output, 17);
    PrintCurrentVar(output, 18);
    PrintCurrentVar(output, 19);
    std::cout << std::endl;

    //std::cout << "Output" << std::endl;
    //PrintOutput(output);
    //std::cout << std::endl;

    Print(output.GetOutputVarValue(16), output.GetOutputVarValue(17), output.GetOutputVarValue(18), output.GetOutputVarValue(19));
    output.SetInputBitValue(0, true);
    Print(output.GetOutputVarValue(16), output.GetOutputVarValue(17), output.GetOutputVarValue(18), output.GetOutputVarValue(19));
}

void SmallExperiment()
{
    BitExpressionStates input;
    Program program;

    auto h = input.AddVariable("h", false, 0x6c6c6548);
    auto a = input.AddVariable("a", false, 0x0F0F0F0F);
    auto b = input.AddVariable("b", true, 0x00000FFF);

    OrRA::Create(program, a, b);
    LetRA::Create(program, h, a);

    FullState start_state;
    start_state.Copy(input);
    Print(program, start_state);
    std::cout << std::endl;

    std::cout << "Input" << std::endl;
    PrintInput(input);
    std::cout << std::endl;

    BitExpressionStates output;
    Execute(program, input, output);

    std::cout << "Bit expressions" << std::endl;
    PrintCurrentVar(output, h);
    std::cout << std::endl;

    std::cout << "Output" << std::endl;
    PrintOutput(output);
    std::cout << std::endl;
}
