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

inline void Print(const Program& program, const FullState& info)
{
    for (size_t index = 0; index < program.statements.size(); ++index)
    {
        std::cout << program.statements[index]->Print(info) << std::endl;
    }
}

inline void PrintInput(const BitExpressionStates& state)
{
    for (size_t var_index = 0; var_index < state.GetVariableCount(); ++var_index)
    {
        std::cout << "Var " << state.GetVarName(var_index) << ": " << state.GetInputVarValue(var_index) << std::endl;
    }
}

inline void PrintOutput(const BitExpressionStates& state)
{
    for (size_t var_index = 0; var_index < state.GetVariableCount(); ++var_index)
    {
        std::cout << "Var " << state.GetVarName(var_index) << ": " << state.GetOutputVarValue(var_index) << std::endl;
    }
}

inline void PrintCurrentBit(const BitExpressionStates& state, size_t var_index, size_t bit_number)
{
    std::string value;
    size_t bit_index = BitExpressionStates::GetBitIndex(var_index, bit_number);
    if (state.IsCurrentBitConstant(bit_index))
    {
        value = state.GetCurrentBitValue(bit_index) ? "1" : "0";
    }
    else
    {
        value = state.GetBitExpression(bit_index)->ToString(state);
        std::cout << "Var " << state.GetVarName(var_index) << "." << bit_number << " = " << value << std::endl;
    }
    //std::cout << "Var " << state.GetVarName(var_index) << "." << bit_number << " = " << value << std::endl;
}

inline void PrintCurrentVar(const BitExpressionStates& state, size_t var_index)
{
    for (size_t bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
    {
        PrintCurrentBit(state, var_index, bit_number);
    }
}

inline void PrintCurrent(const BitExpressionStates& state)
{
    for (size_t var_index = 0; var_index < state.GetVariableCount(); ++var_index)
    {
        PrintCurrentVar(state, var_index);
    }
}
