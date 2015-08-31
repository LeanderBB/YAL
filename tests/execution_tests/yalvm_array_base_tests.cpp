#include <cstring>
#include <string>
#include <stdarg.h>
#include <gtest/gtest.h>
#include <yalvm/yalvm_array.h>
#include <yalvm/yalvm_error.h>


static const yalvm_u32 sg_elem_size = sizeof(yalvm_i32);

class ScopedArray
{
public:
    ScopedArray(const yalvm_u32 capacity = 0)
    {
        yalvm_array_base_init(&array, sg_elem_size, capacity);
    }

    ~ScopedArray()
    {
        yalvm_array_base_destroy(&array);
    }


    yalvm_array_base_t array;
};

#define CHECK_ARRRAY_EQUAL(array, expected, size) \
    for (unsigned i = 0; i < size; ++i) \
{ \
    int* array_elem = static_cast<int*>(yalvm_array_base_get(array, sg_elem_size, i)); \
    EXPECT_EQ(*array_elem, expected[i]); \
}

TEST(YALVM_ArrayBase, append)
{
    ScopedArray scp_array;
    yalvm_array_base_t* array = &scp_array.array;
    yalvm_array_base_init(array, sg_elem_size, 0);


    const int expected_array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i < 10; ++i)
    {
        const yalvm_bool append_result = yalvm_array_base_append(array, sg_elem_size, &i);
        EXPECT_EQ(append_result, yalvm_true);
    }

    CHECK_ARRRAY_EQUAL(array, expected_array, 10)
}


TEST(YALVM_ArrayBase, copy)
{
    ScopedArray scp_array;
    ScopedArray scp_dst;
    yalvm_array_base_t* array = &scp_array.array;
    yalvm_array_base_init(array, sg_elem_size, 0);


    const int expected_array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i < 10; ++i)
    {
        const yalvm_bool append_result = yalvm_array_base_append(array, sg_elem_size, &i);
        EXPECT_EQ(append_result, yalvm_true);
    }

    CHECK_ARRRAY_EQUAL(array, expected_array, 10)

    EXPECT_EQ(yalvm_array_base_clone(&scp_dst.array, array, sg_elem_size), yalvm_true);

    for (int i = 0; i < 10; ++i)
    {
        const yalvm_bool append_result = yalvm_array_base_append(&scp_dst.array, sg_elem_size, &i);
        EXPECT_EQ(append_result, yalvm_true);
    }
}

TEST(YALVM_ArrayBase, insert)
{
    ScopedArray scp_array;
    yalvm_array_base_t* array = &scp_array.array;
    yalvm_array_base_init(array, sg_elem_size, 0);


    const int expected_array1[5] = {0, 1, 2, 3, 4};
    const int expected_array2[6] = {0, 1, 2, 5, 3, 4};
    const int expected_array3[7] = {6, 0, 1, 2, 5, 3, 4};
    const int expected_array4[8] = {6, 0, 1, 2, 5, 3, 4, 7};
    const int expected_array5[9] = {6, 0, 1, 2, 5, 3, 4, 8, 7};

    int i;
    for (i = 0; i < 5; ++i)
    {
        const yalvm_bool append_result = yalvm_array_base_append(array, sg_elem_size, &i);
        EXPECT_EQ(append_result, yalvm_true);
    }

    CHECK_ARRRAY_EQUAL(array, expected_array1, YALVM_ARRAY_SIZE(expected_array1))

    int value = i++;
    yalvm_bool result = yalvm_array_base_insert(array, sg_elem_size, 3 ,&value);
    EXPECT_EQ(result, yalvm_true);

    CHECK_ARRRAY_EQUAL(array, expected_array2, YALVM_ARRAY_SIZE(expected_array2))

    // insert front
    value = i++;
    result = yalvm_array_base_insert(array, sg_elem_size, 0 ,&value);
    EXPECT_EQ(result, yalvm_true);

    CHECK_ARRRAY_EQUAL(array, expected_array3, YALVM_ARRAY_SIZE(expected_array3))

    // insert back
    value = i++;
    result = yalvm_array_base_insert(array, sg_elem_size, array->size ,&value);
    EXPECT_EQ(result, yalvm_true);

    CHECK_ARRRAY_EQUAL(array, expected_array4, YALVM_ARRAY_SIZE(expected_array4))

    // insert before back
    value = i++;
    result = yalvm_array_base_insert(array, sg_elem_size, array->size - 1 ,&value);
    EXPECT_EQ(result, yalvm_true);

    CHECK_ARRRAY_EQUAL(array, expected_array5, YALVM_ARRAY_SIZE(expected_array5))
}

TEST(YALVM_ArrayBase, remove)
{
    ScopedArray scp_array;
    yalvm_array_base_t* array = &scp_array.array;
    yalvm_array_base_init(array, sg_elem_size, 0);


    const int expected_array1[5] = {0, 1, 2, 3, 4};
    const int expected_array2[6] = {0, 1, 2, 5, 3, 4};
    const int expected_array3[7] = {6, 0, 1, 2, 5, 3, 4};
    const int expected_array4[8] = {6, 0, 1, 2, 5, 3, 4, 7};
    const int expected_array5[9] = {6, 0, 1, 2, 5, 3, 4, 8, 7};

    unsigned i;
    for (i = 0; i < YALVM_ARRAY_SIZE(expected_array5); ++i)
    {
        const yalvm_bool append_result = yalvm_array_base_append(array, sg_elem_size,
                                                                 &expected_array5[i]);
        EXPECT_EQ(append_result, yalvm_true);
    }

    CHECK_ARRRAY_EQUAL(array, expected_array5, YALVM_ARRAY_SIZE(expected_array5))

    yalvm_bool result = yalvm_false;

    // remove before back
    result = yalvm_array_base_remove(array, sg_elem_size, array->size - 2);
    EXPECT_EQ(result, yalvm_true);

    CHECK_ARRRAY_EQUAL(array, expected_array4, YALVM_ARRAY_SIZE(expected_array4))

    // remove back
    result = yalvm_array_base_remove(array, sg_elem_size, array->size - 1);
    EXPECT_EQ(result, yalvm_true);

    CHECK_ARRRAY_EQUAL(array, expected_array3, YALVM_ARRAY_SIZE(expected_array3))

    // remove front
    result = yalvm_array_base_remove(array, sg_elem_size, 0);
    EXPECT_EQ(result, yalvm_true);

    CHECK_ARRRAY_EQUAL(array, expected_array2, YALVM_ARRAY_SIZE(expected_array2))

    // remove at idx 3
    result = yalvm_array_base_remove(array, sg_elem_size, 3);
    EXPECT_EQ(result, yalvm_true);

    CHECK_ARRRAY_EQUAL(array, expected_array1, YALVM_ARRAY_SIZE(expected_array1))

}
