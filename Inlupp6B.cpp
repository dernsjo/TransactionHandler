#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;


//--------------------------------------------------------
// Globala konstanter:
const int MAX_PERSONER = 10;
const int MAX_TRANSAKTIONER = 30;


//--------------------------------------------------------
// Klassdeklarationer
class Transaktion
 {
 private:
   string  datum;
   string  typ;
   string  namn;
   double  belopp;
   int     antal_kompisar;
   string *kompisar;

  public:
   Transaktion();
   ~Transaktion();
   Transaktion& operator=( const Transaktion& t);
   string hamtaNamn() const;
   double hamtaBelopp() const;
   int    hamtaAntalKompisar() const;
   bool   finnsKompis(const string &namnet) const;
   bool   lasIn(istream &is);
   void   skrivUt(ostream &os) const;
   void   skrivTitel(ostream &os) const;
 }; 

class Person
 {
 private:
   string namn;
   double betalat_andras; // ligger ute med totalt
   double skyldig;        // skyldig totalt

 public:
   Person();
   Person(const string &namn, double betalat_andras, double skyldig);
   string hamtaNamn();
   double hamtaBetalat();
   double hamtaSkyldig();
   void   skrivUt(ostream &os);
};

class PersonLista
 {
 private:
   int    antal_personer;
   Person *personer;

 public:
   PersonLista();
   ~PersonLista();
   PersonLista &operator=(const PersonLista &p_lista);
   void   laggTill(Person ny_person);
   void   skrivUtOchFixa(ostream &os);
   double summaSkyldig();
   double summaBetalat();
   bool   finnsPerson(const string& namn);
 }; 

class TransaktionsLista
 {
 private:
   int         antal_transaktioner;
   Transaktion *transaktioner;

 public:
   TransaktionsLista();
   ~TransaktionsLista();
   TransaktionsLista &operator=(const TransaktionsLista &t_lista);
   void   lasIn(istream &is);
   void   skrivUt(ostream & os);
   void   laggTill(Transaktion & t);
   double totalKostnad();
   double liggerUteMed(const string &namnet);
   double arSkyldig(const string &namnet);
   PersonLista FixaPersoner();
 }; 


int main()
{
  cout << "Startar med att läsa in transaktioner från fil." << endl;
  TransaktionsLista transaktioner; //Skapar en transaktions lista 

  std::ifstream is("resa.txt"); //Öppnar fil
  transaktioner.lasIn(is); //Läser in och lägger till alla transaktioner
  
  int operation = 1;
  while (operation != 0)
    {
      cout << endl;
      cout << "Välj i menyn nedan:" << endl;
      cout << "0. Avsluta. Alla transaktioner sparas på fil." << endl;
      cout << "1. Skriv ut information om alla transaktioner." << endl;
      cout << "2. Läs in en transaktion från tangentbordet." << endl;
      cout << "3. Beräkna totala kostnaden." << endl;
      cout << "4. Hur mycket är en viss person skyldig?" << endl;
      cout << "5. Hur mycket ligger en viss person ute med?" << endl;
      cout << "6. Lista alla personer mm och FIXA" << endl;

      cin >> operation;
      cout << endl;

      switch (operation)
        {
          case 1:
            {
              transaktioner.skrivUt(cout);
              break;
            }
          case 2:
            {
              Transaktion transaktion;
              cout << "Ange transaktion i följande format" << endl;
              transaktion.skrivTitel(cout);
              transaktion.lasIn(cin);
              transaktioner.laggTill(transaktion);
              break;
            }
          case 3:
            {
              cout << "Den totala kostnanden för resan var "
                   << transaktioner.totalKostnad() << endl;
              break;
            }
          case 4:
            {
              cout << "Ange personen: ";
              string namn;
              cin >> namn;
              double ar_skyldig = transaktioner.arSkyldig(namn);
              if (ar_skyldig == 0.)
                cout << "Personen har ingen skuld " << namn << endl;
              else
                cout << namn << " är skyldig " << ar_skyldig << endl;
              break;
            }
          case 5:
            {
              cout << "Ange personen: ";
              string namn;
              cin >> namn;
              double ligger_ute_med = transaktioner.liggerUteMed(namn);
              if (ligger_ute_med == 0.)
                cout << "Kan inte hitta personen " << namn << endl;
              else
                cout << namn << " ligger ute med " << ligger_ute_med << endl;
              break;
            }
          case 6:
            {
              cout << "Nu skapar vi en personarray och reder ut det hela!" << endl;
              PersonLista lista = transaktioner.FixaPersoner();
              lista.skrivUtOchFixa(cout);
              break;
            }
        }
    }

  std::ofstream os("transaktioner.txt");
  transaktioner.skrivUt(os);
//
  return 0;
}

