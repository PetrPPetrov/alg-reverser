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

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <set>
#include <map>

#include "Common.h"

class CVar
{
    size_t index;

    void Init(State& state, unsigned int initial_value)
    {
        Var new_var;
        new_var.value = initial_value;
        index = state.vars.size();
        state.vars.push_back(new_var);
    }

public:
    CVar(size_t index_): index(index_)
    {}

    CVar(State& state, unsigned int initial_value = 0)
    {
        Init(state, initial_value);
    }

    CVar(State& state, const std::string& name, CVarInfo& var_names, unsigned int initial_value = 0)
    {
        Init(state, initial_value);
        var_names.AddVariable(name, index);
    }

    CVar(State& state, const std::string& name, CVarInfo& var_names, unsigned int initial_value, int array_size)
    {
        Init(state, initial_value);
        var_names.AddArrayVariable(name, index, array_size);
        for (int i = 0; i < array_size - 1; i++)
        {
            Var new_var;
            new_var.value = initial_value;
            state.vars.push_back(new_var);
        }
    }

    size_t GetIndex() const
    {
        return index;
    }
};

class CLabel
{
    size_t index;
    std::set<size_t> statements_which_use_forward_link;
    std::string name;
    CVarInfo* label_names;

    void Init(const Program& program)
    {
        index = program.statements.size();
    }

public:
    CLabel(size_t index_) : index(index_), label_names(0)
    {
    }

    CLabel(const Program& program, CVarInfo& label_names_) : label_names(&label_names_)
    {
        Init(program);
    }

    CLabel(const Program& program, const std::string& name_, CVarInfo& label_names_) : label_names(&label_names_), name(name_)
    {
        Init(program);
        label_names->AddLabel(name, index);
    }
    size_t GetDestinationLine() const
    {
        return index;
    }

    CLabel GetForwardLink(const Program& program)
    {
        statements_which_use_forward_link.insert(program.statements.size());
        return CLabel(GetDestinationLine());
    }

    void SetForwardLink(Program& program);
};

