# DoH Client (DNS over TLS)

CC = cc
CF = -Wall -Werror -Wextra
DEBUG = -g
OSSL = -lssl -lcrypto # To run library OpenSSL

NAME = DoH_client

INC = -I include/

SRCD = src/
OBJD = build/

SRC = 	$(SRCD)main.c 			\
		$(SRCD)dns_build.c 		\
		$(SRCD)dns_parse.c 		\
		$(SRCD)tools.c			\
		$(SRCD)socket_tcp.c		\
		$(SRCD)tls_session.c	\
		$(SRCD)http2_parse.c	\
		$(SRCD)http2_build.c

OBJ = $(patsubst $(SRCD)%.c, $(OBJD)%.o, $(SRC))

all : $(NAME)

$(NAME): $(OBJ)
		@echo "$(GC) Compiling $(NAME) program..  $(END)"
		$(CC) $(CF) $(DEBUG) $(INC) -o $(NAME) $(OBJ) $(OSSL)
		@echo "$(RC) Program $(NAME) is ready to use. $(END)"

$(OBJD)%.o:	$(SRCD)%.c
			@mkdir -p $(dir $@)
			$(CC) $(CF) $(INC) -c $< -o $@

clean:  
		@rm -rf $(OBJD)

fclean:	clean
		@rm -f $(NAME)

re:     fclean all 

.PHONY:     re fclean clean all 
