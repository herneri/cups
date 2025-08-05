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
	"net/http"
	"io/ioutil"
	"encoding/json"
	"os"
)

const API_ENDPOINT string = "https://api.dictionaryapi.dev/api/v2/entries/en/"

type Definition struct {
	Definition string `json:"definition"`
	Synonyms []string `json:"synonyms"`
	Antonyms []string `json:"antonyms"`
}

type Entry struct {
	Part_speech string `json:"partOfSpeech"`
	Definitions []Definition `json:"definitions"`
}

type Result struct {
	Word string `json:"word"`
	Meanings []Entry `json:"meanings"`
}

func api_word_fetch(word string) []Result {
	var result []Result

	http_response, err := http.Get(API_ENDPOINT + word)
	if err != nil {
		fmt.Fprintf(os.Stderr, "dc: HTTP request failed: %s \n", err.Error)
		os.Exit(1)
	}

	defer http_response.Body.Close()

	json_bytes, err := ioutil.ReadAll(http_response.Body)
	if err != nil {
		fmt.Fprintf(os.Stderr, "dc: Failed to read bytes from HTTP response: %s \n", err.Error)
		os.Exit(1)
	}

	err = json.Unmarshal(json_bytes, &result)
	if err != nil {
		fmt.Fprintf(os.Stderr, "dc: Failed to load JSON: %s \n", err.Error)
		os.Exit(1)
	}

	return result
}

func display_definitions(payload []Result) {
	for _, meaning := range payload[0].Meanings {
		fmt.Println("______________________________\n")
		fmt.Println(meaning.Part_speech)

		for i := 0; i < len(meaning.Part_speech); i++ {
			fmt.Print("-")
		}

		fmt.Println()

		for _, definition := range meaning.Definitions {
			fmt.Println("* " + definition.Definition + "\n")
		}
	}

	return
}

func main() {
	var arg_count int = len(os.Args)
	var payload []Result

	if arg_count < 2 {
		fmt.Fprintf(os.Stderr, "dc: At least one argument must be entered\n")
		os.Exit(1)
	}

	payload = api_word_fetch(os.Args[1])
	display_definitions(payload)
}
