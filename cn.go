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
	"bufio"
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
	var stdin *bufio.Reader = bufio.NewReader(os.Stdin)

	note_file, err := os.OpenFile(CN_DIR + note_name, os.O_APPEND | os.O_CREATE | os.O_WRONLY, 0644)
	if err != nil {
		fmt.Fprintf(os.Stderr, "cn: Failed to open file for note writing\n")
		return
	}

	text, _ = stdin.ReadString('\n')
	for text != ">X\n" {
		note_file.WriteString(text)
		text, _ = stdin.ReadString('\n')
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

func find_argument_deficiency(arg_count int, i int, option string) {
	if i + 1 > arg_count - 1 {
		fmt.Fprintf(os.Stderr, "cn: Not enough arguments, %s requires a note name \n", option);
		os.Exit(1)
	}

	return
}

func main() {
	var arg_count int = len(os.Args)

	check_dir_exists()

	for i := 1; i < arg_count; i++ {
		if os.Args[i][0] == '-' {
			switch os.Args[i][1] {
			case 'l':
				list_notes()
				i++
			case 'p':
				find_argument_deficiency(arg_count, i, "-p")
				print_note(os.Args[i + 1])
				i++
			case 'n':
				find_argument_deficiency(arg_count, i, "-n")
				write_note(os.Args[i + 1])
				i++
			case 'r':
				find_argument_deficiency(arg_count, i, "-r")
				delete_note(os.Args[i + 1])
				i++
			case 'h':
				fmt.Println("usage: cn [OPTION] \n\n-l\t\tList all notes\n-p [NAME]\tPrint note",
				"\n-n [NAME]\tCreate note\n-r [NAME]\tDelete note")
			default:
				fmt.Fprintf(os.Stderr, "cn: \"%c\" is an invalid argument \n", os.Args[i][1])
			}
		}
	}

	return
}
