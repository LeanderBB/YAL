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
#include <gtest/gtest.h>
#include <yal/yal.h>
#include <limits>
#include <yal/util/format.h>
#include <gtest_utils.h>


TEST(Format, Simple) {
    yal::FormaterStack<1024> formater;
    const size_t result = yal::Format(formater, "my pretty %", "string");
    const std::string expected = "my pretty string";
    const yal::StringRef string = formater.toStringRef();
    EXPECT_EQ(result, expected.size());
    EXPECT_EQ(string, expected);
}


TEST(Format, Multiple) {
    yal::FormaterStack<1024> formater;
    const size_t result = yal::Format(formater, "my pretty %, is it %", "string", false);
    const std::string expected = "my pretty string, is it false";
    const yal::StringRef string = formater.toStringRef();
    EXPECT_EQ(result, expected.size());
    EXPECT_EQ(string, expected);
}


TEST(Format, MissingArgs) {
    yal::FormaterStack<1024> formater;
    const size_t result = yal::Format(formater, "my pretty %, is it % or is it %", "string", false);
    const std::string expected = "my pretty string, is it false or is it %";
    const yal::StringRef string = formater.toStringRef();
    EXPECT_EQ(result, expected.size());
    EXPECT_EQ(string, expected);
}

TEST(Format, NotEnoughSpace) {
    yal::FormaterStack<15> formater;
    const size_t result = yal::Format(formater, "my pretty %, is it % or is it %", "string", false);
    const std::string expected = "my pretty strin";
    const yal::StringRef string = formater.toStringRef();
    EXPECT_EQ(result, expected.size());
    EXPECT_EQ(string, expected);
}


TEST(Format, NotEnoughSpaceInteger) {
    yal::FormaterStack<12> formater;
    const size_t result = yal::Format(formater, "my pretty %", 2048u);
    const std::string expected = "my pretty 2";
    const yal::StringRef string = formater.toStringRef();
    EXPECT_EQ(result, expected.size());
    EXPECT_EQ(string, expected);
}
