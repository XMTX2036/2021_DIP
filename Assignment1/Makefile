CC = clang++
OBJS = main.o bmp.o
RES = gray.bmp another_gray.bmp YUV_to_RGB.bmp

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