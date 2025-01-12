#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <exception>
using namespace std;

template <typename T>
class CosDeCumparaturi {
private:
    vector<shared_ptr<T>> produse;
    static int nrProd;

public:
    static int aflare_NrProd() { return nrProd; }

    void adaugaProdus(shared_ptr<T> produs) {
        produse.push_back(produs);
        nrProd++;
    }

    void afiseazaCos() const {
        cout << "Produse din cos:" << endl;
        for (const auto& produs : produse) {
            cout << *produs << endl;
        }
    }

    void sortare_dupa_pret() {
        sort(produse.begin(), produse.end(), [](shared_ptr<T> p1, shared_ptr<T> p2) {
            return p1->getPret() < p2->getPret();
        });
    }

    ~CosDeCumparaturi() {
//        for (auto produs : produse) {
//            delete produs;
//        }
    }
};

template <typename T>
int CosDeCumparaturi<T>::nrProd = 0;

class Produs {
protected:
    string nume;
    double pret;
    string brand;
    int gramaj;

public:
    Produs(string& n, double p, string& b, int g) : nume(n), pret(p), brand(b), gramaj(g){ }
    Produs() : nume("null"), pret(0), brand("null"), gramaj(0) { }
    Produs(const Produs& ob) : nume(ob.nume), pret(ob.pret), brand(ob.brand), gramaj(ob.gramaj) { }

    Produs& operator=(const Produs& ob) {
        if (this != &ob) {
            nume = ob.nume;
            pret = ob.pret;
            brand = ob.brand;
            gramaj = ob.gramaj;
        }
        return *this;
    }

    friend istream& operator>>(istream& is, Produs& produs) {
        cout << "Introduceti numele produsului: ";
        is >> produs.nume;
        cout << "Introduceti brandul: ";
        is >> produs.brand;
        cout << "Introduceti pretul: ";
        is >> produs.pret;
        cout << "Introduceti gramajul: ";
        is >> produs.gramaj;
        return is;
    }

    friend ostream& operator<<(ostream& ost, const Produs& p) {
        ost << "_____________________________" << endl
            << "Produs: " << p.nume << endl
            << "_____________________________" << endl
            << "Brand: " << p.brand << endl
            << "Pret: " << p.pret << " lei" << endl
            << "Gramaj: " << p.gramaj << " ml" << endl;
        return ost;
    }

    double getPret() const { return pret; }
    const string& getNume() const { return nume; }
    virtual ~Produs() {}
};

class Stoc {
private:
    vector<shared_ptr<Produs>> produse;

    Stoc() = default;

public:
    Stoc(const Stoc&) = delete;
    Stoc& operator=(const Stoc&) = delete;

    static Stoc& getInstanta() {
        static Stoc instanta;
        return instanta;
    }

    void adaugaProdus(shared_ptr<Produs> const &produs) {
        produse.push_back(produs);
    }

    void afiseazaStoc() const {
        cout << "Produse disponibile in stoc:" << endl;
        for (const auto& produs : produse) {
            cout << *produs << endl;
        }
    }
};

class Parfum : public Produs {
private:
    string note;
public:
    Parfum(string& n, double p, string& b, int g, string& nota) : Produs(n, p, b, g), note(nota) { }
    friend ostream& operator<<(ostream& ost, const Parfum& p) {
        ost << static_cast<const Produs&>(p);
        ost << "Note: " << p.note << endl;
        return ost;
    }
};

class Machiaj : public Produs {
private:
    string tip;
    string culoare;
public:
    Machiaj(string& n, double p, string b, int g, string& t, string& c) : Produs(n, p, b, g), tip(t), culoare(c) { }
    friend ostream& operator<<(ostream& ost, const Machiaj& m) {
        ost << static_cast<const Produs&>(m);
        ost << "Tip: " << m.tip << endl
            << "Culoare: " << m.culoare << endl;
        return ost;
    }
};

class Crema : public Produs {
private:
    string TipPiele;
public:
    Crema(string& n, double p, string& b, int g, string& tp) : Produs(n, p, b, g), TipPiele(tp) { }
    friend ostream& operator<<(ostream& ost, const Crema& c) {
        ost << static_cast<const Produs&>(c);
        ost << "Tipul de piele: " << c.TipPiele << endl;
        return ost;
    }
};

class ProdusBuilder {
private:
    string nume;
    double pret;
    string brand;
    int gramaj;
    string X; // note parfum sau tip machiaj
    string Y; // culoare machiaj sau TipPiele

public:
    ProdusBuilder()
            : nume("null"), pret(0.0), brand("null"), gramaj(0), X("null"), Y("null") {}
    ProdusBuilder& setNume (const string& n) {
        nume = n;
        return *this;
    }
    ProdusBuilder& setPret(double p) {
        pret = p;
        return *this;
    }
    ProdusBuilder& setBrand(const string& b) {
        brand = b;
        return *this;
    }
    ProdusBuilder& setGramaj(int g) {
        gramaj = g;
        return  *this;
    }
    ProdusBuilder& setX(const string& x) {
        X = x;
        return *this;
    }
    ProdusBuilder& setY(const string& y) {
        Y = y;
        return *this;
    }

