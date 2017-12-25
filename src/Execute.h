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

#include "Common.h"
#include "Basic.h"

inline State Execute(const Program& program, State initial_state, const CVarInfo& info)
{
    FullState work_state;
    work_state.vars = initial_state.vars;
    while (work_state.statement_index < program.statements.size())
    {
        //std::cout << "=============================" << std::endl;
        //std::cout << info.GetLabelName(work_state.statement_index) << ":\t" << program.statements[work_state.statement_index]->Print(info) << std::endl;
        FullState next_state = program.statements.at(work_state.statement_index)->Execute(work_state);
        //Basic::DumpDiff(next_state, work_state, info);
        work_state = next_state;
    }
    return work_state;
}

inline State ExecuteAndGenerate(const Program& program, State initial_state, const CVarInfo& info, Program& new_program, State& new_input)
{
    FullState work_state;
    work_state.vars = initial_state.vars;
    while (work_state.statement_index < program.statements.size())
    {
        //std::cout << "=============================" << std::endl;
        //std::cout << info.GetLabelName(work_state.statement_index) << ":\t" << program.statements[work_state.statement_index]->Print(info) << std::endl;
        FullState next_state = program.statements.at(work_state.statement_index)->ExecuteAndGenerate(work_state, new_program, new_input);
        //Basic::DumpDiff(next_state, work_state, info);
        work_state = next_state;
    }
    return work_state;
}
