# example: how to run DELEqC-II

./bin/Release/DE 50 0.9 0.6 10000 11 3 1 0.000001 40 5


# a test using valgrind

valgrind -v --leak-check=full --track-origins=yes ./bin/Debug/DE 100 0.9 0.6 100 104 3 1 0.000001 3 1

