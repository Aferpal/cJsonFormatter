TARGET = app

CXX = gcc

CXXFLAGS = -Wall -g

INCLUDE = -I./include

LINK = -L./lib -ljson

DYNAMIC_LIBS = -lmysqlcppconnx

SOURCE_FOLDER = ./src

OUTPUT_FOLDER = ./out

OBJS_FOLDER = ./objects

TESTS_FOLDER = ./test

LIB_FOLDER = ./lib


libs:
	$(CXX) $(CXXFLAGS) $(SOURCE_FOLDER)/*.c -shared -o $(LIB_FOLDER)/libjson.so -fPIC $(INCLUDE)

tests:
	export LD_LIBRARY_PATH=/home/abel_uma/projects/jsonFormatter/lib; \
	for i in $(TESTS_FOLDER)/*.c; do\
		echo "\n\n⚙️  running test $$i ⚙️\n\n"; \
		$(CXX) $(CXXFLAGS) $$i -o $(OUTPUT_FOLDER)/test $(INCLUDE) $(LINK); \
		$(OUTPUT_FOLDER)/test; \
	done
clean:
	rm $(OBJS_FOLDER)/*
	rm $(OUTPUT_FOLDER)/*
	rm $(LIB_FOLER)/*
