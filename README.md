Emugrind
========

Valgrind tool to instrument emulation using VEX RISC-like language to target multiple architectures.


How to Install
--------------

1. You need the latest valgrind svn

``` 
      $ svn co svn://svn.valgrind.org/valgrind/trunk valgrind
```

2. You need to apply the patch provided

```
	$ cd valgrind
	$ wget https://raw.githubusercontent.com/stricaud/emugrind/master/patches/valgrind-emugrind.autotools.patch
	$ path -p0 < valgrind-emugrind.autotools.patch
	$ rm valgrind-emugrind.autotools.patch
```

3. Copy the source directory inside the valgrind repository

```
	(you are in the valgrind source directory, we suppose emugrind is one level before)
	$ cp -r ../emugrind/emugrind .
```

4. Compile valgrind with the tool


```
	$ ./autogen.sh
	$ ./configure --prefix=$HOME/opt
	$ make install
```

Now you can enjoy running $HOME/opt/bin/valgrind --tool=emugrind <you_program_here>

