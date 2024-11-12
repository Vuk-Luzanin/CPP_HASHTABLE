// i = 3            ->    dvostruko hesiranje
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

class AddressFunction;
struct Field {
    int address{};
    string word;
};

class HashTable {
public:
    HashTable(int m, AddressFunction* af);
    ~HashTable();
    string findKey(int k);
    bool insertKey(int k, string s);
    bool deleteKey(int k);
    double avgAccessSuccess();
    double avgAccessUnsuccess();
    void resetStatistics();
    int keyCount();
    int tableSize() {return m;};
    friend ostream& operator<<(ostream& os, HashTable& h);
    double fillRatio() { return keyCount()*1.0 / m; };
    int findNearestPrimeNumber();
    int getMod() { return mod; }
    void evaluationOfPerformances(int iter);
    void clear();

private:
    int m;
    int mod;
    int EveryUnsuccessfulAccess = 0;
    int AllUnsuccessfulKeys = 0;
    vector<Field*> table;
    AddressFunction* addressFunction;                             //nullptr - slobodno za umetanje
    bool isPrime(int n);                                          //ima pokazivac na Field, ali je key = 0 - obrisano - zauzeto je samo zbog ispitnog niza
    void expanseTable();
    void smallerTable();
};

class AddressFunction {
public:
    virtual int getAddress(int k, int a, int i) = 0;
};

class DoubleHashing: public AddressFunction{
public:
    int m;
    DoubleHashing(int m): m(m) {}
    int getAddress(int k, int a, int i) override;
};


int DoubleHashing::getAddress(int k, int a, int i) {
    return (a + i * (1 + (k % (m-2))));
}

HashTable::HashTable(int m, AddressFunction* af): m(m), addressFunction(af){
    vector<Field*> table(m);
    this->table = table;
}

HashTable::~HashTable() {
    clear();
    table.clear();
}

string HashTable::findKey(int k) {
    mod = findNearestPrimeNumber();
    int index = k % mod;
    if (table[index] == nullptr) {
        EveryUnsuccessfulAccess++;
        AllUnsuccessfulKeys++;
        return "";
    }
    if (table[index]->address == k) {
        return table[index]->word;
    }
    int i = 1;              //broj pokusaja po redu
    while (table[index] != nullptr && i < m) {
        index = (addressFunction->getAddress(k, index, i)) % m;
        i++;
        EveryUnsuccessfulAccess++;
        if (table[index] == nullptr) {
            AllUnsuccessfulKeys++;
            return "";
        }
        if (table[index]->address == k) {
            return table[index]->word;
        }
    }
    AllUnsuccessfulKeys++;
    return "";
}

bool HashTable::insertKey(int k, string s) {
    if (fillRatio() > 0.75 || avgAccessSuccess() > 3) {
        expanseTable();
    }
    mod = findNearestPrimeNumber();
    int index = k % mod;
    if (table[index] == nullptr) {
        Field* f = new Field();
        f->address = k;
        f->word = s;
        table[index] = f;
        return true;
    }
    if (table[index]->address == 0) {
        table[index]->address = k;
        table[index]->word = s;
        return true;
    }
    if (table[index]->address == k) {
        cout << endl << "Zadati ključ već postoji u tabeli." << endl << endl;
        return false;
    }
    int i = 1;
    while (table[index] != nullptr && i < m) {
        index = (addressFunction->getAddress(k, index, i)) % m;
        i++;
        if (table[index] == nullptr) {
            Field* f = new Field();
            f->address = k;
            f->word = s;
            table[index] = f;
            return true;
        }
        if (table[index]->address == 0) {
            table[index]->address = k;
            table[index]->word = s;
            return true;
        }
        if (table[index]->address == k) {
            cout << "Zadati ključ već postoji u tabeli." << endl;
            return false;
        }
    }
    return false;
}

bool HashTable::deleteKey(int k) {
    if (fillRatio() < 0.35) {
        smallerTable();
    }
    mod = findNearestPrimeNumber();
    int index = k % mod;
    if (table[index] == nullptr) {
        return false;
    }
    if (table[index]->address == k) {
        table[index]->address = 0;
        return true;
    }
    int i = 1;                          //broj pokusaja po redu
    while (table[index] != nullptr && i < m) {
        index = (addressFunction->getAddress(k, index, i)) % m;
        i++;
        if (table[index] == nullptr) {
            return false;
        }
        if (table[index]->address == k) {
            table[index]->address = 0;
            return true;
        }
    }
    return false;
}

