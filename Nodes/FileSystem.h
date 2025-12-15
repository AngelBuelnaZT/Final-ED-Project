#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "FileSystemNode.h"
#include "Trie.h"
#include "json.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

class FileSystem {
private:
    FileSystemNode* root;
    FileSystemNode* currentDirectory;
    Trie* searchEngine;
    int idCounter;

    // --- HELPERS PRIVADOS ---

    FileSystemNode* searchNodeRecursive(FileSystemNode* current, std::string targetId) {
        if (!current) return nullptr;
        if (current->id == targetId) return current;
        for (auto child : current->children) {
            FileSystemNode* found = searchNodeRecursive(child, targetId);
            if (found) return found;
        }
        return nullptr;
    }

    FileSystemNode* findParent(FileSystemNode* current, std::string targetId) {
        if (!current) return nullptr;
        for (auto child : current->children) {
            if (child->id == targetId) return current;
            FileSystemNode* res = findParent(child, targetId);
            if (res) return res;
        }
        return nullptr;
    }

    // Busca un hijo por nombre SOLAMENTE en el nodo padre indicado
    FileSystemNode* findChildByName(FileSystemNode* parent, std::string name) {
        for (auto child : parent->children) {
            if (child->name == name) return child;
        }
        return nullptr;
    }

    // --- JSON HELPERS ---
    json nodeToJson(FileSystemNode* node) {
        json j;
        j["id"] = node->id;
        j["name"] = node->name;
        j["type"] = (node->type == FOLDER ? "FOLDER" : "FILE");
        j["content"] = node->content;
        j["children"] = json::array();
        for (auto child : node->children) {
            j["children"].push_back(nodeToJson(child));
        }
        return j;
    }

    FileSystemNode* jsonToNode(json j) {
        NodeType type = (j["type"] == "FOLDER" ? FOLDER : FILE_NODE);
        FileSystemNode* node = new FileSystemNode(j["id"], j["name"], type, j.value("content", ""));

        searchEngine->insert(node->name);

        if (j.contains("children")) {
            for (auto& element : j["children"]) {
                node->children.push_back(jsonToNode(element));
            }
        }
        return node;
    }

public:
    FileSystem() {
        root = new FileSystemNode("root_0", "root", FOLDER);
        currentDirectory = root;
        searchEngine = new Trie();
        idCounter = 1;
        // Inicializar papelera al arrancar
        initTrash();
    }

    ~FileSystem() {
        delete root;
        delete searchEngine;
    }

    std::string generateId() {
        return "node_" + std::to_string(idCounter++);
    }

    // Asegura que exista la carpeta .trash
    void initTrash() {
        FileSystemNode* trash = findChildByName(root, ".trash");
        if (!trash) {
            // Creamos la papelera manualmente para no usar createNode y evitar logs extra
            trash = new FileSystemNode("trash_bin", ".trash", FOLDER);
            root->children.push_back(trash);
        }
    }

    // --- OPERACIONES CORE ---

    bool createNode(std::string parentId, std::string newId, std::string name, NodeType type, std::string content = "") {
        FileSystemNode* parent = searchNodeRecursive(root, parentId);
        if (!parent || parent->type != FOLDER) return false;

        for (auto child : parent->children) {
            if (child->name == name) {
                std::cout << "Error: El nombre '" << name << "' ya existe." << std::endl;
                return false;
            }
        }

        FileSystemNode* newNode = new FileSystemNode(newId, name, type, content);
        parent->children.push_back(newNode);
        searchEngine->insert(name);
        return true;
    }

    // --- NUEVAS FUNCIONES DÍA 10 (PAPELERA Y MOVIMIENTO) ---

    // Soft Delete: Mueve a .trash en lugar de borrar
    void moveToTrash(std::string name) {
        initTrash(); // Asegurarnos de que existe

        if (name == ".trash") {
            std::cout << "No puedes borrar la papelera." << std::endl;
            return;
        }

        FileSystemNode* target = findChildByName(currentDirectory, name);
        if (!target) {
            std::cout << "Error: '" << name << "' no encontrado." << std::endl;
            return;
        }

        FileSystemNode* trash = findChildByName(root, ".trash");

        // 1. Desconectar del directorio actual
        for (auto it = currentDirectory->children.begin(); it != currentDirectory->children.end(); ++it) {
            if (*it == target) {
                currentDirectory->children.erase(it);
                break;
            }
        }

        // 2. Conectar a la papelera
        trash->children.push_back(target);
        std::cout << "Elemento '" << name << "' movido a la papelera." << std::endl;
    }

    // Listar contenido de la papelera
    void showTrash() {
        FileSystemNode* trash = findChildByName(root, ".trash");
        if (!trash || trash->children.empty()) {
            std::cout << "La papelera esta vacia." << std::endl;
            return;
        }
        std::cout << "--- Papelera de Reciclaje (.trash) ---" << std::endl;
        for (auto child : trash->children) {
            std::string tipo = (child->type == FOLDER ? "<DIR>" : "<FILE>");
            std::cout << tipo << " " << child->name << " (ID Original: " << child->id << ")" << std::endl;
        }
    }

