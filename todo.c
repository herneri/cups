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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Todo list items are delimited by the newline character. */

/* The elements in the list member must be freed. */
struct todo_list {
	char **list;
	int length;
};

/* Reads a todo list from a text file into a heap-allocated array with realloc. */
struct todo_list *todo_load_list(void) {
	char path[255];
	sprintf(path, "%s/.local/share/cups/todo.txt", getenv("HOME"));
	FILE *entry_file = fopen(path, "r");
	struct todo_list *loaded_data = NULL;
	char *buffer = NULL;
	size_t buffer_size = 255;

	int list_index = 0;
	int newline_index = 0;

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
		loaded_data->length = 0;
		fclose(entry_file);
		return loaded_data;
	}

	loaded_data->list[list_index] = strdup(buffer);

	newline_index = strnlen(buffer, buffer_size) - 1;

	loaded_data->list[list_index][newline_index] = '\0';
	loaded_data->length++;

	while (getline(&buffer, &buffer_size, entry_file) != -1) {
		// TODO: Fix memory leak for interactive mode in the event realloc returns NULL by freeing
		// the previous list and its elements
		list_index = loaded_data->length;
		newline_index = strnlen(buffer, buffer_size) - 1;

		loaded_data->list = realloc(loaded_data->list, (loaded_data->length + 1) * sizeof(char *));

		loaded_data->list[list_index] = strdup(buffer);
		loaded_data->list[list_index][newline_index] = '\0';

		loaded_data->length++;
	}

	free(buffer);
	fclose(entry_file);
	return loaded_data;
}

/* Frees a todo_list and it's elements. */
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

/* See whether the list is loaded. If it isn't, then load it. */
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

/* Check whether the given index is within the list. */
int todo_index_check(int list_length, int index) {
	if (index < 0 || index > list_length - 1) {
		fprintf(stderr, "todo: %d is out of bounds for this todo list\n", index);
		return -1;
	}

	return 0;
}

/* Output list contents to the todo list file. */
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
		fputs("\n", todo_file);
	}

	fclose(todo_file);
	return 0;
}

/* Append a new value to the todo list. */
int todo_append_value(struct todo_list **list, char *value) {
	if (todo_check_list(list) == -1) {
		return -1;
	}

	if ((*list)->length != 0) {
		(*list)->list = realloc((*list)->list, sizeof(char *) * (*list)->length);

		if ((*list)->list == NULL) {
			fprintf(stderr, "todo: Failed to append to todo list\n");
			return -1;
		}
	}

	(*list)->list[(*list)->length] = strdup(value);
	(*list)->length++;
	return 0;
}

/* Change the text of a list item at the given index. */
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

/* Delete a value at the given index. */
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

/* Print a loaded todo list. */
void todo_print_list(struct todo_list *todo_list) {
	if (todo_check_list(&todo_list) == -1) {
		return;
	}

	for (int i = 0; i < todo_list->length; i++) {
		printf("%d: %s\n", i, todo_list->list[i]);
	}

	return;
}

int main(int argc, char *argv[]) {
	struct todo_list *list = NULL;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			continue;
		}

		switch (argv[i][1]) {
		case 'p':
			todo_print_list(list);
			break;
		case 'a':
			if (argc <= i + 1) {
				fprintf(stderr, "todo: A value is required for todo appending\n");
				return 1;
			}

			todo_append_value(&list, argv[i + 1]);
			todo_write_list(&list);
			i++;
			break;
		case 'r':
			if (argc <= i + 1) {
				fprintf(stderr, "todo: An index is required for todo removal\n");
				return 1;
			}

			todo_delete_value(&list, atoi(argv[i + 1]));
			todo_write_list(&list);
			i++;
			break;
		case 'e':
			if (argc <= i + 2) {
				fprintf(stderr, "todo: An index and value is required for todo editing\n");
				return 1;
			}

			todo_update_value(&list, atoi(argv[i + 1]), argv[i + 2]);
			todo_write_list(&list);
			i += 2;
			break;
		default:
			fprintf(stderr, "todo: Invalid option\n");
			return 2;
		}
	}

	return 0;
}
