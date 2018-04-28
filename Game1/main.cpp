#include "App.hpp"

FILE *stdStream;
FILE *errStream;


int main()
{
	//freopen_s(&stdStream, "stdout.txt", "w", stdout);
	//freopen_s(&stdStream, "stderr.txt", "w", stderr);

	auto myApp = std::make_unique<App>("test");
	myApp->init();
	myApp->run();

    return 0;
}
