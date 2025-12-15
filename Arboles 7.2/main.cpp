#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> // Para std::max

using namespace std;

// ==========================================
// 1. MODELO DE DATOS (Nodo)
// ==========================================
struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int value) : key(value), left(nullptr), right(nullptr) {}
};

// ==========================================
// 2. CLASE BST (Lógica del Árbol)
// ==========================================
class BST {
private:
    Node* root;

    // Métodos auxiliares privados (recursivos)
    Node* insertRec(Node* node, int key) {
        if (node == nullptr) {
            return new Node(key);
        }
        if (key < node->key) {
            node->left = insertRec(node->left, key);
        } else if (key > node->key) {
            node->right = insertRec(node->right, key);
        }
        // Si es igual, no insertamos duplicados (comportamiento estándar BST)
        return node;
    }

    Node* searchRec(Node* node, int key) {
        if (node == nullptr || node->key == key) {
            return node;
        }
        if (key < node->key) {
            cout << "L -> "; // Mostrar ruta visualmente
            return searchRec(node->left, key);
        }
        cout << "R -> "; // Mostrar ruta visualmente
        return searchRec(node->right, key);
    }

    Node* findMin(Node* node) {
        while (node->left != nullptr) node = node->left;
        return node;
    }

    Node* deleteRec(Node* node, int key) {
        if (node == nullptr) return node;

        if (key < node->key) {
            node->left = deleteRec(node->left, key);
        } else if (key > node->key) {
            node->right = deleteRec(node->right, key);
        } else {
            // Caso 1: Sin hijos (Hoja) o Caso 2: Un hijo
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Caso 3: Dos hijos
            // Obtener el sucesor inorden (el menor del subárbol derecho)
            Node* temp = findMin(node->right);
            node->key = temp->key; // Copiar el valor
            node->right = deleteRec(node->right, temp->key); // Eliminar el sucesor
        }
        return node;
    }

    void inorderRec(Node* node) {
        if (node != nullptr) {
            inorderRec(node->left);
            cout << node->key << " ";
            inorderRec(node->right);
        }
    }

    void preorderRec(Node* node) {
        if (node != nullptr) {
            cout << node->key << " ";
            preorderRec(node->left);
            preorderRec(node->right);
        }
    }

    void postorderRec(Node* node) {
        if (node != nullptr) {
            postorderRec(node->left);
            postorderRec(node->right);
            cout << node->key << " ";
        }
    }

    int heightRec(Node* node) {
        if (node == nullptr) return -1; // Altura de árbol vacío es -1 (o 0 según convención)
        return 1 + max(heightRec(node->left), heightRec(node->right));
    }

    int sizeRec(Node* node) {
        if (node == nullptr) return 0;
        return 1 + sizeRec(node->left) + sizeRec(node->right);
    }

    void exportInorderRec(Node* node, ofstream& file) {
        if (node != nullptr) {
            exportInorderRec(node->left, file);
            file << node->key << " ";
            exportInorderRec(node->right, file);
        }
    }

    // Limpieza de memoria
    void destroyRec(Node* node) {
        if (node) {
            destroyRec(node->left);
            destroyRec(node->right);
            delete node;
        }
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroyRec(root); }

    void insert(int key) { root = insertRec(root, key); }

    void search(int key) {
        cout << "Ruta: Root -> ";
        Node* result = searchRec(root, key);
        if (result) cout << "[ENCONTRADO]" << endl;
        else cout << "[NO ENCONTRADO]" << endl;
    }

    void remove(int key) {
        root = deleteRec(root, key);
        cout << "Nodo " << key << " eliminado (si existia)." << endl;
    }

    void inorder() { inorderRec(root); cout << endl; }
    void preorder() { preorderRec(root); cout << endl; }
    void postorder() { postorderRec(root); cout << endl; }

    int height() { return heightRec(root); }
    int size() { return sizeRec(root); }

    void export_inorder(string filename) {
        ofstream file(filename);
        if (file.is_open()) {
            exportInorderRec(root, file);
            file.close();
            cout << "Arbol exportado a " << filename << endl;
        } else {
            cout << "Error al abrir el archivo." << endl;
        }
    }
};

// ==========================================
// 3. INTERFAZ DE CONSOLA (Main)
// ==========================================
void printHelp() {
    cout << "--- Comandos Disponibles ---\n";
    cout << "insert <num>  : Insertar numero\n";
    cout << "search <num>  : Buscar numero\n";
    cout << "delete <num>  : Eliminar numero\n";
    cout << "inorder       : Mostrar recorrido Inorden\n";
    cout << "preorder      : Mostrar recorrido Preorden\n";
    cout << "postorder     : Mostrar recorrido Posorden\n";
    cout << "height        : Mostrar altura del arbol\n";
    cout << "size          : Mostrar numero de nodos\n";
    cout << "export        : Guardar en 'tree_dump.txt'\n";
    cout << "help          : Mostrar este menu\n";
    cout << "exit          : Salir\n";
    cout << "----------------------------\n";
}

int main() {
    BST tree;
    string command;
    int value;

    cout << "=== Gestor de Numeros con BST ===\n";
    printHelp();

    while (true) {
        cout << "> ";
        cin >> command;

        if (command == "exit") break;
        else if (command == "help") printHelp();
        else if (command == "insert") {
            if (cin >> value) tree.insert(value);
            else { cout << "Entrada invalida.\n"; cin.clear(); cin.ignore(1000, '\n'); }
        }
        else if (command == "search") {
            if (cin >> value) tree.search(value);
        }
        else if (command == "delete") {
            if (cin >> value) tree.remove(value);
        }
        else if (command == "inorder") tree.inorder();
        else if (command == "preorder") tree.preorder();
        else if (command == "postorder") tree.postorder();
        else if (command == "height") cout << "Altura: " << tree.height() << endl;
        else if (command == "size") cout << "Nodos: " << tree.size() << endl;
        else if (command == "export") tree.export_inorder("tree_dump.txt");
        else cout << "Comando desconocido.\n";
    }

    return 0;
}