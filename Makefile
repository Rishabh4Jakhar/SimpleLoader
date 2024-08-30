#invoke make inside following directories and in this order: loader, launch, fib

# All:
# We access loader, launcher and test step by step in the given order and run their respective MAKEFILE
# Then we run the install command for the shared library and copy the contents from launcher/launch to bin/
all:
	cd loader && $(MAKE) 
	cd launcher && $(MAKE)
	cd test && $(MAKE)
	make install
	cd launcher && gcc -m32 -o launch launch.c -L../bin -l_simpleloader -Wl,-rpath,'$$ORIGIN/../bin'
	cp launcher/launch bin/

#move the lib_simpleloader.so and launch binaries inside bin directory

# We make a folder named "bin", -p is used to make parent directories as needed
# Then we move to bin and create the shared library "lib_simpleloader.so"
install:
	mkdir -p bin
	cd bin && gcc -m32 -shared -o lib_simpleloader.so ../loader/loader.c

#Provide the command for cleanup

# We first clean all the files from bin directory
# Then we access loader, launcher and test in the given order and run their $ Clean command to clean them as well
clean:
	rm -rf bin
	cd loader && $(MAKE) clean
	cd launcher && $(MAKE) clean
	cd test && $(MAKE) clean 	