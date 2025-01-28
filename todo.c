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
#include <string.h>

int todo_write_list(char *list_name) {
	FILE *entry_file = fopen(list_name, "w");

	if (entry_file == NULL) {
		return 1;
	}

	for (int i = 0; i < 2; i++) {
		// TODO: Write from todo linked list
	}

	fclose(entry_file);
	return 0;
}

int todo_read_list(char *list_name) {
	FILE *entry_file = fopen(list_name, "r");
	char buffer[255];

	if (entry_file == NULL) {
		return 1;
	}

	while (fgets(buffer, 255, entry_file)) {
		// TODO: Add to todo linked list
	}

	fclose(entry_file);
	return 0;
}
