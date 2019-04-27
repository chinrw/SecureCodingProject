CC=cc
CFLAGS = -O2 -Wall -std=c99

all: echod echo daytimed daytime

echod: echod.c
	$(CC) echod.c -o echod $(NVCCFLAGS)

echo: echo.c
	$(CC) echo.c -o echo $(NVCCFLAGS)

daytimed: daytimed.c
	$(CC) daytimed.c -o daytimed $(NVCCFLAGS)

daytime: daytime.c
	$(CC) daytime.c -o daytime $(NVCCFLAGS)


clean:
	rm -rf echod echo daytimed daytime