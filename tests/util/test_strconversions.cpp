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
#include <gtest/gtest.h>
#include <yal/yal.h>
#include <limits>
#include <gtest_utils.h>
#include <yal/util/strconversions.h>

TEST(StringConversion, int8Pos) {
    const yal::StringRef input ="127";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, uint64_t(127));
    EXPECT_EQ(isNegative, false);
}

TEST(StringConversion, int8Neg) {
    const yal::StringRef input ="-128";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(static_cast<int64_t>(value), int64_t(-128));
    EXPECT_EQ(isNegative, true);
}

TEST(StringConversion, uint8) {
    const yal::StringRef input ="255";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, uint64_t(255));
    EXPECT_EQ(isNegative, false);
}


TEST(StringConversion, int16Pos) {
    const yal::StringRef input ="32767";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, uint64_t(32767));
    EXPECT_EQ(isNegative, false);
}

TEST(StringConversion, int16Neg) {
    const yal::StringRef input ="-32768";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(static_cast<int64_t>(value), int64_t(-32768));
    EXPECT_EQ(isNegative, true);
}

TEST(StringConversion, uint16) {
    const yal::StringRef input ="65535";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, uint64_t(65535));
    EXPECT_EQ(isNegative, false);
}


TEST(StringConversion, int32Pos) {
    const yal::StringRef input ="2147483647";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, uint64_t(2147483647));
    EXPECT_EQ(isNegative, false);
}

TEST(StringConversion, int32Neg) {
    const yal::StringRef input ="-2147483648";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(static_cast<int64_t>(value), int64_t(-2147483648));
    EXPECT_EQ(isNegative, true);
}

TEST(StringConversion, uint32) {
    const yal::StringRef input ="4294967295";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, uint64_t(4294967295));
    EXPECT_EQ(isNegative, false);
}

TEST(StringConversion, int64Pos) {
    const yal::StringRef input ="9223372036854775807";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, uint64_t(9223372036854775807));
    EXPECT_EQ(isNegative, false);
}

TEST(StringConversion, int64Neg) {
    const yal::StringRef input ="-9223372036854775808";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(static_cast<int64_t>(value), std::numeric_limits<int64_t>::lowest());
    EXPECT_EQ(isNegative, true);
}

TEST(StringConversion, uint64) {
    const yal::StringRef input ="18446744073709551615";
    uint64_t value = 0;
    bool isNegative = false;
    const bool result = yal::StringRefToInteger(value, input, isNegative);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, std::numeric_limits<uint64_t>::max());
    EXPECT_EQ(isNegative, false);
}


TEST(StringConversion, doublePositive) {
    const yal::StringRef input ="3.14534534345";
    double value = 0;
    const double expected = 3.14534534345;
    const bool result = yal::StringRefToDecimal(value, input);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, expected);
}

TEST(StringConversion, doubleNegative) {
    const yal::StringRef input ="-3.14534534345";
    double value = 0;
    const double expected = -3.14534534345;
    const bool result = yal::StringRefToDecimal(value, input);
    EXPECT_EQ(result, true);
    EXPECT_EQ(value, expected);
}

TEST(StringReplace, pattern_bigger) {
    const yal::StringRef input = "test";
    const std::string result = input.replace("adeafeafafe", "ad");
    EXPECT_EQ(input.toString(), result);
}

TEST(StringReplace, pattern_replace) {
    const yal::StringRef input = "test::module::type";
    const yal::StringRef expected = "test_module_type";
    const std::string result = input.replace("::", "_");
    EXPECT_EQ(expected.toString(), result);
}

TEST(StringReplace, pattern_replace_end) {
    const yal::StringRef input = "test::module::type::";
    const yal::StringRef expected = "test_module_type_";
    const std::string result = input.replace("::", "_");
    EXPECT_EQ(expected.toString(), result);
}

