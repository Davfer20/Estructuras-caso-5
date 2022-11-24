#include <iostream>
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include "json.hpp"
#include "Registered.h"
#include <algorithm>
#include <sstream>
#include <iterator>
#include <bits/stdc++.h>
#include "../arbolBpoderoso/Arbol_B+.h"
#include "../match/Person.h"
#include "../graph/grafo.h"
#include "../CircularTrade/CircularTradeB.h"
#include "../TopTrades/BarTradesB.h"
#include "../RelatedMarket/RelatedMarketB.h"
#include "../ValueChain/ValueChainB.h"

#define USERS_IN_BP 7

using namespace std;
using json = nlohmann::json;

#define CHUNK_SIZE 2048

string createRegistroJsonTemplate = R"(
                    {
                        "fields" : {
                            "nickname" : {
                                "en-US" : ""
                            },
                            "postdate" : {
                                "en-US" : ""
                            },
                            "password" : {
                                "en-US" : ""
                            },
                            "offer" : {
                                "en-US" : ""
                            },
                            "demand" : {
                                "en-US" : ""
                            }
                        }
                    }
                )";

typedef struct
{
    unsigned char *buffer;
    size_t len;
    size_t buflen;
} get_request;

// static method to act as callback for curl
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t realsize = size * nmemb;
    get_request *req = (get_request *)userdata;

    printf("receive chunk of %zu bytes\n", realsize);

    while (req->buflen < req->len + realsize + 1)
    {
        req->buffer = (unsigned char *)realloc(req->buffer, req->buflen + CHUNK_SIZE);
        req->buflen += CHUNK_SIZE;
    }
    memcpy(&req->buffer[req->len], ptr, realsize);
    req->len += realsize;
    req->buffer[req->len] = 0;

    return realsize;
};

class Contenful
{
private:
    CURL *curl;
    CURLcode res;

    void publishEntry(string pEntryId)
    {
        // init structs and objects
        struct curl_slist *headers = NULL;

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        // add contenful headers to curl object, this token is fixed
        // asumming the update of the version 1 only
        headers = curl_slist_append(headers, "Authorization: Bearer CFPAT-C3IxS--BahDuak8g39hA9uhJgNJhXtiBUMYBzI78zvg");
        headers = curl_slist_append(headers, "X-Contentful-Version: 1");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // build the URL with the proper entryid
        string url = "https://api.contentful.com/spaces/9yr18gr54ppj/environments/master/entries/" + pEntryId + "/published";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // clean the curl objects
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    };

    string createEntry(string pNickname, string pOffer, string pDemand, string pPostdate, string pPassword)
    {
        string entryId;

        /* In windows, this will init the winsock stuff */
        curl_global_init(CURL_GLOBAL_ALL);

        /* get a curl handle */
        curl = curl_easy_init();

        if (curl)
        {
            // init the structs
            struct curl_slist *headers = NULL;
            get_request req = {.buffer = NULL, .len = 0, .buflen = 0};

            // prepare the payload for the post request
            json requestJson = json::parse(createRegistroJsonTemplate);
            requestJson["fields"]["nickname"]["en-US"] = pNickname;
            requestJson["fields"]["postdate"]["en-US"] = pPostdate;
            requestJson["fields"]["password"]["en-US"] = pPassword;
            requestJson["fields"]["offer"]["en-US"] = pOffer;
            requestJson["fields"]["demand"]["en-US"] = pDemand;

            string jsontext = to_string(requestJson);

            // set the headers, the token and content type is fixed
            headers = curl_slist_append(headers, "Authorization: Bearer CFPAT-C3IxS--BahDuak8g39hA9uhJgNJhXtiBUMYBzI78zvg");
            headers = curl_slist_append(headers, "Content-Type: application/vnd.contentful.management.v1+json");
            headers = curl_slist_append(headers, "X-Contentful-Content-Type: registros");

            /* set our custom set of headers */
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // set the url, space and environment is fixed
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.contentful.com/spaces/9yr18gr54ppj/environments/master/entries");
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

            /* Now specify the POST data */
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsontext.c_str());

            // prepare the callback function to catch the result json
            req.buffer = (unsigned char *)malloc(CHUNK_SIZE);
            req.buflen = CHUNK_SIZE;

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&req);

            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);

            /* Check for errors */
            if (res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

                // clean and free the curl object
                curl_easy_cleanup(curl);
                curl_global_cleanup();
            }
            else
            {
                // printf("Total received bytes: %zu\n", req.len);
                // printf("Received data:/n%s\n", req.buffer);
                json responseData = json::parse(req.buffer);
                entryId = responseData["sys"]["id"].get<string>();

                free(req.buffer);

                curl_easy_cleanup(curl);
                curl_global_cleanup();
            }
        }

        return entryId;
    };

