CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi

OBJ_DIR = obj
SINGLE_OBJS = $(patsubst %.o, $(OBJ_DIR)/%.o, single_corrector.o distance_dict.o metaphone_dict.o)
SINGLET_OBJS = $(patsubst %.o, $(OBJ_DIR)/%.o, single_tester.o distance_dict.o metaphone_dict.o)
SENTEN_OBJS = $(patsubst %.o, $(OBJ_DIR)/%.o, sentence_corrector.o ngram_dict.o distance_dict.o)
EXES = single_corrector single_tester sentence_corrector

all: $(EXES)

$(OBJ_DIR):
	@mkdir -p $@

single_tester: $(SINGLET_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

single_corrector: $(SINGLE_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

sentence_corrector: $(SENTEN_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/single_tester.o: single_tester.cpp distance_dict.h metaphone_dict.h| $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/single_corrector.o: single_corrector.cpp distance_dict.h metaphone_dict.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/sentence_corrector.o: sentence_corrector.cpp distance_dict.h ngram_dict.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/metaphone_dict.o: metaphone_dict.cpp metaphone_dict.h double_metaphone.h| $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/ngram_dict.o: ngram_dict.cpp ngram_dict.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/distance_dict.o: distance_dict.cpp distance_dict.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXES)
