File System - Documentación del Proyecto

Materia: Estructura de Datos

Lenguaje: C++

1. Introducción y Objetivo

El objetivo de este proyecto es implementar una simulación de un Sistema de Archivos Jerárquico en consola (CLI). A diferencia de los sistemas operativos reales que manipulan sectores del disco duro, este proyecto crea un sistema de archivos virtual que reside en la memoria RAM y se persiste en un archivo JSON.

El propósito principal es demostrar la aplicación práctica de estructuras de datos no lineales, específicamente:

Árboles Generales (N-arios): Para modelar la jerarquía de carpetas y archivos.

Tries (Árboles de Prefijos): Para optimizar la búsqueda de archivos por nombre.

2. Arquitectura del Sistema

El sistema está diseñado de manera modular, separando la estructura de datos de la lógica de negocio y la interfaz de usuario.

2.1. Estructuras de Datos Utilizadas

A. Árbol General (N-ary Tree)

La base del sistema es un árbol donde cada nodo representa un archivo o una carpeta.

Justificación: Un sistema de archivos es intrínsecamente jerárquico. Una carpeta puede contener $N$ cantidad de subcarpetas o archivos, lo que hace que un árbol N-ario sea la representación natural.

Implementación:

struct FileSystemNode {
    string name;
    NodeType type; // FOLDER o FILE
    vector<FileSystemNode*> children; // Lista dinámica de hijos
    // ...
};


B. Trie (Prefix Tree)

Se implementó un Trie para indexar los nombres de todos los archivos creados.

Justificación: A medida que el sistema crece (ej. 5,000 archivos), buscar un archivo recorriendo todo el árbol general sería lento ($O(N)$). El Trie permite búsquedas con una complejidad de tiempo basada en la longitud de la palabra buscada ($O(L)$), lo cual es instantáneo para efectos prácticos.

Funcionalidad: Soporta autocompletado y búsqueda por prefijo (ej. buscar "proy" encuentra "proyecto.cpp" y "proyeccion.pdf").

C. Persistencia JSON

Se utilizó la librería nlohmann/json para serializar el árbol.

Funcionamiento: Al ejecutar el comando save, el programa recorre el árbol recursivamente y lo transforma en un objeto JSON que se escribe en filesystem.json. Al iniciar, hace el proceso inverso (Deserialización).

3. Descripción de Módulos (Código)

Archivo

Responsabilidad

FileSystemNode.h

Define la unidad básica (Nodo). Contiene los atributos id, name, type, content y el vector children.

Trie.h

Contiene la lógica del motor de búsqueda. Inserta nombres y busca coincidencias por prefijo de manera independiente al árbol principal.

FileSystem.h

Controlador Principal. Gestiona el puntero root (raíz), mantiene el puntero currentDirectory (navegación), valida reglas de negocio (no duplicados) y coordina la persistencia y el Trie.

main.cpp

Interfaz de Usuario (CLI). Se encarga de leer la entrada del teclado, parsear los comandos (separar texto) y llamar a las funciones del controlador.

4. Manual de Usuario

4.1. Instalación y Ejecución

El proyecto no requiere instalación, es un ejecutable portable.

Asegúrese de tener el archivo filesystem.exe (o el binario compilado en Linux/Mac).

Haga doble clic o ejecútelo desde la terminal: ./filesystem.

4.2. Comandos Disponibles

Navegación y Creación

ls: Muestra el contenido del directorio donde se encuentra el usuario.

mkdir <nombre>: Crea un nuevo directorio.

touch <nombre> [contenido]: Crea un archivo. El contenido es opcional.

cd <nombre>: Cambia de directorio.

cd ..: Sube un nivel (al padre).

cd /: Regresa a la raíz.

Manipulación

mv <origen> <destino>: Mueve un archivo o carpeta a otra ubicación.

rm <nombre>: Borrado Suave (Soft Delete). No elimina el archivo permanentemente, sino que lo mueve a la carpeta .trash.

Herramientas

trash: Muestra los elementos que han sido eliminados.

search <prefijo>: Busca archivos en todo el sistema instantáneamente.

save: Guarda los cambios en el disco duro.

stress: Ejecuta una prueba de carga (crea 5,000 archivos) para demostrar estabilidad.

help: Muestra la lista de ayuda.

5. Pruebas de Rendimiento (Stress Test)

Se implementó un comando stress para validar la robustez del manejo de memoria y la eficiencia del Trie.

Escenario: Inserción de 5,000 nodos en una carpeta.

Resultados:

Tiempo de inserción: < 1 segundo (aprox).

Búsqueda en Trie con 5,000 elementos: Instantánea.

Estabilidad: El programa no presenta fugas de memoria críticas ni crashes.

6. Conclusión

 La combinación de un Árbol N-ario para la estructura y un Trie para la indexación demostró ser una arquitectura eficiente para simular un sistema de archivos. La persistencia en JSON permite que la aplicación sea útil entre sesiones, y la implementación de la papelera de reciclaje añade una capa de seguridad para el usuario.

Autores

[Buelna Bojorquez Angel Andres]
[Carlos Agustin Lerma Felix]
