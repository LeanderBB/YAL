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

#include "yal/error/errorreporter.h"
#include "yal/error/error.h"

namespace yal {

    ErrorReporter::ErrorReporter():
    m_errors(),
    m_treatAllErrorsAsFatal(0),
    m_hasErrors(0),
    m_hasWarnings(0),
    m_hasSuggestions(0),
    m_hasFatalError(0) {

    }

    ErrorReporter::~ErrorReporter() {

    }

    void
    ErrorReporter::report(ErrorPtr&& error) {
        if (m_treatAllErrorsAsFatal == 1) {
            m_hasFatalError = 1;
            m_hasErrors = 1;
        } else {
            m_hasErrors = m_hasErrors || error->isError();
            m_hasWarnings = m_hasWarnings || error->isWarning();
            m_hasSuggestions = m_hasSuggestions || error->isSuggestion();
            m_hasFatalError = m_hasFatalError || error->isFatal();
        }
        m_errors.emplace_back(std::move(error));
    }

    const Error*
    ErrorReporter::getFirstError() const {
        return m_errors.empty()
                ? nullptr
                : m_errors.front().get();
    }

    const Error*
    ErrorReporter::getLastError() const {
        return m_errors.empty()
                ? nullptr
                : m_errors.back().get();
    }

    void
    ErrorReporter::forEarchError(Callable callable) const {
        for(auto& error : m_errors) {
            callable(*error);
        }
    }

    void
    ErrorReporter::clear() {
        m_errors.clear();
    }


    void
    ErrorReporter::setTreatAllErrorsAsFatal(const bool v) {
        m_treatAllErrorsAsFatal = v ? 1 : 0;
    }
}
