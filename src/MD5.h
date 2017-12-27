/*
 * AlgReverser reverses algorithms
 * Copyright (C) 2017 Petr Petrovich Petrov
 *
 * This file is part of AlgReverser.
 *
 * AlgReverser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AlgReverser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with AlgReverser.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <iostream>
#include <string>

#include "Common.h"
#include "Basic.h"

void CreateMD5(State& state, Program& program, CVarInfo& info)
{
    CVar w(state, "w", info, 0, 16);
    CVar h(state, "h", info, 0, 4);
    CVar a(state, "a", info);
    CVar b(state, "b", info);
    CVar c(state, "c", info);
    CVar d(state, "d", info);
    CVar i(state, "i", info);
    CVar c16(state, "c16", info);
    CVar c32(state, "c32", info);
    CVar c48(state, "c48", info);
    CVar c64(state, "c64", info);
    CVar f(state, "f", info);
    CVar g(state, "g", info);
    CVar temp(state, "temp", info);
    CVar r(state, "r", info, 0, 64);
    CVar k(state, "k", info, 0, 64);

    SetConstant(CVar(h.GetIndex() + 0x0), 0x67452301, program);
    SetConstant(CVar(h.GetIndex() + 0x1), 0xefcdab89, program);
    SetConstant(CVar(h.GetIndex() + 0x2), 0x98badcfe, program);
    SetConstant(CVar(h.GetIndex() + 0x3), 0x10325476, program);

    SetConstant(a, 0x67452301, program);
    SetConstant(b, 0xefcdab89, program);
    SetConstant(c, 0x98badcfe, program);
    SetConstant(d, 0x10325476, program);

    SetConstant(CVar(r.GetIndex() + 0x0),  7, program, false);
    SetConstant(CVar(r.GetIndex() + 0x1), 12, program, false);
    SetConstant(CVar(r.GetIndex() + 0x2), 17, program, false);
    SetConstant(CVar(r.GetIndex() + 0x3), 22, program, false);
    SetConstant(CVar(r.GetIndex() + 0x4),  7, program, false);
    SetConstant(CVar(r.GetIndex() + 0x5), 12, program, false);
    SetConstant(CVar(r.GetIndex() + 0x6), 17, program, false);
    SetConstant(CVar(r.GetIndex() + 0x7), 22, program, false);
    SetConstant(CVar(r.GetIndex() + 0x8),  7, program, false);
    SetConstant(CVar(r.GetIndex() + 0x9), 12, program, false);
    SetConstant(CVar(r.GetIndex() + 0xA), 17, program, false);
    SetConstant(CVar(r.GetIndex() + 0xB), 22, program, false);
    SetConstant(CVar(r.GetIndex() + 0xC),  7, program, false);
    SetConstant(CVar(r.GetIndex() + 0xD), 12, program, false);
    SetConstant(CVar(r.GetIndex() + 0xE), 17, program, false);
    SetConstant(CVar(r.GetIndex() + 0xF), 22, program, false);

    SetConstant(CVar(r.GetIndex() + 0x10),  5, program, false);
    SetConstant(CVar(r.GetIndex() + 0x11),  9, program, false);
    SetConstant(CVar(r.GetIndex() + 0x12), 14, program, false);
    SetConstant(CVar(r.GetIndex() + 0x13), 20, program, false);
    SetConstant(CVar(r.GetIndex() + 0x14),  5, program, false);
    SetConstant(CVar(r.GetIndex() + 0x15),  9, program, false);
    SetConstant(CVar(r.GetIndex() + 0x16), 14, program, false);
    SetConstant(CVar(r.GetIndex() + 0x17), 20, program, false);
    SetConstant(CVar(r.GetIndex() + 0x18),  5, program, false);
    SetConstant(CVar(r.GetIndex() + 0x19),  9, program, false);
    SetConstant(CVar(r.GetIndex() + 0x1A), 14, program, false);
    SetConstant(CVar(r.GetIndex() + 0x1B), 20, program, false);
    SetConstant(CVar(r.GetIndex() + 0x1C),  5, program, false);
    SetConstant(CVar(r.GetIndex() + 0x1D),  9, program, false);
    SetConstant(CVar(r.GetIndex() + 0x1E), 14, program, false);
    SetConstant(CVar(r.GetIndex() + 0x1F), 20, program, false);

    SetConstant(CVar(r.GetIndex() + 0x20),  4, program, false);
    SetConstant(CVar(r.GetIndex() + 0x21), 11, program, false);
    SetConstant(CVar(r.GetIndex() + 0x22), 16, program, false);
    SetConstant(CVar(r.GetIndex() + 0x23), 23, program, false);
    SetConstant(CVar(r.GetIndex() + 0x24),  4, program, false);
    SetConstant(CVar(r.GetIndex() + 0x25), 11, program, false);
    SetConstant(CVar(r.GetIndex() + 0x26), 16, program, false);
    SetConstant(CVar(r.GetIndex() + 0x27), 23, program, false);
    SetConstant(CVar(r.GetIndex() + 0x28),  4, program, false);
    SetConstant(CVar(r.GetIndex() + 0x29), 11, program, false);
    SetConstant(CVar(r.GetIndex() + 0x2A), 16, program, false);
    SetConstant(CVar(r.GetIndex() + 0x2B), 23, program, false);
    SetConstant(CVar(r.GetIndex() + 0x2C),  4, program, false);
    SetConstant(CVar(r.GetIndex() + 0x2D), 11, program, false);
    SetConstant(CVar(r.GetIndex() + 0x2E), 16, program, false);
    SetConstant(CVar(r.GetIndex() + 0x2F), 23, program, false);

    SetConstant(CVar(r.GetIndex() + 0x30),  6, program, false);
    SetConstant(CVar(r.GetIndex() + 0x31), 10, program, false);
    SetConstant(CVar(r.GetIndex() + 0x32), 15, program, false);
    SetConstant(CVar(r.GetIndex() + 0x33), 21, program, false);
    SetConstant(CVar(r.GetIndex() + 0x34),  6, program, false);
    SetConstant(CVar(r.GetIndex() + 0x35), 10, program, false);
    SetConstant(CVar(r.GetIndex() + 0x36), 15, program, false);
    SetConstant(CVar(r.GetIndex() + 0x37), 21, program, false);
    SetConstant(CVar(r.GetIndex() + 0x38),  6, program, false);
    SetConstant(CVar(r.GetIndex() + 0x39), 10, program, false);
    SetConstant(CVar(r.GetIndex() + 0x3A), 15, program, false);
    SetConstant(CVar(r.GetIndex() + 0x3B), 21, program, false);
    SetConstant(CVar(r.GetIndex() + 0x3C),  6, program, false);
    SetConstant(CVar(r.GetIndex() + 0x3D), 10, program, false);
    SetConstant(CVar(r.GetIndex() + 0x3E), 15, program, false);
    SetConstant(CVar(r.GetIndex() + 0x3F), 21, program, false);

    SetConstant(CVar(k.GetIndex() + 0x00), 0xd76aa478, program);
    SetConstant(CVar(k.GetIndex() + 0x01), 0xe8c7b756, program);
    SetConstant(CVar(k.GetIndex() + 0x02), 0x242070db, program);
    SetConstant(CVar(k.GetIndex() + 0x03), 0xc1bdceee, program);
    SetConstant(CVar(k.GetIndex() + 0x04), 0xf57c0faf, program);
    SetConstant(CVar(k.GetIndex() + 0x05), 0x4787c62a, program);
    SetConstant(CVar(k.GetIndex() + 0x06), 0xa8304613, program);
    SetConstant(CVar(k.GetIndex() + 0x07), 0xfd469501, program);
    SetConstant(CVar(k.GetIndex() + 0x08), 0x698098d8, program);
    SetConstant(CVar(k.GetIndex() + 0x09), 0x8b44f7af, program);
    SetConstant(CVar(k.GetIndex() + 0x0A), 0xffff5bb1, program);
    SetConstant(CVar(k.GetIndex() + 0x0B), 0x895cd7be, program);
    SetConstant(CVar(k.GetIndex() + 0x0C), 0x6b901122, program);
    SetConstant(CVar(k.GetIndex() + 0x0D), 0xfd987193, program);
    SetConstant(CVar(k.GetIndex() + 0x0E), 0xa679438e, program);
    SetConstant(CVar(k.GetIndex() + 0x0F), 0x49b40821, program);

    SetConstant(CVar(k.GetIndex() + 0x10), 0xf61e2562, program);
    SetConstant(CVar(k.GetIndex() + 0x11), 0xc040b340, program);
    SetConstant(CVar(k.GetIndex() + 0x12), 0x265e5a51, program);
    SetConstant(CVar(k.GetIndex() + 0x13), 0xe9b6c7aa, program);
    SetConstant(CVar(k.GetIndex() + 0x14), 0xd62f105d, program);
    SetConstant(CVar(k.GetIndex() + 0x15), 0x02441453, program);
    SetConstant(CVar(k.GetIndex() + 0x16), 0xd8a1e681, program);
    SetConstant(CVar(k.GetIndex() + 0x17), 0xe7d3fbc8, program);
    SetConstant(CVar(k.GetIndex() + 0x18), 0x21e1cde6, program);
    SetConstant(CVar(k.GetIndex() + 0x19), 0xc33707d6, program);
    SetConstant(CVar(k.GetIndex() + 0x1A), 0xf4d50d87, program);
    SetConstant(CVar(k.GetIndex() + 0x1B), 0x455a14ed, program);
    SetConstant(CVar(k.GetIndex() + 0x1C), 0xa9e3e905, program);
    SetConstant(CVar(k.GetIndex() + 0x1D), 0xfcefa3f8, program);
    SetConstant(CVar(k.GetIndex() + 0x1E), 0x676f02d9, program);
    SetConstant(CVar(k.GetIndex() + 0x1F), 0x8d2a4c8a, program);

    SetConstant(CVar(k.GetIndex() + 0x20), 0xfffa3942, program);
    SetConstant(CVar(k.GetIndex() + 0x21), 0x8771f681, program);
    SetConstant(CVar(k.GetIndex() + 0x22), 0x6d9d6122, program);
    SetConstant(CVar(k.GetIndex() + 0x23), 0xfde5380c, program);
    SetConstant(CVar(k.GetIndex() + 0x24), 0xa4beea44, program);
    SetConstant(CVar(k.GetIndex() + 0x25), 0x4bdecfa9, program);
    SetConstant(CVar(k.GetIndex() + 0x26), 0xf6bb4b60, program);
    SetConstant(CVar(k.GetIndex() + 0x27), 0xbebfbc70, program);
    SetConstant(CVar(k.GetIndex() + 0x28), 0x289b7ec6, program);
    SetConstant(CVar(k.GetIndex() + 0x29), 0xeaa127fa, program);
    SetConstant(CVar(k.GetIndex() + 0x2A), 0xd4ef3085, program);
    SetConstant(CVar(k.GetIndex() + 0x2B), 0x04881d05, program);
    SetConstant(CVar(k.GetIndex() + 0x2C), 0xd9d4d039, program);
    SetConstant(CVar(k.GetIndex() + 0x2D), 0xe6db99e5, program);
    SetConstant(CVar(k.GetIndex() + 0x2E), 0x1fa27cf8, program);
    SetConstant(CVar(k.GetIndex() + 0x2F), 0xc4ac5665, program);

    SetConstant(CVar(k.GetIndex() + 0x30), 0xf4292244, program);
    SetConstant(CVar(k.GetIndex() + 0x31), 0x432aff97, program);
    SetConstant(CVar(k.GetIndex() + 0x32), 0xab9423a7, program);
    SetConstant(CVar(k.GetIndex() + 0x33), 0xfc93a039, program);
    SetConstant(CVar(k.GetIndex() + 0x34), 0x655b59c3, program);
    SetConstant(CVar(k.GetIndex() + 0x35), 0x8f0ccc92, program);
    SetConstant(CVar(k.GetIndex() + 0x36), 0xffeff47d, program);
    SetConstant(CVar(k.GetIndex() + 0x37), 0x85845dd1, program);
    SetConstant(CVar(k.GetIndex() + 0x38), 0x6fa87e4f, program);
    SetConstant(CVar(k.GetIndex() + 0x39), 0xfe2ce6e0, program);
    SetConstant(CVar(k.GetIndex() + 0x3A), 0xa3014314, program);
    SetConstant(CVar(k.GetIndex() + 0x3B), 0x4e0811a1, program);
    SetConstant(CVar(k.GetIndex() + 0x3C), 0xf7537e82, program);
    SetConstant(CVar(k.GetIndex() + 0x3D), 0xbd3af235, program);
    SetConstant(CVar(k.GetIndex() + 0x3E), 0x2ad7d2bb, program);
    SetConstant(CVar(k.GetIndex() + 0x3F), 0xeb86d391, program);

    SetConstant(i, 0, program, false);
    SetConstant(c16, 16, program, false);
    SetConstant(c32, 32, program, false);
    SetConstant(c48, 48, program, false);
    SetConstant(c64, 64, program, false);
    CLabel main_iteration(program, "main", info);
    CLabel end(program, "end", info);
    CLabel common(program, "common", info);
CLabel next(program, "next", info);
    IfALessBGoto(i, c64, main_iteration.GetForwardLink(program), program);
    Goto(end.GetForwardLink(program), program);

main_iteration.SetForwardLink(program);

    CVar exp0(state, "exp0", info);
    CVar exp1(state, "exp1", info);

    CLabel li16(program, "li16", info);
    CLabel li32(program, "li32", info);
    CLabel li48(program, "li48", info);
    IfALessBGoto(i, c16, li16.GetForwardLink(program), program);
    IfALessBGoto(i, c32, li32.GetForwardLink(program), program);
    IfALessBGoto(i, c48, li48.GetForwardLink(program), program);
CLabel li64(program, "li64", info);

    LetRA(exp0, b, program);
    LetRA(exp1, d, program);
    InverseR(exp1, program);
    OrRA(exp0, exp1, program);
    LetRA(f, c, program);
    XorRA(f, exp0, program);

    SetConstant(exp0, 7, program, false);
    MulRA(exp0, i, program);
    RestDivideRA(exp0, c16, program);
    LetRA(g, exp0, program);
    Goto(common.GetForwardLink(program), program);

li16.SetForwardLink(program);
    LetRA(exp0, b, program);
    AndRA(exp0, c, program);
    LetRA(exp1, b, program);
    InverseR(exp1, program);
    AndRA(exp1, d, program);
    OrRA(exp0, exp1, program);
    LetRA(f, exp0, program);
    LetRA(g, i, program);
    Goto(common.GetForwardLink(program), program);

li32.SetForwardLink(program);
    LetRA(exp0, d, program);
    AndRA(exp0, b, program);
    LetRA(exp1, d, program);
    InverseR(exp1, program);
    AndRA(exp1, c, program);
    OrRA(exp0, exp1, program);
    LetRA(f, exp0, program);

    SetConstant(exp0, 5, program, false);
    MulRA(exp0, i, program);
    SetConstant(exp1, 1, program, false);
    AddRA(exp0, exp1, program);
    RestDivideRA(exp0, c16, program);
    LetRA(g, exp0, program);
    Goto(common.GetForwardLink(program), program);

li48.SetForwardLink(program);
    LetRA(exp0, b, program);
    XorRA(exp0, c, program);
    XorRA(exp0, d, program);
    LetRA(f, exp0, program);

    SetConstant(exp0, 3, program, false);
    MulRA(exp0, i, program);
    SetConstant(exp1, 5, program, false);
    AddRA(exp0, exp1, program);
    RestDivideRA(exp0, c16, program);
    LetRA(g, exp0, program);

common.SetForwardLink(program);
    LetRA(temp, d, program);
    LetRA(d, c, program);
    LetRA(c, b, program);

    LetRA(exp1, a, program);
    AddRA(exp1, f, program);
    LetRAI(exp0, k, i, program);
    AddRA(exp1, exp0, program);
    LetRAI(exp0, w, g, program);
    AddRA(exp1, exp0, program);
    LetRAI(exp0, r, i, program);
    LcrRA(exp1, exp0, program);
    AddRA(b, exp1, program);
    LetRA(a, temp, program);

    //PrintVar(a, "a", program);
    //PrintVar(b, "b", program);
    //PrintVar(c, "c", program);
    //PrintVar(d, "d", program);

    IncR(i, program);
    Goto(next, program);

end.SetForwardLink(program);
    AddRA(CVar(h.GetIndex() + 0x0), a, program);
    AddRA(CVar(h.GetIndex() + 0x1), b, program);
    AddRA(CVar(h.GetIndex() + 0x2), c, program);
    AddRA(CVar(h.GetIndex() + 0x3), d, program);

    //Program program1;
}

void print_md5_hash(const State& result)
{
    uint8_t *p;
    p = (uint8_t *)&result.vars[16].value;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);

    p = (uint8_t *)&result.vars[17].value;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);

    p = (uint8_t *)&result.vars[18].value;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);

    p = (uint8_t *)&result.vars[19].value;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
    puts("");
}

void MD5Experiment2()
{
    State input0;
    Program program1;
    CVarInfo info;
    CreateMD5(input0, program1, info);

    Basic::Print(program1, info);
    std::cout << std::endl;

    input0.vars[0].value = 0x6c6c6548;
    input0.vars[0].constant = false;
    input0.vars[1].value = 0x0080216f;
    input0.vars[2].value = 0x00000000;
    input0.vars[3].value = 0x00000000;
    input0.vars[4].value = 0x00000000;
    input0.vars[5].value = 0x00000000;
    input0.vars[6].value = 0x00000000;
    input0.vars[7].value = 0x00000000;
    input0.vars[8].value = 0x00000000;
    input0.vars[9].value = 0x00000000;
    input0.vars[10].value = 0x00000000;
    input0.vars[11].value = 0x00000000;
    input0.vars[12].value = 0x00000000;
    input0.vars[13].value = 0x00000000;
    input0.vars[14].value = 0x00000030;
    input0.vars[15].value = 0x00000000;

    State result = Execute(program1, input0, info);
    print_md5_hash(result);

    BitExpressionState expressions(input0);
    State result2 = ExecuteAndGenerateBoolExpressions(program1, input0, info, expressions);
    print_md5_hash(result);

    std::cout << "h[0].0 = " << expressions.bit_expressions.at(expressions.get_bit_index(16, 0))->to_string(info) << std::endl;
}

void SmallExperiment()
{
    State input;
    Program program;
    CVarInfo info;

    CVar h(input, "h", info);
    CVar a(input, "a", info);
    CVar b(input, "b", info);
    AndRA(a, b, program);
    LetRA(h, a, program);

    Basic::Print(program, info);
    std::cout << std::endl;

    input.vars[0].value = 0x6c6c6548;
    input.vars[0].constant = false;
    input.vars[1].value = 0x0F0F0F0F;
    input.vars[1].constant = false;
    input.vars[2].value = 0x00000FFF;
    input.vars[2].constant = true;

    std::cout << "input:" << std::endl;
    Basic::Dump(input, info);
    State result = Execute(program, input, info);
    std::cout << "result:" << std::endl;
    Basic::Dump(result, info);

    BitExpressionState expressions(input);
    State result2 = ExecuteAndGenerateBoolExpressions(program, input, info, expressions);
    std::cout << "result2:" << std::endl;
    Basic::Dump(result2, info);

    expressions.optimize(input);
    Basic::Dump(expressions, 0, info);
    //std::cout << "h[0].0 = " << bool_expressions.expressions.at(bool_expressions.get_bit_index(0, 0))->to_string(info) << std::endl;
}
