CC = clang++
OBJS = main.o bmpio.o morph.o
RES = *_*

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
	rm $(RES)