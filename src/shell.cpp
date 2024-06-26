#include <iostream>
#include <sstream>
#include "../include/shell.hpp"
#include "../include/command.hpp"

Shell::Shell() {
    cout << "Welcome to luna.sh!" << endl;
    while(true) {
        cout << "luna $ ";

        // Use getline to read the entire line (also reads in newlines)
        getline(cin, command);

        // Gracefully exit the shell
        if (command == "exit") {
            break;
        }

        if (command.size() >= 1) {
            vector<string> tokens = tokenize();
            parseUserInput(tokens);
        }
    }
}

Shell::~Shell() {
    /* Clean up resources */
}

/**
 * @brief  Tokenizes user input string
 *
 * This function takes in the user input string and tokenizes by white space.
 *
 * @return vector of tokens for command parsing
 */
vector<string> Shell::tokenize() {
    istringstream iss(command);
    vector<string> tokens;
    string token;

    // extraction operator default delimeter is whitespace
    while (iss >> token) {
        tokens.push_back(token);
    }

    size_t tokens_length = tokens.size();
    // if echo command used, text enclosed in quotations is treated as one token
    if (tokens[0] == "echo" && tokens_length > 1 && tokens[1][0] == '"') {
        //second is an edge case for when user goes to new line right after first quotation
        while (tokens[tokens_length - 1].back() != '"' || tokens[1].size() == 1){
            cout << ":( > ";
            string additional;
            getline(cin, additional);
            command = command + "\n" + additional;
            if (additional.back() == '"') {
                break;
            }
        }

        size_t echo_input_start = command.find("\"");
        /* Gets substring from first quotation, upto last quotation.
        Second parameter is no. of characters to include in substring towards the end */
        tokens = {"echo", command.substr(echo_input_start + 1, command.size() - echo_input_start - 2)};
    }
    return tokens;
}

/**
 * @brief  Parses command and instantiates corresponding project
 *
 * This function takes in the user input tokens and looks at the first command.
 * Afterwards it will instantiate the appropriate Command object and call it's method
 * for the command to execute.
 *
 * @param tokens: tokenized user input string
 *
 * @return vector of tokens for command parsing
 */
void Shell::parseUserInput(const vector<string>& tokens) {
    string command = tokens[0];
    if (command == "echo") {
        EchoCommand echo(tokens);
        echo.echo_fn();
        cout << echo.result << endl;
    } else if (command == "pwd") {
        PwdCommand pwd(tokens);
        pwd.pwd_fn();
        cout << pwd.result;
    } else if (command == "cd") {
        CdCommand cd(tokens);
        cd.cd_fn();
        cout << cd.result;
    } else if (command == "mkdir") {
        MkDirCommand mkdir(tokens);
        mkdir.mkdir_fn();
        cout << mkdir.result;
    } else if (command == "rm" || tokens[0] == "rmdir") {
        RmCommand rm(tokens);
        rm.rm_fn();
        cout << rm.result;
    } else if (command == "ls") {
        LsCommand ls(tokens);
        ls.ls_fn();
        cout << ls.result;
    } else if (command == "mv") {
        MvCommand mv(tokens);
        mv.mv_fn();
        cout << mv.result;
    } else if (command == "cat") {
        CatCommand cat(tokens);
        cat.cat_fn();
        cout << cat.result;
    }
    else {
        cout << "luna.sh: command not found: " << tokens[0] << endl;
    }
}
