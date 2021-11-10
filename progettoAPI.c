#define maxHeapLength 170000


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
//#include "mP.h"
//#include "rbTrees.h"
#include <string.h>

typedef struct node 
{
    int nVerticeArrivo;
    long unsigned int pesoLink;
    struct node* next;
} nodoListaAdiacenza;

//typedef enum {White, Gray, Black} color;

/**typedef struct 
{
    int numeroVertice;
    long long unsigned int distanza;
} heapElement;**/
typedef enum {Black, Red} color;

//Definizione del nodo
typedef struct nodo
{
    color nodeColor;
    long long unsigned int key; //Somma delle distanze secondo dijkstra
    long unsigned int nGrafo; //nGrafo, va immagazzinato in un long int probabilmente
    struct nodo* p;
    struct nodo* left;
    struct nodo* right;

} treeNode;

typedef treeNode* treeNodePtr;

typedef struct 
{
    treeNode** root;
    treeNode* NIL; //Occhio ad avere un nodo NIL per ogni albero in modo che siano diversi
} redBlackTree;

void leftRotate(redBlackTree t, treeNodePtr x);
void rightRotate(redBlackTree t, treeNodePtr x);

typedef struct
{
    int numeroVertice;
    long long unsigned int distanza;
} heapNode;

typedef struct 
{
    int length; //Numero di elementi massimi
    int heapSize; //Numero di elementi effettivamente contenuti nell'heap
    heapNode list[maxHeapLength];
    int position_Map[maxHeapLength]; //Consente di sapere dato il nVertice -> a quale indice si trova nello heap
}
 heap;

 typedef heap* heap_Ptr;


void leftRotate(redBlackTree t, treeNodePtr x);
void rightRotate(redBlackTree t, treeNodePtr x);



//____________________________________________________CODICE PER L'ALBERO________________________________________________________________
treeNodePtr treeMinimum(redBlackTree T, treeNodePtr x)
{
    assert(x != NULL && x != (T.NIL));
    while(x->left != (T.NIL)) x = x->left;
    return x;
}

treeNodePtr treeMaximum(redBlackTree T, treeNodePtr x)
{
    assert(x != NULL);
    if(x == T.NIL) return T.NIL;
    while(x->right != (T.NIL)) x = x->right;
    return x;
}

void inOrderTreeWalk(treeNodePtr x, redBlackTree T, treeNodePtr treeMax)
{
    assert(x!=NULL && *(T.root) != NULL);
    if(x != T.NIL)
    {
        inOrderTreeWalk(x->left, T, treeMax);
        //printf(" %llu ", x->key);
        printf("%lu", x->nGrafo);
        if(x == treeMax) return;
        printf(" ");
        inOrderTreeWalk(x->right, T, treeMax);
    }
}

//Tree search ricorsiva
treeNodePtr treeSearch(treeNodePtr x, redBlackTree T, int k)
{
    //Funziona se devo restituire un solo nodo
    if(x == T.NIL || k == x->key)
    {
        return x;
    }
    if (k < x->key)
    {
        return treeSearch(x->left, T, k);
    }
    else return treeSearch(x->right, T, k);

}

//Tree search ma iterativa
treeNodePtr fastTreeSearch(treeNodePtr x, redBlackTree T, int k) //x è il nodo root del sottoalbero nel quale si vuole cercare
{
    assert(x != NULL && *(T.root) != NULL);
    while (x !=T.NIL && k != x->key)
    {
        if(k < x->key) x = x->left;
        else x = x->right;
    }
    return x;
}

void rbTransplant(redBlackTree T, treeNodePtr u, treeNodePtr v)
{
    if(u->p == T.NIL) *(T.root) = v;
    else if (u == u->p->left)   u->p->left = v;
    else u->p->right = v;
    v->p = u->p;
}

