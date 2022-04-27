CC = clang++
OBJS = main.o bmp.o geo.o

result: $(OBJS)
	$(CC) $(OBJS) -o $@
.c.o:
	$(CC) -c $< -o $@
.PHONY: run
run:
	./result
.PHONY: clean
clean:
	rm -f $(OBJS) result