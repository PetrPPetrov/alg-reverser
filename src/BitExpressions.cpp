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

#include "BitExpressions.h"

size_t BitExpressionStates::GetBitIndex(size_t var_index, size_t bit_number)
{
    return var_index * bit_count + bit_number;
}

bool BitExpressionStates::ExtractBit(work_type value, size_t bit_number)
{
    const work_type mask = 1 << bit_number;
    return (value & mask) == mask;
}

size_t BitExpressionStates::GetVarIndex(const std::string& name) const
{
    for (size_t i = 0; i < names.size(); ++i)
    {
        if (name == names[i])
        {
            return i;
        }
    }
    throw std::runtime_error("BitExpressionStates::GetVarIndex(): variable name was not found");
}

size_t BitExpressionStates::GetVarIndex(const std::string& name, size_t index) const
{
    const size_t var_index = GetVarIndex(name);
    if (array_sizes.at(var_index) > index)
    {
        return var_index + index;
    }
    throw std::runtime_error("BitExpressionStates::GetVarIndex(): variable name was not found");
}

std::string BitExpressionStates::GetVarName(size_t var_index) const
{
    const std::string name = names.at(var_index);
    if (array_sizes.at(var_index))
    {
        const size_t array_index = var_index - array_starts.at(var_index);
        return name + "[" + std::to_string(array_index) + "]";
    }
    return name;
}

size_t BitExpressionStates::AddVariable(const std::string& name, bool constant, work_type initial_value)
{
    const size_t var_index = input_variables.size();
    input_variables.push_back(initial_value);
    array_sizes.push_back(0);
    array_starts.push_back(0);
    names.push_back(name);
    for (size_t bit_number = 0; bit_number < bit_count; ++bit_number)
    {
        input_bit_constants.push_back(constant);
        bit_expressions.push_back(std::make_shared<VariableBitExpression>(var_index, bit_number));
    }
    return var_index;
}

size_t BitExpressionStates::AddArray(const std::string& name, bool constant, size_t size, work_type initial_value)
{
    const size_t start_index = input_variables.size();
    for (size_t i = 0; i < size; ++i)
    {
        AddVariable(name, constant, initial_value);
        array_sizes.back() = size;
        array_starts.back() = start_index;
    }
    return start_index;
}

size_t BitExpressionStates::GetVariableCount() const
{
    return input_variables.size();
}

void BitExpressionStates::SetInputVarConstant(size_t var_index, bool constant)
{
    for (size_t bit_number = 0; bit_number < bit_count; ++bit_number)
    {
        const size_t bit_index = GetBitIndex(var_index, bit_number);
        input_bit_constants.at(bit_index) = constant;
    }
}

bool BitExpressionStates::GetInputVarConstant(size_t var_index) const
{
    for (size_t bit_number = 0; bit_number < bit_count; ++bit_number)
    {
        const size_t bit_index = GetBitIndex(var_index, bit_number);
        if (!input_bit_constants.at(bit_index))
        {
            return false;
        }
    }
    return true;
}

void BitExpressionStates::SetInputVarValue(size_t var_index, BitExpressionStates::work_type value)
{
    input_variables.at(var_index) = value;
}

BitExpressionStates::work_type BitExpressionStates::GetInputVarValue(size_t var_index) const
{
    return input_variables.at(var_index);
}

void BitExpressionStates::SetInputBitConstant(size_t bit_index, bool constant)
{
    input_bit_constants.at(bit_index) = constant;
}

bool BitExpressionStates::GetInputBitConstant(size_t bit_index) const
{
    return input_bit_constants.at(bit_index);
}

void BitExpressionStates::SetInputBitValue(size_t bit_index, bool value)
{
    const size_t var_index = bit_index / bit_count;
    const size_t bit_number = bit_index % bit_count;
    const work_type mask = 1 << bit_number;
    if (value)
    {
        input_variables.at(var_index) |= mask;
    }
    else if (ExtractBit(input_variables.at(var_index), bit_number))
    {
        input_variables.at(var_index) ^= mask;
    }
}

