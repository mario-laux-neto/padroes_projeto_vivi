#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <typeinfo>

class Relatorio {
public:
    virtual ~Relatorio() = default;
    virtual std::string gerar() const = 0;
};

class RelatorioPaciente : public Relatorio {
public:
    std::string gerar() const override {
        return "RelatorioPaciente";
    }
};

class RelatorioFinanceiro : public Relatorio {
public:
    std::string gerar() const override {
        return "RelatorioFinanceiro";
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

class Consulta {
public:
    virtual ~Consulta() = default;
    virtual void executar() = 0;
};

class ConsultaBasica : public Consulta {
public:
    void executar() override {}
};

class ConsultaDecorator : public Consulta {
protected:
    std::unique_ptr<Consulta> consultaDecorada;
public:
    explicit ConsultaDecorator(std::unique_ptr<Consulta> consulta)
        : consultaDecorada(std::move(consulta)) {}
    void executar() override {
        if (consultaDecorada) {
            consultaDecorada->executar();
        }
    }
};

class NotificacaoDecorator : public ConsultaDecorator {
public:
    explicit NotificacaoDecorator(std::unique_ptr<Consulta> consulta)
        : ConsultaDecorator(std::move(consulta)) {}
    void executar() override {
        ConsultaDecorator::executar();
    }
};

class ExameDecorator : public ConsultaDecorator {
public:
    explicit ExameDecorator(std::unique_ptr<Consulta> consulta)
        : ConsultaDecorator(std::move(consulta)) {}
    void executar() override {
        ConsultaDecorator::executar();
    }
};

class MockConsulta : public Consulta {
public:
    bool executadoChamado = false;

    void executar() override {
        executadoChamado = true;
    }
};

void teste_FactoryMethod_CriaRelatorioPacienteCorretamente() {
    auto factory = GeradorRelatorioPacienteFactory();
    auto relatorio = factory.criarRelatorio();
    
    assert(dynamic_cast<RelatorioPaciente*>(relatorio.get()) != nullptr);
    assert(relatorio->gerar() == "RelatorioPaciente");

    std::cout << "PASSOU: teste_FactoryMethod_CriaRelatorioPacienteCorretamente" << std::endl;
}

void teste_FactoryMethod_CriaRelatorioFinanceiroCorretamente() {
    auto factory = GeradorRelatorioFinanceiroFactory();
    auto relatorio = factory.criarRelatorio();
    
    assert(dynamic_cast<RelatorioFinanceiro*>(relatorio.get()) != nullptr);
    assert(relatorio->gerar() == "RelatorioFinanceiro");
    
    std::cout << "PASSOU: teste_FactoryMethod_CriaRelatorioFinanceiroCorretamente" << std::endl;
}

void teste_Decorator_ChamaMetodoDoObjetoEnvelopado() {
    auto mock = std::make_unique<MockConsulta>();
    MockConsulta* mockPtr = mock.get();
    
    auto decorator = NotificacaoDecorator(std::move(mock));
    
    decorator.executar();
    
    assert(mockPtr->executadoChamado == true);
    
    std::cout << "PASSOU: teste_Decorator_ChamaMetodoDoObjetoEnvelopado" << std::endl;
}

void teste_Decorator_EmpilhamentoFuncionaCorretamente() {
    auto mock = std::make_unique<MockConsulta>();
    MockConsulta* mockPtr = mock.get();
    
    std::unique_ptr<Consulta> consultaFinal = std::make_unique<ExameDecorator>(
        std::make_unique<NotificacaoDecorator>(
            std::move(mock)
        )
    );
    
    consultaFinal->executar();
    
    assert(mockPtr->executadoChamado == true);
    
    std::cout << "PASSOU: teste_Decorator_EmpilhamentoFuncionaCorretamente" << std::endl;
}

int main() {
    std::cout << "--- A EXECUTAR TESTES UNITÁRIOS ---\n" << std::endl;

    teste_FactoryMethod_CriaRelatorioPacienteCorretamente();
    teste_FactoryMethod_CriaRelatorioFinanceiroCorretamente();

    std::cout << std::endl;

    teste_Decorator_ChamaMetodoDoObjetoEnvelopado();
    teste_Decorator_EmpilhamentoFuncionaCorretamente();

    std::cout << "\n--- TODOS OS TESTES PASSARAM COM SUCESSO! ---" << std::endl;
    
    return 0;
}