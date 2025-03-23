#include <catch.hpp>
#include "/Users/maximsnytsarev/CLionProjects/my-repo/tasks/archiver/src/Huffman.cpp"

TEST_CASE("Nodes Compare"){
    Node a = Node(1, 1);
    Node b = Node(2, 1);
    Node c = Node(1, 2);
    REQUIRE(a < b);
    REQUIRE(a < c);
    std::string test1 = Int2binHuffman(3, 5);
    REQUIRE(test1 == "00011");
}