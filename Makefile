#------------------------------------------------------------------------------#
#                                                                              #
#                               DECLARATIONS                                   #
#                                                                              #
#------------------------------------------------------------------------------#

#------------------------------------#
#             COMPILATION            #
#------------------------------------#

NAME = webserv

CXX 		= clang++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g3

LDFLAGS		=

#------------------------------------#
#                SOURCES             #
#------------------------------------#

SRCDIR		=	src/
SRCFILE		=	webserv.cpp

CLASSFILE	=	\
				config/HTTPConfig.cpp \
				server/Epoll.cpp \
				server/HTTPServer.cpp \
				server/VirtualServer.cpp \

SRC			=	$(addprefix $(SRCDIR), $(SRCFILE))
SRC			+=	$(addprefix $(SRCDIR), $(CLASSFILE))

#------------------------------------#
#              INCLUDES              #
#------------------------------------#

INCDIR		=	includes/
INCFILE		=	webserv.hpp \
				ansi_colors.hpp

INC			=	$(addprefix $(INCDIR), $(INCFILE))
INC			+=	$(addprefix $(SRCDIR), $(CLASSFILE:.cpp=.hpp))

#------------------------------------#
#               OBJECTS              #
#------------------------------------#

OBJDIR		=	obj/
OBJ			=	$(subst $(SRCDIR),$(OBJDIR),$(SRC:.cpp=.o))

#------------------------------------------------------------------------------#
#                                                                              #
#                                   RULES                                      #
#                                                                              #
#------------------------------------------------------------------------------#

.PHONY: all
all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.cpp $(INC)
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(INCDIR) -I $(SRCDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)

.PHONY: fclean
fclean: clean
	rm -rf $(NAME)

.PHONY: re
re: fclean all

.PHONY: run
run: $(NAME)
	./$(NAME)

# .PHONY: run
# run: $(NAME)
# 	./$(NAME) $(filter-out $@, $(MAKECMDGOALS))

# .PHONY: test
# test:
# 	echo $(INC)