public:
    vector<Registered *> getRecords()
    {

        vector<Registered *> result;
        get_request req = {.buffer = NULL, .len = 0, .buflen = 0};

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        // build the URL with the proper entryid
        string url = "https://cdn.contentful.com/spaces/9yr18gr54ppj/environments/master/entries?select=fields&access_token=nekO8xKQJ6Ftx9d4JaIcCf3dBuNQbJMJRHRG_Ekd79s&content_type=registros";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        // prepare the callback function to catch the result json
        req.buffer = (unsigned char *)malloc(CHUNK_SIZE);
        req.buflen = CHUNK_SIZE;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&req);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res == CURLE_OK)
        {
            json responseData = json::parse(req.buffer);
            for (auto &item : responseData["items"])
            {
                Registered *newItem = new Registered(item["fields"]["nickname"].get<string>(),
                                                     item["fields"]["offer"].get<string>(),
                                                     item["fields"]["demand"].get<string>(),
                                                     item["fields"]["postdate"].get<string>());

                result.push_back(newItem);
            }

            free(req.buffer);
        }
        else
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // clean the curl objects
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return result;
    };

    void registerUser(string pNickname, string pOffer, string pDemand, string pPassword, int pDay, int pMonth, int pYear)
    {

        string postDate(((pMonth / 10) > 0 ? to_string(pMonth) : "0" + to_string(pMonth)) + "/" + ((pDay / 10) > 0 ? to_string(pDay) : "0" + to_string(pDay)) + "/" + to_string(pYear));
        string entryId = createEntry(pNickname, pOffer, pDemand, postDate, pPassword);
        publishEntry(entryId);
    }
};

struct Product
{
    int reps;
    string word;
};

Contenful regs;
vector<Registered *> allrecords = regs.getRecords();
Grafo GobizNetwork(false);
vector<string> matchesVector;
vector<Product> products;

vector<string> filterText(string pText)
{

    transform(pText.begin(), pText.end(), pText.begin(), ::tolower);

    stringstream ss(pText);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> keyWords(begin, end);

    vector<string> keyWords2;
    for (auto &word : keyWords)
    {
        if (!(word.length() < 4) && word != "para")
        {
            keyWords2.push_back(word.substr(0, 4));
        }
    }

    return keyWords2;
}

void addData(Registered *pUser, string text, BTree *pTree)
{
    vector<string> keyWords = filterText(text);
    for (auto &word : keyWords)
    {
        pTree->insert(new Person(pUser, word));
    }
}

void addToGraph(Registered *data)
{

    try
    {
        GobizNetwork.getNodo(data->getId());
    }
    catch (const std::exception &e)
    {
        GobizNetwork.addNode(data);
    }
}

void addProduct(string pWord)
{
    bool founded = false;
    for (Product &s : products)
    {
        if (s.word == pWord)
        {
            s.reps += 1;
            founded = true;
            break;
        }
    }
    if (!founded)
    {
        Product newProduct;
        newProduct.reps = 1;
        newProduct.word = pWord;
        products.push_back(newProduct);
    }
}