bool BitExpressionStates::GetInputBitValue(size_t bit_index) const
{
    const size_t var_index = bit_index / bit_count;
    const size_t bit_number = bit_index % bit_count;
    return ExtractBit(input_variables.at(var_index), bit_number);
}

void BitExpressionStates::SetBitExpression(size_t bit_index, const std::shared_ptr<IBitExpression>& expresssion)
{
    bit_expressions.at(bit_index) = expresssion;
}

std::shared_ptr<IBitExpression> BitExpressionStates::GetBitExpression(size_t bit_index) const
{
    return bit_expressions.at(bit_index);
}

bool BitExpressionStates::GetCurrentBitConstant(size_t bit_index) const
{
    return bit_expressions.at(bit_index)->Constant(*this);
}

bool BitExpressionStates::GetCurrentBitValue(size_t bit_index) const
{
    return bit_expressions.at(bit_index)->Calculate(*this);
}

bool BitExpressionStates::GetCurrentVarConstant(size_t var_index) const
{
    for (size_t bit_number = 0; bit_number < bit_count; ++bit_number)
    {
        const size_t bit_index = GetBitIndex(var_index, bit_number);
        if (!bit_expressions.at(bit_index)->Constant(*this))
        {
            return false;
        }
    }
    return true;
}

BitExpressionStates::work_type BitExpressionStates::GetCurrentVarValue(size_t var_index) const
{
    if (!GetCurrentVarConstant(var_index))
        throw std::runtime_error("BitExpressionStates::GetCurrentVarValue(): variable is not constant");

    return GetOutputVarValue(var_index);
}

BitExpressionStates::work_type BitExpressionStates::GetOutputVarValue(size_t var_index) const
{
    work_type result = 0;
    for (size_t bit_number = 0; bit_number < bit_count; ++bit_number)
    {
        const size_t bit_index = GetBitIndex(var_index, bit_number);
        const work_type bit_value = bit_expressions.at(bit_index)->Calculate(*this) ? 1 : 0;
        const work_type mask = bit_value << bit_number;
        result |= mask;
    }
    return result;
}

void BitExpressionStates::Optimize()
{
    for(size_t i=0; i<bit_expressions.size(); ++i)
    {
        bit_expressions[i]->Optimize(bit_expressions[i], *this);
    }
}

void BitExpressionStates::CopyInputVarValues(const BitExpressionStates& from)
{
    input_variables = from.input_variables;
}

void BitExpressionStates::CopyNames(const BitExpressionStates& from)
{
    array_sizes = from.array_sizes;
    array_starts = from.array_starts;
    names = from.names;
}

void BitExpressionStates::CopyInputConstants(const BitExpressionStates& from)
{
    input_bit_constants = from.input_bit_constants;
}

void BitExpressionStates::CopyBitExpressions(const BitExpressionStates& from)
{
    bit_expressions.clear();
    for (size_t i = 0; i<from.bit_expressions.size(); ++i)
    {
        bit_expressions.push_back(from.bit_expressions[i]->DeepCopy());
    }
}

void BitExpressionStates::Copy(const BitExpressionStates& from)
{
    CopyInputVarValues(from);
    CopyNames(from);
    CopyInputConstants(from);
    CopyBitExpressions(from);
}

ConstBitExpression::ConstBitExpression(bool value_) : value(value_)
{
}

std::string ConstBitExpression::ToString(const BitExpressionStates& info) const
{
    return value ? "1" : "0";
}

bool ConstBitExpression::Constant(const BitExpressionStates& input) const
{
    return true;
}

bool ConstBitExpression::Calculate(const BitExpressionStates& input) const
{
    return value;
}

int ConstBitExpression::Priority() const
{
    return 4;
}

std::shared_ptr<IBitExpression> ConstBitExpression::DeepCopy() const
{
    return std::make_shared<ConstBitExpression>(value);
}

bool ConstBitExpression::Equals(const std::shared_ptr<IBitExpression>& other_) const
{
    ConstBitExpression* other = dynamic_cast<ConstBitExpression*>(other_.get());
    if (other)
    {
        return value == other->value;
    }
    return false;
}

void ConstBitExpression::Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input)
{
}

VariableBitExpression::VariableBitExpression(size_t var_index_, size_t bit_number_) : var_index(var_index_), bit_number(bit_number_)
{
}

