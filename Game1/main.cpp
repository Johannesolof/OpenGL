#include "App.hpp"

FILE *stdStream;
FILE *errStream;


int main()
{
	//freopen_s(&stdStream, "stdout.txt", "w", stdout);
	//freopen_s(&stdStream, "stderr.txt", "w", stderr);

	App* myApp = new App("Test");

	myApp->run();

	delete myApp;
    return 0;
}