struct Nop : public IStatement
{
    Nop(Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    Nop(size_t line_number);
};

struct SetConstant : public IStatement
{
    SetConstant(const CVar& result, unsigned int value, Program& program, bool hex_ = true);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    SetConstant(size_t line_number, size_t result, unsigned int value_, bool hex_);
    size_t result_index;
    unsigned int value;
    bool hex;
};

struct AddRA : public IStatement
{
    AddRA(const CVar& result, const CVar& argument, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    AddRA(size_t line_number, size_t result, size_t argument);
    size_t result_index;
    size_t argument_index;
};

struct LetRA : public IStatement
{
    LetRA(const CVar& result, const CVar& argument, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    LetRA(size_t line_number, size_t result, size_t argument);
    size_t result_index;
    size_t argument_index;
};

struct LetRAI : public IStatement
{
    LetRAI(const CVar& result, const CVar& argument, const CVar& index_var, unsigned int max_index, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    LetRAI(size_t line_number, size_t result, size_t argument, size_t index_var, unsigned int max_index);
    size_t result_index;
    size_t argument_index;
    size_t index_var_index;
    unsigned int max_index;
};

struct IncR : public IStatement
{
    IncR(const CVar& result, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    IncR(size_t line_number, size_t result);
    size_t result_index;
};

struct AndRA : public IStatement
{
    AndRA(const CVar& result, const CVar& argument, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    AndRA(size_t line_number, size_t result, size_t argument);
    size_t result_index;
    size_t argument_index;
};

struct OrRA : public IStatement
{
    OrRA(const CVar& result, const CVar& argument, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    OrRA(size_t line_number, size_t result, size_t argument);
    size_t result_index;
    size_t argument_index;
};

struct XorRA : public IStatement
{
    XorRA(const CVar& result, const CVar& argument, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    XorRA(size_t line_number, size_t result, size_t argument);
    size_t result_index;
    size_t argument_index;
};

struct InverseR : public IStatement
{
    InverseR(const CVar& result, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    InverseR(size_t line_number, size_t result);
    size_t result_index;
};

struct MulRA : public IStatement
{
    MulRA(const CVar& result, const CVar& argument, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    MulRA(size_t line_number, size_t result, size_t argument);
    size_t result_index;
    size_t argument_index;
};

struct RestDivideRA : public IStatement
{
    RestDivideRA(const CVar& result, const CVar& argument, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    RestDivideRA(size_t line_number, size_t result, size_t argument);
    size_t result_index;
    size_t argument_index;
};

struct LcrRA : public IStatement
{
    LcrRA(const CVar& result, const CVar& argument, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    LcrRA(size_t line_number, size_t result, size_t argument);
    size_t result_index;
    size_t argument_index;
};

struct Goto : public IStatement
{
    Goto(const CLabel& label, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    void SetDestinationLine(size_t destination_line);
    std::string Print(const Info& info) const;
protected:
    Goto(size_t line_number, size_t destination_index_);
    size_t GetDestinationIndex() const;
private:
    size_t destination_index;
};

void CLabel::SetForwardLink(Program& program)
{
    index = program.statements.size();
    if (label_names)
    {
        label_names->AddLabel(name, index);
    }
    for (auto it : statements_which_use_forward_link)
    {
        auto* goto_statement = dynamic_cast<Goto*>(program.statements.at(it));
        if (goto_statement)
        {
            goto_statement->SetDestinationLine(index);
        }
    }
}

struct IfAMoreBGoto : public Goto
{
    IfAMoreBGoto(const CVar& a, const CVar& b, const CLabel& destination, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    IfAMoreBGoto(size_t line_number, size_t destination, size_t a, size_t b);
    size_t a_index;
    size_t b_index;
};

struct IfALessBGoto : public Goto
{
    IfALessBGoto(const CVar& a, const CVar& b, const CLabel& destination, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    IfALessBGoto(size_t line_number, size_t destination, size_t a, size_t b);
    size_t a_index;
    size_t b_index;
};

struct PrintVar : public IStatement
{
    PrintVar(const CVar& argument, const std::string& text, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    PrintVar(size_t line_number, size_t argument, const std::string& text_);
    size_t argument_index;
    std::string text;
};

struct PrintText : public IStatement
{
    PrintText(const std::string& text, Program& program);
    FullState Execute(const FullState& state) const;
    FullState ExecuteAndGenerate(const FullState& state, Program& program, State& input) const;
    std::string Print(const Info& info) const;
private:
    PrintText(size_t line_number, const std::string& text);
    std::string text;
};

Nop::Nop(Program& program) : IStatement(0)
{
    program.statements.push_back(new Nop(program.statements.size()));
}

FullState Nop::Execute(const FullState& state) const
{
    FullState result(state);
    ++result.statement_index;
    return result;
}

FullState Nop::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    return Execute(state);
}

std::string Nop::Print(const Info& info) const
{
    return "Nop";
}

Nop::Nop(size_t line_number) : IStatement(line_number)
{
}

AddRA::AddRA(const CVar& result, const CVar& argument, Program& program) : IStatement(0)
{
    program.statements.push_back(new AddRA(program.statements.size(), result.GetIndex(), argument.GetIndex()));
}

FullState AddRA::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value += state.vars.at(argument_index).value;
    ++result.statement_index;
    return result;
}

FullState AddRA::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(result_index).constant && !state.vars.at(argument_index).constant)
    {
        SetConstant(CVar(result_index), state.vars.at(result_index).value, program);
        AddRA(CVar(result_index), CVar(argument_index), program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = false;
        return result;
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        SetConstant(CVar(argument_index), state.vars.at(argument_index).value, program);
        AddRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
    else
    {
        AddRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
}

std::string AddRA::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " + " + info.GetVarName(argument_index);
}

AddRA::AddRA(size_t line_number, size_t result, size_t argument) : IStatement(line_number), result_index(result), argument_index(argument)
{
    if (result_index == argument_index)
        throw std::runtime_error("AddRA: result and argument indices are the same");
}

SetConstant::SetConstant(const CVar& result, unsigned int value, Program& program, bool hex_) : IStatement(0)
{
    program.statements.push_back(new SetConstant(program.statements.size(), result.GetIndex(), value, hex_));
}

FullState SetConstant::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value = value;
    ++result.statement_index;
    return result;
}

FullState SetConstant::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant)
    {
        return Execute(state);
    }
    else
    {
        SetConstant(CVar(result_index), value, program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = true;
        return result;
    }
}

std::string SetConstant::Print(const Info& info) const
{
    if (hex)
    {
        std::stringstream ss;
        ss << std::hex << value;
        return "Let " + info.GetVarName(result_index) + " = 0x" + ss.str();
    }
    else
    {
        return "Let " + info.GetVarName(result_index) + " = " + std::to_string(value);
    }
}

SetConstant::SetConstant(size_t line_number, size_t result, unsigned int value_, bool hex_) : IStatement(line_number), result_index(result), value(value_), hex(hex_)
{
}

LetRA::LetRA(const CVar& result, const CVar& argument, Program& program) : IStatement(0)
{
    program.statements.push_back(new LetRA(program.statements.size(), result.GetIndex(), argument.GetIndex()));
}

FullState LetRA::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value = state.vars.at(argument_index).value;
    ++result.statement_index;
    return result;
}

FullState LetRA::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(result_index).constant && !state.vars.at(argument_index).constant)
    {
        LetRA(CVar(result_index), CVar(argument_index), program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = false;
        return result;
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        SetConstant(CVar(result_index), state.vars.at(argument_index).value, program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = true;
        return result;
    }
    else
    {
        LetRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
}

std::string LetRA::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(argument_index);
}

LetRA::LetRA(size_t line_number, size_t result, size_t argument) : IStatement(line_number), result_index(result), argument_index(argument)
{
    if (result_index == argument_index)
        throw std::runtime_error("LetRA: result and argument indices are the same");
}

LetRAI::LetRAI(const CVar& result, const CVar& argument, const CVar& index_var, unsigned int max_index_, Program& program) : IStatement(0)
{
    program.statements.push_back(new LetRAI(program.statements.size(), result.GetIndex(), argument.GetIndex(), index_var.GetIndex(), max_index_));
}

FullState LetRAI::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value = state.vars.at(argument_index + state.vars.at(index_var_index).value).value;
    ++result.statement_index;
    return result;
}

FullState LetRAI::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant && state.vars.at(index_var_index).constant && state.vars.at(argument_index + state.vars.at(index_var_index).value).constant)
    {
        return Execute(state);
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(index_var_index).constant && state.vars.at(argument_index + state.vars.at(index_var_index).value).constant)
    {
        FullState result = Execute(state);
        SetConstant(CVar(result_index), result.vars.at(result_index).value, program);
        result.vars.at(result_index).constant = true;
        return result;
    }
    else if (state.vars.at(result_index).constant && !state.vars.at(index_var_index).constant)
    {
        throw std::runtime_error("not implemented");
        return Execute(state);
        //for (unsigned int i = 0; i < max_index; ++i)
        //{
        //    if (state.vars.at(argument_index + i).constant)
        //    {
        //        // generate IF
        //    }
        //}
        //LetRAI(CVar(result_index), CVar(argument_index), CVar(index_var_index), max_index, program);
        // 2 case
    }
    else if (!state.vars.at(result_index).constant && !state.vars.at(index_var_index).constant)
    {
        throw std::runtime_error("not implemented");
        return Execute(state);
        // 2 case
    }
    else if (state.vars.at(result_index).constant && state.vars.at(index_var_index).constant && !state.vars.at(argument_index + state.vars.at(index_var_index).value).constant)
    {
        FullState result = Execute(state);
        LetRA(CVar(result_index), CVar(argument_index + state.vars.at(index_var_index).value), program);
        result.vars.at(result_index).constant = false;
        return result;
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(index_var_index).constant && !state.vars.at(argument_index + state.vars.at(index_var_index).value).constant)
    {
        FullState result = Execute(state);
        LetRA(CVar(result_index), CVar(argument_index + state.vars.at(index_var_index).value), program);
        return result;
    }
    return Execute(state);
}

std::string LetRAI::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = " + info.GetArrayName(argument_index) + "[" + info.GetVarName(index_var_index) + "]";
}

LetRAI::LetRAI(size_t line_number, size_t result, size_t argument, size_t index_var, unsigned int max_index_) : IStatement(line_number), result_index(result), argument_index(argument), index_var_index(index_var), max_index(max_index_)
{
    if (result_index == argument_index)
        throw std::runtime_error("LetRAI: result and argument indices are the same");
}

IncR::IncR(const CVar& result, Program& program) : IStatement(0)
{
    program.statements.push_back(new IncR(program.statements.size(), result.GetIndex()));
}

FullState IncR::Execute(const FullState& state) const
{
    FullState result(state);
    ++result.vars.at(result_index).value;
    ++result.statement_index;
    return result;
}

FullState IncR::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant)
    {
        return Execute(state);
    }
    else
    {
        IncR(CVar(result_index), program);
        return Execute(state);
    }
}

std::string IncR::Print(const Info& info) const
{
    return "Inc " + info.GetVarName(result_index);
}

IncR::IncR(size_t line_number, size_t result) : IStatement(line_number), result_index(result)
{
}

AndRA::AndRA(const CVar& result, const CVar& argument, Program& program) : IStatement(0)
{
    program.statements.push_back(new AndRA(program.statements.size(), result.GetIndex(), argument.GetIndex()));
}

FullState AndRA::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value &= state.vars.at(argument_index).value;
    ++result.statement_index;
    return result;
}

FullState AndRA::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(result_index).constant && !state.vars.at(argument_index).constant)
    {
        if (state.vars.at(result_index).value == 0)
        {
            return Execute(state);
        }
        SetConstant(CVar(result_index), state.vars.at(result_index).value, program);
        AndRA(CVar(result_index), CVar(argument_index), program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = false;
        return result;
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        if (state.vars.at(argument_index).value == 0)
        {
            SetConstant(CVar(result_index), state.vars.at(argument_index).value, program);
            FullState result = Execute(state);
            result.vars.at(result_index).constant = true;
            return result;
        }
        SetConstant(CVar(argument_index), state.vars.at(argument_index).value, program);
        AndRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
    else
    {
        AndRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
}

std::string AndRA::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " & " + info.GetVarName(argument_index);
}

AndRA::AndRA(size_t line_number, size_t result, size_t argument) : IStatement(line_number), result_index(result), argument_index(argument)
{
}

OrRA::OrRA(const CVar& result, const CVar& argument, Program& program) : IStatement(0)
{
    program.statements.push_back(new OrRA(program.statements.size(), result.GetIndex(), argument.GetIndex()));
}

FullState OrRA::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value |= state.vars.at(argument_index).value;
    ++result.statement_index;
    return result;
}

FullState OrRA::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(result_index).constant && !state.vars.at(argument_index).constant)
    {
        if (state.vars.at(result_index).value == std::numeric_limits<unsigned int>::max())
        {
            return Execute(state);
        }
        SetConstant(CVar(result_index), state.vars.at(result_index).value, program);
        OrRA(CVar(result_index), CVar(argument_index), program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = false;
        return result;
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        if (state.vars.at(argument_index).value == std::numeric_limits<unsigned int>::max())
        {
            SetConstant(CVar(result_index), state.vars.at(argument_index).value, program);
            FullState result = Execute(state);
            result.vars.at(result_index).constant = true;
            return result;
        }
        SetConstant(CVar(argument_index), state.vars.at(argument_index).value, program);
        OrRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
    else
    {
        OrRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
}

std::string OrRA::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " | " + info.GetVarName(argument_index);
}

OrRA::OrRA(size_t line_number, size_t result, size_t argument) : IStatement(line_number), result_index(result), argument_index(argument)
{
}

XorRA::XorRA(const CVar& result, const CVar& argument, Program& program) : IStatement(0)
{
    program.statements.push_back(new XorRA(program.statements.size(), result.GetIndex(), argument.GetIndex()));
}

FullState XorRA::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value ^= state.vars.at(argument_index).value;
    ++result.statement_index;
    return result;
}

FullState XorRA::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(result_index).constant && !state.vars.at(argument_index).constant)
    {
        SetConstant(CVar(result_index), state.vars.at(result_index).value, program);
        XorRA(CVar(result_index), CVar(argument_index), program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = false;
        return result;
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        SetConstant(CVar(argument_index), state.vars.at(argument_index).value, program);
        XorRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
    else
    {
        XorRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
}

std::string XorRA::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " ^ " + info.GetVarName(argument_index);
}

XorRA::XorRA(size_t line_number, size_t result, size_t argument) : IStatement(line_number), result_index(result), argument_index(argument)
{
}

InverseR::InverseR(const CVar& result, Program& program) : IStatement(0)
{
    program.statements.push_back(new InverseR(program.statements.size(), result.GetIndex()));
}

FullState InverseR::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value = ~result.vars.at(result_index).value;
    ++result.statement_index;
    return result;
}

FullState InverseR::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant)
    {
        return Execute(state);
    }
    else
    {
        InverseR(CVar(result_index), program);
        return Execute(state);
    }
}