std::string VariableBitExpression::ToString(const BitExpressionStates& info) const
{
    return info.GetVarName(var_index) + "." + std::to_string(bit_number);
}

bool VariableBitExpression::Constant(const BitExpressionStates& input) const
{
    return input.GetInputBitConstant(BitExpressionStates::GetBitIndex(var_index, bit_number));
}

bool VariableBitExpression::Calculate(const BitExpressionStates& input) const
{
    return input.GetInputBitValue(BitExpressionStates::GetBitIndex(var_index, bit_number));
}

int VariableBitExpression::Priority() const
{
    return 4;
}

std::shared_ptr<IBitExpression> VariableBitExpression::DeepCopy() const
{
    return std::make_shared<VariableBitExpression>(var_index, bit_number);
}

bool VariableBitExpression::Equals(const std::shared_ptr<IBitExpression>& other_) const
{
    VariableBitExpression* other = dynamic_cast<VariableBitExpression*>(other_.get());
    if (other)
    {
        return var_index == other->var_index && bit_number == other->bit_number;
    }
    return false;
}

void VariableBitExpression::Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input)
{
    if (Constant(input))
    {
        output = std::make_shared<ConstBitExpression>(Calculate(input));
    }
}

NegBitExpression::NegBitExpression(const std::shared_ptr<IBitExpression>& argument_) : argument(argument_)
{
}

std::string NegBitExpression::ToString(const BitExpressionStates& info) const
{
    if (argument->Priority() < Priority())
    {
        return "!(" + argument->ToString(info) + ")";
    }
    else
    {
        return "!" + argument->ToString(info);
    }
}

bool NegBitExpression::Constant(const BitExpressionStates& input) const
{
    return argument->Constant(input);
}

bool NegBitExpression::Calculate(const BitExpressionStates& input) const
{
    return !argument->Calculate(input);
}

int NegBitExpression::Priority() const
{
    return 3;
}

std::shared_ptr<IBitExpression> NegBitExpression::DeepCopy() const
{
    return std::make_shared<NegBitExpression>(argument->DeepCopy());
}

bool NegBitExpression::Equals(const std::shared_ptr<IBitExpression>& other_) const
{
    NegBitExpression* other = dynamic_cast<NegBitExpression*>(other_.get());
    if (other)
    {
        return argument->Equals(other->argument);
    }
    return false;
}

void NegBitExpression::Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input)
{
    if (Constant(input))
    {
        output = std::make_shared<ConstBitExpression>(Calculate(input));
    }
}

OrBitExpression::OrBitExpression(const std::shared_ptr<IBitExpression>& left_, const std::shared_ptr<IBitExpression>& right_) : left(left_), right(right_)
{
}

std::string OrBitExpression::ToString(const BitExpressionStates& info) const
{
    std::string left_str = left->ToString(info);
    if (left->Priority() < Priority())
    {
        left_str = "(" + left_str + ")";
    }
    std::string right_str = right->ToString(info);
    if (right->Priority() < Priority())
    {
        right_str = "(" + right_str + ")";
    }
    return left_str + "+" + right_str;
}

bool OrBitExpression::Constant(const BitExpressionStates& input) const
{
    return left->Constant(input) && right->Constant(input);
}

bool OrBitExpression::Calculate(const BitExpressionStates& input) const
{
    return left->Calculate(input) || right->Calculate(input);
}

int OrBitExpression::Priority() const
{
    return 0;
}

std::shared_ptr<IBitExpression> OrBitExpression::DeepCopy() const
{
    return std::make_shared<OrBitExpression>(left->DeepCopy(), right->DeepCopy());
}

bool OrBitExpression::Equals(const std::shared_ptr<IBitExpression>& other_) const
{
    OrBitExpression* other = dynamic_cast<OrBitExpression*>(other_.get());
    if (other)
    {
        return left->Equals(other->left) && right->Equals(other->right);
    }
    return false;
}

