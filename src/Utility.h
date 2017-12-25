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

void Dump(const std::string& text, const State& state)
{
    std::cout << text << std::endl;
    for (size_t i = 0; i < state.vars.size(); ++i)
    {
        std::cout << "Var" << i << ": " << state.vars[i].value << std::endl;
    }
}