    Parfum* buildParfum() {
        return new Parfum(nume, pret, brand, gramaj, X);
    }
    Machiaj* buildMachiaj() {
        return new Machiaj(nume, pret, brand, gramaj, X, Y);
    }
    Crema* buildCrema() {
        return new Crema(nume, pret, brand, gramaj, X);
    }
};

class Utilizator {
protected:
    string nume;
public:
    explicit Utilizator(const string& n) : nume(n) { }
    virtual void descriere() const = 0;
    const string& getNume() const { return nume; }
    virtual ~Utilizator() {}
};

class Client : public Utilizator {
private:
    CosDeCumparaturi<Produs> cosCumparaturi;

public:
    explicit Client(const string& n) : Utilizator(n) { }

    void afisare_cos() const {
        cout << "Cosul utilizatorului " << nume << " contine: " << endl
             << "Numar de produse in cos: " << CosDeCumparaturi<Produs>::aflare_NrProd() << endl
             << "----------------------------------------------" << endl;
        cosCumparaturi.afiseazaCos();
    }


    void adaugaInCos(shared_ptr<Produs> produs) {
        cosCumparaturi.adaugaProdus(produs);
    }

    void descriere() const override {
        cout << "Clientul: " << nume << endl;
    }

    void sortare_cos() {
        cosCumparaturi.sortare_dupa_pret();
    }
};

class Administrator : public Utilizator {
public:
    explicit Administrator(const string& n) : Utilizator(n) { }
    void descriere() const override {
        cout << "Administratorul: " << nume << endl;
    }

    void Gestionare_Stoc() const {
        cout << nume << " gestioneaza stocul magazinului." << endl;
    }
};

class Comanda {
private:
    shared_ptr<Client> client;
    vector<shared_ptr<Produs>> produse;

public:
    explicit Comanda(shared_ptr<Client> c) : client(c) {}

    void adaugaProdus(shared_ptr<Produs> produs) {
        produse.push_back(produs);
    }

    void finalizeazaComanda() {
        cout << "---------------------------------------------------" << endl
             << "Comanda clientului " << client->getNume() << " a fost procesata." << endl
             << "---------------------------------------------------" << endl;
        double total = 0;
        for (const auto& produs : produse) {
            cout << *produs << endl;
            total += produs->getPret();
        }

        cout << "------------------------" << endl
             << "Total de plata: " << total << " lei." << endl
             << "------------------------" << endl;
    }
};

int main() {
    auto& stoc = Stoc::getInstanta();
    ProdusBuilder builder;

    Produs produs1;
    cin >> produs1;

    cout << "Produsul introdus este:" << endl;
    cout << produs1 << endl;

    auto parfum =shared_ptr<Parfum>(builder.setNume("Good Girl")
                                            .setPret(400.0)
                                            .setBrand("Carolina Herrera")
                                            .setGramaj(80)
                                            .setX("Floral-Fructat")
                                            .buildParfum());

    auto ruj = shared_ptr<Machiaj>(builder.setNume("Ruj satinat")
                                           .setPret(65.0)
                                           .setBrand("L'oreal")
                                           .setGramaj(10)
                                           .setX("Ruj")
                                           .setY("Rosu")
                                           .buildMachiaj());

    auto crema = shared_ptr<Crema>(builder.setNume("Crema hidratanta")
                                           .setPret(125.0)
                                           .setBrand("Lancome")
                                           .setGramaj(200)
                                           .setX("Piele uscata")
                                           .buildCrema());

//    stoc.adaugaProdus(shared_ptr<Produs>(parfum));
//    stoc.adaugaProdus(shared_ptr<Produs>(ruj));
//    stoc.adaugaProdus(shared_ptr<Produs>(crema));

    stoc.adaugaProdus(parfum);
    stoc.adaugaProdus(ruj);
    stoc.adaugaProdus(crema);

    stoc.afiseazaStoc();

    auto client = make_shared<Client>("Maria Popescu");
    client->adaugaInCos(parfum);
    client->adaugaInCos(ruj);
    client->adaugaInCos(crema);

    // Afișarea coșului înainte de sortare
    cout << "Cosul inainte de sortare:" << endl;
    client->afisare_cos();

    // Sortarea coșului și afișarea după sortare
    client->sortare_cos();
    cout << "Cosul dupa sortare:" << endl;
    client->afisare_cos();

    auto comanda = make_shared<Comanda>(client);
    comanda->adaugaProdus(parfum);
    comanda->adaugaProdus(ruj);
    comanda->adaugaProdus(crema);
    comanda->finalizeazaComanda();

    Administrator* admin = new Administrator("Ion Ionescu");
    admin->descriere();
    admin->Gestionare_Stoc();

//    delete client;
//    delete comanda;
    delete admin;
//
//    delete parfum;
//    delete ruj;
//    delete crema;

    return 0;
}
