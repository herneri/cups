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

	free(buffer);
	fclose(entry_file);
	return loaded_data;
}

void todo_free_list(struct todo_list **list) {
	if (*list == NULL) {
		return;
	}

	for (int i = 0; i < (*list)->length; i++) {
		free((*list)->list[i]);
	}

	free((*list)->list);
	free(*list);
	*list = NULL;
	return;
}

int todo_check_list(struct todo_list **list) {
	if (*list == NULL) {
		*list = todo_load_list();

		if (*list == NULL) {
			fprintf(stderr, "todo: Failed to load list\n");
			return -1;
		}
	}

	return 0;
}

int todo_index_check(int list_length, int index) {
	if (index < 0 || index > list_length - 1) {
		fprintf(stderr, "todo: %d is out of bounds for this todo list\n", index);
		return -1;
	}

	return 0;
}

int todo_write_list(struct todo_list **list) {
	FILE *todo_file = NULL;
	char path[255];
	sprintf(path, "%s/.local/share/cups/todo.txt", getenv("HOME"));

	if (*list == NULL) {
		return -1;
	}

	todo_file = fopen(path, "w");
	if (todo_file == NULL) {
		return -2;
	}

	for (int i = 0; i < (*list)->length; i++) {
		fputs((*list)->list[i], todo_file);
	}

	fclose(todo_file);
	return 0;
}

int todo_append_value(struct todo_list **list, char *value) {
	if (todo_check_list(list) == -1) {
		return -1;
	}

	(*list)->list = realloc((*list)->list, sizeof(char *) * (*list)->length);
	if ((*list)->list == NULL) {
		fprintf(stderr, "todo: Failed to append to todo list\n");
		return -1;
	}

	(*list)->list[(*list)->length] = strdup(value);
	(*list)->length++;
	return 0;
}

void todo_update_value(struct todo_list **list, int index, char *value) {
	if (todo_check_list(list) == -1) {
		return;
	}

	if (todo_index_check((*list)->length, index) == -1) {
		return;
	}

	(*list)->list[index] = value;
	return;
}

void todo_delete_value(struct todo_list **list, int index) {
	char **new_content = NULL;

	if (todo_check_list(list) == -1) {
		return;
	}

	if (todo_index_check((*list)->length, index) == -1) {
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
	if (todo_check_list(&todo_list) == -1) {
		return;
	}

	for (int i = 0; i < todo_list->length; i++) {
		printf("%d: %s", i, todo_list->list[i]);
	}

	return;
}
