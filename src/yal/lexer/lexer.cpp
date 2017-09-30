#include "yal/lexer/lexer.h"
#include "yal/io/memorystream.h"
#include "yal/lexer/tokens.h"

namespace yal {

    Lexer::Status
    Lexer::scan() {
        return re2cExecute();
    }

}