// -------------------------------------------------------
// Klassimplementationen

//Klassen Transaktion
Transaktion::Transaktion(){
  datum = "";
  typ = "";
  namn = "";
  belopp = 0.0;
  antal_kompisar = 0;
  kompisar=0;
}

Transaktion::~Transaktion(){
    delete [] kompisar;
    kompisar=0;
}

Transaktion& Transaktion::operator=(const Transaktion& t)
{
  if (this != &t)
    {
      delete[] kompisar;
      datum          = t.datum;
      typ            = t.typ;
      namn           = t.namn;
      belopp         = t.belopp;
      antal_kompisar = t.antal_kompisar;
      kompisar       = new string[antal_kompisar];
      for (int i=0; i < antal_kompisar; i++)
	    kompisar[i] = t.kompisar[i];
    }
  return *this;
}

string Transaktion::hamtaNamn() const{
  return namn;
}

int Transaktion::hamtaAntalKompisar() const {
  return antal_kompisar;
}

double Transaktion::hamtaBelopp() const {
  return belopp;
}

bool Transaktion::finnsKompis(const string &namnet) const{
  
  for (int i=0; i< antal_kompisar;i++){
    if (kompisar[i]==namnet)
      return true;
  }

  return false;
}

bool Transaktion::lasIn(istream &is){

  is >> datum;
  is >> typ;
  is >> namn;
  is >> belopp;
  is >> antal_kompisar;
  
  delete[] kompisar;
  kompisar = new string[antal_kompisar];


  for (int i=0; i < antal_kompisar; i++){
    is >> kompisar[i];
  }
   
  is.get();   // bort med returtecknet...
  return !is.eof();
}

void Transaktion::skrivTitel(ostream &os) const {
  os << "Datum" << "\t" << "Typ"<<"\t"<<"Person"<<"\t"<<"Belopp"<<"\t"<<"Antal kompisar"<<"\t"<< "Kompisar"<< endl;  
}

void Transaktion::skrivUt(ostream &os) const {


  os << datum <<"\t"<< typ << "\t"<< namn<< "\t" << belopp<< "\t" << antal_kompisar << "\t";
  
  for (int i=0; i<antal_kompisar; i++){
    os << kompisar[i] <<"\t";
  }

  os << endl;
}


//Klassen Person
Person::Person(){
  namn = "";
  betalat_andras = 0.0; // ligger ute med totalt
  skyldig = 0.0;
}

Person::Person(const string &namn, double betalat_andras, double skyldig){
  Person::namn = namn;
  Person::betalat_andras = betalat_andras;
  Person::skyldig = skyldig;
}

string Person::hamtaNamn(){
  return Person::namn;
}

double Person::hamtaBetalat(){
  return betalat_andras;
}

double Person::hamtaSkyldig(){
  return skyldig;
}

//Klassen Personlista
PersonLista::PersonLista(){
  antal_personer = 0;
  personer = 0; 
}

PersonLista::~PersonLista(){
    delete [] personer;
    personer = 0;
}

PersonLista &PersonLista::operator=(const PersonLista &p_lista){
  if (this != &p_lista)
    {
    delete[] personer;
    antal_personer  = p_lista.antal_personer;
    personer        = new Person[antal_personer];

    for (int i = 0; i < antal_personer; i++){
      personer[i] = p_lista.personer[i];
    }
  }
  return *this;
}

void PersonLista::laggTill(Person ny_person){

  Person *ny_lista = 0;
  ny_lista = new Person[antal_personer+1];  
  
  for(int i=0; i<antal_personer; i++){
      ny_lista[i] = personer[i];
  }

  ny_lista[antal_personer]=ny_person;
  antal_personer++;

  delete[] personer;
  personer = ny_lista;

}