    // Mover nodo de una carpeta a otra por nombre (Comando mv)
    void moveEntry(std::string sourceName, std::string destName) {
        FileSystemNode* target = findChildByName(currentDirectory, sourceName);
        if (!target) {
            std::cout << "Error: Origen '" << sourceName << "' no encontrado." << std::endl;
            return;
        }

        // Determinar destino: ¿Es '..' o una carpeta hija?
        FileSystemNode* destFolder = nullptr;

        if (destName == "..") {
             if (currentDirectory != root) {
                destFolder = findParent(root, currentDirectory->id);
             } else {
                 std::cout << "Ya estas en la raiz." << std::endl;
                 return;
             }
        } else {
            destFolder = findChildByName(currentDirectory, destName);
        }

        if (!destFolder || destFolder->type != FOLDER) {
            std::cout << "Error: Destino invalido (no existe o no es carpeta)." << std::endl;
            return;
        }

        // Mover: Desconectar y Conectar
        for (auto it = currentDirectory->children.begin(); it != currentDirectory->children.end(); ++it) {
            if (*it == target) {
                currentDirectory->children.erase(it);
                break;
            }
        }
        destFolder->children.push_back(target);
        std::cout << "Movido '" << sourceName << "' a '" << destFolder->name << "'." << std::endl;
    }

    // --- PRUEBA DE ESTRÉS / PERFORMANCE (DÍA 10) ---
    void runStressTest(int quantity) {
        std::cout << "Iniciando prueba de estres con " << quantity << " archivos..." << std::endl;

        std::string folderName = "stress_test_" + std::to_string(idCounter);
        createNode(currentDirectory->id, generateId(), folderName, FOLDER);
        FileSystemNode* testFolder = currentDirectory->children.back();

        for (int i = 0; i < quantity; ++i) {
            std::string name = "file_" + std::to_string(i) + ".dat";
            // Creamos directamente para evitar la sobrecarga de couts del createNode normal
            FileSystemNode* node = new FileSystemNode(generateId(), name, FILE_NODE, "data");
            testFolder->children.push_back(node);
            searchEngine->insert(name);
        }
        std::cout << "Finalizado. Se creo la carpeta '" << folderName << "' con " << quantity << " nodos." << std::endl;
    }

    // --- NAVEGACIÓN Y CONSOLA ---

    void changeDirectory(std::string name) {
        if (name == "..") {
            if (currentDirectory == root) return;
            FileSystemNode* parent = findParent(root, currentDirectory->id);
            if (parent) currentDirectory = parent;
        } else if (name == "/") {
            currentDirectory = root;
        } else {
            FileSystemNode* target = findChildByName(currentDirectory, name);
            if (target && target->type == FOLDER) {
                currentDirectory = target;
            } else {
                std::cout << "Directorio no encontrado." << std::endl;
            }
        }
    }

    void printCurrentPath() {
        std::cout << "/" << currentDirectory->name << " $ ";
    }

    void mkdir(std::string name) {
        if (createNode(currentDirectory->id, generateId(), name, FOLDER))
            std::cout << "Carpeta creada." << std::endl;
    }

    void touch(std::string name, std::string content) {
        if (createNode(currentDirectory->id, generateId(), name, FILE_NODE, content))
            std::cout << "Archivo creado." << std::endl;
    }

    void ls() {
        if (currentDirectory->children.empty()) {
            std::cout << "(Vacio)" << std::endl;
            return;
        }
        for (auto child : currentDirectory->children) {
            // Ocultar la papelera del ls normal si estamos en root
            if (child->name == ".trash") continue;

            std::string tipo = (child->type == FOLDER ? "<DIR>" : "<FILE>");
            std::cout << tipo << "\t" << child->name << std::endl;
        }
    }

    void searchFiles(std::string prefix) {
        auto results = searchEngine->searchByPrefix(prefix);
        std::cout << "Resultados busqueda '" << prefix << "':" << std::endl;
        for (auto r : results) std::cout << "- " << r << std::endl;
    }

    // --- PERSISTENCIA ---

    void saveSystem() {
        json j = nodeToJson(root);
        std::ofstream file("filesystem.json");
        if (file.is_open()) {
            file << j.dump(4);
            file.close();
            std::cout << "Guardado exitoso." << std::endl;
        }
    }

    void loadSystem() {
        std::ifstream file("filesystem.json");
        if (!file.is_open()) return;

        try {
            json j;
            file >> j;
            delete root;
            delete searchEngine;
            searchEngine = new Trie();

            root = jsonToNode(j);
            currentDirectory = root;
            initTrash(); // Asegurar papelera tras cargar
            std::cout << "Sistema cargado." << std::endl;
        } catch (...) {
            std::cout << "Error cargando JSON. Iniciando sistema nuevo." << std::endl;
        }
    }
};

#endif