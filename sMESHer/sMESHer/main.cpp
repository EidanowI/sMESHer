#include <Windows.h>

int main() {
	HANDLE cmdHande = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(cmdHande, "Hello World! I`m sMESHer.", 26, nullptr, 0);
}