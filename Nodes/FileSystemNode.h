#ifndef FILESYSTEMNODE_H
#define FILESYSTEMNODE_H

#include <string>
#include <vector>
#include <iostream>

// Enumeración para distinguir entre archivo y carpeta de forma clara
enum NodeType {
    FOLDER,
    FILE_NODE
};

struct FileSystemNode {
    // Campos requeridos por las especificaciones [cite: 7]
    std::string id;
    std::string name;
    NodeType type;
    std::string content; // Solo tendrá valor si es FILE_NODE

    // Lista de hijos. Usamos punteros para manejar estructuras dinámicas.
    // Se puede usar std::vector<FileSystemNode*> o std::shared_ptr para gestión moderna de memoria.
    std::vector<FileSystemNode*> children;

    // Constructor para inicializar fácilmente
    FileSystemNode(std::string _id, std::string _name, NodeType _type, std::string _content = "")
        : id(_id), name(_name), type(_type), content(_content) {}

    // Destructor (simple) para limpiar memoria (se implementará lógica completa después)
    ~FileSystemNode() {
        for (auto child : children) {
            delete child;
        }
        children.clear();
    }
};

#endif