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

#include <sstream>

#include "Program.h"

FullState::FullState() : statement_index(0)
{
}

size_t IStatement::GetLineNumber() const
{
    return line_number;
}

IStatement::~IStatement()
{
}

std::string IStatement::Print(const FullState& info) const
{
    std::string prefix = "";
    if (info.statement_index == GetLineNumber())
    {
        prefix = ">";
    }
    if (label.empty())
    {
        return prefix + "\t";
    }
    else
    {
        return prefix + label + ":\t";
    }
}

IStatement::IStatement(size_t line_number_, const std::string& label_)
    : line_number(line_number_), label(label_)
{
}

std::shared_ptr<Nop> Nop::Create(Program& program, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<Nop>(Nop(line_number, label));
    program.statements.push_back(new_statement);
    return new_statement;
}

void Nop::Execute(FullState& state) const
{
    ++state.statement_index;
}

std::string Nop::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Nop";
}

Nop::Nop(size_t line_number_, const std::string& label) : IStatement(line_number_, label)
{
}

std::shared_ptr<SetConstant> SetConstant::Create(Program& program, size_t result_index, BitExpressionStates::work_type value, bool hex, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<SetConstant>(SetConstant(line_number, label, result_index, value, hex));
    program.statements.push_back(new_statement);
    return new_statement;
}

void SetConstant::Execute(FullState& state) const
{
    for (size_t bit_number = 0; bit_number < sizeof(BitExpressionStates::work_type)*8; ++bit_number)
    {
        const size_t result_bit_index = state.GetBitIndex(result_index, bit_number);
        state.SetBitValue(result_bit_index, BitExpressionStates::ExtractBit(value, bit_number));
        state.SetBitConstant(result_bit_index, true);
    }
    ++state.statement_index;
}

std::string SetConstant::Print(const FullState& info) const
{
    std::string value_str = std::to_string(value);

    if (hex)
    {
        std::stringstream ss;
        ss << std::hex << "0x" << value;
        value_str = ss.str();
    }

    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + value_str;
}

SetConstant::SetConstant(size_t line_number_, const std::string& label_, size_t result_index_, BitExpressionStates::work_type value_, bool hex_)
    : IStatement(line_number_, label_), result_index(result_index_), value(value_), hex(hex_)
{
}