void rbDeleteFixup(redBlackTree T, treeNodePtr x)
{
    while(x != *(T.root) && x->nodeColor== Black)
    {
        if(x == x->p->left)
        {
            treeNodePtr w = x->p->right;
            if(w->nodeColor == Red)
            {
                w->nodeColor = Black;
                x->p->nodeColor = Red;
                leftRotate(T, x->p);
                w = x->p->right;
            }

            if (w->left->nodeColor == Black && w->right->nodeColor == Black)
            {
                w->nodeColor = Red;
                x = x->p;
            }
            else
            {
                if(w->right->nodeColor == Black)
                {
                    w->left->nodeColor = Black;
                    w->nodeColor = Red;
                    rightRotate(T, w);
                    w = x->p->right;
                }

                w->nodeColor = x->p->nodeColor;
                x->p->nodeColor = Black;
                w->right->nodeColor = Black;
                leftRotate(T, x->p);
                x = *(T.root);
            }
        }
        else
        {
            treeNodePtr w = x->p->left;
            if(w->nodeColor == Red)
            {
                w->nodeColor = Black;
                x->p->nodeColor = Red;
                rightRotate(T, x->p);
                w = x->p->left;
            }

            if (w->right->nodeColor == Black && w->left->nodeColor == Black)
            {
                w->nodeColor = Red;
                x = x->p;
            }
            else
            {
                if(w->left->nodeColor == Black)
                {
                    w->right->nodeColor = Black;
                    w->nodeColor = Red;
                    leftRotate(T, w);
                    w = x->p->left;
                }

                w->nodeColor = x->p->nodeColor;
                x->p->nodeColor = Black;
                w->left->nodeColor = Black;
                rightRotate(T, x->p);
                x = *(T.root);
            }
        }

    }
    x->nodeColor = Black;
}

