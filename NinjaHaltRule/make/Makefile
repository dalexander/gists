all: 1 2 3 4

2 3 4:
	sleep 5 && echo "output" > $@

1:
	sleep 2 && exit 1

clean:
	rm -f 1 2 3 4
