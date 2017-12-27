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

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

#include "VarInfo.h"

struct IBitExpression;



struct BitExpressionStates
{
    typedef uint32_t work_type;

    size_t get_bit_index(size_t var_index, size_t bit_number) const
    {
        return var_index * sizeof(work_type) + bit_number;
    }

    size_t AddVariable(const std::string& name, bool constant, work_type initial_value = 0)
    {
        variables.push_back(initial_value);
        names.push_back(name);
        array_sizes.push_back(0);
        for (size_t i = 0; i < sizeof(work_type); ++i)
        {
            bit_constants.push_back(constant);
            if (constant)
            {

            }
            else
            {

            }
            VariableBitExpression* new_value = new VariableBitExpression();
            new_value->var_index = i;
            new_value->bit_number = b;
            bit_expressions.push_back(std::shared_ptr<IBitExpression>(new_value));
        }
    }

    void Optimize()
    {
        for (size_t i = 0; i < bit_expressions.size(); ++i)
        {
            bit_expressions[i]->optimize(bit_expressions[i], input);
        }
    }
private:
    std::vector<work_type> variables;
    std::vector<std::string> names;
    std::vector<size_t> array_sizes;
    std::vector<bool> bit_constants;
    std::vector<std::shared_ptr<IBitExpression> > bit_expressions;
};

struct BitExpressionState : public State
{
    std::vector<bool> bit_constants;
    std::vector<std::shared_ptr<IBitExpression> > bit_expressions;

    unsigned int get_bit_index(size_t var_index, unsigned int bit_number) const
    {
        return static_cast<unsigned int>(var_index) * 32 + bit_number;
    }

    BitExpressionState(const State& state, const std::vector<bool>& var_is_const)
    {
        bit_constants.clear();
        bit_constants.reserve(state.vars.size() * 32);
        bit_expressions.clear();
        bit_expressions.reserve(state.vars.size() * 32);
        for (size_t i = 0; i < state.vars.size(); ++i)
        {
            for (unsigned int b = 0; b < 32; ++b)
            {
                bit_constants.push_back(var_is_const.at(i));
                VariableBitExpression* new_value = new VariableBitExpression();
                new_value->var_index = i;
                new_value->bit_number = b;
                bit_expressions.push_back(std::shared_ptr<IBitExpression>(new_value));
            }
        }
    }

    void optimize(const State& input)
    {
        for (size_t i = 0; i < bit_expressions.size(); ++i)
        {
            bit_expressions[i]->optimize(bit_expressions[i], input);
        }
    }
};

struct Program;

struct IStatement
{
    size_t GetLineNumber() const
    {
        return line_number;
    }

    virtual ~IStatement()
    {
    }
    virtual FullState Execute(const FullState& state) const = 0;
    virtual FullState GenerateBitExpressions(const FullState& state, BitExpressionState& bool_expressions) const = 0;
    virtual std::string Print(const Info& info) const = 0;

protected:
    IStatement(size_t line_number_) : line_number(line_number_)
    {
    }

private:
    size_t line_number;
};

struct Program
{
    std::vector<IStatement*> statements;

    ~Program()
    {
        for (size_t i = 0; i < statements.size(); ++i)
        {
            if (statements[i])
                delete statements[i];
        }
    }
};
