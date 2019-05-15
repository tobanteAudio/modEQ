SOURCE_DIR = Source
TEST_DIR = Tests

# DOCS
documentation:
	mkdir -p Builds/Documentation
	doxygen Doxyfile.in
	
# MISC
stats:
	cloc $(SOURCE_DIR) $(TEST_DIR)

format:
	find $(SOURCE_DIR) -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i	
	find $(TEST_DIR) -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i	
