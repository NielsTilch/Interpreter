main : testaro.o testaroSub.o testaroFonciton.o
	gcc -g -c testaro.c
	gcc -g -c testaroSub.c
	gcc -g -c testaroFonciton.c
	gcc -g -o testaro testaro.o testaroSub.o testaroFonciton.o

clean :
	rm *.o
	rm testaro

testFile :
	make main
	./testaro ./tests/fileTest/file1.txt
	./testaro ./tests/fileTest/file2.txt

testBoucle :
	make main
	./testaro ./tests/testBoucleLecture/test.txt

testIndentation :
	make main
	./testaro ./tests/testIndentation_suppression/test.txt

testIndentationRemove :
	rm -r fichierTest1
	rm -r fichierTest2
	echo "on peut faire le test d'indentation !\n >> make testIndentation"