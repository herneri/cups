CC = gcc
CPP = g++
GC = gccgo
OUT = todo cn dc

# Compile all cups programs
default: $(OUT)

# Install all cups programs
install: default
	cp $(OUT) /usr/local/bin

cn: cn.go
	$(GC) $^ -o cn

todo: todo.c
	$(CC) $^ -o todo

dc: dc.go
	$(GC) $^ -o dc
