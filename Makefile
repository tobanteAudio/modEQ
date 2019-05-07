SOURCE_DIR = Source

# MISC
stats:
	cloc $(SOURCE_DIR)

format:
	find $(SOURCE_DIR) -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i	
