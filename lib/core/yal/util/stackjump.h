/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
 *
 *  This file is part of YAL.
 *
 *  YAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  YAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with YAL. If not, see <http://www.gnu.org/licenses/>.
 */


#include <csetjmp>

namespace yal {

    class StackJump {
    public:
        static const constexpr int kJumpValue = 101;

        void trigger() {
            longjmp(m_env, kJumpValue);
        }

    public:
        jmp_buf m_env;
    };


#define YAL_STACKJUMP_MARK(sj) setjmp(sj.m_env) == yal::StackJump::kJumpValue
}