std::string InverseR::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = ~" + info.GetVarName(result_index);
}

InverseR::InverseR(size_t line_number, size_t result) : IStatement(line_number), result_index(result)
{
}

MulRA::MulRA(const CVar& result, const CVar& argument, Program& program) : IStatement(0)
{
    program.statements.push_back(new MulRA(program.statements.size(), result.GetIndex(), argument.GetIndex()));
}

FullState MulRA::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value *= state.vars.at(argument_index).value;
    ++result.statement_index;
    return result;
}

FullState MulRA::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(result_index).constant && !state.vars.at(argument_index).constant)
    {
        if (state.vars.at(result_index).value == 0)
        {
            return Execute(state);
        }
        SetConstant(CVar(result_index), state.vars.at(result_index).value, program);
        MulRA(CVar(result_index), CVar(argument_index), program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = false;
        return result;
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        if (state.vars.at(argument_index).value == 0)
        {
            SetConstant(CVar(result_index), 0, program);
            FullState result = Execute(state);
            result.vars.at(result_index).constant = true;
            return result;
        }
        SetConstant(CVar(argument_index), state.vars.at(argument_index).value, program);
        MulRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
    else
    {
        MulRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
}

std::string MulRA::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " * " + info.GetVarName(argument_index);
}

MulRA::MulRA(size_t line_number, size_t result, size_t argument) : IStatement(line_number), result_index(result), argument_index(argument)
{
}

RestDivideRA::RestDivideRA(const CVar& result, const CVar& argument, Program& program) : IStatement(0)
{
    program.statements.push_back(new RestDivideRA(program.statements.size(), result.GetIndex(), argument.GetIndex()));
}

FullState RestDivideRA::Execute(const FullState& state) const
{
    FullState result(state);
    result.vars.at(result_index).value %= state.vars.at(argument_index).value;
    ++result.statement_index;
    return result;
}

FullState RestDivideRA::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(result_index).constant && !state.vars.at(argument_index).constant)
    {
        if (state.vars.at(result_index).value == 0 || state.vars.at(result_index).value == 1)
        {
            return Execute(state);
        }
        SetConstant(CVar(result_index), state.vars.at(result_index).value, program);
        RestDivideRA(CVar(result_index), CVar(argument_index), program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = false;
        return result;
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        SetConstant(CVar(argument_index), state.vars.at(argument_index).value, program);
        RestDivideRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
    else
    {
        RestDivideRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
}

std::string RestDivideRA::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " % " + info.GetVarName(argument_index);
}

RestDivideRA::RestDivideRA(size_t line_number, size_t result, size_t argument) : IStatement(line_number), result_index(result), argument_index(argument)
{
}

LcrRA::LcrRA(const CVar& result, const CVar& argument, Program& program) : IStatement(0)
{
    program.statements.push_back(new LcrRA(program.statements.size(), result.GetIndex(), argument.GetIndex()));
}

FullState LcrRA::Execute(const FullState& state) const
{
    FullState result(state);
    const unsigned int x = result.vars.at(result_index).value;
    const unsigned int c = result.vars.at(argument_index).value;
    result.vars.at(result_index).value = (x << c) | (x >> (32 - c));
    ++result.statement_index;
    return result;
}

FullState LcrRA::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(result_index).constant && !state.vars.at(argument_index).constant)
    {
        SetConstant(CVar(result_index), state.vars.at(result_index).value, program);
        LcrRA(CVar(result_index), CVar(argument_index), program);
        FullState result = Execute(state);
        result.vars.at(result_index).constant = false;
        return result;
    }
    else if (!state.vars.at(result_index).constant && state.vars.at(argument_index).constant)
    {
        SetConstant(CVar(argument_index), state.vars.at(argument_index).value, program);
        LcrRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
    else
    {
        LcrRA(CVar(result_index), CVar(argument_index), program);
        return Execute(state);
    }
}

