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

#ifndef CUPS_TODO_H
#define CUPS_TODO_H

/* Todo list items are delimited by the newline character. */

/* The elements in the list member must be freed. */
struct todo_list {
	char **list;
	int length;
};

/* Reads a todo list from a text file into a heap-allocated array with realloc. */
struct todo_list *todo_load_list(char *string);

/* Change the text of a list item at the given index. */
void todo_update_value(struct todo_list **list, char *list_name, int index, char *value);

/* Print a loaded todo list. */
void todo_print_list(char *list_name, struct todo_list *todo_list);

#endif /* CUPS_TODO_H */
