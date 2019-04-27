CC=cc
CFLAGS = -O2 -Wall -std=c99

all: echod echo daytimed daytime

echod: echod.c
	$(CC) echod.c -o echod $(CFLAGS)

echo: echo.c
	$(CC) echo.c -o echo $(CFLAGS)

daytimed: daytimed.c
	$(CC) daytimed.c -o daytimed $(CFLAGS)

daytime: daytime.c
	$(CC) daytime.c -o daytime $(CFLAGS)


clean:
	rm -rf echod echo daytimed daytime