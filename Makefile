all:
	g++ -I /usr/local/include  -c mainFunction.cpp -std=c++23 -lstdc++fs -lcgicc -lcurl `libpng-config --cflags`
	g++ -o index.cgi mainFunction.o -std=c++23 -lcrypto -lstdc++fs -lcgicc -lcurl `libpng-config --ldflags`
	chgrp -R wadmins css
	chgrp -R wadmins js
	chgrp -R wadmins srcSecretCode
	chmod -R 774 css
	chmod -R 774 js
	chmod -R 770 srcSecretCode
	chmod 664 *.txt
	chmod 600 mainFunction.cpp
	chmod 775 index.cgi
	rm mainFunction.o

clean:
	rm -rf srcSecretCode
	rm -rf mainFunction.cpp
	

