#include <catch.hpp>
#include "/Users/maximsnytsarev/CLionProjects/my-repo/tasks/archiver/src/writing.cpp"

TEST_CASE("writing char"){
    Buffer write = Buffer("output.txt");
    write.WriteBits("01101000");
    write.Close();
    std::ifstream fin("output.txt");
    REQUIRE(fin.get() == 'h');
}