all:
	g++ -pthread WaitStudents_CS20BTECH11063_A.cpp -o waita -Wall -W -Werror
	g++ -pthread WaitStudents_CS20BTECH11063_B.cpp -o waitb -Wall -W -Werror
	g++ -pthread WaitStudents_CS20BTECH11063_C.cpp -o waitc -Wall -W -Werror
makec:
	g++ -pthread WaitStudents_CS20BTECH11063_C.cpp -o waitc -Wall -W -Werror
makeb:
	g++ -pthread WaitStudents_CS20BTECH11063_B.cpp -o waitb -Wall -W -Werror
makea:
	g++ -pthread WaitStudents_CS20BTECH11063_A.cpp -o waita -Wall -W -Werror
runa:
	./waita
runb:
	./waitb
runc:
	./waitc
runall: runa runb runc
clean:
	rm -f waita waitb waitc