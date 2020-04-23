SOURCE_DIR = plugin
TEST_DIR = test

# DOCS
documentation:
	mkdir -p build_documentation
	doxygen Doxyfile.in
	
# MISC
stats:
	cloc cmake CMakeLists.txt README.md Doxyfile.in $(SOURCE_DIR) $(TEST_DIR)

format:
	find $(SOURCE_DIR) -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i	
	find $(TEST_DIR) -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i	