std::string LcrRA::Print(const Info& info) const
{
    return "Let " + info.GetVarName(result_index) + " = " + info.GetVarName(result_index) + " <<< " + info.GetVarName(argument_index);
}

LcrRA::LcrRA(size_t line_number, size_t result, size_t argument) : IStatement(line_number), result_index(result), argument_index(argument)
{
}

Goto::Goto(const CLabel& label, Program& program) : IStatement(0)
{
    program.statements.push_back(new Goto(program.statements.size(), label.GetDestinationLine()));
}

FullState Goto::Execute(const FullState& state) const
{
    FullState result(state);
    result.statement_index = destination_index;
    return result;
}

FullState Goto::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    return Execute(state);
}

void Goto::SetDestinationLine(size_t destination_line)
{
    destination_index = destination_line;
}

std::string Goto::Print(const Info& info) const
{
    return "Goto " + info.GetLabelName(destination_index);
}

Goto::Goto(size_t line_number, size_t destination_index_) : IStatement(line_number), destination_index(destination_index_)
{
}

size_t Goto::GetDestinationIndex() const
{
    return destination_index;
}

IfAMoreBGoto::IfAMoreBGoto(const CVar& a, const CVar& b, const CLabel& destination, Program& program) : Goto(0, 0)
{
    program.statements.push_back(new IfAMoreBGoto(program.statements.size(), destination.GetDestinationLine(), a.GetIndex(), b.GetIndex()));
}

