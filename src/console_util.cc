#include "console_util.h"


const char* HorizontalLine() {
	return "========================================" \
		   "========================================\n";
}
void NewScreen() {
	std::system("cls");
}
void Pause() {
	std::system("pause >nul");
}