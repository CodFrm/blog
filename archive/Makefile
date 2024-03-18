export DEBUG
# -fexec-charset=GBK
g=gcc
ifeq ($(DEBUG), TRUE) 
	g+= -g
endif

build:
	$(g) -g -fsanitize=address "$(FILE)" -o debug/$(NAME) -lm

rebuild:clean build

test:test.o
	$(g) debug/test.o -o debug/test

test.o:
	$(g) -c test.c

clean:
	@rm debug/$(NAME).o debug/$(NAME) debug/$(NAME).h.gch debug/$(NAME).exe
