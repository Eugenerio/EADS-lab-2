#include "catch2/catch_test_macros.hpp"
#include "bi_ring.h"
#include <iostream>

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

TEST_CASE("Push Front")
{
    bi_ring<int, string> ring;

    // Push at the front
    ring.push_front(1, "one");
    CHECK(ring.getLength() == 1);
    CHECK(ring.begin().key() == 1);
    CHECK(ring.begin().info() == "one");

    // Push more at the front
    ring.push_front(0, "zero");
    CHECK(ring.getLength() == 2);
    CHECK(ring.begin().key() == 0);
    CHECK(ring.begin().info() == "zero");
    CHECK((++ring.begin()).key() == 1);
    CHECK((++ring.begin()).info() == "one");

}

TEST_CASE("pushback")
{
    bi_ring<int, string> ring;

    // Push at the back
    ring.push_back(3, "three");
    CHECK(ring.getLength() == 1);
    CHECK(ring.begin().key() == 3);
    CHECK(ring.begin().info() == "three");

    // Push more at the back
    ring.push_back(4, "four");
    CHECK(ring.getLength() == 2);
    CHECK(ring.begin().key() == 3);
    CHECK(ring.begin().info() == "three");
    CHECK((++ring.begin()).key() == 4);
    CHECK((++ring.begin()).info() == "four");

}

TEST_CASE("erase")
{
    bi_ring<int, string> ring;
    ring.insert(ring.cend(), 1, "one");
    ring.insert(ring.cend(), 2, "two");
    ring.insert(ring.cend(), 3, "three");

    // Erase from the middle
    auto it5 = ++ring.cbegin();
    CHECK(it5.key() == 2);
    auto it6 = ring.erase(it5);
    CHECK(ring.getLength() == 2);
    CHECK(it6 == --ring.end());

    // Erase from the beginning
    auto it1 = ring.cbegin();
    CHECK(it1.key() == 1);
    auto it2 = ring.erase(it1);
    CHECK(ring.getLength() == 1);
    CHECK(it2.key() == 3);

    // Erase from the end
    auto it3 = --ring.cend();
    CHECK(it3.key() == 3);
    auto it4 = ring.erase(it3);
    CHECK(ring.getLength() == 0);
    CHECK(it4 == ring.end());
    CHECK(it4 == ring.begin());

}

TEST_CASE("pop front")
{
    bi_ring<int, string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");

    // Pop from the front
    auto it1 = ring.cbegin();
    CHECK(it1.key() == 1);
    auto it2 = ring.pop_front();
    CHECK(ring.getLength() == 1);
    CHECK(it2 == ring.begin());
    CHECK(it2.key() == 2);
    CHECK(it2.info() == "two");

    // Pop more from the front
    auto it3 = ring.cbegin();
    CHECK(it3.key() == 2);
    auto it4 = ring.pop_front();
    CHECK(ring.getLength() == 0);
    CHECK(it4 == ring.end());

    // Pop from an empty ring
    bi_ring<int, string> emptyRing;
    auto it5 = emptyRing.pop_front();
    CHECK(emptyRing.getLength() == 0);
    CHECK(it5 == emptyRing.end());

}

TEST_CASE("pop back")
{
    bi_ring<int, string> ring;
    ring.push_back(2, "two");
    ring.push_back(3, "three");

    // Pop from the back
    auto it1 = --ring.cend();
    CHECK(it1.key() == 3);
    auto it2 = ring.pop_back();
    CHECK(ring.getLength() == 1);
    CHECK(it2.key() == 2);
    CHECK(it2.info() == "two");

    // Pop more from the back
    auto it3 = --ring.cend();
    CHECK(it3.key() == 2);
    auto it4 = ring.pop_back();
    CHECK(ring.getLength() == 0);
    CHECK(it4 == ring.end());

    // Pop from an empty ring
    bi_ring<int, string> emptyRing;
    auto it5 = emptyRing.pop_back();
    CHECK(emptyRing.getLength() == 0);
    CHECK(it5 == emptyRing.end());

}

TEST_CASE("clear")
{
    bi_ring<int, string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");
    ring.push_back(3, "three");

    CHECK(ring.getLength() == 3);
    ring.clear();
    CHECK(ring.getLength() == 0);
    CHECK(ring.isEmpty() == true);

    ring.clear(); // clearing empty ring
    CHECK(ring.isEmpty() == true);

}

TEST_CASE("copy constructor")
{
    // Original ring
    bi_ring<int, string> originalRing;
    originalRing.push_back(1, "one");
    originalRing.push_back(2, "two");
    originalRing.push_back(3, "three");

    // Copy constructor
    bi_ring<int, string> copiedRing(originalRing);

    // Test the size of the copied ring
    CHECK(copiedRing.getLength() == originalRing.getLength());

    // Test the content of the copied ring
    auto originalIt = originalRing.cbegin();
    auto copiedIt = copiedRing.cbegin();
    while (originalIt != originalRing.cend() && copiedIt != copiedRing.cend())
    {
        CHECK(originalIt.key() == copiedIt.key());
        CHECK(originalIt.info() == copiedIt.info());

        originalIt.next();
        copiedIt.next();
    }

    // Test the end iterator of the copied ring
    CHECK(copiedIt == copiedRing.cend());

    originalRing.clear();
    CHECK(originalRing.isEmpty() == true);
    CHECK(copiedRing.getLength() == 3);

}

