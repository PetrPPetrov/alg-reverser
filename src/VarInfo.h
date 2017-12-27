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
#include <vector>
#include <string>

struct Info
{
    virtual std::string GetVarName(size_t index) const = 0;
    virtual std::string GetArrayName(size_t index) const = 0;
    virtual std::string GetLabelName(size_t index) const = 0;
};

class CVarInfo : public Info
{
    struct ObjectInfo
    {
        std::string name;
        size_t index;
        int array_size;
    };
    std::vector<ObjectInfo> arrays;
    std::vector<ObjectInfo> variables;
    std::vector<ObjectInfo> labels;
public:
    bool unnamed_labels;

    CVarInfo() : unnamed_labels(false)
    {
    }

    void AddArrayVariable(const std::string& name, size_t index, int array_size)
    {
        ObjectInfo new_item;
        new_item.name = name;
        new_item.index = index;
        new_item.array_size = array_size;
        arrays.push_back(new_item);
    }
    void AddVariable(const std::string& name, size_t index)
    {
        ObjectInfo new_item;
        new_item.name = name;
        new_item.index = index;
        new_item.array_size = 1;
        variables.push_back(new_item);
    }
    void AddLabel(const std::string& name, size_t index)
    {
        ObjectInfo new_item;
        new_item.name = name;
        new_item.index = index;
        new_item.array_size = 1;
        labels.push_back(new_item);
    }
    std::string GetVarName(size_t index) const
    {
        for (size_t i = 0; i < arrays.size(); ++i)
        {
            if (index >= arrays[i].index && index < arrays[i].index + arrays[i].array_size)
                return arrays[i].name + "[" + std::to_string(index - arrays[i].index) + "]";
        }
        for (size_t i = 0; i < variables.size(); ++i)
        {
            if (index == variables[i].index)
                return variables[i].name;
        }
        return "V" + std::to_string(index) + "";
    }
    std::string GetArrayName(size_t index) const
    {
        for (size_t i = 0; i < arrays.size(); ++i)
        {
            if (index >= arrays[i].index && index < arrays[i].index + arrays[i].array_size)
                return arrays[i].name;
        }
        return "V" + std::to_string(index) + "";
    }
    std::string GetLabelName(size_t index) const
    {
        std::string result;
        for (size_t i = 0; i < labels.size(); ++i)
        {
            if (index == labels[i].index)
                result = labels[i].name;
        }
        if (result.empty() && unnamed_labels)
        {
            result = std::to_string(index);
        }
        return result;
    }
};
