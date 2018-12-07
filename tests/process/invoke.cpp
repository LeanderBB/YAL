/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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


#include <yal/yal.h>
#include <yal/io/memorystream.h>
#include <yal/os/process.h>

using namespace yal;

int main() {
    Process p;
    ProcessArgs args;
    args.captureOutput = true;
    args.binPath = "./redirect";



    StringRef catInput =
            "Adaefaefka;fka;lkf;laekf;lkaefae;fkae;kf;eak\n"
            "asfaf qfq-1-31-0i31- -1--013fk-13-0fk 1k-013 f13\n";

    MemoryStream inputStream;
    inputStream.attach(catInput.data(), catInput.size(), false);
    args.input = &inputStream;

    MemoryStream outputStream;
    if (!outputStream.create(catInput.size(),
                             (ByteStream::kModeReadWrite |ByteStream::kFlagSeekable))) {
         std::cerr << "Failed to allocate output buffer\n";
    }

    ProcessResult result = p.run(args);
    if (result.output) {
        ByteStream& stream = *result.output;
        char buffer[2048];
        size_t bytesRead = 0;
        do {
            bytesRead = stream.read(buffer, sizeof(buffer));
            outputStream.write(buffer, bytesRead);
        } while(bytesRead != 0);
    }

    outputStream.seek(0);

    if (result.status != ProcessStatus::Ok) {
        std::cerr << "Failed to invoke command \n";
        return EXIT_FAILURE;
    }
    const int execResult = p.join();
    if (execResult != 0) {
        return EXIT_FAILURE;
    }

    StringRef outputStr(reinterpret_cast<const char*>(outputStream.getPtr()),
                        outputStream.getSizeBytes());

    if (outputStr == catInput) {
        std::cout << "Output matches input\n";
        return EXIT_SUCCESS;
    } else {
        std::cerr << "Output does not match input \n";
    }

    return EXIT_FAILURE;
}
