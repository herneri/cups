/*
	Copyright 2025 Eric Hernandez

	This file is part of cups.

	cups is free software: you can redistribute it and/or modify it under the terms of the GNU General 
	Public License as published by the Free Software Foundation, either version 3 of the License,
	or (at your option) any later version.

	cups is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
	the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	General Public License for more details.

	You should have received a copy of the GNU General Public License along with cups.
	If not, see <https://www.gnu.org/licenses/>.
*/

#include "todo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct todo_list *todo_load_list(void) {
	char path[255];
	sprintf(path, "%s/.local/share/cups/todo.txt", getenv("HOME"));
	FILE *entry_file = fopen(path, "r");
	struct todo_list *loaded_data = NULL;
	char *buffer = NULL;
	size_t buffer_size = 255;

	if (entry_file == NULL) {
		return NULL;
	}

	loaded_data = malloc(sizeof(struct todo_list));
	if (loaded_data == NULL) {
		return NULL;
	}

	loaded_data->list = malloc(sizeof(char *));
	if (loaded_data->list == NULL) {
		free(loaded_data);
		fclose(entry_file);
		return NULL;
	}

	if (getline(&buffer, &buffer_size, entry_file) == -1) {
		free(loaded_data->list);
		free(loaded_data);
		fclose(entry_file);
		return NULL;
	}

	loaded_data->list[0] = strdup(buffer);
	loaded_data->length++;

	while (getline(&buffer, &buffer_size, entry_file) != -1) {
		// TODO: Fix memory leak for interactive mode in the event realloc returns NULL by freeing
		// the previous list and its elements
		loaded_data->list = realloc(loaded_data->list, (loaded_data->length + 1) * sizeof(char *));
		loaded_data->list[loaded_data->length] = strdup(buffer);
		loaded_data->length++;
	}

	fclose(entry_file);
	return loaded_data;
}

void todo_update_value(struct todo_list **list, int index, char *value) {
	if (*list == NULL) {
		*list = todo_load_list();

		if (*list == NULL) {
			fprintf(stderr, "todo: Failed to load list\n");
			return;
		}
	}

	if (index < 0 || index > (*list)->length - 1) {
		fprintf(stderr, "todo: %d is out of bounds for this todo list\n", index);
		return;
	}

	(*list)->list[index] = value;
	return;
}

void todo_delete_value(struct todo_list **list, int index) {
	char **new_content = NULL;

	if (*list == NULL) {
		*list = todo_load_list();

		if (*list == NULL) {
			fprintf(stderr, "todo: Failed to load list\n");
			return;
		}
	}

	if (index < 0 || index > (*list)->length - 1) {
		fprintf(stderr, "todo: %d is out of bounds for this todo list\n", index);
		return;
	}

	new_content = malloc(sizeof(char *) * ((*list)->length - 1));
	if (new_content == NULL) {
		// FIXME: Print error and set list to NULL so that main can exit
		return;
	}

	for (int i = 0; i < index; i++) {
		new_content[i] = (*list)->list[i];
	}

	for (int i = index + 1, j = index; i < (*list)->length; i++, j++) {
		new_content[j] = (*list)->list[i];
	}

	// FIXME: Use todo_free_list()
	(*list)->list = new_content;
	(*list)->length--;
	return;
}

void todo_print_list(struct todo_list *todo_list) {
	if (todo_list == NULL) {
		todo_list = todo_load_list();

		if (todo_list == NULL) {
			fprintf(stderr, "todo: Failed to load list\n");
			return;
		}
	}

	for (int i = 0; i < todo_list->length; i++) {
		printf("%d: %s", i, todo_list->list[i]);
	}

	return;
}
