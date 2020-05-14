CXX=g++
CXXFLAGS=-std=c++11 -Wall -I include/
LDLIBS=-L/usr/lib64 -lstdc++ `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`
SRC_DIR=./src
OBJ_DIR=./obj
CLIENT_DIR=./src/client
SERVER_DIR=./src/server
TEST_DIR=./tests
TEST_OUT_DIR=./tests/bin
BUILD_DIR=./build

BRACKET_STYLE=java

OUTPUT=Poker++

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
CLIENT_FILES := $(wildcard $(CLIENT_DIR)/*.cpp)
SERVER_FILES := $(wildcard $(SERVER_DIR)/*.cpp)
TEST_FILES := $(wildcard $(TEST_DIR)/*.cpp)

OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
CLIENT_OBJ := $(patsubst $(CLIENT_DIR)/%.cpp,$(OBJ_DIR)/client/%.o,$(CLIENT_FILES))
SERVER_OBJ := $(patsubst $(SERVER_DIR)/%.cpp,$(OBJ_DIR)/server/%.o,$(SERVER_FILES))
TEST_OUT := $(patsubst $(TEST_DIR)/%.cpp,$(TEST_DIR)/bin/%.test,$(TEST_FILES))

default: objprepender ${OBJ_FILES} clientprepender client serverprepender server
	ln -srf Images ./build/Images

client: ${OBJ_FILES} ${CLIENT_OBJ}
	${CXX} ${CXXFLAGS} $^ -o ${BUILD_DIR}/${OUTPUT}_Client ${LDLIBS} 

server: ${OBJ_FILES} ${SERVER_OBJ}
	${CXX} ${CXXFLAGS} $^ -o ${BUILD_DIR}/${OUTPUT}_Server ${LDLIBS}

test: ${TEST_OUT}	

preprocess: CXXFLAGS+=-E
preprocess: default

debug: CXXFLAGS+=-g 
debug: clean default 

docgen:
	@doxygen
	@firefox ./docs/html/index.html

${TEST_OUT_DIR}/%.test: ${OBJ_FILES} ${TEST_DIR}/%.cpp 
	$(CXX) ${CXXFLAGS} $^ -o $@ ${LDLIBS}
	$@

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp 
	$(CXX) ${CXXFLAGS} -c -o $@ $< ${LDLIBS}

*/%.o: ${SRC_DIR}/%.cpp 
	$(CXX) ${CXXFLAGS} -c -o $@ $< ${LDLIBS}

objprepender:
	@echo -e  "==============================================\n==============MAKING=OBJ_FILES================\n=============================================="

clientprepender:
	@echo -e "==============================================\n==============MAKING=CLIENT===================\n=============================================="

serverprepender:
	@echo -e "==============================================\n==============MAKING=SERVER===================\n=============================================="

prettify:
	find -type f \( -iname \*.h -o -iname \*.hpp -o -iname \*.cpp \) -exec astyle -n --style=${BRACKET_STYLE} {} \; | grep ^Formatted*

clean:
	-rm -rf ${BUILD_DIR}/*
	-rm -rf ${OBJ_DIR}/*.o
	-rm -rf ${OBJ_DIR}/*/*.o
	-rm -rf ${TEST_DIR}/*.test