double HashTable::avgAccessSuccess() {
    int everyAccess = 0;
    for (int j = 0; j<m; j++) {
        if (table[j] == nullptr) continue;
        if (table[j]->address == 0) continue;
        int k = table[j]->address;

        mod = findNearestPrimeNumber();
        int index = k % mod;
        if (table[index] == nullptr) {
            continue;
        }
        if (table[index]->address == k) {
            everyAccess++;
            continue;
        }
        int i = 1;                          //broj pokusaja po redu
        while (table[index] != nullptr && i < m) {
            index = (addressFunction->getAddress(k, index, i)) % m;
            i++;
            everyAccess++;
            if (table[index] == nullptr) {
                continue;
            }
            if (table[index]->address == k) {
                everyAccess++;
                break;
            }
        }
    }
    return static_cast<double>(everyAccess) / keyCount();
}

double HashTable::avgAccessUnsuccess() {
    return static_cast<double>(EveryUnsuccessfulAccess) / AllUnsuccessfulKeys;
}

void HashTable::resetStatistics() {
    EveryUnsuccessfulAccess = 0;
    AllUnsuccessfulKeys = 0;
}

void HashTable::clear() {
    for (int i = 0; i<m; i++) {
        delete table[i];
    }
}

int HashTable::keyCount() {
    int count = 0;
    for (int i = 0; i<m; i++) {
        if (table[i] != nullptr) {
            if (table[i]->address != 0) {
                count++;
            }
        }
    }
    return count;
}

ostream &operator<<(ostream &os, HashTable& h) {
    for (int i = 0; i<h.m; i++) {
        if (h.table[i] != nullptr) {
            if (h.table[i]->address == 0) {
                os << "EMPTY" << endl;
                continue;
            }
            os << "Key: " << h.table[i]->address << ", Word: " << h.table[i]->word << endl;
        } else {
            os << "EMPTY" << endl;
        }
    }
    return os;
}

bool HashTable::isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int HashTable::findNearestPrimeNumber() {
    for (int i = m - 1; i >= 2; --i) {
        if (isPrime(i)) {
            return i;
        }
    }
    return -1;
}

void HashTable::evaluationOfPerformances(int iter) {
    ifstream inputFile("/Users/vukluzanin/Downloads/ASP2 treci domaci/DZ3_Recnik_10K.txt");
    if (!inputFile.is_open()) {
        cerr << "Neispravno otvorena datoteka." << endl;
        exit(1);
    }
    string line;
    int min = 10000000, max = 0;
    int numberOfKeys = 0;
    while (getline(inputFile, line) && numberOfKeys != iter) {
        istringstream iss(line);
        int intValue;
        string stringValue;
        string tmp;
        getline(iss, stringValue, '\t');
        getline(iss, tmp, ' ');
        intValue = stoi(tmp);
        if (min > intValue) min = intValue;
        if (max < intValue) max = intValue;
        insertKey(intValue, stringValue);
        numberOfKeys++;
    }
    inputFile.close();

    //srand(time(nullptr));
    for (int i = 0; i < 10*iter; ++i) {
        int randomNumber = rand() % max + min;                  // Generiše random broj između min i max
        findKey(randomNumber);
    }
    cout << "Prosečan broj pristupa tabeli prilikom uspešnog traženja ključeva: " << avgAccessSuccess() << endl << endl;
    cout << "Prosečan broj pristupa tabeli prilikom neuspešnog traženja ključeva: " << avgAccessUnsuccess() << endl << endl;
}

void HashTable::expanseTable() {
    vector<Field*> tableTmp(m);
    for (int i=0; i<m; i++) {
        if (table[i] == nullptr) continue;
        if (table[i]->address == 0) continue;
        tableTmp[i] = new Field(*table[i]);
    }
    int prev = m;
    m *= 2;
    mod = findNearestPrimeNumber();
    table.resize(m);
    for (int i=0; i<m; i++) {
        table[i] = nullptr;
    }
    for (int i=0; i<prev; i++) {
        if (tableTmp[i] == nullptr) continue;
        if (tableTmp[i]->address == 0) continue;
        insertKey(tableTmp[i]->address, tableTmp[i]->word);
    }
    tableTmp.clear();
}

void HashTable::smallerTable() {
    vector<Field*> tableTmp(m);
    for (int i=0; i<m; i++) {
        if (table[i] == nullptr) continue;
        if (table[i]->address == 0) continue;
        tableTmp[i] = new Field(*table[i]);
    }
    int prev = m;
    m /= 2;
    mod = findNearestPrimeNumber();
    table.resize(m);
    for (int i=0; i<m; i++) {
        table[i] = nullptr;
    }
    for (int i=0; i<prev; i++) {
        if (tableTmp[i] == nullptr) continue;
        if (tableTmp[i]->address == 0) continue;
        insertKey(tableTmp[i]->address, tableTmp[i]->word);
    }
    tableTmp.clear();
}


