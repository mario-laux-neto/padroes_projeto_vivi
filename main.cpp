#include <iostream>
#include <string>
#include <vector>
#include <memory>

// --- PADRÃO DE PROJETO: FACTORY METHOD ---
class Relatorio {
public:
    virtual ~Relatorio() = default;
    virtual void gerar() const = 0;
};

class RelatorioPaciente : public Relatorio {
public:
    void gerar() const override {
        std::cout << "Relatório de Pacientes: Gerando lista de pacientes atendidos no período..." << std::endl;
    }
};

class RelatorioFinanceiro : public Relatorio {
public:
    void gerar() const override {
        std::cout << "Relatório Financeiro: Gerando fluxo de caixa, contas a pagar/receber..." << std::endl;
    }
};

class GeradorRelatorioFactory {
public:
    virtual ~GeradorRelatorioFactory() = default;
    virtual std::unique_ptr<Relatorio> criarRelatorio() const = 0;
};

class GeradorRelatorioPacienteFactory : public GeradorRelatorioFactory {
public:
    std::unique_ptr<Relatorio> criarRelatorio() const override {
        return std::make_unique<RelatorioPaciente>();
    }
};

class GeradorRelatorioFinanceiroFactory : public GeradorRelatorioFactory {
public:
    std::unique_ptr<Relatorio> criarRelatorio() const override {
        return std::make_unique<RelatorioFinanceiro>();
    }
};


// --- PADRÃO DE PROJETO: DECORATOR ---
class Consulta {
public:
    virtual ~Consulta() = default;
    virtual void exibirDetalhes() const = 0;
};

class ConsultaBasica : public Consulta {
public:
    void exibirDetalhes() const override {
        std::cout << "-> Consulta Básica agendada.";
    }
};

class ConsultaDecorator : public Consulta {
protected:
    std::unique_ptr<Consulta> consultaDecorada;

public:
    explicit ConsultaDecorator(std::unique_ptr<Consulta> consulta)
        : consultaDecorada(std::move(consulta)) {}

    void exibirDetalhes() const override {
        if (consultaDecorada) {
            consultaDecorada->exibirDetalhes();
        }
    }
};

class NotificacaoDecorator : public ConsultaDecorator {
public:
    explicit NotificacaoDecorator(std::unique_ptr<Consulta> consulta)
        : ConsultaDecorator(std::move(consulta)) {}

    void exibirDetalhes() const override {
        ConsultaDecorator::exibirDetalhes();
        adicionarNotificacao();
    }

private:
    void adicionarNotificacao() const {
        std::cout << "\n   [DECORATOR] Adicionada notificação por E-mail/WhatsApp.";
    }
};

class ExameDecorator : public ConsultaDecorator {
public:
    explicit ExameDecorator(std::unique_ptr<Consulta> consulta)
        : ConsultaDecorator(std::move(consulta)) {}

    void exibirDetalhes() const override {
        ConsultaDecorator::exibirDetalhes();
        anexarPedidoExame();
    }

private:
    void anexarPedidoExame() const {
        std::cout << "\n   [DECORATOR] Anexado pedido de exame(s).";
    }
};


// --- FUNÇÃO PRINCIPAL COM A DEMONSTRAÇÃO ---
int main() {
    std::cout << "--- DEMONSTRAÇÃO DOS PADRÕES DE PROJETO NO SISTEMA DA CLÍNICA ---" << std::endl;

    // --- Demonstração do Factory Method ---
    std::cout << "\n\n--- 1. Testando o Padrão Factory Method ---\n" << std::endl;

    std::cout << "Administrador solicitou um relatório de pacientes." << std::endl;
    auto factoryPaciente = std::make_unique<GeradorRelatorioPacienteFactory>();
    auto relatorioPacientes = factoryPaciente->criarRelatorio();
    relatorioPacientes->gerar();

    std::cout << std::endl;

    std::cout << "Administrador agora solicitou um relatório financeiro." << std::endl;
    auto factoryFinanceiro = std::make_unique<GeradorRelatorioFinanceiroFactory>();
    auto relatorioFinanceiro = factoryFinanceiro->criarRelatorio();
    relatorioFinanceiro->gerar();
    std::cout << "\nO Factory Method permitiu criar diferentes relatórios usando o mesmo tipo de interface, facilitando a extensibilidade." << std::endl;

    // --- Demonstração do Decorator ---
    std::cout << "\n\n--- 2. Testando o Padrão Decorator ---\n" << std::endl;

    std::cout << "Cenário 1: Agendamento de uma consulta simples." << std::endl;
    auto consultaSimples = std::make_unique<ConsultaBasica>();
    consultaSimples->exibirDetalhes();
    std::cout << "\n" << std::endl;

    std::cout << "Cenário 2: Agendamento de uma consulta com lembrete." << std::endl;
    auto consultaComLembrete = std::make_unique<NotificacaoDecorator>(
        std::make_unique<ConsultaBasica>()
    );
    consultaComLembrete->exibirDetalhes();
    std::cout << "\n" << std::endl;

    std::cout << "Cenário 3: Agendamento de consulta com lembrete e pedido de exame." << std::endl;
    std::unique_ptr<Consulta> consultaCompleta = std::make_unique<ConsultaBasica>();
    consultaCompleta = std::make_unique<NotificacaoDecorator>(std::move(consultaCompleta));
    consultaCompleta = std::make_unique<ExameDecorator>(std::move(consultaCompleta));

    consultaCompleta->exibirDetalhes();
    std::cout << "\n\nO Decorator permitiu adicionar funcionalidades a uma consulta de forma dinâmica e combinada." << std::endl;

    return 0;
}