#pragma warning(disable : 4996)
#include <iostream>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <tuple>
#include "InfInt.h"
#include <boost/multiprecision/cpp_int.hpp>
class RSA {
public:
    void generarClaves() {
        limpiarValores();
        srand(time(nullptr));
        p = generarPrimo();
        q = generarPrimo();
        n = p * q;
        phi = (p - 1) * (q - 1);
        e = generarE();
        d = get<1>(euclidesExtendido(e, phi));
        while (d < 0) d += phi;
    }
    void mostrarClaves() {
        std::cout << "Módulo (n): " << n << "\n";
        std::cout << "Clave pública (e, n): (" << e << ", " << n << ")\n";
        std::cout << "Clave privada (d, n): (" << d << ", " << n << ")\n";
        std::cout << "El mensaje debe estar en el rango [0, " << n - 1 << "]\n";
    }
    std::vector<InfInt> encriptar(std::string _mensaje) {
        mensajeOriginal = _mensaje;
        std::cout << "Se encriptará: " + _mensaje + "\n";
        for (auto letra : _mensaje) {
            mensajeEncriptado.push_back(mod_pow((int)letra, e, n));
        }
        return mensajeEncriptado;
    }
    std::string desencriptar(std::vector<InfInt> _mensajeEncriptado) {
        std::cout << "Desencriptando...\nValores Desencriptados:";
        mensajeDesencriptado = "";
        std::cout << "[ ";
        std::vector<InfInt> mensajeNumDesencriptado;
        for (auto letra : _mensajeEncriptado) {
            auto letra_c = mod_pow(letra, d, n);
            std::cout << letra_c << " ";
            mensajeNumDesencriptado.push_back(letra_c);
            mensajeDesencriptado += (char)letra_c.toInt();
        }
        std::cout << "]\n";
        return mensajeDesencriptado;
    }
    void mostrarMensajeEncriptado() {
        std::cout << "[ ";
        for (auto letra : mensajeEncriptado) {
            std::cout << letra << " ";
        }
        std::cout << "]\n";
    }

    void mostrarMensajeDesencriptado() {
        if (mensajeOriginal != mensajeDesencriptado) {
            std::cout << "Ocurrió un error al desencriptar.\n";
            return;
        }
        std::cout << mensajeDesencriptado << "\n";
    }

private:
    InfInt p, q, n, phi, e, d;
    std::vector<InfInt> mensajeEncriptado;
    std::string mensajeDesencriptado, mensajeOriginal;
    bool esPrimo(InfInt _numero) {
        if (_numero == 1) return false;
        for (int i = 2; i <= sqrt(_numero.toInt()); i++) {
            if (_numero % i == 0) return false;
        }
        return true;
    }
    InfInt generarPrimo() {
        InfInt numero;
        do {
            numero = rand() % 100;
        } while (!esPrimo(numero));
        return numero;
    }
    InfInt generarE() {
        InfInt e;
        while (phi.toInt() <= 2) {
            p = generarPrimo();
            q = generarPrimo();
            n = p * q;
            phi = (p - 1) * (q - 1);
        }
        do {
            e = rand() % (phi.toInt() - 2) + 2;
        } while (mcd(e, phi) != 1);
        return e;
    }
    InfInt mcd(InfInt a, InfInt b) {
        if (b == 0) return a;
        return mcd(b, a % b);
    }
    std::tuple< InfInt, InfInt, InfInt> euclidesExtendido(InfInt a, InfInt b) {
        if (a == 0) return std::make_tuple(b, 0, 1);
        InfInt mcd, x, y;
        std::tie(mcd, x, y) = euclidesExtendido(b % a, a);
        return std::make_tuple(mcd, y - (b / a) * x, x);
    }
    InfInt mod_pow(InfInt _base, InfInt _exponente, InfInt _modulo) {
        InfInt resultado = 1;
        while (--_exponente >= 0) {
            resultado *= _base;
            resultado %= _modulo;
        }
        return resultado;
    }
    void limpiarValores() {
        mensajeEncriptado.clear();
		mensajeDesencriptado = "";
        mensajeOriginal = "";
    }
};

int main() {
    RSA rsa;
    std::string mensaje;
    do {
        rsa.generarClaves();
        rsa.mostrarClaves();
        std::cout << "Si desea salir, escriba solamente espacio\n";
        std::cout << "Ingrese el mensaje a encriptar: ";
        std::getline(std::cin, mensaje);
        if (mensaje == " ") break;
        std::vector<InfInt> mensajeEncriptado = rsa.encriptar(mensaje);
        std::cout << "Mensaje encriptado: ";
        rsa.mostrarMensajeEncriptado();
        std::string mensajeDesencriptado = rsa.desencriptar(mensajeEncriptado);
        std::cout << "Mensaje desencriptado: ";
        rsa.mostrarMensajeDesencriptado();
        getch();
        system("cls");
    } while (true);
    std::cout << "Gracias por usar el programa.\n";
    return 0;
}