FullState IfAMoreBGoto::Execute(const FullState& state) const
{
    FullState result(state);
    if (state.vars.at(a_index).value > state.vars.at(b_index).value)
    {
        result.statement_index = GetDestinationIndex();
    }
    else
    {
        ++result.statement_index;
    }
    return result;
}

FullState IfAMoreBGoto::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(a_index).constant && state.vars.at(b_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(a_index).constant && state.vars.at(a_index).value == 0)
    {
        return Execute(state);
    }
    else
    {
        IfAMoreBGoto(CVar(a_index), CVar(b_index), CLabel(GetDestinationIndex()), program);
        return Execute(state);
    }
}

std::string IfAMoreBGoto::Print(const Info& info) const
{
    return "If " + info.GetVarName(a_index) + " > " + info.GetVarName(b_index) + " Then Goto " + info.GetLabelName(GetDestinationIndex());
}

IfAMoreBGoto::IfAMoreBGoto(size_t line_number, size_t destination, size_t a, size_t b) : Goto(line_number, destination), a_index(a), b_index(b)
{
    if (a_index == b_index)
        throw std::runtime_error("IfAMoreBGoto: passed the same a and b indices");
}

IfALessBGoto::IfALessBGoto(const CVar& a, const CVar& b, const CLabel& destination, Program& program) : Goto(0, 0)
{
    program.statements.push_back(new IfALessBGoto(program.statements.size(), destination.GetDestinationLine(), a.GetIndex(), b.GetIndex()));
}

