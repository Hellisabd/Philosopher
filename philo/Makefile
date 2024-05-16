NAME	= philo


DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
ORANGE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m


SRCS	= 	libft_utils.c \
			thread.c


SRCS_MAIN	= main.c $(SRCS)

OBJS	= $(SRCS_MAIN:.c=.o)

CC	= @cc

CFLAGS	+= -Wall -Wextra -Werror -fsanitize=thread -g3

RM	= @rm -rf

CURRENT_DATE	:= $(shell date +"%Y-%m-%d %H:%M:%S")

all	: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -pthread $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(MAGENTA)Make Done$(DEF_COLOR)"

clean : 
	$(RM) $(OBJS) 
	@echo "$(ORANGE)OBJS CLEAR MY FRIEND!$(DEF_COLOR)"

fclean : 
	$(RM) $(NAME) $(OBJS)
	@echo "$(MAGENTA)¯|_(ツ)_/¯ I'M SO PROUD OF U!$(DEF_COLOR)"


re :	fclean all

ex :	re clean

valfd :
	@echo "$(GREEN)(•̀ᴗ•́)و ̑̑ALL FINE!(•̀ᴗ•́)و$(DEF_COLOR)"
	@echo "$(CYAN)(☞ﾟ∀ﾟ)☞ TYPE URE THINGS(☞ﾟ∀ﾟ)☞$(DEF_COLOR)"
	@valgrind --track-fds=yes --leak-check=full --quiet ./philo

val :
	@echo "$(GREEN)(•̀ᴗ•́)و ̑̑ALL FINE!(•̀ᴗ•́)و$(DEF_COLOR)"
	@echo "$(CYAN)(☞ﾟ∀ﾟ)☞ TYPE URE THINGS(☞ﾟ∀ﾟ)☞$(DEF_COLOR)"
	@valgrind --leak-check=full --quiet ./philo

git	: fclean
	@$(RM) */a.out a.out
	@git add . > /dev/null 2>&1
	@git commit -m "main $(CURRENT_DATE)" > /dev/null 2>&1
	@git push > /dev/null 2>&1
	@echo "$(GREEN)(•̀ᴗ•́)و ̑̑GIT UPDATE!(•̀ᴗ•́)و ̑̑$(DEF_COLOR)"