void filterRegs(string search, Registered *mainUser)
{
    mainUser->setMathLevel(0);
    BTree *tree = new BTree(6);
    addToGraph(mainUser);

    vector<string> keyWords = filterText(search == "offer" ? mainUser->getOffer() : mainUser->getDemand());
    for (auto &word : keyWords)
    {
        Person *mainPerson = new Person(word, mainUser);
        tree->insert(mainPerson);
    }

    int count = USERS_IN_BP;
    for (auto &otherUser : allrecords)
    {
        otherUser->setMathLevel(0);
        if (otherUser->getNickname() == mainUser->getNickname())
        {
            continue;
        }

        if (search == "demand" && otherUser->getOffer() != "")
        {
            addData(otherUser, otherUser->getOffer(), tree);
            count--;
        }
        else if (otherUser->getDemand() != "")
        {
            addData(otherUser, otherUser->getDemand(), tree);
            count--;
        }
        if (!count)
        {
            break;
        }
    }

    vector<Comparable *> vec = tree->getConjuntoS();
    vector<Person *> vec3;

    for (int i = 0; i < vec.size(); i++)
    {

        Person *user = dynamic_cast<Person *>(vec[i]);

        if (user->getUser() != NULL || user->getNickname() == "!")
        {
            vec3.push_back(user);
        }
    };

    string actualKeyWord;
    for (auto &persona : vec3)
    {
        if (persona->getNickname() == "!")
        {
            actualKeyWord = persona->getKeyWord();
        }
        else if (persona->getKeyWord() == actualKeyWord)
        {
            addProduct(actualKeyWord);
            persona->incCompatibility();
            // cout << persona->getKeyWord() << " - " << persona->getNickname() << endl;
        }
    }

    // for (auto &item : vec3)
    //{
    //     cout << item->getNickname() << "; ";
    // }
    // cout << endl;

    vector<Person *> vec3Aux;
    while (!vec3.empty())
    {
        Person *A = vec3.back();
        vec3.pop_back();
        if (A->getNickname() != mainUser->getNickname())
        {
            vec3Aux.push_back(A);
        }
    }
    vec3 = vec3Aux;

    while (!vec3.empty())
    {
        Person *persona = vec3.at(0);
        if (persona->isMatch())
        {
            addToGraph(persona->getUser());

            string origen = mainUser->getNickname();
            string destino = persona->getNickname();
            string matches = to_string(persona->getUser()->getMatchLevel());
            if (search == "offer")
            {
                GobizNetwork.addArc(mainUser, persona->getUser(), persona->getUser()->getMatchLevel());
                matchesVector.push_back(origen + "," + destino + "," + matches);
            }
            else
            {
                GobizNetwork.addArc(persona->getUser(), mainUser, persona->getUser()->getMatchLevel());
                matchesVector.push_back(destino + "," + origen + "," + matches);
            }
        }
        vector<Person *> vec3Aux;
        while (!vec3.empty())
        {
            Person *A = vec3.back();
            vec3.pop_back();
            if (A->getNickname() != persona->getNickname())
            {
                vec3Aux.push_back(A);
            }
        }
        vec3 = vec3Aux;
    }
}

void matches(Registered *pUser)
{
    NodoGrafo *userNode = GobizNetwork.getNodo(pUser->getId());
    vector<Arco *> *conections = userNode->getArcs();

    cout << "◌ » —— ╫ < Lista de Matches > ╫ —— « ◌" << endl
         << endl;

    cout << "——————————————————————————————————————" << endl;
    for (int i = 0; i < conections->size(); i++)
    {
        Arco *arc = conections->at(i);
        NodoGrafo *destinoNodeG = static_cast<NodoGrafo *>(arc->getDestino());
        NodoGrafo *origenNodeG = static_cast<NodoGrafo *>(arc->getOrigen());
        Registered *origen = reinterpret_cast<Registered *>(origenNodeG->getInfo());
        Registered *destino = reinterpret_cast<Registered *>(destinoNodeG->getInfo());

        if (origen->getNickname() == pUser->getNickname())
        {
            cout << "— " << destino->getNickname() << "        " << destino->getPostdate() << endl
                 << "Oferta:" << destino->getOffer() << endl;
        }
        else
        {
            cout
                << "— " << origen->getNickname() << "        " << origen->getPostdate() << endl
                << "Demanda:" << origen->getDemand() << endl;
        }
        cout << "——————————————————————————————————————" << endl;
    }
}
void valueChain()
{
    for (auto &i : allrecords)
    {
        // cout << endl;
        //  cout << "- " << i->getNickname() << " Oferta" << endl;
        filterRegs("offer", i);
        // cout << endl;
        //  cout << "- " << i->getNickname() << " Demanda" << endl;
        filterRegs("demand", i);
        // cout << endl;
    }
    ValueChainB *valueVector = new ValueChainB(matchesVector);
    valueVector->getGraphBar();
}
void circularTrade()
{
    for (auto &i : allrecords)
    {
        // cout << endl;
        //  cout << "- " << i->getNickname() << " Oferta" << endl;
        filterRegs("offer", i);
        // cout << endl;
        //  cout << "- " << i->getNickname() << " Demanda" << endl;
        filterRegs("demand", i);
        // cout << endl;
    }
    CircularTradeB *circulito = new CircularTradeB(matchesVector);
    circulito->getBase();
}
void top10Trades()
{

    vector<string> top10format;
    vector<Product> top10;

    Product test10;
    test10.word = "No_Content";
    test10.reps = 0;
    top10.push_back(test10);

    for (Product &prod : products)
    {
        for (int i = 0; i < top10.size(); i++)
        {
            if (prod.reps >= top10.at(i).reps)
            {
                top10.insert(top10.begin() + i, prod);
                break;
            }
        }
        if (top10.size() > 10)
        {
            top10.pop_back();
        }
    }
    for (Product &prod : top10)
    {
        top10format.push_back(prod.word + "," + to_string(prod.reps));
        cout << prod.word + "," + to_string(prod.reps) << endl;
    }

    BarTradesB topTrade(top10format);
    topTrade.getGraphBar();
}

