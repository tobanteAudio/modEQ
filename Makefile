SOURCE_DIR = Source

# MISC
stats:
	cloc Source
	
format:
	find $(SOURCE_DIR) -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i	