void OrBitExpression::Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input)
{
    if (Constant(input))
    {
        output = std::make_shared<ConstBitExpression>(Calculate(input));
    }
    else if (left->Constant(input))
    {
        if (left->Calculate(input))
        {
            output = std::make_shared<ConstBitExpression>(true);
        }
        else
        {
            output = right;
        }
    }
    else if (right->Constant(input))
    {
        if (right->Calculate(input))
        {
            output = std::make_shared<ConstBitExpression>(true);
        }
        else
        {
            output = left;
        }
    }
}

AndBitExpression::AndBitExpression(const std::shared_ptr<IBitExpression>& left_, const std::shared_ptr<IBitExpression>& right_) : left(left_), right(right_)
{
}

std::string AndBitExpression::ToString(const BitExpressionStates& info) const
{
    std::string left_str = left->ToString(info);
    if (left->Priority() < Priority())
    {
        left_str = "(" + left_str + ")";
    }
    std::string right_str = right->ToString(info);
    if (right->Priority() < Priority())
    {
        right_str = "(" + right_str + ")";
    }
    return left_str + "*" + right_str;
}

bool AndBitExpression::Constant(const BitExpressionStates& input) const
{
    return left->Constant(input) && right->Constant(input);
}

bool AndBitExpression::Calculate(const BitExpressionStates& input) const
{
    return left->Calculate(input) && right->Calculate(input);
}

int AndBitExpression::Priority() const
{
    return 2;
}

std::shared_ptr<IBitExpression> AndBitExpression::DeepCopy() const
{
    return std::make_shared<AndBitExpression>(left->DeepCopy(), right->DeepCopy());
}

bool AndBitExpression::Equals(const std::shared_ptr<IBitExpression>& other_) const
{
    AndBitExpression* other = dynamic_cast<AndBitExpression*>(other_.get());
    if (other)
    {
        return left->Equals(other->left) && right->Equals(other->right);
    }
    return false;
}

void AndBitExpression::Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input)
{
    if (Constant(input))
    {
        output = std::make_shared<ConstBitExpression>(Calculate(input));
    }
    else if (left->Constant(input))
    {
        if (!left->Calculate(input))
        {
            output = std::make_shared<ConstBitExpression>(false);
        }
        else
        {
            output = right;
        }
    }
    else if (right->Constant(input))
    {
        if (!right->Calculate(input))
        {
            output = std::make_shared<ConstBitExpression>(false);
        }
        else
        {
            output = left;
        }
    }
}

XorBitExpression::XorBitExpression(const std::shared_ptr<IBitExpression>& left_, const std::shared_ptr<IBitExpression>& right_) : left(left_), right(right_)
{
}

std::string XorBitExpression::ToString(const BitExpressionStates& info) const
{
    std::string left_str = left->ToString(info);
    if (left->Priority() < Priority())
    {
        left_str = "(" + left_str + ")";
    }
    std::string right_str = right->ToString(info);
    if (right->Priority() < Priority())
    {
        right_str = "(" + right_str + ")";
    }
    return left_str + "^" + right_str;
}

bool XorBitExpression::Constant(const BitExpressionStates& input) const
{
    return left->Constant(input) && right->Constant(input);
}

bool XorBitExpression::Calculate(const BitExpressionStates& input) const
{
    const bool left_value = left->Calculate(input);
    const bool right_value = right->Calculate(input);
    return (left_value || right_value) && (!(left_value && right_value));
}

int XorBitExpression::Priority() const
{
    return 1;
}

std::shared_ptr<IBitExpression> XorBitExpression::DeepCopy() const
{
    return std::make_shared<XorBitExpression>(left->DeepCopy(), right->DeepCopy());
}

bool XorBitExpression::Equals(const std::shared_ptr<IBitExpression>& other_) const
{
    XorBitExpression* other = dynamic_cast<XorBitExpression*>(other_.get());
    if (other)
    {
        return left->Equals(other->left) && right->Equals(other->right);
    }
    return false;
}

void XorBitExpression::Optimize(std::shared_ptr<IBitExpression>& output, const BitExpressionStates& input)
{
    if (Constant(input))
    {
        output = std::make_shared<ConstBitExpression>(Calculate(input));
    }
    else if (left->Equals(right))
    {
        output = std::make_shared<ConstBitExpression>(false);
    }
}