FullState IfALessBGoto::IfALessBGoto::Execute(const FullState& state) const
{
    FullState result(state);
    if (state.vars.at(a_index).value < state.vars.at(b_index).value)
    {
        result.statement_index = GetDestinationIndex();
    }
    else
    {
        ++result.statement_index;
    }
    return result;
}

FullState IfALessBGoto::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(a_index).constant && state.vars.at(b_index).constant)
    {
        return Execute(state);
    }
    else if (state.vars.at(b_index).constant && state.vars.at(b_index).value == 0)
    {
        return Execute(state);
    }
    else
    {
        IfALessBGoto(CVar(a_index), CVar(b_index), CLabel(GetDestinationIndex()), program);
        return Execute(state);
    }
}

std::string IfALessBGoto::Print(const Info& info) const
{
    return "If " + info.GetVarName(a_index) + " < " + info.GetVarName(b_index) + " Then Goto " + info.GetLabelName(GetDestinationIndex());
}

IfALessBGoto::IfALessBGoto(size_t line_number, size_t destination, size_t a, size_t b) : Goto(line_number, destination), a_index(a), b_index(b)
{
    if (a_index == b_index)
        throw std::runtime_error("IfALessBGoto: passed the same a and b indices");
}

PrintVar::PrintVar(const CVar& argument, const std::string& text, Program& program) : IStatement(0)
{
    program.statements.push_back(new PrintVar(program.statements.size(), argument.GetIndex(), text));
}

