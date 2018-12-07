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


#include <cstdlib>
#include <cstdio>


int main() {

    char buffer[512];

    size_t bytesRead = 0;
    size_t bytesWritten = 0;

    while (!feof(stdin)) {
        bytesRead = fread(buffer,1, sizeof(buffer), stdin);
        if (bytesRead != 0) {
            bytesWritten = fwrite(buffer,1, bytesRead, stdout);
            if (bytesWritten == 0) {
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}
