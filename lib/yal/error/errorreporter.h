/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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

#include "yal/yal.h"

#include <memory>
#include <vector>
#include <functional>

namespace yal {

    class Error;
    using ErrorPtr = std::unique_ptr<const Error>;

    class ErrorReporter {
    public:
        using Callable = std::function<void(const Error&)>;

        ErrorReporter();

        ~ErrorReporter();

        YAL_MOVE_ONLY_CLASS(ErrorReporter);

        void report(ErrorPtr&& error);

        const Error* getLastError() const;

        void forEarchError(Callable callable) const;

        void clear();

        bool hasErrors() const {
            return m_hasErrors == 1;
        }

        bool hasWarnings() const {
            return m_hasWarnings == 1;
        }

        bool hasSuggestions() const {
            return m_hasSuggestions == 1;
        }

        bool hasFatalError() const {
            return m_hasFatalError;
        }

        bool empty() const {
            return m_errors.empty();
        }

        void setTreatAllErrorsAsFatal(const bool v);

    private:
        using ErrorList = std::vector<ErrorPtr>;
        ErrorList m_errors;
        unsigned m_treatAllErrorsAsFatal:1;
        unsigned m_hasErrors:1;
        unsigned m_hasWarnings:1;
        unsigned m_hasSuggestions:1;
        unsigned m_hasFatalError:1;
    };
}
