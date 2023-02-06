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
#          SOURCES AND CLASSES       #
#------------------------------------#

SRCDIR		=	src/
SRCFILE		=	main.cpp

SRC			=	$(addprefix $(SRCDIR), $(SRCFILE))

#------------------------------------#
#              INCLUDES              #
#------------------------------------#

INCDIR		=	includes/
INCFILE		=	HTTPConfig.hpp \
				HTTPServer.hpp

INC			=	$(addprefix $(INCDIR), $(INCFILE))

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
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(INCDIR)

.PHONY: clean
clean:

.PHONY: fclean
fclean:

.PHONY: re
re: fclean all