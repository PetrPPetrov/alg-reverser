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
    for (size_t bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
    {
        const size_t result_bit_index = state.GetBitIndex(result_index, bit_number);
        const bool bit_value = BitExpressionStates::ExtractBit(value, bit_number);
        state.SetBitExpression(result_bit_index, std::make_shared<ConstBitExpression>(bit_value));
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

std::shared_ptr<LetRA> LetRA::Create(Program& program, size_t result_index, size_t argument_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<LetRA>(LetRA(line_number, label, result_index, argument_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void LetRA::Execute(FullState& state) const
{
    for (size_t bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
    {
        const size_t result_bit_index = state.GetBitIndex(result_index, bit_number);
        const size_t source_bit_index = state.GetBitIndex(argument_index, bit_number);
        state.SetBitExpression(result_bit_index, state.GetBitExpression(source_bit_index)->DeepCopy());
    }
    ++state.statement_index;
}

std::string LetRA::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(argument_index);
}

LetRA::LetRA(size_t line_number_, const std::string& label_, size_t result_index_, size_t argument_index_)
    : IStatement(line_number_, label_), result_index(result_index_), argument_index(argument_index_)
{
}

std::shared_ptr<LetRAI> LetRAI::Create(Program& program, size_t result_index, size_t argument_index, size_t index_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<LetRAI>(LetRAI(line_number, label, result_index, argument_index, index_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void LetRAI::Execute(FullState& state) const
{
    if (state.GetCurrentVarConstant(index_index))
    {
        const size_t source_index = argument_index + state.GetCurrentVarValue(index_index);
        for (size_t bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
        {
            const size_t result_bit_index = state.GetBitIndex(result_index, bit_number);
            const size_t source_bit_index = state.GetBitIndex(source_index, bit_number);
            state.SetBitExpression(result_bit_index, state.GetBitExpression(source_bit_index)->DeepCopy());
        }
    }
    else
    {
        //TODO:
        throw std::runtime_error("not implemented");
    }
    ++state.statement_index;
}

std::string LetRAI::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(argument_index) + "[" + info.GetVarName(index_index) + "]";
}

LetRAI::LetRAI(size_t line_number_, const std::string& label_, size_t result_index_, size_t argument_index_, size_t index_index_)
    : IStatement(line_number_, label_), result_index(result_index_), argument_index(argument_index_), index_index(index_index_)
{
}

std::shared_ptr<AndRA> AndRA::Create(Program& program, size_t result_index, size_t argument_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<AndRA>(AndRA(line_number, label, result_index, argument_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void AndRA::Execute(FullState& state) const
{
    for (unsigned int bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
    {
        const size_t result_bit_index = state.GetBitIndex(result_index, bit_number);
        const size_t source_bit_index = state.GetBitIndex(argument_index, bit_number);
        auto new_operation = std::make_shared<AndBitExpression>(state.GetBitExpression(result_bit_index), state.GetBitExpression(source_bit_index)->DeepCopy());
        state.SetBitExpression(result_bit_index, new_operation);
    }
    ++state.statement_index;
}

std::string AndRA::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " & " + info.GetVarName(argument_index);
}

AndRA::AndRA(size_t line_number_, const std::string& label_, size_t result_index_, size_t argument_index_)
    : IStatement(line_number_, label_), result_index(result_index_), argument_index(argument_index_)
{
}

std::shared_ptr<OrRA> OrRA::Create(Program& program, size_t result_index, size_t argument_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<OrRA>(OrRA(line_number, label, result_index, argument_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void OrRA::Execute(FullState& state) const
{
    for (unsigned int bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
    {
        const size_t result_bit_index = state.GetBitIndex(result_index, bit_number);
        const size_t source_bit_index = state.GetBitIndex(argument_index, bit_number);
        auto new_operation = std::make_shared<OrBitExpression>(state.GetBitExpression(result_bit_index), state.GetBitExpression(source_bit_index)->DeepCopy());
        state.SetBitExpression(result_bit_index, new_operation);
    }
    ++state.statement_index;
}

std::string OrRA::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " | " + info.GetVarName(argument_index);
}

OrRA::OrRA(size_t line_number_, const std::string& label_, size_t result_index_, size_t argument_index_)
    : IStatement(line_number_, label_), result_index(result_index_), argument_index(argument_index_)
{
}

std::shared_ptr<XorRA> XorRA::Create(Program& program, size_t result_index, size_t argument_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<XorRA>(XorRA(line_number, label, result_index, argument_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void XorRA::Execute(FullState& state) const
{
    for (unsigned int bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
    {
        const size_t result_bit_index = state.GetBitIndex(result_index, bit_number);
        const size_t source_bit_index = state.GetBitIndex(argument_index, bit_number);
        auto new_operation = std::make_shared<XorBitExpression>(state.GetBitExpression(result_bit_index), state.GetBitExpression(source_bit_index)->DeepCopy());
        state.SetBitExpression(result_bit_index, new_operation);
    }
    ++state.statement_index;
}

std::string XorRA::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " ^ " + info.GetVarName(argument_index);
}

XorRA::XorRA(size_t line_number_, const std::string& label_, size_t result_index_, size_t argument_index_)
    : IStatement(line_number_, label_), result_index(result_index_), argument_index(argument_index_)
{
}

std::shared_ptr<InverseR> InverseR::Create(Program& program, size_t result_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<InverseR>(InverseR(line_number, label, result_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void InverseR::Execute(FullState& state) const
{
    for (unsigned int bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
    {
        const size_t result_bit_index = state.GetBitIndex(result_index, bit_number);
        auto new_operation = std::make_shared<NegBitExpression>(state.GetBitExpression(result_bit_index));
        state.SetBitExpression(result_bit_index, new_operation);
    }
    ++state.statement_index;
}

std::string InverseR::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = ~" + info.GetVarName(result_index);
}

InverseR::InverseR(size_t line_number_, const std::string& label_, size_t result_index_)
    : IStatement(line_number_, label_), result_index(result_index_)
{
}

std::shared_ptr<AddRA> AddRA::Create(Program& program, size_t result_index, size_t argument_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<AddRA>(AddRA(line_number, label, result_index, argument_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void AddRA::Execute(FullState& state) const
{
    //TODO:
    throw std::runtime_error("not implemented");
}

std::string AddRA::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " + " + info.GetVarName(argument_index);
}

AddRA::AddRA(size_t line_number_, const std::string& label_, size_t result_index_, size_t argument_index_)
    : IStatement(line_number_, label_), result_index(result_index_), argument_index(argument_index_)
{
}

std::shared_ptr<IncR> IncR::Create(Program& program, size_t result_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<IncR>(IncR(line_number, label, result_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void IncR::Execute(FullState& state) const
{
    //TODO:
    throw std::runtime_error("not implemented");
}

std::string IncR::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Inc " + info.GetVarName(result_index);
}

IncR::IncR(size_t line_number_, const std::string& label_, size_t result_index_)
    : IStatement(line_number_, label_), result_index(result_index_)
{
}

std::shared_ptr<MulRA> MulRA::Create(Program& program, size_t result_index, size_t argument_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<MulRA>(MulRA(line_number, label, result_index, argument_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void MulRA::Execute(FullState& state) const
{
    //TODO:
    throw std::runtime_error("not implemented");
}

std::string MulRA::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " * " + info.GetVarName(argument_index);
}

MulRA::MulRA(size_t line_number_, const std::string& label_, size_t result_index_, size_t argument_index_)
    : IStatement(line_number_, label_), result_index(result_index_), argument_index(argument_index_)
{
}

std::shared_ptr<RestDivideRA> RestDivideRA::Create(Program& program, size_t result_index, size_t argument_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<RestDivideRA>(RestDivideRA(line_number, label, result_index, argument_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void RestDivideRA::Execute(FullState& state) const
{
    //TODO:
    throw std::runtime_error("not implemented");
}

std::string RestDivideRA::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " % " + info.GetVarName(argument_index);
}

RestDivideRA::RestDivideRA(size_t line_number_, const std::string& label_, size_t result_index_, size_t argument_index_)
    : IStatement(line_number_, label_), result_index(result_index_), argument_index(argument_index_)
{
}

std::shared_ptr<LcrRA> LcrRA::Create(Program& program, size_t result_index, size_t argument_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<LcrRA>(LcrRA(line_number, label, result_index, argument_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void LcrRA::Execute(FullState& state) const
{
    if (state.GetCurrentVarConstant(argument_index))
    {
        const BitExpressionStates::work_type shift = state.GetCurrentVarValue(argument_index);
        std::vector<std::shared_ptr<IBitExpression> > saved_bit_expressions;
        saved_bit_expressions.reserve(BitExpressionStates::bit_count);

        for (size_t bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
        {
            const size_t bit_index = state.GetBitIndex(result_index, bit_number);
            saved_bit_expressions.push_back(state.GetBitExpression(bit_index));
        }

        for (size_t bit_number = 0; bit_number < BitExpressionStates::bit_count; ++bit_number)
        {
            const size_t result_bit_number = (bit_number + shift) % BitExpressionStates::bit_count;
            const size_t result_bit_index = state.GetBitIndex(result_index, result_bit_number);
            state.SetBitExpression(result_bit_index, saved_bit_expressions[bit_number]);
        }
    }
    else
    {
        //TODO:
        throw std::runtime_error("not implemented");
    }
    ++state.statement_index;
}

std::string LcrRA::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " <<< " + info.GetVarName(argument_index);
}

LcrRA::LcrRA(size_t line_number_, const std::string& label_, size_t result_index_, size_t argument_index_)
    : IStatement(line_number_, label_), result_index(result_index_), argument_index(argument_index_)
{
}

std::shared_ptr<Goto> Goto::Create(Program& program, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<Goto>(Goto(line_number, label));
    program.statements.push_back(new_statement);
    return new_statement;
}

void Goto::Execute(FullState& state) const
{
    state.statement_index = GetDestinationLine();
}

std::string Goto::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Goto " + std::to_string(destination_line);
}

void Goto::SetDestinationLine(size_t destination_line_)
{
    destination_line = destination_line_;
}

size_t Goto::GetDestinationLine() const
{
    return destination_line;
}

Goto::Goto(size_t line_number_, const std::string& label_) : IStatement(line_number_, label_)
{
}

std::shared_ptr<IfAMoreBGoto> IfAMoreBGoto::Create(Program& program, size_t a_index, size_t b_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<IfAMoreBGoto>(IfAMoreBGoto(line_number, label, a_index, b_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void IfAMoreBGoto::Execute(FullState& state) const
{
    if (state.GetCurrentVarConstant(a_index) && state.GetCurrentVarConstant(b_index))
    {
        if (state.GetCurrentVarValue(a_index) > state.GetCurrentVarValue(b_index))
            state.statement_index = GetDestinationLine();
        else
            ++state.statement_index;
    }
    else
    {
        //TODO:
        throw std::runtime_error("not implemented");
    }
}

std::string IfAMoreBGoto::Print(const FullState& info) const
{
    return IStatement::Print(info) + "If " + info.GetVarName(a_index) + " > " + info.GetVarName(b_index) + " Then Goto " + std::to_string(GetDestinationLine());
}

IfAMoreBGoto::IfAMoreBGoto(size_t line_number_, const std::string& label_, size_t a_index_, size_t b_index_)
    : Goto(line_number_, label_), a_index(a_index_), b_index(b_index_)
{
}

std::shared_ptr<IfALessBGoto> IfALessBGoto::Create(Program& program, size_t a_index, size_t b_index, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<IfALessBGoto>(IfALessBGoto(line_number, label, a_index, b_index));
    program.statements.push_back(new_statement);
    return new_statement;
}

void IfALessBGoto::Execute(FullState& state) const
{
    if (state.GetCurrentVarConstant(a_index) && state.GetCurrentVarConstant(b_index))
    {
        if (state.GetCurrentVarValue(a_index) < state.GetCurrentVarValue(b_index))
            state.statement_index = GetDestinationLine();
        else
            ++state.statement_index;
    }
    else
    {
        //TODO:
        throw std::runtime_error("not implemented");
    }
}

std::string IfALessBGoto::Print(const FullState& info) const
{
    return IStatement::Print(info) + "If " + info.GetVarName(a_index) + " < " + info.GetVarName(b_index) + " Then Goto " + std::to_string(GetDestinationLine());
}

IfALessBGoto::IfALessBGoto(size_t line_number_, const std::string& label_, size_t a_index_, size_t b_index_)
    : Goto(line_number_, label_), a_index(a_index_), b_index(b_index_)
{
}

std::shared_ptr<PrintVar> PrintVar::Create(Program& program, size_t argument_index, const std::string& text, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<PrintVar>(PrintVar(line_number, label, argument_index, text));
    program.statements.push_back(new_statement);
    return new_statement;
}

void PrintVar::Execute(FullState& state) const
{
}

std::string PrintVar::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Print 'Var " + info.GetVarName(argument_index) + ": ' + " + info.GetVarName(argument_index);
}

PrintVar::PrintVar(size_t line_number_, const std::string& label_, size_t argument_index_, const std::string& text_)
    : IStatement(line_number_, label_), argument_index(argument_index_), text(text_)
{
}

std::shared_ptr<PrintText> PrintText::Create(Program& program, const std::string& text, const std::string& label)
{
    const size_t line_number = program.statements.size();
    auto new_statement = std::make_shared<PrintText>(PrintText(line_number, label, text));
    program.statements.push_back(new_statement);
    return new_statement;
}

void PrintText::Execute(FullState& state) const
{
}

std::string PrintText::Print(const FullState& info) const
{
    return IStatement::Print(info) + "Print '" + text + "'";
}

PrintText::PrintText(size_t line_number_, const std::string& label_, const std::string& text_)
    : IStatement(line_number_, label_), text(text_)
{
}
