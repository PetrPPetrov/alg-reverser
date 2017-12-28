#
# AlgReverser reverses algorithms
# Copyright (C) 2017 Petr Petrovich Petrov
#
# This file is part of AlgReverser.
#
# AlgReverser is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# AlgReverser is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with AlgReverser.  If not, see <http://www.gnu.org/licenses/>.
#

project(alg_reverser)

cmake_minimum_required(VERSION 2.8)

add_executable(alg_reverser
  ${alg_reverser_SOURCE_DIR}/src/BitExpressions.h
  ${alg_reverser_SOURCE_DIR}/src/BitExpressions.cpp
  ${alg_reverser_SOURCE_DIR}/src/Program.h
  ${alg_reverser_SOURCE_DIR}/src/Program.cpp
  ${alg_reverser_SOURCE_DIR}/src/Execute.h
  ${alg_reverser_SOURCE_DIR}/src/MD5.h
  ${alg_reverser_SOURCE_DIR}/src/Utility.h
  ${alg_reverser_SOURCE_DIR}/src/VarInfo.h
  ${alg_reverser_SOURCE_DIR}/src/Main.cpp
)
