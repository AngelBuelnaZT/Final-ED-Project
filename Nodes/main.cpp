#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "FileSystem.h"

// Helper para dividir el texto del usuario
std::vector<std::string> splitInput(std::string input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) tokens.push_back(token);
    return tokens;
}

int main() {
    FileSystem fs;
    fs.loadSystem(); // Intenta cargar datos previos

    std::string input;
    bool running = true;

    std::cout << "=== FILE SYSTEM SHELL (DIA 10) ===" << std::endl;
    std::cout << "Comandos: ls, mkdir, touch, cd, rm, mv, trash, stress, search, save, exit" << std::endl;

    while (running) {
        fs.printCurrentPath();
        std::getline(std::cin, input);
        if (input.empty()) continue;

        std::vector<std::string> tokens = splitInput(input);
        std::string cmd = tokens[0];

        if (cmd == "exit") {
            running = false;
        }
        else if (cmd == "ls") {
            fs.ls();
        }
        else if (cmd == "save") {
            fs.saveSystem();
        }
        else if (cmd == "mkdir") {
            if (tokens.size() > 1) fs.mkdir(tokens[1]);
            else std::cout << "Uso: mkdir <nombre>" << std::endl;
        }
        else if (cmd == "touch") {
            if (tokens.size() > 1) {
                std::string content = "";
                for (size_t i = 2; i < tokens.size(); ++i) content += tokens[i] + " ";
                fs.touch(tokens[1], content);
            } else {
                std::cout << "Uso: touch <nombre> [contenido]" << std::endl;
            }
        }
        else if (cmd == "cd") {
            if (tokens.size() > 1) fs.changeDirectory(tokens[1]);
            else std::cout << "Uso: cd <carpeta>" << std::endl;
        }
        else if (cmd == "search") {
            if (tokens.size() > 1) fs.searchFiles(tokens[1]);
            else std::cout << "Uso: search <prefijo>" << std::endl;
        }
        // --- NUEVOS COMANDOS DIA 10 ---
        else if (cmd == "rm") {
            if (tokens.size() > 1) fs.moveToTrash(tokens[1]);
            else std::cout << "Uso: rm <nombre>" << std::endl;
        }
        else if (cmd == "mv") {
            if (tokens.size() > 2) fs.moveEntry(tokens[1], tokens[2]);
            else std::cout << "Uso: mv <origen> <destino>" << std::endl;
        }
        else if (cmd == "trash") {
            fs.showTrash();
        }
        else if (cmd == "stress") {
            // Lanza prueba con 5000 nodos por defecto
            fs.runStressTest(5000);
        }
        else {
            std::cout << "Comando desconocido." << std::endl;
        }
    }

    return 0;
}