void rbDelete(redBlackTree T, treeNodePtr z)
{
    treeNodePtr y = z;
    treeNodePtr x;
    color originalYColor = y->nodeColor;
    if(z->left == T.NIL)
    {
        x = z->right;
        rbTransplant(T, z, z->right);
    }
    else if (z->right == T.NIL)
    {
        x = z->left;
        rbTransplant(T, z, z->left);
    }
    else 
    {
        y = treeMinimum(T, z->right);
        originalYColor = y->nodeColor;
        x = y->right;
        if(y->p == z) x->p = z;
        else
        {
            rbTransplant(T, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        rbTransplant(T, z, y);
        y->left = z->left;
        y->left->p = y;
        y->nodeColor = z->nodeColor;
    }
    if(originalYColor == Black) rbDeleteFixup(T, x);

    free(z); //controllare che sia al posto giusto
}

void rbDeleteMax(redBlackTree tree)
{
    treeNodePtr node = treeMaximum(tree, *(tree.root));
    if(node != tree.NIL)
    {
        rbDelete(tree, node);
    }
}

void rbDeleteKey(redBlackTree tree, int key)
{
    treeNodePtr x = fastTreeSearch(*(tree.root), tree, key);
    if(x != tree.NIL) rbDelete(tree, x);
    else printf("WARNING! Key was not found in the tree\n");
}

void leftRotate(redBlackTree tree, treeNodePtr x)
{
    //EDGE CASES tree NULL, x NULL, x->right NULL (o uguali a tree.NIL)
    treeNodePtr y;
    if(x == tree.NIL || *(tree.root) == tree.NIL)
    {
        printf("ERROR! NIL node as x or root");
        return;
    }

    if(x->right != tree.NIL)
    {
        y = x->right;
        x->right = y->left;
        if (y->left != tree.NIL)    y->left->p = x;
        y->p = x->p;
        if(x->p == tree.NIL) *(tree.root) = y;
        else if (x == x->p->left) x->p->left = y;
        else x->p->right = y;

        y->left = x;
        x->p = y;
    }
    else
    {
        printf("WARNING! Left rotation not feasable\n");
    }
}

void rightRotate(redBlackTree tree, treeNodePtr x)
{
    //EDGE CASES tree NULL, x NULL, x->right NULL (o uguali a tree.NIL)
    treeNodePtr y;
    if(x == tree.NIL || *(tree.root) == tree.NIL)
    {
        printf("ERROR! NIL node as x or root");
        return;
    }

    if(x->left != tree.NIL)
    {
        y=x->left;
        x->left = y->right;
        if(y->right != tree.NIL) y->right->p = x;
        y->p = x->p;
        if(x->p == tree.NIL) *(tree.root) = y;
        else if(x == x->p->left) x->p->left = y;
        else x->p->right = y;

        y->right = x;
        x->p = y;
    }
    else
    {
        printf("WARNING! Right rotation non feasable\n");
    }
}

void rbInsertFixup(redBlackTree tree, treeNodePtr z)
{
    treeNodePtr y;
    while(z->p->nodeColor == Red)
    {
        assert(z->p->p != tree.NIL);
        if(z->p == z->p->p->left)
        {
            y = z->p->p->right;
            if(y->nodeColor == Red)
            {
                z->p->nodeColor = Black;
                y->nodeColor = Black;
                z->p->p->nodeColor = Red;
                z = z->p->p;
            }
            else 
            {
                if(z == z->p->right)
                {
                    z = z->p;
                    leftRotate(tree, z);
                }
                z->p->nodeColor = Black;
                z->p->p->nodeColor = Red;
                rightRotate(tree, z->p->p);
            }
        }
        else
        {
            y = z->p->p->left;
            if(y->nodeColor == Red)
            {
                z->p->nodeColor = Black;
                y->nodeColor = Black;
                z->p->p->nodeColor = Red;
                z = z->p->p;
            }
            else
            { 
                if(z == z->p->left)
                {
                    z = z->p;
                    rightRotate(tree, z);
                }
                z->p->nodeColor = Black;
                z->p->p->nodeColor = Red;
                leftRotate(tree, z->p->p);
            }
        }
    }
    (*(tree.root))->nodeColor = Black;
}

void rbInsert(redBlackTree tree, treeNodePtr z)
{
    treeNodePtr x,y;
    y = tree.NIL;
    x = *(tree.root);
    while(x != tree.NIL)
    {
        y = x;
        if(z->key < x->key) 
        {
            x = x->left;
        }
        else x = x->right;
    }
    
    z->p = y;
    if(y == tree.NIL)   *(tree.root) = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;

    z->left = tree.NIL;
    z->right = tree.NIL;
    z->nodeColor = Red;
    rbInsertFixup(tree, z);
}


//_____________________Codice per la indexed Min Priority list__________________________

 int isEmpty(heap_Ptr S)
 {
     if(S->heapSize <= 0)
     {
         return 1;
     }
     else return 0;
 }

 int parent(int i)
 {
     return i/2;
 }

 int left(int i)
 {
     return 2*i;
 }

 int right(int i)
 {
     return 2*i+1;
 }

 void minHeapify(heap_Ptr S, int indexOfAddedNode)
 {
     int leftIndex = left(indexOfAddedNode);
     int rightIndex = right(indexOfAddedNode);
     int min;
     if(leftIndex < S->heapSize && S->list[leftIndex].distanza < S->list[indexOfAddedNode].distanza) min = leftIndex;
     else min = indexOfAddedNode;
     if(rightIndex < S->heapSize && S->list[rightIndex].distanza < S->list[min].distanza) min = rightIndex;
     if(min != indexOfAddedNode)
     {
         heapNode temp;
         int temp_Vertex;
         int vertex_Min = S->list[min].numeroVertice;
         int vertex_Current = S->list[indexOfAddedNode].numeroVertice;

         //Scambio i due nella position map
         temp_Vertex = S->position_Map[vertex_Current];
         S->position_Map[vertex_Current] = S->position_Map[vertex_Min];
         S->position_Map[vertex_Min] = temp_Vertex;

         temp.distanza = S->list[indexOfAddedNode].distanza;
         temp.numeroVertice = S->list[indexOfAddedNode].numeroVertice;
         S->list[indexOfAddedNode].distanza = S->list[min].distanza;
         S->list[indexOfAddedNode].numeroVertice = S->list[min].numeroVertice;
         S->list[min].distanza = temp.distanza;
         S->list[min].numeroVertice = temp.numeroVertice;
         minHeapify(S, min);
     }
     
 }

 heapNode minimum(heap_Ptr S)
 {
     return S->list[0]; //L'elemento minimo è sempre il primo
 }

 heapNode extractMin(heap_Ptr S)
 {
     //estrae il minimo e toglie il mapping dalla position_Map
     if(S->heapSize < 1)
     {
         printf("ERROR! Heap underflow\n");
     }
     heapNode min = minimum(S);
     //Aggiorno la position_Map per eliminare il vertice estratto dalla lista
     S->position_Map[S->list[0].numeroVertice] = -1;
     S->list[0].distanza = S->list[S->heapSize-1].distanza;
     S->list[0].numeroVertice = S->list[S->heapSize-1].numeroVertice;
     S->position_Map[S->list[S->heapSize-1].numeroVertice] = 0;
     S->heapSize--;
     minHeapify(S, 0);
     return min;
 }

void decreaseKey(heap_Ptr S, int x, int k) //x è l'indice dello heap il cui nodo->distanza vogliamo diminuire
 {
     if(k > S->list[x].distanza && S->list[x].distanza != -1)
     {
         //Se la distanza è -1 lo heap non è ancora stato inizializzato, quindi non devo dare un messaggio di errore
         //controllo che la chiave passata non sia maggiore di quella che voglio inserire
         printf("ERROR! New key is bigger than old one\n");
     }
     S->list[x].distanza = k;
     heapNode temp;
     int vertex_Son, vertex_Parent;
     int vertex_Temp;

     while(x > 0 && S->list[parent(x)].distanza > S->list[x].distanza)
     {
         vertex_Son = S->list[x].numeroVertice;
         vertex_Parent = S->list[parent(x)].numeroVertice;

         //Devo anche swappare i valori nella position_Map
         vertex_Temp = S->position_Map[vertex_Son];
        S->position_Map[vertex_Son] = S->position_Map[vertex_Parent];
        S->position_Map[vertex_Parent] = vertex_Temp;

        temp.distanza = S->list[x].distanza;
        temp.numeroVertice = S->list[x].numeroVertice;
        S->list[x].distanza = S->list[parent(x)].distanza;
        S->list[x].numeroVertice = S->list[parent(x)].numeroVertice;
        S->list[parent(x)].distanza = temp.distanza;
        S->list[parent(x)] = temp;
        x = parent(x);
     }
 }

 void decreaseKeyUsingIndex(heap_Ptr S, int keyIndex, int newKey)
 {
     if(S->position_Map[keyIndex] != -1)
     {
         if(S->list[S->position_Map[keyIndex]].distanza > newKey)
         {
             decreaseKey(S, S->position_Map[keyIndex], newKey);
         }
     }
     else
     {
         //printf("WARNING! No key with that index\n"); //Il nodo è già stato tolto da dijsktra, non va aggiornato
     }
 }

 void insertHeapNode(heap_Ptr S, int dist, int nVertice)
 {
     if(S->heapSize >= maxHeapLength)
     {
         printf("ERROR! HEAP OVERFLOW!\n");
     }
     S->list[S->heapSize].distanza = -1;
     S->list[S->heapSize].numeroVertice = nVertice;
     S->position_Map[nVertice] = S->heapSize;
     S->heapSize++;
     decreaseKey(S, S->heapSize - 1, dist);
 }

 void initializeMapping(heap_Ptr S)
 {
     for(int i=0; i < S->length; i++)
     {
         S->position_Map[i] = -1;
     }
 }

 void printHeap(heap_Ptr S)
 {
     printf("DISTANZA       NUMERO VERTICE\n");
     for(int i = 0; i < S->heapSize; i++)
     {
         printf("%llu   %d\n", S->list[i].distanza, S->list[i].numeroVertice);
     }
 }

 //_____________________________CODICE PER DIJSKTRA_____________________________________


 void initializeSingleSources(int nVertex, long long unsigned int dist[], heap_Ptr minPriorityList)
{
    initializeMapping(minPriorityList);
    for(int i = 0; i < nVertex; i++)
    {
        dist[i] = -1;
        //dist[i] = i;
        //padre[i] = -1;
        //minPriorityList->list[i].distanza = -1;
        //minPriorityList->list[i].numeroVertice = i;
        insertHeapNode(minPriorityList, -1, i);
    }
    dist[0] = 0;
    //decreaseKey(heap_Ptr, 0, 0);
    //insertHeapNode(minPriorityList, 0, 0); // 0,0 poichè devo dare peso 0 al nodo sorgente (0)
    decreaseKeyUsingIndex(minPriorityList, 0, 0);
}

void relax(int u, int nVertex, long long unsigned int dist[nVertex], nodoListaAdiacenza* cursore, heap_Ptr minPList)
{
    int v = cursore->nVerticeArrivo;
    //assert(u<nVertex-1 && v<nVertex-1)
    long unsigned int weight = cursore->pesoLink;
    if(dist[u] == -1)
    {
        return;
    }
    unsigned long somma = dist[u] + weight;
    if(dist[v] > somma)
    {
        dist[v] = somma;
        //padre[v] = u;
        //insertHeapNode(minPList, dist[v], v);
        decreaseKeyUsingIndex(minPList, v, dist[v]);
    }
}

long long unsigned int dijkstraSum(int nVertex, long long unsigned int dist[nVertex], nodoListaAdiacenza* adj[nVertex], heap_Ptr minPList)
{
    initializeSingleSources(nVertex, dist, minPList);
    long long unsigned int totalPathSum = 0;
    heapNode u;
    while(isEmpty(minPList) == 0)
    {
        u = extractMin(minPList);
        if(u.distanza == -1)
        {
            return totalPathSum; //Return da qui se il grafo NON è raggiungibile, nella minPList sono rimaste solo distanze infinite
        }
        if(u.distanza <= dist[u.numeroVertice])
        {
            totalPathSum += u.distanza;
            nodoListaAdiacenza* cursore = adj[u.numeroVertice];
            while(cursore != NULL)
            {
                relax(u.numeroVertice, nVertex, dist, cursore, minPList);
                cursore = cursore->next;
            }
        }
    }
    //printf("ERROR! totalPathSum should not be returned from here! (Graph may be empty)");
    return totalPathSum; //Return da qui se il grafo è raggiungibile
}

//Funzione per inserire chiavi nell'albero topK
void rbInsertKey(redBlackTree tree, long long unsigned key, long int nGrafo)
{
    treeNodePtr newNode = malloc(sizeof(treeNode));
    newNode->key = key;
    newNode->nGrafo = nGrafo;
    rbInsert(tree, newNode);
}


//____________________________________________MAIN______________________________________

//lettura veloce IO
unsigned fastscan()
{
    register int c;
  
    unsigned number = 0;
    c = getchar_unlocked(); //versione single_thread più veloce
    // Continuo ad estrarre valori se sono interi
    while (c>='0' && c<='9')
    {
        number = (number << 1) + (number << 3) + c - 48; //sll al posto della moltiplicazione per velocizzare
        c=getchar_unlocked();
    }
        
    return number;
}


int main()
{
    int nVertex;
    int nGrafo = 0;
    long long unsigned int graphSum;
    int topK_parameter;
    int numberOfGraphsInTree = 0;
    unsigned value;

    //Inizializzo albero rb per contenere topK
    treeNodePtr testaPtr;
    treeNode nodoNIL;
    treeNodePtr nodoNILPtr;
	treeNodePtr tempNode;

    testaPtr = &nodoNIL;
    nodoNILPtr = &nodoNIL;
    nodoNIL.nodeColor = Black;
    nodoNIL.left = NULL;
    nodoNIL.right = NULL;
    nodoNIL.p = NULL;

    redBlackTree topKTree = {&testaPtr, nodoNILPtr};


    if(scanf("%d%*c%d%*c", &nVertex, &topK_parameter) <= 0) printf("ERROR IN INPUT!\n");

    struct grafo{
        nodoListaAdiacenza* adj[nVertex];
        //queueElement* vertexPointer[nOfVertex];
        long long unsigned int distanza[nVertex];
        //color colore[nVertex]; //Probabilmente inutile
        //int padre[nVertex]; //Forse non necessario
        heap minPriorityHeap;
    } graph;

    graph.minPriorityHeap.length = nVertex;
    graph.minPriorityHeap.heapSize = 0;

    nodoListaAdiacenza* temp, *cursore;
    nodoListaAdiacenza* lastLinkedNode;
    //Trasformo la matrice di adiacenza in un grafo di adiacenza
    //Per grafi sparsi va bene, controllare se si può ottimizzare con matrice di adiacenza lo spazio occupato
    //Riutilizzo la struttura dati del grafo per non dover cancellare ogni volta il grafo vecchio, ma sovrascrivendo

    char comando[13];
    //Inizializzo a NULL tutti i puntatori della lista adiacenza affinchè funzioni il controllo if(cursore != NULL)
    for(int j=0; j<nVertex; j++)
    {
        graph.adj[j] = NULL;
    }

    while(scanf("%s%*c", comando) > 0)
    {
        //Interpretazione dei comandi
        //scanf("%s%*c", comando);
        if(strcmp(comando, "AggiungiGrafo") == 0)
        {
        //Riutilizzo la struttura dati già inserita
            for(int i = 0; i < nVertex; i++)
            {
                cursore = graph.adj[i];
                lastLinkedNode = NULL;
                for(int k = 0; k < nVertex; k++)
                {
                    value = fastscan();
                    if(value != 0)
                    {
                        if(cursore != NULL)
                        {
                            cursore->pesoLink = value;
                            cursore->nVerticeArrivo = k;
                            lastLinkedNode = cursore;
                            cursore = cursore->next;
                        }
                        else
                        {
                            temp = malloc(sizeof(nodoListaAdiacenza));
                            temp->nVerticeArrivo = k;
                            temp->pesoLink = value;
                            temp->next = NULL; //Istruzione non necessaria

                            if(lastLinkedNode == NULL)
                            {
                                graph.adj[i] = temp;
                            }
                            else
                            {  
                                lastLinkedNode->next = temp;
                            }
                                lastLinkedNode = temp;
                        }
                    }
                }
                nodoListaAdiacenza* oldCursore;

                while(cursore != NULL)
                {
                    oldCursore = cursore;
                    cursore = cursore->next;
                    free(oldCursore);
                }

                if(lastLinkedNode != NULL) lastLinkedNode->next = NULL;
                else
                {
                    graph.adj[i] = NULL;
                }


            }
            graphSum = dijkstraSum(nVertex, graph.distanza, graph.adj, &(graph.minPriorityHeap));
            //printf("\nDijkstra sum of graph: %d is: %llu\n",nGrafo, graphSum);

            //Aggiungo il grafo all'albero topK
            if(numberOfGraphsInTree < topK_parameter)
            {
                rbInsertKey(topKTree, graphSum, nGrafo);
                numberOfGraphsInTree++;
            }
            else
            {
				tempNode = treeMaximum(topKTree, *(topKTree.root));
				if(tempNode != topKTree.NIL && tempNode->key > graphSum)
				{
					rbInsertKey(topKTree, graphSum, nGrafo);
					rbDelete(topKTree, tempNode);
				}
                //rbDeleteMax(topKTree);
            }
            //Sommo 1 al numero di grafi
            nGrafo++;
        }
    
        else if(strncmp(comando, "TopK", 4) == 0)
        {
            inOrderTreeWalk(*(topKTree.root), topKTree, treeMaximum(topKTree, *(topKTree.root)));
            printf("\n");
        }
        else printf("ERROR! COMMAND NOT RECOGNIZED\n");
    }
    
    
    return 0;
}