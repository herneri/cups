package main

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

import (
	"fmt"
	"os"
)

var CN_DIR string = os.Getenv("HOME") + "/.local/share/cups/cn/"

func check_dir_exists() {
	_, err := os.ReadDir(CN_DIR)
	if err != nil {
		fmt.Fprintf(os.Stderr, "cn: Directory not found, creating new one\n")
		os.MkdirAll(CN_DIR, os.ModePerm)
	}

	return
}

func list_notes() {
	entries, err := os.ReadDir(CN_DIR)
	if err != nil {
		check_dir_exists()
		return
	}

	for i := 0; i < len(entries); i++ {
		if entries[i].IsDir() == false {
			fmt.Println("* " + entries[i].Name())
			fmt.Println("------")
		}
	}

	return
}

func print_note(note_name string) bool {
	note_text, err := os.ReadFile(CN_DIR + note_name)
	if err != nil {
		fmt.Fprintf(os.Stderr, "cn: Failed to read file\n")
		return false
	}

	fmt.Printf("%s\n", note_text)
	return true
}

func write_note(note_name string) {
	var text string
	note_file, err := os.OpenFile(CN_DIR + note_name, os.O_APPEND | os.O_CREATE | os.O_WRONLY, 0644)
	if err != nil {
		fmt.Fprintf(os.Stderr, "cn: Failed to open file for note writing\n")
		return
	}

	fmt.Scan(&text)
	for text != ">X" {
		note_file.WriteString(text + "\n")
		fmt.Scan(&text)
	}

	return
}

func delete_note(note_name string) bool {
	var err error = os.Remove(CN_DIR + note_name)
	if err != nil {
		fmt.Fprintf(os.Stderr, "cn: Failed to remove note\n")
		return false
	}

	return true
}

func main() {
	check_dir_exists()
	// TODO: Write CLI argument handling
}
