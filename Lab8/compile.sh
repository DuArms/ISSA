clear

rm server.exe
rm client.exe

gcc server.c -pthread -o server.exe
gcc client.c -o client.exe

./server.exe
#./client.exe