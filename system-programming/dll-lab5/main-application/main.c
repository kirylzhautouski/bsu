#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>

#include "../menu/menu.h" // static library
#include "../plugins/disk_space_info/disk_space_info.h" // plugin 3 load-time dll

#define PLUGINNUMBER 3

typedef struct Plugin
{
	const TCHAR* name;
	BOOL is_uploaded;
	const BOOL unloadable; // uploadable too
	HINSTANCE lib_handle;
} Plugin;

Plugin plugins[PLUGINNUMBER] = {
	{ "disk_space_info.dll", (TRUE), (FALSE), NULL },
	{ "page_size_info.dll", (FALSE), (TRUE), NULL },
	{ "processor_info.dll", (FALSE), (TRUE), NULL }
};

void UploadPlugins()
{
	for (int i = 0; i < PLUGINNUMBER; i++)
	{
		if (plugins[i].is_uploaded == (FALSE) && plugins[i].unloadable == (TRUE))
		{
			plugins[i].lib_handle = LoadLibrary(_TEXT(plugins[i].name));

			if (plugins[i].lib_handle != NULL)
			{
				plugins[i].is_uploaded = (TRUE);
				_tprintf(_TEXT("%s is successfully uploaded\n"), plugins[i].name);
			}
			else
			{
				_tprintf(_TEXT("upload of %s failed\n"), plugins[i].name);
			}
		}
	}
}

void UnloadPlugins()
{
	for (int i = 0; i < PLUGINNUMBER; i++)
	{
		if (plugins[i].is_uploaded == (TRUE) && plugins[i].unloadable == (TRUE))
		{
			BOOL result = FreeLibrary(plugins[i].lib_handle);

			if (result == (TRUE))
			{
				plugins[i].is_uploaded = (FALSE);
				plugins[i].lib_handle = (NULL);
				_tprintf(_TEXT("%s is successfully unloaded\n"), plugins[i].name);
			}
			else
			{
				_tprintf(_TEXT("upload of %s failed\n"), plugins[i].name);
			}
		}
	}
}

int _tmain()
{
	unsigned int action_number = 0;

	while (1)
	{
		ShowMenu();

		_tscanf("%u", &action_number);

		if (action_number == 1)
		{
			UploadPlugins();
		}
		else if (action_number == 2)
		{
			UnloadPlugins();
		}
		else if (action_number == 3)
		{
			// show uploaded plugins (names)
			for (int i = 0; i < PLUGINNUMBER; i++)
			{
				if (plugins[i].is_uploaded == (TRUE))
					_tprintf(_TEXT("%d. %s\n"), i + 1, plugins[i].name);
			}
		}
		else if (action_number == 4)
		{
			// show plugins info
			TCHAR* author_buffer = malloc(100);
			TCHAR* description_buffer = malloc(100);
			DWORD bytes_written;

			for (int i = 0; i < PLUGINNUMBER; i++)
			{
				if (plugins[i].is_uploaded)
				{
					if (plugins[i].unloadable)
					{
						FARPROC author_function_address = (FARPROC)GetProcAddress(plugins[i].lib_handle, "GetAuthor");
						FARPROC description_function_address = (FARPROC)GetProcAddress(plugins[i].lib_handle,
							"GetDescription");

						(author_function_address)(author_buffer, (DWORD)100, &bytes_written);
						(description_function_address)(description_buffer, (DWORD)100, &bytes_written);
					}
					else
					{
						GetAuthor(author_buffer, 100, &bytes_written);
						GetDescription(description_buffer, 100, &bytes_written);
					}

					_tprintf(_TEXT("%s:\n  Author: %s\n  Description: %s\n"), plugins[i].name, author_buffer, description_buffer);
				}
			}

			free(author_buffer);
			free(description_buffer);
		}
		else if (action_number == 5)
		{
			// show available plugins and execute one
			int uploaded_plugins = 0;
			for (int i = 0; i < PLUGINNUMBER; i++)
			{
				if (plugins[i].is_uploaded)
				{
					uploaded_plugins++;
					_tprintf(_TEXT("%d. %s\n"), uploaded_plugins, plugins[i].name);
				}
			}

			_tprintf(_TEXT("enter the number of plugin you want to execute: "));

			int chosen_plugin_number = 0;
			_tscanf("%u", &chosen_plugin_number);

			int uploaded_plugin_number = 0;
			if (chosen_plugin_number >= 1 && chosen_plugin_number <= uploaded_plugins)
			{
				for (int i = 0; i < PLUGINNUMBER; i++)
				{
					if (plugins[i].is_uploaded)
					{
						uploaded_plugin_number++;
						
						if (uploaded_plugin_number == chosen_plugin_number)
						{
							if (plugins[i].unloadable)
							{
								FARPROC execute_function_address = 
									(FARPROC)GetProcAddress(plugins[i].lib_handle, "Execute");

								(execute_function_address)();
							}
							else
							{
								Execute();
							}
						}
					}
				}
			}
			else
				_tprintf(_TEXT("invalid plugin number\n"));
		}
		else if (action_number == 6)
		{
			// exit
			UnloadPlugins();
			break;
		}
		else
		{
			_tprintf(_TEXT("invalid operation\n"));
		}

		_tprintf(_TEXT("\n\n"));
	}

	return 0;
}