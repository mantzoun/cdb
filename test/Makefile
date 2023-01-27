include makefile.common

TESTS = test_cdb_io

SRC_OBJ_DIR = src
.PHONY: my_test clean

all: $(BIN_HOME) $(TESTS)

test_cdb_io:
	make -f makefile.test_cdb_io my_test

$(BIN_HOME):
	@mkdir -p $(BIN_HOME)

clean:
	@rm -rf $(BIN_HOME) $(CPPUTEST_OBJS_DIR) $(CPPUTEST_LIB_DIR) $(SRC_OBJ_DIR)