TEST_CASE("iterator increment test")
{
    bi_ring<int, string> originalRing;
    originalRing.push_back(1, "A");
    originalRing.push_back(2, "A");
    originalRing.push_back(3, "A");
    originalRing.push_back(4, "A");
    originalRing.push_back(5, "A");
    originalRing.push_back(6, "A");
    originalRing.push_back(7, "A");

    int i = 1;
    for (auto it = originalRing.cbegin(); it != originalRing.cend(); it.next())
    {
        CHECK(it.key() == i);
        i++;
    }

    auto it = originalRing.begin();
    for (int i = 1; i <= 7; i++)
    {
        CHECK(it.key() == i);
        it++;
    }
    // jumping over sentinel
    CHECK(it.key() == 1);

    auto end = --originalRing.cend();
    end.next();
    CHECK(end.key() == 0); // sentinel node, default key is 0

}

TEST_CASE("iterator decrement")
{
    bi_ring<int, string> originalRing;
    originalRing.push_back(1, "A");
    originalRing.push_back(2, "A");
    originalRing.push_back(3, "A");
    originalRing.push_back(4, "A");
    originalRing.push_back(5, "A");
    originalRing.push_back(6, "A");
    originalRing.push_back(7, "A");

    int i = 7;
    for (auto it = --originalRing.cend(); it != originalRing.cbegin(); it.prev())
    {
        CHECK(it.key() == i);
        i--;
    }

    auto it = --originalRing.end();
    for (int i = 7; i >= 1; i--)
    {
        CHECK(it.key() == i);
        it--;
    }

    // Jumping over sentinel
    CHECK(it.key() == 7);

    auto begin = originalRing.cbegin();
    begin.prev();
    CHECK(begin.key() == 0); // Sentinel node, default key is 0

}

TEST_CASE("find key")
{
    bi_ring<int, string> ring;
    ring.push_back(1, "One");
    ring.push_back(2, "Two");
    ring.push_back(3, "Three");
    ring.push_back(3, "SecondThree");
    ring.push_back(4, "Four");
    ring.push_back(3, "ThirdThree");
    ring.push_back(5, "Five");

    // Test with modifying_iterator
    {
        auto it = ring.begin();
        auto search_from = ring.begin();
        auto search_till = ring.end();

        bool found = ring.find_key(it, 3, search_from, search_till);

        CHECK(found);
        CHECK(it.key() == 3);
        CHECK(it.info() == "Three");
    }

    // Test with constant_iterator
    {
        auto it = ring.cbegin();
        auto search_from = ring.cbegin();
        auto search_till = ring.cend();

        bool found = ring.find_key(it, 3, search_from, search_till);

        CHECK(found);
        CHECK(it.key() == 3);
        CHECK(it.info() == "Three");
    }

    // Search_from is after search_till
    {
        auto it = ring.begin();
        auto search_from = --ring.end();
        CHECK(search_from.key() == 5);
        auto search_till = search_from - 1;
        CHECK(search_till.key() == 3);

        bool found = ring.find_key(it, 4, search_from, search_till);

        CHECK(found == true);
        CHECK(it.key() == 4);
        CHECK(it.info() == "Four");
    }

    // Test with a key that doesn't exist
    {
        auto it = ring.begin();
        auto search_from = ring.begin();
        auto search_till = ring.end();

        bool found = ring.find_key(it, 99, search_from, search_till);

        CHECK(!found);
    }

    // Finding different occurrences
    {

        string Infos[] = {"Three", "SecondThree", "ThirdThree"};
        auto it = ring.begin();
        auto search_from = ring.begin();
        auto search_till = ring.end();
        int i = 0;
        while (ring.find_key(it, 3, search_from, search_till))
        {
            search_from = it + 1;
            CHECK(it.key() == 3);
            CHECK(it.info() == Infos[i]);
            i++;
        }
    }
}

TEST_CASE("iterator operators ")
{
    bi_ring<int, string> originalRing;
    originalRing.push_back(1, "A");
    originalRing.push_back(2, "A");
    originalRing.push_back(3, "A");
    originalRing.push_back(4, "A");
    originalRing.push_back(5, "A");
    originalRing.push_back(6, "A");
    originalRing.push_back(7, "A");

    auto it = originalRing.begin();
    it = it + 3;
    CHECK(it.key() == 4);
    it = it - 2;
    CHECK(it.key() == 2);
    it = it + originalRing.getLength();
    CHECK(it.key() == 2);
    it = it + 1000 * originalRing.getLength();
    CHECK(it.key() == 2);
    it = it + 1000 * originalRing.getLength() - 1;
    CHECK(it.key() == 1);
    it = it - originalRing.getLength();
    CHECK(it.key() == 1);
    it = it - 1000 * originalRing.getLength();
    CHECK(it.key() == 1);
    it = it - 1000 * originalRing.getLength() - 1;
    CHECK(it.key() == 7);
}

