rm *.o
g++ *.c* packets/*.c* handlers/*.c* -o packetserver -I. -std=c++0x
