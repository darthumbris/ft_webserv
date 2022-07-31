NAME = webserv

CXX := c++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++11

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes

SRC =	main.cpp \
        WebServ.cpp \
        Config.cpp \
        Server.cpp \
        Location.cpp \
        RequestHandler.cpp \

SRC_EXT = cpp

OBJ := $(addprefix $(OBJ_DIR)/, $(SRC:%.$(SRC_EXT)=%.o))
SRC :=  $(addprefix $(SRC_DIR)/, $(SRC))

INC = -I $(INC_DIR)

COM_COLOR   = \033[0;33m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m
PRG_COLOR	= \033[0;35m

OK_STRING    = "[OK]"
ERROR_STRING = "[ERROR]"
WARN_STRING  = "[WARNING]"
COM_STRING   = "Compiling"

ifeq ($(DEBUG),1)
	CXXFLAGS += -g3
    COM_STRING = "Compiling[DEBUG]"
endif
ifeq ($(DEBUG),2)
	CXXFLAGS += -g3 -fsanitize=address
    COM_STRING = "Compiling[LEAKS]"
endif

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) 2> $@.log; \
        RESULT=$$?; \
        if [ $$RESULT -ne 0 ]; then \
            printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(PRG_COLOR) $@" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"; \
        elif [ -s $@.log ]; then \
            printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(PRG_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"; \
        else  \
            printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(PRG_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"; \
        fi; \
        cat $@.log; \
        rm -f $@.log; \
        exit $$RESULT

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT) | $(OBJ_DIR)
	
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $< 2> $@.log; \
        RESULT=$$?; \
        if [ $$RESULT -ne 0 ]; then \
            printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"; \
        elif [ -s $@.log ]; then \
            printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"; \
        else  \
            printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"; \
        fi; \
        cat $@.log; \
        rm -f $@.log; \
        exit $$RESULT

clean:
	@printf "%b" "$(ERROR_COLOR)Removing $(OBJ_COLOR)object files\n"
	@rm -rf $(OBJ_DIR)

fclean:	clean
	@printf "%b" "$(ERROR_COLOR)Removing $(PRG_COLOR)$(NAME)\n"
	@rm -f $(NAME)

re: fclean all

TEST_NAME = test.out

test: $(TEST_NAME)
	c++ $(CXXFLAGS) -I includes -I Unity/src -c tests/test.cpp -o obj/test.o
	c++ $(CXXFLAGS) -L Unity -lunity -I includes -I Unity/src obj/test.o obj/WebServ.o obj/Config.o obj/Server.o obj/Location.o obj/RequestHandler.o -o test.out
	./test.out

testd: $(TEST_NAME)
	c++ $(CXXFLAGS) -g3 -fsanitize=address -I includes -I Unity/src -c tests/test.cpp -o obj/test.o
	c++ $(CXXFLAGS) -g3 -fsanitize=address -L Unity -lunity -I includes -I Unity/src obj/test.o obj/WebServ.o obj/Config.o obj/Server.o obj/Location.o obj/RequestHandler.o -o test.out
	./test.out

.PHONY: all clean fclean re
