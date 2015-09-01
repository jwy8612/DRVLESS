all:
	cd ./lib;make
	cd ./app;make
	
//.PHONY: clean
clean: 
		cd ./lib;make clean
		cd ./app;make clean