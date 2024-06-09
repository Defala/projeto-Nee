#include <iostream>
#include <cstdlib>
#include <cstring>

#define MAX_SIZE 100
#define MAX_COMMANDS 100
#define MAX_NAME_LENGTH 2
#define MAX_COMMAND_LENGTH 20
#define MAX_FUNCTIONS 10

// Lista Estática
int arr[MAX_SIZE];
int count = 0;

// Pilha Dinâmica
struct Node {
    char command[MAX_COMMAND_LENGTH];
    Node* next;
};

Node* top = NULL;

struct CommandList {
    char commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];
    int count = 0;
};

struct CommandMap {
    char name[MAX_NAME_LENGTH];
    CommandList commandList;
};

CommandMap commands[MAX_FUNCTIONS];
int numCommands = 0;

char callStack[MAX_FUNCTIONS][MAX_NAME_LENGTH];
int callStackCount = 0;

void push(const char* command) {
    Node* newNode = new Node();
    std::strcpy(newNode->command, command);
    newNode->next = top;
    top = newNode;
}

void pop() {
    if (top != NULL) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

bool isInCallStack(const char* command) {
    for (int i = 0; i < callStackCount; i++) {
        if (std::strcmp(callStack[i], command) == 0) {
            return true;
        }
    }
    return false;
}

void execute(const char* command) {
    if (isInCallStack(command)) {
        std::cout << "Erro: Detected circular dependency or infinite loop in command: " << command << std::endl;
        return;
    }
    std::strcpy(callStack[callStackCount++], command);

    if (std::strncmp(command, "PRINT", 5) == 0) {
        int num = std::atoi(command + 6);
        if (count < MAX_SIZE) {
            arr[count++] = num;
        } else {
            std::cout << "Lista cheia, não é possível inserir mais elementos." << std::endl;
        }
        std::cout << num << " ";
    } else {
        for (int i = 0; i < numCommands; i++) {
            if (std::strcmp(commands[i].name, command) == 0) {
                push(command);
                for (int j = commands[i].commandList.count - 1; j >= 0; --j) {
                    execute(commands[i].commandList.commands[j]);
                }
                pop();
                break;
            }
        }
    }

    callStackCount--;
}

int main() {
    char line[100];
    std::cout << "Digite os comandos (linha vazia para terminar):" << std::endl;
    while (true) {
        std::cin.getline(line, 100);
        if (std::strlen(line) == 0) {
            // Fim da definição dos comandos
            break;
        }

        if (std::strlen(line) < 4 || line[1] != ':') {
            std::cout << "Entrada inválida. Formato esperado: <nome_comando>: <comando>" << std::endl;
            continue;
        }

        char commandName[MAX_NAME_LENGTH];
        commandName[0] = line[0];
        commandName[1] = '\0';
        char command[MAX_COMMAND_LENGTH];
        std::strcpy(command, line + 3);

        bool found = false;
        for (int i = 0; i < numCommands; i++) {
            if (std::strcmp(commands[i].name, commandName) == 0) {
                if (commands[i].commandList.count < MAX_COMMANDS) {
                    std::strcpy(commands[i].commandList.commands[commands[i].commandList.count++], command);
                } else {
                    std::cout << "Número máximo de comandos atingido para o comando " << commandName << std::endl;
                }
                found = true;
                break;
            }
        }

        if (!found) {
            std::strcpy(commands[numCommands].name, commandName);
            std::strcpy(commands[numCommands].commandList.commands[commands[numCommands].commandList.count++], command);
            numCommands++;
        }
    }

    std::cout << "Executando o comando 'Z':" << std::endl;
    execute("Z");

    return 0;
}
