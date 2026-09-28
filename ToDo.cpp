#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

struct Tarefa{
    enum class Prioridade {
        baixa = 1,
        media = 2,
        alta = 3
    };

    int id;
    bool status;
    Prioridade prioridade;
    std::string descricao;
};
std::vector<Tarefa> tarefas;
std::string StatusToString(bool status){
    return status ? "[x]" : "[ ]";
}
void salvar(){
    std::ofstream arquivo("tarefas.txt", std::ios::trunc);
    if(!arquivo.is_open()){
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return;
    }
    for(const auto& tarefa : tarefas){
        arquivo << 
        "ID: " << tarefa.id << "\n" << 
        "Descricao: " << tarefa.descricao << "\n" << 
        "Status: " << StatusToString(tarefa.status) << "\n" << 
        "Prioridade: " << static_cast<int>(tarefa.prioridade) << 
        "\n-------------------------" <<std::endl;
    }
    arquivo.close();
}
void carregar(){
    std::ifstream arquivo("tarefas.txt");
    if(!arquivo.is_open()){
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return;
    }
    Tarefa tarefa;
    std::string linha;
    while(std::getline(arquivo, linha)){
        if(linha.empty() || linha == "-------------------------"){
            continue;
        }
        tarefa.id = std::stoi(linha);
        std::getline(arquivo, tarefa.descricao);
        std::getline(arquivo, linha);
        tarefa.status = (linha == "[x]");
        std::getline(arquivo, linha);
        tarefa.prioridade = static_cast<Tarefa::Prioridade>(std::stoi(linha));
        tarefas.push_back(tarefa);
    }
    arquivo.close();
}
int MarcarStatus(bool status){
    int id;
    std::cout << "Digite o ID da tarefa: ";
    std::cin >> id;
    auto it = std::find_if(tarefas.begin(), tarefas.end(), 
        [id](const Tarefa& tarefa){
        return tarefa.id == id;
        });
    if(it != tarefas.end()){
        it->status = status;
        salvar();
        return 1;
    } else {
        return 0;
    }
}
int CriarID(){
    int id = 1;
    for(const auto& tarefa : tarefas){
        if(tarefa.id >= id){
            id = tarefa.id + 1;
        }
    }
    return id;
}
int LerPropriedade(){
    std::string entrada;
    while(true){
        std::cout << "Prioridade (1-baixa, 2-media, 3-alta): ";
        std::getline(std::cin, entrada);
        try{
            int propriedade = std::stoi(entrada);
            if(propriedade >= 1 && propriedade <= 3){
                return propriedade;
            }
            std::cout << "Entrada invalida. Digite um numero entre 1 e 3: ";
        }
        catch(const std::invalid_argument&){
            std::cout << "Entrada invalida. Digite um numero inteiro: ";
        }
        } 
    }


std::string LerDescricao(){
    std::string descricao;
    std::cout << "Descricao: ";
    std::cin.ignore();
    std::getline(std::cin, descricao);
    return descricao;
}
bool LerStatus(){
    std::string entrada;
    while(true){
        std::cout << "Status (0 ou 1): ";
        std::getline(std::cin, entrada);
        if(entrada == "0"){
            return false;
        } else if(entrada == "1"){
            return true;
        } else {
            std::cout << "Entrada invalida. Digite 0 ou 1." << std::endl;
        }
    }
}

void criar(){
    Tarefa tarefa;
    std::ofstream arquivo("tarefas.txt", std::ios::app);
    if(!arquivo.is_open()){
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return;
    }
    tarefa.id = CriarID();
    tarefa.descricao = LerDescricao();
    tarefa.status = LerStatus();
    int prioridade = LerPropriedade();
    tarefa.prioridade = static_cast<Tarefa::Prioridade>(prioridade);
    tarefas.push_back(tarefa);
    salvar();
    arquivo.close();
}
void adicionar(){
    char opcao;
    do{
        criar();
        std::cout << "Deseja adicionar outra tarefa? (s/n): ";
        std::cin >> opcao;
    } while(opcao == 's' || opcao == 'S');
}

void listar(){
    std::ifstream arquivo("tarefas.txt");
    if (arquivo.is_open()){
        std::string linha;
        while (std::getline(arquivo, linha)){
            std::cout << linha << std::endl;
        }
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }
}
void RemoverPorID(){
    int id;
    std::cout << "Digite o ID da tarefa a ser removida: ";
    std::cin >> id;

    auto it = std::find_if(tarefas.begin(), tarefas.end(), 
        [id](const Tarefa& tarefa){
        return tarefa.id == id;
        });
    if(it != tarefas.end()){
        int id = it - tarefas.begin();
        tarefas.erase(tarefas.begin() + id);
        std::cout << "Tarefa removida com sucesso." << std::endl;
        salvar();
    } else {
        std::cout << "Tarefa nao encontrada." << std::endl;
    }
}

int main(){
    carregar();
    int opcao;
    do{
        std::cout << "1. Adicionar tarefa" << std::endl;
        std::cout << "2. Listar tarefas" << std::endl;
        std::cout << "3. Remover tarefa por ID" << std::endl;
        std::cout << "4. Marcar tarefa como concluida" << std::endl;
        std::cout << "5. Sair" << std::endl;
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;
        switch(opcao){
            case 1:
                adicionar();
                break;
            case 2:
                listar();
                break;
            case 3:
                RemoverPorID();
                break;
            case 4:
                MarcarStatus(true);
                break;
            case 5:
                std::cout << "Saindo..." << std::endl;
                break;
            default:
                std::cout << "Opcao invalida." << std::endl;
        }
    } while(opcao != 5);
    return 0;
}