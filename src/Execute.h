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

#include "BitExpressions.h"
#include "Program.h"

inline void Execute(const Program& program, const BitExpressionStates& initial_state, BitExpressionStates& output_state)
{
    FullState work_state;
    work_state.Copy(initial_state);
    while (work_state.statement_index < program.statements.size())
    {
        program.statements.at(work_state.statement_index)->Execute(work_state);
    }
    output_state.Copy(work_state);
}
