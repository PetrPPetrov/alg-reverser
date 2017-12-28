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
#include <set>
#include <string>

#include "BitExpressions.h"

struct FullState : public BitExpressionStates
{
    size_t statement_index;

    FullState();
};

struct IStatement;

struct Program
{
    std::vector<std::shared_ptr<IStatement> > statements;
};

struct IStatement
{
    size_t GetLineNumber() const;
    virtual ~IStatement();
    virtual void Execute(FullState& state) const = 0;
    virtual std::string Print(const FullState& info) const;
    std::string GetLabel() const;

protected:
    IStatement(const Program& program, size_t line_number, const std::string& label);
    const Program& program;
private:
    size_t line_number;
    std::string label;
};

struct Nop : public IStatement
{
    static std::shared_ptr<Nop> Create(Program& program, const std::string& label="");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    Nop(const Program& program, size_t line_number, const std::string& label);
};

struct SetConstant : public IStatement
{
    static std::shared_ptr<SetConstant> Create(Program& program, size_t result_index, BitExpressionStates::work_type value, bool hex = true, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    SetConstant(const Program& program, size_t line_number, const std::string& label, size_t result_index, BitExpressionStates::work_type value, bool hex);
    size_t result_index;
    BitExpressionStates::work_type value;
    bool hex;
};

struct LetRA : public IStatement
{
    static std::shared_ptr<LetRA> Create(Program& program, size_t result_index, size_t argument_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    LetRA(const Program& program, size_t line_number, const std::string& label, size_t result_index, size_t argument_index);
    size_t result_index;
    size_t argument_index;
};

struct LetRAI : public IStatement
{
    static std::shared_ptr<LetRAI> Create(Program& program, size_t result_index, size_t argument_index, size_t index_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    LetRAI(const Program& program, size_t line_number, const std::string& label, size_t result_index, size_t argument_index, size_t index_index);
    size_t result_index;
    size_t argument_index;
    size_t index_index;
};

struct AndRA : public IStatement
{
    static std::shared_ptr<AndRA> Create(Program& program, size_t result_index, size_t argument_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    AndRA(const Program& program, size_t line_number, const std::string& label, size_t result_index, size_t argument_index);
    size_t result_index;
    size_t argument_index;
};

struct OrRA : public IStatement
{
    static std::shared_ptr<OrRA> Create(Program& program, size_t result_index, size_t argument_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    OrRA(const Program& program, size_t line_number, const std::string& label, size_t result_index, size_t argument_index);
    size_t result_index;
    size_t argument_index;
};

struct XorRA : public IStatement
{
    static std::shared_ptr<XorRA> Create(Program& program, size_t result_index, size_t argument_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    XorRA(const Program& program, size_t line_number, const std::string& label, size_t result_index, size_t argument_index);
    size_t result_index;
    size_t argument_index;
};

struct InverseR : public IStatement
{
    static std::shared_ptr<InverseR> Create(Program& program, size_t result_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    InverseR(const Program& program, size_t line_number, const std::string& label, size_t result_index);
    size_t result_index;
};

struct AddRA : public IStatement
{
    static std::shared_ptr<AddRA> Create(Program& program, size_t result_index, size_t argument_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    AddRA(const Program& program, size_t line_number, const std::string& label, size_t result_index, size_t argument_index);
    size_t result_index;
    size_t argument_index;
};

struct IncR : public IStatement
{
    static std::shared_ptr<IncR> Create(Program& program, size_t result_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    IncR(const Program& program, size_t line_number, const std::string& label, size_t result_index);
    size_t result_index;
};

struct MulRA : public IStatement
{
    static std::shared_ptr<MulRA> Create(Program& program, size_t result_index, size_t argument_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    MulRA(const Program& program, size_t line_number, const std::string& label, size_t result_index, size_t argument_index);
    size_t result_index;
    size_t argument_index;
};

struct RestDivideRA : public IStatement
{
    static std::shared_ptr<RestDivideRA> Create(Program& program, size_t result_index, size_t argument_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    RestDivideRA(const Program& program, size_t line_number, const std::string& label, size_t result_index, size_t argument_index);
    size_t result_index;
    size_t argument_index;
};

struct LcrRA : public IStatement
{
    static std::shared_ptr<LcrRA> Create(Program& program, size_t result_index, size_t argument_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    LcrRA(const Program& program, size_t line_number, const std::string& label, size_t result_index, size_t argument_index);
    size_t result_index;
    size_t argument_index;
};

struct Goto : public IStatement
{
    static std::shared_ptr<Goto> Create(Program& program, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
    void SetDestinationLine(size_t destination_line);
    size_t GetDestinationLine() const;
    std::string GetDestinationLabel() const;
protected:
    Goto(const Program& program, size_t line_number, const std::string& label);
private:
    size_t destination_line;
};

struct IfAMoreBGoto : public Goto
{
    static std::shared_ptr<IfAMoreBGoto> Create(Program& program, size_t a_index, size_t b_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    IfAMoreBGoto(const Program& program, size_t line_number, const std::string& label, size_t a_index, size_t b_index);
    size_t a_index;
    size_t b_index;
};

struct IfALessBGoto : public Goto
{
    static std::shared_ptr<IfALessBGoto> Create(Program& program, size_t a_index, size_t b_index, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    IfALessBGoto(const Program& program, size_t line_number, const std::string& label, size_t a_index, size_t b_index);
    size_t a_index;
    size_t b_index;
};

struct PrintVar : public IStatement
{
    static std::shared_ptr<PrintVar> Create(Program& program, size_t argument_index, const std::string& text, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    PrintVar(const Program& program, size_t line_number, const std::string& label, size_t argument_index, const std::string& text);
    size_t argument_index;
    std::string text;
};

struct PrintText : public IStatement
{
    static std::shared_ptr<PrintText> Create(Program& program, const std::string& text, const std::string& label = "");
    void Execute(FullState& state) const;
    std::string Print(const FullState& info) const;
private:
    PrintText(const Program& program, size_t line_number, const std::string& label, const std::string& text);
    std::string text;
};
