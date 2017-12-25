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

//std::pair<FullState, bool> ReversalExecuteImpl(const Program& program, FullState final_state)
//{
//    if (final_state.statement_index == 0)
//    {
//        return std::make_pair(final_state, true);
//    }
//    for (size_t j = 0; j < program.statements.size(); ++j)
//    {
//        const size_t possible_previous_state_count = program.statements[j]->GetPreviousStateCount(final_state);
//        for (size_t p = 0; p < possible_previous_state_count; ++p)
//        {
//            FullState previous_state = program.statements[j]->GetPreviousState(p, final_state);
//            std::pair<FullState, bool> result = ReversalExecuteImpl(program, previous_state);
//            if (result.second)
//                return result;
//        }
//    }
//    return std::make_pair(final_state, program.statements.empty());
//}
//
//std::pair<State, bool> ReversalExecute(const Program& program, State final_state)
//{
//    FullState work_state;
//    work_state.vars = final_state.vars;
//    work_state.statement_index = program.statements.size();
//    return ReversalExecuteImpl(program, work_state);
//}
//
//void ReversalExecuteImpl(const Program& program, FullState final_state, std::vector<State>& results)
//{
//    if (final_state.statement_index == 0)
//    {
//        results.push_back(final_state);
//        return;
//    }
//    for (size_t j = 0; j < program.statements.size(); ++j)
//    {
//        const size_t possible_previous_state_count = program.statements[j]->GetPreviousStateCount(final_state);
//        for (size_t p = 0; p < possible_previous_state_count; ++p)
//        {
//            FullState previous_state = program.statements[j]->GetPreviousState(p, final_state);
//            ReversalExecuteImpl(program, previous_state, results);
//        }
//    }
//    if (program.statements.empty())
//    {
//        results.push_back(final_state);
//        return;
//    }
//}
//
//void ReversalExecute(const Program& program, State final_state, std::vector<State>& results)
//{
//    FullState work_state;
//    work_state.vars = final_state.vars;
//    work_state.statement_index = program.statements.size();
//    ReversalExecuteImpl(program, work_state, results);
//}
