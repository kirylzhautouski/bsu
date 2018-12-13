#include "menu.h"

#include <stdio.h>
#include <tchar.h>

void ShowMenu()
{
	_tprintf(_TEXT("{{1}} Upload plugins\n{{2}} Unload plugins\n{{3}} Show uploaded plugins\n{{4}} Show uploaded plugins information\n{{5}} Execute plugin\n{{6}} Exit\n"));
	_tprintf(_TEXT("Choose an action: "));
}