int main() {
    cout << "TRECI DOMACI ZADATAK IZ ALGORITAMA I STRUKTURA PODATAKA 2:" << endl << endl;
    cout << "Unesite inicijalnu velicinu tabele: " << endl;
    int size;
    cin >> size;
    DoubleHashing doubleHashing(size);
    HashTable h(size, &doubleHashing);
    cout << "Izaberite stavku:" << endl << endl;
    int choice = 0, k;
    string tmpS;
    do {
        cout << endl << "Navođenjem broja stavke možete izabrati operaciju." << endl;
        cout << "0. Prekid programa." << endl;
        cout << "1. Proveri da li se kljuc nalazi u tabeli." << endl;
        cout << "2. Umetanje kljuca i sadrzaja." << endl;
        cout << "3. Brisanje kljuca." << endl;
        cout << "4. Prosečan broj pristupa tabeli prilikom uspešnog traženja ključeva." << endl;
        cout << "5. Prosečan broj pristupa tabeli prilikom neuspešnog traženja ključeva." << endl;
        cout << "6. Reset statistike." << endl;
        cout << "7. Isprazni tabelu." << endl;
        cout << "8. Izracunaj broj kljuceva." << endl;
        cout << "9. Izracunaj velicinu tabele." << endl;
        cout << "10. Ispisi tabelu." << endl;
        cout << "11. Izracunaj popunjenost." << endl;
        cout << "12. Evaluacija performansi." << endl;
        cin >> choice;
        switch (choice) {
            case 1: {
                cout << "Unesite kljuc koji zelite da proverite: " << endl;
                cin >> k;
                if (h.findKey(k) == "") {
                    cout << "Kljuc nije u tabeli." << endl;
                } else {
                    cout << "Kljuc jeste u tabeli." << endl;
                    cout << "Sadrzaj je: " << h.findKey(k) << endl;
                }
                break;
            } case 2: {
                cout << "Unesite kljuc koji zelite da umetnete: " << endl;
                cin >> k;
                cout << "Unesite rec koju zelite da umetnete: " << endl;
                cin >> tmpS;
                h.insertKey(k, tmpS);

                break;
            } case 3: {
                cout << "Unesite kljuc koji zelite da obrisete: " << endl;
                cin >> k;
                if (h.deleteKey(k)) {
                    cout << "Kljuc je obrisan." << endl;
                } else {
                    cout << "Kljuc nije obrisan." << endl;
                }

                break;
            } case 4: {
                cout << "Prosečan broj pristupa tabeli prilikom uspešnog traženja ključeva: " << h.avgAccessSuccess() << endl << endl;

                break;
            } case 5: {
                cout << "Prosečan broj pristupa tabeli prilikom neuspešnog traženja ključeva: " << h.avgAccessUnsuccess() << endl << endl;

            } break;
            case 6: {
                h.resetStatistics();

            } break;
            case 7: {
                h.clear();

            } break;
            case 8: {
                cout << "Broj zauzetih kljuceva je: "<< h.keyCount() << endl;

            } break;
            case 9: {
                cout << "Velicina tabele je: "<< h.tableSize() << endl;

            } break;
            case 10: {
                cout << "Izgled tabele:"<< endl<< h << endl;

            } break;
            case 11: {
                cout << endl << "Fill Ratio is: " << h.fillRatio() << endl;

            } break;
            case 12: {
                int iter;
                cout << "Unesite broj ključeva koji želite da ubacite u tabelu: " << endl;
                cin >> iter;
                h.evaluationOfPerformances(iter);

            } break;
            case 0: cout << "Kraj programa."; break;
            default:
                cout << "Nekorektna opcija. Molimo vas da unesete ponovo." << endl;
        }
    }while (choice);

    /*h.insertKey(2, "aaa");
    h.insertKey(5, "bbb");
    h.insertKey(12, "ccc");
    h.insertKey(13, "ddd");
    h.insertKey(14, "eee");
    h.insertKey(15, "fff");
    h.insertKey(17, "ggg");
    h.insertKey(18, "hhh");
    h.findKey(12);
    h.findKey(86);
    h.findKey(14);
    h.findKey(15);
    h.findKey(24);
    h.findKey(17);*/
    return 0;
}