void relatedMarket()
{
    vector<string> relatedMformat;
    for (int i = 0; i < products.size(); i++)
    {
        relatedMformat.push_back(products[i].word + "," + to_string(products[i].reps));
        // cout << products[i].word + "," + to_string(products[i].reps) << endl;
    }
    RelatedMarketB donut(relatedMformat);
    donut.getBase();
}

// Pantalla del menu principal
void mainMenu(Registered *pUser)
{
    int resp;
    while (true)
    {
        cout << endl
             << "◌ » —— ╫ <" << pUser->getNickname() << "> ╫ —— « ◌" << endl
             << "1. Matches" << endl
             << "2. Comercio Circular" << endl
             << "3. Cadenas de Valor" << endl
             << "4. Top 10 Solicitados" << endl
             << "5. Grafico de Presencia" << endl
             << "0. Salir" << endl
             << endl
             << "Digite la opcion: ";
        cin >> resp;
        if (resp == 1)
        {
            matches(pUser);
        }
        else if (resp == 2)
        {
            circularTrade();
        }
        else if (resp == 3)
        {
            valueChain();
        }
        else if (resp == 4)
        {
            top10Trades();
        }
        else if (resp == 5)
        {
            relatedMarket();
        }
        else if (resp == 0)
        {
            break;
        }
        else
        {
            cout << "Error: Opcion Invalida ._." << endl;
        }
    }
}

// Pantalla de inicio de sesion
void loginTUI()
{

    string resp;
    Registered *user;
    while (true)
    {
        bool founded = false;
        cout << endl
             << "◌ » —— ╫ ( ❈ ) ╫ —— « ◌" << endl
             << "Digite su nombre de usuario:" << endl
             << "Respuesta: ";
        cin >> resp;

        for (Registered *tUser : allrecords)
        {
            if (tUser->getNickname() == resp)
            {
                founded = true;
                user = tUser;
                break;
            }
        }
        if (founded)
        {
            break;
        }
        else
        {
            cout << "Error: Usuario no encontrado, vuelva a intentarlo" << endl;
        }
    }
    cout << "Inicio de sesion exitoso" << endl;
    mainMenu(user);
}

