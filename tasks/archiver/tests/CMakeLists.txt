add_custom_target(
        test_archiver
        WORKING_DIRECTORY
        DEPENDS archiver
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/test.py ${CMAKE_BINARY_DIR}/archiver ${CMAKE_CURRENT_SOURCE_DIR}/data
)
add_catch(test_read ../src/reading.cpp test_read.cpp)
add_catch(test_huffman ../src/Huffman.cpp test_Huffman.cpp)
add_catch(test_writing ../src/writing.cpp test_writing.cpp)