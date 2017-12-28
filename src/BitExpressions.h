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

struct IBitExpression;

struct BitExpressionStates
{
    typedef uint32_t work_type;
    static const size_t bit_count = sizeof(BitExpressionStates::work_type) * 8;

    static size_t GetBitIndex(size_t var_index, size_t bit_number);
    static bool ExtractBit(work_type value, size_t bit_number);

    size_t GetVarIndex(const std::string& name) const;
    size_t GetVarIndex(const std::string& name, size_t index) const;
    std::string GetVarName(size_t var_index) const;

    size_t AddVariable(const std::string& name, bool constant, work_type initial_value = 0);
    size_t AddArray(const std::string& name, bool constant, size_t size, work_type initial_value = 0);
    size_t GetVariableCount() const;

    void SetInputVarConstant(size_t var_index, bool constant);
    bool IsInputVarConstant(size_t var_index) const;
    void SetInputVarValue(size_t var_index, work_type value);
    work_type GetInputVarValue(size_t var_index) const;

    void SetInputBitConstant(size_t bit_index, bool constant);
    bool IsInputBitConstant(size_t bit_index) const;
    void SetInputBitValue(size_t bit_index, bool value);
    bool GetInputBitValue(size_t bit_index) const;

    void SetBitExpression(size_t bit_index, const std::shared_ptr<IBitExpression>& expresssion);
    std::shared_ptr<IBitExpression> GetBitExpression(size_t bit_index) const;

    bool IsCurrentBitConstant(size_t bit_index) const;
    bool GetCurrentBitValue(size_t bit_index) const;
    bool IsCurrentVarConstant(size_t var_index) const;
    work_type GetCurrentVarValue(size_t var_index) const;
    work_type GetOutputVarValue(size_t var_index) const;

    void Optimize();

    void CopyInputVarValues(const BitExpressionStates& from);
    void CopyNames(const BitExpressionStates& from);
    void CopyInputConstants(const BitExpressionStates& from);
    void CopyBitExpressions(const BitExpressionStates& from);
    void Copy(const BitExpressionStates& from);
private:
    std::vector<work_type> input_variables;
    std::vector<size_t> array_sizes;
    std::vector<size_t> array_starts;
    std::vector<std::string> names;
    std::vector<bool> input_bit_constants;
    std::vector<std::shared_ptr<IBitExpression> > bit_expressions;
};

struct IBitExpression
{
    virtual std::string ToString(const BitExpressionStates& info) const = 0;
    virtual bool Constant(const BitExpressionStates& input) const = 0;
    virtual bool Calculate(const BitExpressionStates& input) const = 0;
    virtual int Priority() const = 0;
    virtual std::shared_ptr<IBitExpression> DeepCopy() const = 0;
    virtual bool Equals(const std::shared_ptr<IBitExpression>& other) const = 0;
    virtual void Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input) = 0;
};

struct ConstBitExpression : public IBitExpression
{
    explicit ConstBitExpression(bool value);
    std::string ToString(const BitExpressionStates& info) const;
    bool Constant(const BitExpressionStates& input) const;
    bool Calculate(const BitExpressionStates& input) const;
    int Priority() const;
    std::shared_ptr<IBitExpression> DeepCopy() const;
    bool Equals(const std::shared_ptr<IBitExpression>& other) const;
    void Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input);
private:
    bool value;
};

struct VariableBitExpression : public IBitExpression
{
    explicit VariableBitExpression(size_t var_index, size_t bit_number);
    std::string ToString(const BitExpressionStates& info) const;
    bool Constant(const BitExpressionStates& input) const;
    bool Calculate(const BitExpressionStates& input) const;
    int Priority() const;
    std::shared_ptr<IBitExpression> DeepCopy() const;
    bool Equals(const std::shared_ptr<IBitExpression>& other) const;
    void Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input);
private:
    size_t var_index;
    size_t bit_number;
};

struct NegBitExpression : public IBitExpression
{
    explicit NegBitExpression(const std::shared_ptr<IBitExpression>& argument);
    std::string ToString(const BitExpressionStates& info) const;
    bool Constant(const BitExpressionStates& input) const;
    bool Calculate(const BitExpressionStates& input) const;
    int Priority() const;
    std::shared_ptr<IBitExpression> DeepCopy() const;
    bool Equals(const std::shared_ptr<IBitExpression>& other) const;
    void Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input);
    std::shared_ptr<IBitExpression> GetArgument() const;
private:
    std::shared_ptr<IBitExpression> argument;
};

struct OrBitExpression : public IBitExpression
{
    OrBitExpression(const std::shared_ptr<IBitExpression>& left, const std::shared_ptr<IBitExpression>& right);
    std::string ToString(const BitExpressionStates& info) const;
    bool Constant(const BitExpressionStates& input) const;
    bool Calculate(const BitExpressionStates& input) const;
    int Priority() const;
    std::shared_ptr<IBitExpression> DeepCopy() const;
    bool Equals(const std::shared_ptr<IBitExpression>& other) const;
    void Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input);
    std::shared_ptr<IBitExpression> GetLeftArgument() const;
    std::shared_ptr<IBitExpression> GetRightArgument() const;
private:
    std::shared_ptr<IBitExpression> left, right;
};

struct AndBitExpression : public IBitExpression
{
    AndBitExpression(const std::shared_ptr<IBitExpression>& left, const std::shared_ptr<IBitExpression>& right);
    std::string ToString(const BitExpressionStates& info) const;
    bool Constant(const BitExpressionStates& input) const;
    bool Calculate(const BitExpressionStates& input) const;
    int Priority() const;
    std::shared_ptr<IBitExpression> DeepCopy() const;
    bool Equals(const std::shared_ptr<IBitExpression>& other) const;
    void Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input);
    std::shared_ptr<IBitExpression> GetLeftArgument() const;
    std::shared_ptr<IBitExpression> GetRightArgument() const;
private:
    std::shared_ptr<IBitExpression> left, right;
};

struct XorBitExpression : public IBitExpression
{
    XorBitExpression(const std::shared_ptr<IBitExpression>& left, const std::shared_ptr<IBitExpression>& right);
    std::string ToString(const BitExpressionStates& info) const;
    bool Constant(const BitExpressionStates& input) const;
    bool Calculate(const BitExpressionStates& input) const;
    int Priority() const;
    std::shared_ptr<IBitExpression> DeepCopy() const;
    bool Equals(const std::shared_ptr<IBitExpression>& other) const;
    void Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input);
    std::shared_ptr<IBitExpression> GetLeftArgument() const;
    std::shared_ptr<IBitExpression> GetRightArgument() const;
private:
    std::shared_ptr<IBitExpression> left, right;
};

std::shared_ptr<IBitExpression> const_bool(bool value);
std::shared_ptr<IBitExpression> operator&(const std::shared_ptr<IBitExpression>& left, const std::shared_ptr<IBitExpression>& right);
std::shared_ptr<IBitExpression> operator|(const std::shared_ptr<IBitExpression>& left, const std::shared_ptr<IBitExpression>& right);
std::shared_ptr<IBitExpression> operator^(const std::shared_ptr<IBitExpression>& left, const std::shared_ptr<IBitExpression>& right);
std::shared_ptr<IBitExpression> operator~(const std::shared_ptr<IBitExpression>& argument);

std::shared_ptr<IBitExpression> operator!=(const std::shared_ptr<IBitExpression>& left, const std::shared_ptr<IBitExpression>& right);
std::shared_ptr<IBitExpression> operator==(const std::shared_ptr<IBitExpression>& left, const std::shared_ptr<IBitExpression>& right);

