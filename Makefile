hepsi: derle calistir

derle:
	gcc -o ./KabukProjesi.o -c ./KabukProjesi.c
	gcc -o ./KabukProjesi ./KabukProjesi.c

calistir:
	./KabukProjesi