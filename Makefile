SRC_DIR := ./src
OBJ_DIR := ./obj
TEST_DIR := ./test
TEST_DIR_FULL := ./test
TEST_DIR_PAR := $(filter-out $(TEST_DIR)/TestMain.cpp, $(wildcard $(TEST_DIR)/*.cpp))
TEST_DIR_PAR_O := $(filter-out $(TEST_DIR)/TestMain.o, $(wildcard $(TEST_DIR)/*.o))
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES := $(wildcard $(TEST_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
TESTER_FILES_FULL := $(patsubst $(TEST_DIR)/%.cpp,$(TEST_DIR)/%.o,$(TEST_FILES))
TESTER_FILES := $(filter-out $(TESTER_FILES_FULL)/TestMain.cpp, $(wildcard $(TESTER_FILES_FULL)/*.o))
TESTER_OBJ_FILES := $(filter-out $(OBJ_DIR)/main.o, $(wildcard $(OBJ_DIR)/*.o))
CPPFLAGS := -Iinclude
CFLAGS := -std=c++11 -g -Werror -O3

MAIN = PORAMRun
TEST = PORAMTest
TEST_FULL = PORAMTest

all: clean $(MAIN)
test: clean_test $(TEST)
test_full: clean_test_full $(TEST_FULL)

.PHONY: clean clean_test
clean:
	$(RM) $(OBJ_DIR)/*.o *~ $(MAIN)

clean_test:
	$(RM) $(TEST_DIR_PAR_O) *~ $(TEST) 

clean_test_full:
	$(RM) $(TEST_DIR_FULL)/*.o *~ $(TEST) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(TEST_DIR_FULL)/%.o: $(TEST_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CFLAGS) -c -o $@ $< 

$(TEST_DIR)/%.o: $(TEST_DIR_PAR)
	g++ $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(MAIN): $(OBJ_FILES) 
	g++ $(CPPFLAGS) $(CFLAGS) -o $@ $^

$(TEST): $(TESTER_FILES) 
	g++ $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(TESTER_OBJ_FILES) 

$(TEST_FULL): $(TESTER_FILES_FULL) 
	g++ $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(TESTER_OBJ_FILES) 