TEST_CASE("occurrences of ")
{
    bi_ring<int, string> ring;
    ring.push_back(1, "One");
    ring.push_back(2, "Two");
    ring.push_back(1, "One");
    ring.push_back(3, "Three");
    ring.push_back(1, "One");
    ring.push_back(4, "Four");

    {
        unsigned int count = ring.occurrencesOf(1);
        CHECK(count == 3);
    }

    // Test with a key that doesn't exist
    {
        unsigned int count = ring.occurrencesOf(99);
        CHECK(count == 0);
    }

    // Test with an empty ring
    {
        bi_ring<int, string> emptyRing;
        unsigned int count = emptyRing.occurrencesOf(1);
        CHECK(count == 0);
    }

    // Test with a complex key (assuming Key supports ==)
    {
        bi_ring<pair<int, char>, string> complexRing;
        complexRing.push_back({1, 'a'}, "Pair A");
        complexRing.push_back({2, 'b'}, "Pair B");
        complexRing.push_back({1, 'a'}, "Pair A");

        unsigned int count = complexRing.occurrencesOf({1, 'a'});
        CHECK(count == 2);
    }
}

bool pred(const string &str)
{
    return str.size() > 3;
}

TEST_CASE("filter ")
{
    bi_ring<string, int> source;
    string keys[] = {"uno", "due", "tre", "quattro", "cinque", "sei", "sette", "otto"};
    for (int i = 0; i < 8; i++)
    {
        source.push_back(keys[i], (i + 1) * 3);
    }

    auto res = filter(source, pred);

    string res_keys[] = {"quattro", "cinque", "sette", "otto"};
    int res_infos[] = {12, 15, 21, 24};
    int i = 0;
    for (auto it = res.cbegin(); it != res.cend(); it.next())
    {
        CHECK(it.key() == res_keys[i]);
        CHECK(it.info() == res_infos[i]);
        i++;
    }
}

template <typename Key, typename Info>
Info _concatenate_info(const Key &, const Info &i1, const Info &i2)
{
    return i1 + "-" + i2;
}

TEST_CASE("unique")
{
    bi_ring<int, string> source;
    string infos_fr[] = {"un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf", "dix"};
    string infos_en[] = {"one","two","three","four","five","six","seven","eight","nine","ten"};
    string infos_ru[] = {"один","два","три","четыре","пять","шесть","семь","восемь","девять","десять"};

    for (int i = 0; i < 10; i++)
    {
        source.push_back(i + 1, infos_fr[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        source.push_back(i + 1, infos_en[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        source.push_back(i + 1, infos_ru[i]);
    }

    source.push_back(777, "Героям слава");

    auto res = unique(source, _concatenate_info);
    string info_expected[] = {"un-one-один", "deux-two-два", "trois-three-три", "quatre-four-четыре", "cinq-five-пять", "six-six-шесть", "sept-seven-семь", "huit-eight-восемь", "neuf-nine-девять", "dix-ten-десять"};
    int i = 1;
    for (auto it = res.cbegin(); it != res.cend() - 1; it.next())
    {
        CHECK(it.key() == i);
        CHECK(it.info() == info_expected[i - 1]);
        i++;
    }
    CHECK((res.cend() - 1).key() == 777);
    CHECK((res.cend() - 1).info() == "Героям слава");
}

TEST_CASE("join")
{
    bi_ring<string, int> first;
    first.push_back("uno", 1);
    first.push_back("due", 2);
    first.push_back("tre", 3);
    first.push_back("quattro", 4);

    bi_ring<string, int> second;
    second.push_back("due", 1);
    second.push_back("tre", 1);
    second.push_back("quattro", 3);
    second.push_back("cinque", 5);

    auto res = join(first, second);

    string info_expected[] = {"uno", "due", "tre", "quattro", "cinque"};
    int exp_infos[] = {1, 3, 4, 7, 5};
    auto it = res.cbegin();
    for (int i = 0; i < 5; i++)
    {
        CHECK(it.key() == info_expected[i]);
        CHECK(it.info() == exp_infos[i]);
        it.next();
    }
}

TEST_CASE("shuffle")
{
    bi_ring<string, int> first;
    bi_ring<string, int> second;
    string first_keys[] = {"uno","due","tre","quattro"};
    string second_keys[] = {"bir","iki","uc","dort","bes"};
    for (int i = 0; i < 4; i++)
    {
        first.push_back(first_keys[i], i + 1);
    }
    for (int i = 0; i < 5; i++)
    {
        second.push_back(second_keys[i], i + 1);
    }

    auto res = shuffle(first, 1, second, 2, 3);
    int exp_infos[] = {1, 1, 2, 2, 3, 4, 3, 5, 1};
    string exp_keys[] = {"uno","bir","iki","due","uc","dort","tre","bes","bir"};

    auto it = res.cbegin();
    for (int i = 0; i < 9; i++)
    {
        CHECK(it.key() == exp_keys[i]);
        CHECK(it.info() == exp_infos[i]);
        it.next();
    }
}
