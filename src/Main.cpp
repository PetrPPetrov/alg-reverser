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

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <memory>

struct Var
{
    unsigned int value;

    Var() : value(0)
    {}
};

struct State
{
    std::vector<Var> vars;
};

struct FullState : public State
{
    size_t statement_index;

    FullState() : statement_index(0)
    {}
};

struct IStatement
{
    size_t index;

    IStatement() : index(0)
    {}

    virtual FullState Execute(const FullState& state) const = 0;
    virtual size_t GetPreviousStateCount(const FullState& state) const = 0;
    virtual FullState GetPreviousState(size_t index, const FullState& state) const = 0;
};

struct Program
{
    std::vector<std::unique_ptr<IStatement> > statements;
};

struct Nop : public IStatement
{
    FullState Execute(const FullState& state) const
    {
        FullState result(state);
        ++result.statement_index;
        return result;
    }
};

struct AddRA : public IStatement
{
    size_t result_index;
    size_t argument_index;

    AddRA(size_t result, size_t argument) : result_index(result), argument_index(argument)
    {
        if (result_index == argument_index)
            throw std::runtime_error("AddRA: passed the same result and argument indices");
    }

    FullState Execute(const FullState& state) const
    {
        FullState result(state);
        result.vars.at(result_index).value += state.vars.at(argument_index).value;
        ++result.statement_index;
        return result;
    }

    size_t GetPreviousStateCount(const FullState& state) const
    {
        if (state.statement_index == index + 1)
            return 1;
        else
            return 0;
    }

    FullState GetPreviousState(size_t index, const FullState& state) const
    {
        FullState result(state);
        result.vars.at(result_index).value -= state.vars.at(argument_index).value;
        --result.statement_index;
        return result;
    }
};

struct IGoto : public IStatement
{
    size_t destination_index;

    IGoto(size_t destination) : destination_index(destination)
    {}
};

struct Goto : public IGoto
{
    Goto(size_t destination) : IGoto(destination)
    {}

    FullState Execute(const FullState& state) const
    {
        FullState result(state);
        result.statement_index = destination_index;
        return result;
    }

    size_t GetPreviousStateCount(const FullState& state) const
    {
        if (state.statement_index == destination_index)
            return 1;
        else
            return 0;
    }

    FullState GetPreviousState(size_t index, const FullState& state) const
    {
        FullState result(state);
        result.statement_index = index;
        return result;
    }
};

struct IfAMoreBGoto : public IGoto
{
    size_t a_index;
    size_t b_index;

    IfAMoreBGoto(size_t destination, size_t a, size_t b) : IGoto(destination), a_index(a), b_index(b)
    {
        if (a_index == b_index)
            throw std::runtime_error("IfAMoreBGoto: passed the same a and b indices");
    }

    FullState Execute(const FullState& state) const
    {
        FullState result(state);
        if (state.vars.at(a_index).value > state.vars.at(b_index).value)
        {
            result.statement_index = destination_index;
        }
        else
        {
            ++result.statement_index;
        }
        return result;
    }

    size_t GetPreviousStateCount(const FullState& state) const
    {
        if (state.statement_index == destination_index)
        {
            if (state.vars.at(a_index).value > state.vars.at(b_index).value)
                return 1;
            else
                return 0;
        }
        else if (state.statement_index == index + 1)
        {
            if (state.vars.at(a_index).value <= state.vars.at(b_index).value)
                return 1;
            else
                return 0;
        }
        else
            return 0;
    }

    FullState GetPreviousState(size_t index, const FullState& state) const
    {
        if (state.statement_index == destination_index)
        {
            if (state.vars.at(a_index).value > state.vars.at(b_index).value)
            {
                FullState result(state);
                result.statement_index = index;
                return result;
            }
        }
        else 
        {
            FullState result(state);
            --result.statement_index;
            return result;
        }
    }
};

void Dump(const std::string& text, const State& state)
{
    std::cout << text << std::endl;
    for (size_t i = 0; i < state.vars.size(); ++i)
    {
        std::cout << "Var" << i << ": " << state.vars[i].value << std::endl;
    }
}

void OrderStatements(Program& program)
{
    for (size_t i = 0; i < program.statements.size(); ++i)
    {
        program.statements[i]->index = i;
    }
}

State Execute(const Program& program, State initial_state)
{
    FullState work_state;
    work_state.vars = initial_state.vars;
    while (work_state.statement_index < program.statements.size())
    {
        work_state = program.statements.at(work_state.statement_index)->Execute(work_state);
    }
    return work_state;
}

std::pair<FullState, bool> ReversalExecuteImpl(const Program& program, FullState final_state)
{
    if (final_state.statement_index == 0)
    {
        return std::make_pair(final_state, true);
    }
    for (size_t j = 0; j < program.statements.size(); ++j)
    {
        const size_t possible_previous_state_count = program.statements[j]->GetPreviousStateCount(final_state);
        for (size_t p = 0; p < possible_previous_state_count; ++p)
        {
            FullState previous_state = program.statements[j]->GetPreviousState(p, final_state);
            std::pair<FullState, bool> result = ReversalExecuteImpl(program, previous_state);
            if (result.second)
                return result;
        }
    }
    return std::make_pair(final_state, program.statements.empty());
}

std::pair<State, bool> ReversalExecute(const Program& program, State final_state)
{
    FullState work_state;
    work_state.vars = final_state.vars;
    work_state.statement_index = program.statements.size();
    return ReversalExecuteImpl(program, work_state);
}

void ReversalExecuteImpl(const Program& program, FullState final_state, std::vector<State>& results)
{
    if (final_state.statement_index == 0)
    {
        results.push_back(final_state);
        return;
    }
    for (size_t j = 0; j < program.statements.size(); ++j)
    {
        const size_t possible_previous_state_count = program.statements[j]->GetPreviousStateCount(final_state);
        for (size_t p = 0; p < possible_previous_state_count; ++p)
        {
            FullState previous_state = program.statements[j]->GetPreviousState(p, final_state);
            ReversalExecuteImpl(program, previous_state, results);
        }
    }
    if (program.statements.empty())
    {
        results.push_back(final_state);
        return;
    }
}

void ReversalExecute(const Program& program, State final_state, std::vector<State>& results)
{
    FullState work_state;
    work_state.vars = final_state.vars;
    work_state.statement_index = program.statements.size();
    ReversalExecuteImpl(program, work_state, results);
}

int main()
{
    try
    {
        Program program1;
        program1.statements.push_back(std::make_unique<IfAMoreBGoto>(2, 0, 1));
        program1.statements.push_back(std::make_unique<AddRA>(0, 1));
        program1.statements.push_back(std::make_unique<AddRA>(1, 0));
        OrderStatements(program1);

        State input1;
        input1.vars.resize(2);
        input1.vars[0].value = 4;
        input1.vars[1].value = 5;
        Dump("Input1:", input1);

        State result = Execute(program1, input1);
        Dump("Result:", result);

        std::pair<State, bool> restored_result = ReversalExecute(program1, result);
        if (restored_result.second)
        {
            Dump("Restored result:", restored_result.first);
        }
        else
        {
            std::cout << "Can't restore any initial result" << std::endl;
        }

        std::vector<State> restored_results;
        ReversalExecute(program1, result, restored_results);
        std::cout << "Restored result count " << restored_results.size() << std::endl;
        for (size_t i = 0; i < restored_results.size(); ++i)
        {
            std::cout << "index " << i << std::endl;
            Dump("Result:", restored_results[i]);
        }
    }
    catch (const std::exception& error)
    {
        std::cout << "exception: " << error.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
