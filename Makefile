# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: romlambe <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/03 18:59:35 by jeza              #+#    #+#              #
#    Updated: 2024/05/15 14:29:11 by romlambe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC      =   gcc

CFLAGS  =    -g -Iheaders/
READLINE_INC = -I/opt/homebrew/opt/readline/include
READLINE_LIB = -L/opt/homebrew/opt/readline/lib -lreadline
GETNEXTLINE = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c



NAME    =   minishell

SRCS = Sources/main.c Sources/utils.c \
		Sources/modify_env.c Sources/built_in.c \
		Sources/builtin_export.c Sources/execution.c Sources/main_test_exec.c \
		Sources/signals.c Sources/builtin_cd.c Sources/utils1.c \
		Sources/utils2.c Sources/path_to_exec.c Sources/manage_pipes.c \
		Sources/manage_here_doc.c Sources/builtin_cd_utils.c \
		Sources/builtin_export_utils.c Sources/execute_cmd_only.c \
		Sources/builtin_echo.c Sources/expanser.c Sources/manage_quotes.c \
		Sources/expanser2.c Sources/utils3.c Sources/new_exec.c \
		Parsing/clean_nodes.c Parsing/copy_list.c Parsing/extract_cmd.c \
		Parsing/init_clean_nodes.c Parsing/init_nodes.c \
		Parsing/redirection.c Parsing/tokenize.c Parsing/utils_list.c


OBJS    =   ${SRCS:.c=.o}
GETNEXTLINE_OBJ := $(GETNEXTLINE:.c=.o)

all     :   ${NAME}

${NAME} :   ${OBJS} ${GETNEXTLINE_OBJ}
	@${MAKE} -C libft
	@${CC} ${CFLAGS} ${OBJS} ${GETNEXTLINE_OBJ} -o ${NAME} ${READLINE_INC} ${READLINE_LIB} -L./libft/ -lft
	@echo "\033[92mProgram Compiled Successfully\033[0m"



sanit   :   ${OBJS} ${GETNEXTLINE_OBJ}
			@${MAKE} -C libft
            # Correction de LK_FLAG en LK_FLAGS et ajout des flags pour Readline
			@${CC} ${CFLAGS} ${OBJS} ${GETNEXTLINE_OBJ} -o ${NAME} ${READLINE_INC} ${READLINE_LIB} -L./libft/ -lft

%.o     :   %.c
			@${CC} ${CFLAGS} ${READLINE_INC} -c $< -o $@

clean   :
			@${MAKE} -C libft clean
			@echo "\033[92mCleaned objects\033[0m"

fclean  :   clean
			$(RM) $(OBJS)
			$(RM) $(GETNEXTLINE_OBJ)
			@${MAKE} -C libft fclean
			@rm -rf ${NAME}
			@echo "\033[92mExecutable Removed\033[0m"

re      :   fclean all

.PHONY  :   all clean fclean re bonus sanit
