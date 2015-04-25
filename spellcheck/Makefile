CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi

OBJ_DIR = obj
CORRECT_OBJS = $(patsubst %.o, $(OBJ_DIR)/%.o, corrector.o corrector_dict.o)
TEST_OBJS = $(patsubst %.o, $(OBJ_DIR)/%.o, tester.o corrector_dict.o)
EXES = corrector tester

all: $(EXES)

$(OBJ_DIR):
	@mkdir -p $@

corrector: $(CORRECT_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

tester: $(TEST_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/tester.o: tester.cpp corrector_dict.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/corrector.o: corrector.cpp corrector_dict.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/corrector_dict.o: corrector_dict.cpp corrector_dict.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXES)