// Pantalla de registro
void registerTUI()
{

    string answer;

    string name;
    while (true)
    {
        bool founded = false;
        cout << endl
             << "◌ » —— ╫ ( ❈ ) ╫ —— « ◌" << endl
             << "Digite un nombre de usuario:" << endl
             << "Respuesta: ";
        cin.ignore();
        getline(cin, name);

        for (Registered *tUser : allrecords)
        {
            if (tUser->getNickname() == name)
            {
                founded = true;
                break;
            }
        }
        if (founded)
        {
            cout << "Error: Ese usuario ya esta ocupado, vuelva a intentarlo" << endl;
            continue;
        }
        if (name.size() < 10 || name.size() > 32)
        {
            cout << "Error: Nombre invalido, debe ser de 10 a 32 caracteres" << endl;
            continue;
        }
        break;
    }

    string password;
    string password2;
    while (true)
    {
        bool founded = false;
        cout << endl
             << "◌ » —— ╫ ( ❈ ) ╫ —— « ◌" << endl
             << "Digite una nueva contraseña:" << endl
             << "Respuesta: ";
        cin >> password;

        cout << endl
             << "◌ » —— ╫ ( ❈ ) ╫ —— « ◌" << endl
             << "Digite de nuevo la contraseña para confirmar" << endl
             << "Respuesta: ";
        cin >> password2;

        if (password != password2)
        {
            cout << "Error: Las contraseñas no coinciden" << endl;
            continue;
        }
        break;
    }

    int resp;
    while (true)
    {
        cout << endl
             << "◌ » —— ╫ ( ❈ ) ╫ —— « ◌" << endl
             << "1. Solo quiero ofrecer" << endl
             << "2. Solo quiero comprar" << endl
             << "3. Ambas" << endl
             << endl
             << "Digite la opcion: ";
        cin >> resp;

        if (resp < 1 || resp > 3)
        {
            cout << "Error: Opcion invalida, vuelva a intentarlo" << endl;
            continue;
        }
        break;
    }

    string offert;
    if (resp == 1 || resp == 3)
    {
        while (true)
        {
            cout << endl
                 << "◌ » —— ╫ ( ❈ ) ╫ —— « ◌" << endl
                 << "Escriba su oferta [max 250]" << endl
                 << "Respuesta: ";
            cin.ignore();
            getline(cin, offert);

            if (offert.size() > 250)
            {
                cout << "Error: Excedio el limite de caracteres" << endl;
                continue;
            }
            break;
        }
    }

    string demand;
    if (resp == 2 || resp == 3)
    {
        while (true)
        {
            cout << endl
                 << "◌ » —— ╫ ( ❈ ) ╫ —— « ◌" << endl
                 << "Escriba su demanda [max 250]" << endl
                 << "Respuesta: ";
            cin.ignore();
            getline(cin, demand);

            if (offert.size() > 250)
            {
                cout << "Error: Excedio el limite de caracteres" << endl;
                continue;
            }
            break;
        }
    }
    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);

    int day = tiempoLocal.tm_mday;
    int month = tiempoLocal.tm_mon;
    int year = tiempoLocal.tm_year + 1900;

    // regs.registerUser(name, offert, demand, password, day, month, year);
    cout << name << " " << offert << " " << demand << " " << password << " " << day << " " << month << " " << year << endl;
}

// Pantalla de inicio
void startMenuTUI()
{
    int resp;
    while (true)
    {
        cout << endl
             << "◌ » —— ╫ ( ❈ ) ╫ —— « ◌" << endl
             << "1. Iniciar Sesion" << endl
             << "2. Crear una Cuenta" << endl
             << "0. Salir" << endl
             << endl
             << "Digite la opcion: ";
        cin >> resp;

        if (resp == 1)
        {
            loginTUI();
            break;
        }
        else if (resp == 2)
        {
            registerTUI();
            break;
        }
        else if (resp == 0)
        {
            break;
        }
        cout << "Error: Opcion Invalida ._." << endl;
    }
}

int main(void)
{
    // Registered *i = allrecords[15];
    //  filterRegs("offer", user);

    // regs.registerUser("viva saprisa", "conciertos a estadio lleno de gente escuchando pum pum con el mismo acorde por 2 horas", "transporte y seguridad en todos los paises que visita y mucha fiesta tambien", "conejo123", 16, 11, 2022);
    // cout << allrecords.at(0)->getNickname() << endl;

    // for (Product &prod : products)
    // {
    //     cout << prod.word << endl;
    // }

    // top10Trades();
    //  relatedMarket();
    startMenuTUI();

    // for (auto &s : matchesVector)
    //{
    //     cout << s << endl;
    // }

    // vector<INodo *> result = GobizNetwork.broadPath(allrecords[0]);

    // for (int i = 0; i < result.size(); i++)
    // {
    //     Registered dato = *((Registered *)(void *)result[i]);
    //     cout << dato.getId() << " " << dato.getNickname() << endl;
    // }

    return 0;
}
