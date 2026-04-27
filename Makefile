CC	= gcc

SRC_DIR		=	src/
DIR_OBJ		=	.obj/

SRC_FILES	=	main.c druid.c villager.c
SRC				=	$(addprefix $(SRC_DIR), $(SRC_FILES))
SRC_OBJ		=	$(SRC:%.c=$(DIR_OBJ)%.o)
BINARY		=	panoramix

CFLAGS			=	-Iinclude -Wall -Wextra -Werror -pthread

all:	$(BINARY)

$(BINARY):	$(SRC_OBJ)
		@$(CC) $^ -o $@
		@echo -e "\x1b[32m[OK] Executable $@\x1b[0m"

$(DIR_OBJ)%.o:	%.c
		@mkdir -p $(dir $@)
		@$(CC) $(CFLAGS) -c $< -o $@
		@echo -e "\x1b[36m[Compiling] $<\x1b[0m"

clean:
		@rm -rf $(DIR_OBJ)
		@echo -e "\x1b[35m[Clean] Objects removed\x1b[0m"

fclean:	clean
		@rm -f $(BINARY)
		@echo -e "\x1b[35m[Fclean] Binary removed\x1b[0m"

re:	fclean all

.PHONY: all clean fclean re
