#include <catch.hpp>
#include "/Users/maximsnytsarev/CLionProjects/my-repo/tasks/archiver/src/reading.cpp"

TEST_CASE("opening files"){
    REQUIRE_THROWS(ReadBuffer("bad_input"));
    REQUIRE_NOTHROW(ReadBuffer("/Users/maximsnytsarev/CLionProjects/my-repo/tasks/archiver/tests/test_files/test_input.txt"));
}

TEST_CASE("reading bits"){
    std::string test;
    ReadBuffer read = ReadBuffer("/Users/maximsnytsarev/CLionProjects/my-repo/tasks/archiver/tests/test_files/test_input.txt");
    for (int i = 0; i < 8; ++i) {
        test += read.Read1Bit();
    }
    REQUIRE(test == "01101000");
}