FullState PrintVar::Execute(const FullState& state) const
{
    FullState result(state);
    std::cout << "Var " << text << ": " << std::to_string(result.vars.at(argument_index).value) << std::endl;
    ++result.statement_index;
    return result;
}

FullState PrintVar::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    if (state.vars.at(argument_index).constant)
    {
        std::stringstream ss;
        ss << "Var " << text << ": " << std::to_string(state.vars.at(argument_index).value);
        PrintText(ss.str(), program);
        return Execute(state);
    }
    else
    {
        PrintVar(CVar(argument_index), text, program);
        return Execute(state);
    }
}

std::string PrintVar::Print(const Info& info) const
{
    return "Print " + info.GetVarName(argument_index);
}

PrintVar::PrintVar(size_t line_number, size_t argument, const std::string& text_) : IStatement(line_number), argument_index(argument), text(text_)
{
}

PrintText::PrintText(const std::string& text, Program& program): IStatement(0)
{
    program.statements.push_back(new PrintText(program.statements.size(), text));
}

FullState PrintText::Execute(const FullState& state) const
{
    FullState result(state);
    std::cout << text << std::endl;
    ++result.statement_index;
    return result;
}

FullState PrintText::ExecuteAndGenerate(const FullState& state, Program& program, State& input) const
{
    PrintText(text, program);
    return Execute(state);
}

std::string PrintText::Print(const Info& info) const
{
    return "Print '" + text + "'";
}

PrintText::PrintText(size_t line_number, const std::string& text_) : IStatement(line_number), text(text_)
{
}

namespace Basic
{
    void Print(const Program& program, const CVarInfo& info)
    {
        for (size_t index = 0; index < program.statements.size(); ++index)
        {
            if (!info.GetLabelName(index).empty())
            {
                std::cout << info.GetLabelName(index) << ":\t";
            }
            else
            {
                std::cout << "\t";
            }
            std::cout << program.statements[index]->Print(info) << std::endl;

        }
    }

    void Dump(const State& state, const CVarInfo& info)
    {
        std::map<size_t, std::string> dummy;
        for (size_t i = 0; i < state.vars.size(); ++i)
        {
            std::cout << "Var " << info.GetVarName(i) << ": " << state.vars[i].value << std::endl;
        }
    }

    void DumpDiff(const State& state, const State& state0, const CVarInfo& info)
    {
        std::map<size_t, std::string> dummy;
        for (size_t i = 0; i < state.vars.size(); ++i)
        {
            if (state.vars[i].value != state0.vars[i].value)
            {
                std::cout << "Var " << info.GetVarName(i) << ": " << state.vars[i].value << std::endl;
            }
        }
    }
}
