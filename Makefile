TARGET = app

CXX = gcc

CXXFLAGS = -Wall -g

INCLUDE = 

LINK = 

DYNAMIC_LIBS = -lmysqlcppconnx

SOURCE_FOLDER = ./src

OUTPUT_FOLDER = ./out

OBJS_FOLDER = ./objects

TESTS_FOLDER = ./test



$(OBJS_FOLDER)/%.o: $(SOURCE_FOLDER)/%.c
        @mkdir -p $(@D)
        $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(OBJS_FOLDER)/*