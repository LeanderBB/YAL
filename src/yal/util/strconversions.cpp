/*
 *  Copyright 2017 by Leander Beernaert (leanderbb@gmail.com)
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

#include "yal/util/strconversions.h"
#include "yal/util/stringref.h"
#include <limits>

namespace yal {

    bool StringRefToInteger(uint64_t& out,
                            const StringRef& ref,
                            bool& isNegative) {
        bool parsingStarted = false;
        bool terminateLoop = false;
        isNegative = false;
        out = 0;
        for (size_t i = 0; i < ref.size() && !terminateLoop; ++i) {
            const char ch = ref[i];

            switch (ch) {
            case ' ':
                if (!parsingStarted) {
                    continue;
                } else {
                    terminateLoop = true;
                }
                break;

            case '-':
                isNegative = true;
                continue;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {
                uint64_t result = static_cast<uint64_t>(ch - '0');
                uint64_t nextValue = result;
                if (parsingStarted) {
                    nextValue  = (out * 10) + result;
                    // detect overflow and invalid value
                    if (nextValue < out) {
                        return false;
                    }
                }

                if (isNegative) {
                    if (nextValue > static_cast<uint64_t>(std::abs(std::numeric_limits<int64_t>::lowest()))) {
                        return false;
                    }
                }

                if (parsingStarted) {
                    out *= 10;
                }
                out += result;
                parsingStarted = true;
                break;
            }

            default:
                return false;
            }
        }

        if (parsingStarted) {
            if (isNegative) {
                int64_t& neg = reinterpret_cast<int64_t&>(out);
                neg*= -1;
            }
        }
        return parsingStarted;
    }


}