double PersonLista::summaSkyldig(){
  double skuld;

  for (int i=0; i < antal_personer; i++){
    skuld += personer[i].hamtaSkyldig();
  }

  return skuld;
}

double PersonLista::summaBetalat(){
  double betalt;

  for (int i=0; i < antal_personer; i++){
    betalt += personer[i].hamtaBetalat();
  }

  return betalt;
}

bool PersonLista::finnsPerson(const string& namn){

  for (int i=0; i < antal_personer; i++){
    if (personer[i].hamtaNamn()==namn){
      return true;
    }
  }

  return false;
}

void PersonLista::skrivUtOchFixa(ostream &os){
  
  double skuld, betalt;

  for (int i=0; i < antal_personer; i++){    
    betalt = personer[i].hamtaBetalat();
    skuld = personer[i].hamtaSkyldig();

    os << personer[i].hamtaNamn() << " ligger ute med " << betalt << " och är skyldig " << skuld <<"."; 

    if (betalt-skuld>=0){
      os << " Skall ha " << betalt-skuld << " från potten." << endl;
    }else{
      os << " Skall lägga " << skuld-betalt << " till potten." << endl; 
    }
  }
}

//Klassen Transaktionslista
TransaktionsLista::TransaktionsLista(){

  antal_transaktioner = 0;
  transaktioner = 0; 
}

TransaktionsLista::~TransaktionsLista(){
    delete[] transaktioner;
    transaktioner = 0;
 }

TransaktionsLista &TransaktionsLista::operator=(const TransaktionsLista &t_lista){
  if(this != &t_lista)
  {
    delete[] transaktioner;
    antal_transaktioner  = t_lista.antal_transaktioner;
    transaktioner        = new Transaktion[antal_transaktioner]; 

    for(int i=0; i< antal_transaktioner; i++){
        transaktioner[i] = t_lista.transaktioner[i];
    }   
  }
  return *this;
}

void TransaktionsLista::laggTill(Transaktion & t){
    
  Transaktion *ny_transaktion = 0;
  ny_transaktion = new Transaktion[antal_transaktioner+1];  
  
  for(int i=0; i<antal_transaktioner; i++){
      ny_transaktion[i] = transaktioner[i];
  }

  ny_transaktion[antal_transaktioner]=t;
  antal_transaktioner++;

  delete[] transaktioner;
  transaktioner = ny_transaktion;
}

double TransaktionsLista::totalKostnad(){
  double konstand;
  for (int i=0; i<antal_transaktioner;i++)
    konstand+=transaktioner[i].hamtaBelopp();

  return konstand;
}

double TransaktionsLista::liggerUteMed(const string &namn){
  double summa = 0.;

  for (int i = 0; i < antal_transaktioner; i++)
    if (transaktioner[i].hamtaNamn() == namn)
      summa += transaktioner[i].hamtaBelopp() *
      (1.0 - 1.0 / (transaktioner[i].hamtaAntalKompisar() + 1));

  return summa;
}

double TransaktionsLista::arSkyldig(const string &namnet){
  
  double summa=0.;

  for(int i=0; i < antal_transaktioner; i++)
    if(transaktioner[i].finnsKompis(namnet))
      summa += transaktioner[i].hamtaBelopp()/(transaktioner[i].hamtaAntalKompisar()+1); 

  return summa;
}

PersonLista TransaktionsLista::FixaPersoner(){
    
  PersonLista lista;    
  string namn=transaktioner[0].hamtaNamn();
   
  lista.laggTill(Person(namn,liggerUteMed(namn),arSkyldig(namn)));
  for(int i=1; i < antal_transaktioner; i++){
    namn = transaktioner[i].hamtaNamn();
        
    if(lista.finnsPerson(namn)==false){
        lista.laggTill(Person(namn,liggerUteMed(namn),arSkyldig(namn)));
        }
    }
    return lista;    
}

void TransaktionsLista::lasIn(istream &is){
  Transaktion t;
  // Så länge det går bra att läsa (filen ej slut)
  while (t.lasIn(is))        
   {
     laggTill(t);
   }
}

void TransaktionsLista::skrivUt(ostream & os){

  transaktioner[0].skrivTitel(os);
  for (int i=0; i < antal_transaktioner; i++){
    transaktioner[i].skrivUt(os);
  }
}