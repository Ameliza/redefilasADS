/**************************************************************************
 *
 * icecream.cpp - Priority queue example program.
 *
 * $Id$
 *
 ***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  "License"); you may  not use  this file
 * except in  compliance with the License.   You may obtain  a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the  License is distributed on an  "AS IS" BASIS,
 * WITHOUT  WARRANTIES OR CONDITIONS  OF ANY  KIND, either  express or
 * implied.   See  the License  for  the  specific language  governing
 * permissions and limitations under the License.
 *
 * Copyright 1994-2006 Rogue Wave Software.
 * 
 **************************************************************************/

#include <algorithm>   // for random_shuffle()
#include <iostream>    // for cout
#include <queue>       // for queue
#include <vector>      // for vector

// Execution event in a descrete event driven simulation.
class event {
public:
    // Construct sets time of event.
    event (unsigned int t) : time (t)
        { }

    // Execute event by invoking this method.
    virtual void processEvent () = 0;

    const unsigned int time; // tempo em que o evento vai acontecer
};

struct eventComparator {
    bool operator() (const event * left, const event * right) const {
        return left->time > right->time;
    }
};


// Framework for discrete event-driven simulations.
class simulation {
public:
    simulation () : time (0), eventQueue () 
        {}
    void run ();
    void  scheduleEvent (event * newEvent) {
        eventQueue.push (newEvent);
    }
    unsigned int time;
protected:
    std::priority_queue<event*,
                        std::vector<event *, std::allocator<event*> >,
                        eventComparator> eventQueue;
};


void simulation::run () {

    while (! eventQueue.empty ()) { // pode parar a simulação por tempo também

        event * nextEvent = eventQueue.top (); // pega o evento mais antigo
        eventQueue.pop (); // retira ele da fila
        time = nextEvent->time; // atualiza o tempo de simulação
        nextEvent->processEvent (); 
        delete nextEvent;
    }
}


class NetworkQueueSimulation : public simulation {
public:
    NetworkQueueSimulation () : simulation (), server1(false), server2(false), server3(false)
        { }
    
    // Data fields.
    bool server1, server2, server3; // pode ser trocado por estados
    // T_queue queue1, queue2, queue3; // refletem/implementam o estado das filas 

} NetworkQueue;

/*
class GenFun : public event { // Funcao geradora
public:
    GenFun (unsigned int t)
        : event (t)
        { }
    virtual void setSeed (); // seta a semente
    virtual void expRand (); // gera um numero de 0 à 1
    int seed;
};

void GenFun::expRand(){
    int a = 1103515245;
    int c = 12345;
    int m = 2147483648;
}
*/

int irand (int n)
{
    // chegada de requisições da fila, os tempos entre requisições
    // vão ser gerados a partir de uma distribuição exponencial
    // 3 pseudogeradores randomicos, 1 para cada componente externa
    // int exprand(int z0)
    // z0 é a semente geradora
    // z_next = (a * z_prev + c) % m

    static int seq[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf
    };

    std::random_shuffle (seq, seq + sizeof seq / sizeof *seq);

    const int rnd =
        ((seq [0] << 11) | (seq [1] << 8) | (seq [2] << 4)) + seq [3];

    return rnd % n;
}

float frand(){
    double n;
    int v;

    v = rand() % 11;
    n = v;
    n /= 10;
    // std::cout << n << "\n";

    return n;

}

class A1Event : public event {
public:
    A1Event (unsigned int t)
        : event (t)
        { }
    virtual void processEvent ();
    //fazer estatistica das pessoas que entram (0.7 e 0.3)
    // e guardar o tempo
    
};

class D1Event : public event {
public:
    D1Event (unsigned int t)
        : event (t)
        { }
    virtual void processEvent ();
    
};

class IA2Event : public event {
public:
    IA2Event (unsigned int t)
        : event (t)
        { }
    virtual void processEvent ();
    
};

class IA3Event : public event {
public:
    IA3Event (unsigned int t)
        : event (t)
        { }
    virtual void processEvent ();
    
};

class A2Event : public event {
public:
    A2Event (unsigned int t)
        : event (t)
        { }
    virtual void processEvent ();
    
};

class D2Event : public event {
public:
    D2Event (unsigned int t)
        : event (t)
        { }
    virtual void processEvent ();
    
};

/*****************************************************************************************/

void A1Event::processEvent () { // chegada na fila 1
    // se o servidor estiver IDLE (false), gera evento de partida
    // senão, coloca o registro de que o evento aconteceu na fila

    if(NetworkQueue.server1 == false) { // servidor livre da fila 1
        NetworkQueue.scheduleEvent // escalona o evento (coloca ele na fila de eventos)
            (new D1Event (time + 1 + irand (4)));
    } 
    else{ // servidor da fila 1 está ocupado
    // empurrar/anotar a ocorrência deste evento na fila auxiliar queue1

    }
    // gerar a próxima chegada A1Event
    NetworkQueue.scheduleEvent // escalona o evento (coloca ele na fila de eventos)
            (new A1Event (time + 1 + irand (4)));

}

void D1Event::processEvent () {
    // com probabilidade 0.7 cria e escalonar evento IA2
    // com probabilidade 0.3 cria e escalonar evento IA3
    // e com tempo t+0 (sem retardo)
    // geração uniforme entre 0 e 1, se < 0.7 cria IA2 se 0.7 < x < 1 cria IA3
    // gerar a próxima chegada A2Event
    double prob = frand();
    std::cout << "Prob: " << prob << "\n";

    if (prob <= 0.7){
        std::cout << "ENTROU IA2\n";
        NetworkQueue.scheduleEvent // escalona o evento (coloca ele na fila de eventos)
            (new IA2Event (time + 1 + irand (4)));
    }
    else{
        std::cout << "ENTROU IA3\n";
        NetworkQueue.scheduleEvent // escalona o evento (coloca ele na fila de eventos)
            (new IA3Event (time + 1 + irand (4)));
    }
}

void A2Event::processEvent () { // chegada na fila 2
    // se o servidor estiver IDLE (false), gera evento de partida
    // senão, coloca o registro de que o evento aconteceu na fila

    if(NetworkQueue.server2 == false) { // servidor livre da fila 2
        NetworkQueue.scheduleEvent // escalona o evento (coloca ele na fila de eventos)
            (new D2Event (time + 1 + irand (4)));
    } 
    else{ // servidor da fila 2 está ocupado
    // empurrar/anotar a ocorrência deste evento na fila auxiliar queue1

    }
    // gerar a próxima chegada A2Event
    NetworkQueue.scheduleEvent // escalona o evento (coloca ele na fila de eventos)
            (new A2Event (time + 1 + irand (4)));
}

void IA2Event::processEvent (){
    
}

void IA3Event::processEvent (){
    
}

void D2Event::processEvent(){

}

int main () {

    std::cout << "Ice Cream Store simulation from Chapter 9\n";

    // Load queue with some number of initial events.
    for (unsigned t = 0; t < 20; t += irand (6)) {

        std::cout << "pumping queue with event " << t << '\n';
        NetworkQueue.scheduleEvent (new A1Event (t));
    }

    // Run the simulation.
    NetworkQueue.run ();

    return 0;
}