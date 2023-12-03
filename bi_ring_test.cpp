#include "catch2/catch_test_macros.hpp"
#include "bi_ring.h"
#include <iostream>
#include <sstream>

typedef bi_ring<int, string> ring;

TEST_CASE("Empty") {
    ring empty;
    CHECK(empty.getLength() == 0);
    CHECK(empty.isEmpty());
}

TEST_CASE("Insert"){
    ring ring;

    // Insert at the beginning
    auto it1 = ring.insert(ring.cbegin(), 1, "one");
    CHECK(ring.getLength() == 1);
    CHECK(it1.key() == 1);
    CHECK(it1.info() == "one");

    // Insert at the end
    auto it2 = ring.insert(ring.cend(), 3, "three");
    CHECK(ring.getLength() == 2);
    CHECK(it2.key() == 3);
    CHECK(it2.info() == "three");

    // Insert in the middle (before it2)
    auto it3 = ring.insert(++ring.cbegin(), 2, "two");
    CHECK(ring.getLength() == 3);
    CHECK(it3.key() == 2);
    CHECK(it3.info() == "two");

    // Test sequence
    auto it = ring.cbegin();
    for (int i = 1; i <= 3; i++)
    {
        CHECK(it.key() == i);
        it++;
    }

}

