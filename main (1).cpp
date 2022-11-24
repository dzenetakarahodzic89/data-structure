#include <cmath>
#include <cstdlib>
#include <iostream>


// Node creation
// Generička klasa za čvor
template <typename tip>
struct Node 
{
  tip value;  //nekad je bilo n
  int degree;
  Node *parent;
  Node *child;
  Node *left;
  Node *right;
  bool mark;  // mark

  bool visited; //inace je bilo C
};

// Generička klasa za FIB HEAP
template <typename tip>
class FibonacciHeap 
{
   private:
  int nH;  //red heapa

  Node<tip> *H;  //root node

   public:

FibonacciHeap() 
{ 
    //konsturktor
    H = initializeHeap();
}

  Node<tip> *initializeHeap()
  {
      //Inicijalizacija heapa -> kreiranje prvog čvora u heapu
      Node<tip> *new_heap;
      new_heap = nullptr;
      return new_heap;
  }

  Node<tip> *createNode(tip value)
  {
      // param tip: -> vrijednost koja se spašava u čvor
      //Kreiranje čvora, sa spašavanjem vrijednosti value u čvor
      Node<tip> *x = new Node<tip>;  //Kreiranje pokazivaca za cvor
      x->value = value;
      return x;  // Vraća pokazivač na čvor
  }

  // Insert node
  Node<tip> *fibHeapInsert(Node<tip> *H, Node<tip> *x) 
{
  // param H -> fibonacci heap
  // param x -> čvor koji se ubacuje 
  x->degree = 0;
  x->parent = nullptr;
  x->child = nullptr;
  x->left = x;
  x->right = x;

  x->mark = false;
  x->visited = false;

  if (H != nullptr) // concatenate the root list containing x with root list H
  {
    (H->left)->right = x;
    x->right = H;
    x->left = H->left;
    H->left = x;
    if (x->value < H->value)  // pravilo ako je vrijednost novog cvora manje od vrijednosti root heap cvora -> novi cvor postaje root heap cvor
    {
      H = x;
    }
  } 
  else
  {
    H = x;
  }
  nH = nH + 1;  //red heapa poveca za jedan kad se doda novi cvor
  return H; //vraća pokazivač na root heap-a
}

// Operacija unije dva fib heapa
Node<tip> *fibHeapUnion(Node<tip> *H1, Node<tip> *H2) 
{
  //param H1 -> heap 1
  //param H2 -> heap 2
  Node<tip> *np;
  Node<tip> *H = initializeHeap();
  H = H1;
  // concatenate the root list of H2 with the root list of H
  (H->left)->right = H2;
  (H2->left)->right = H;
  np = H->left;
  H->left = H2->left;
  H2->left = np;
  return H;
}

// Consolidation funkcija
void consolidate(Node<tip> *H1) 
{
  // param H1 -> Heap koji se treba učvrstiti
  int d, i;
  float f = (log(nH)) / (log(2));
  int D = f;
  Node<tip> *A[D];

  for (i = 0; i <= D; i++)
  {
    // inicijalizacija pokazivaca u heap-u A na null pokazivace
    A[i] = nullptr;
  }

  Node<tip> *x = H1;
  Node<tip> *y;
  Node<tip> *np;
  Node<tip> *pt = x;

  do // foreach linija 3 pesudokod
  {
    pt = pt->right;
    d = x->degree;

    while (A[d] != nullptr) //linija 6 pseudokod
    {
      y = A[d];
      if (x->value > y->value)
      {
        np = x;
        x = y;
        y = np;
      }

      if (y == H1)
      {
        H1 = x;
      }

      fibHeapLink(H1, y, x);
      if (x->right == x)
      {
        H1 = x;
      }
      A[d] = nullptr;
      d = d + 1;
    }

    A[d] = x;
    x = x->right;
  }
  while (x != H1);

  H = nullptr;
  for (int j = 0; j <= D; j++) 
  {
    if (A[j] != nullptr) 
    {
      A[j]->left = A[j];
      A[j]->right = A[j];
      if (H != nullptr)
      {
        (H->left)->right = A[j];
        A[j]->right = H;
        A[j]->left = H->left;
        H->left = A[j];
        if (A[j]->value < H->value)
        {
          H = A[j];
        }
      } 
      else 
      {
        H = A[j];
      }
      if (H == nullptr)
      {
        H = A[j];
      }
      else if (A[j]->value < H->value)
      {
        H = A[j];
      }
    }
  }
}


void fibHeapLink(Node<tip> *H1, Node<tip> *y, Node<tip> *x)
 {
  // param H1 -> heap
  // param y -> node koji se linka na x
  // param x -> node na koji se linka y
  (y->left)->right = y->right;
  (y->right)->left = y->left;
  if (x->right == x)
  {
    H1 = x;
  }
  y->left = y;
  y->right = y;
  y->parent = x;

  if (x->child == nullptr)
  {
    x->child = y;
  }

  y->right = x->child;
  y->left = (x->child)->left;
  ((x->child)->left)->right = y;
  (x->child)->left = y;

  if (y->value < (x->child)->value)
  {
    x->child = y;
  }
  x->degree++;  //uvećanje stepena čvora x
}


Node<tip> *fibHeapExtractMin(Node<tip> *H1) 
{
  // ekstrakcija minimalnog cvora u heapu
  // param H1
  Node<tip> *p;
  Node<tip> *ptr;
  Node<tip> *z = H1;  // pomocni cvor z postaje min cvor iz heapa
  p = z;
  ptr = z;
  if (z == nullptr)
  {
    return z;
  }

  Node<tip> *x;
  Node<tip> *np;

  x = nullptr;

  if (z->child != nullptr) // linija pseudokoda 2 
  {
    x = z->child;
  }
  if (x != nullptr) 
  {
    ptr = x;
    do  //add x to the root list of H -> 4
    {
      np = x->right;
      (H1->left)->right = x;
      x->right = H1;
      x->left = H1->left;
      H1->left = x;
      if (x->value < H1->value)
      {
        H1 = x;
      }
      x->parent = nullptr;
      x = np;
    } 
    while (np != ptr);
  }

  (z->left)->right = z->right;
  (z->right)->left = z->left;
  H1 = z->right;

  if (z == z->right && z->child == nullptr)  //pseudokod 8
  {
    H = nullptr;
  }
  else
  {
    // linija pseudokoda 9 - 10
    H1 = z->right;
    consolidate(H1);
  }
  nH = nH - 1;  // linija 11
  return p;
}

// do ove linije funkcije su iz laba 3

// što je u pseudokodu key to je kod nas value

// fib heap decrease  iz teorije skontati značenje za x i k
int fibHeapDecreaseKey(Node<tip> *H1, int x, int k) 
{
  Node<tip> *y;
  if (H1 == nullptr) // Dodatna provjera je li heap prazan
  {
    std::cout << "Heap je prazan\n";
    return 0;
  }
  Node<tip> *ptr = find(H1, x);  //pronađi čvor u heapu koji ima vrijednost x
  if (ptr == nullptr) // provjera je li čvor pronađen
  {
    std::cout << "Čvor nije pronađen\n";
    return 1;
  }

  if (ptr->value < k) 
  {
    std::cout << "Unesena vrijednost je veća od trenutne vrijednosti čvora\n";
    return 0;
  }

  ptr->value = k;  // spasi vrijednost parametra k u value čvora koji je imao vrijednost x
  y = ptr->parent;
  if (y != nullptr && ptr->value < y->value) 
  {
    cut(H1, ptr, y);  // odsjecanje viška čvorova ? treba provjeriti u teroiji šta rade ove dvije funkcije 
    cascadeCut(H1, y);
  }

  if (ptr->value < H->value)  // ako decereasani čvor ima manju vrijednost od min čvora, decreasani čvor postaje min čvor
  {
    H = ptr; 
  }
  return 0;
}

// funkcija cut todo check u teoriji
int cut(Node<tip> *H1, Node<tip> *x, Node<tip> *y)
{
  // briše x iz djece čvora y i smješta čvor x u stablo H1
  // procedura treba da očuva veze u stablu
  if (x == x->right)
  {
    y->child = nullptr;
  }
  (x->left)->right = x->right;
  (x->right)->left = x->left;
  if (x == y->child)
  {
    y->child = x->right;
  }

  y->degree = y->degree - 1;  // umanji stepen y-a za 1
  x->right = x;
  x->left = x;
  (H1->left)->right = x;
  x->right = H1;
  x->left = H1->left;
  H1->left = x;
  x->parent = nullptr;
  x->mark = false;
}

// Cascade cut pomjera djecu node-a y ?
void cascadeCut(Node<tip>  *H1, Node<tip>  *y) 
{
  Node<tip> *z = y->parent;
  if (z != nullptr)
   {
    if (y->mark == false) 
    {
      y->mark = true;
    } 
    else
    {
      cut(H1, y, z);
      cascadeCut(H1, z);
    }
  }
}

// pretražuje čvor u stablu koji ima vrijednost k
Node<tip>  *find(Node<tip>  *H, int k) 
{
 // param H -> stablo
 // param k -> vrijendsot koju traži

  Node<tip>  *x = H;
  x->visited = true;
  Node<tip> *p = nullptr;
  if (x->value == k)  //provjeri je li root stabla ima vrijendost k
  {
    p = x;
    x->visited = false;
    return p;
  }

  if (p == nullptr)  // root stabla nema vrijendost k
   {
    if (x->child != nullptr) // nad djetetom roota pozovi funkciju pretrage
    {
      p = find(x->child, k);
    }
    if ((x->right)->visited != true)
    {
      p = find(x->right, k);
    }
  }
  x->visited = false;
  return p;
}

// Deleting key
void fibHeapDelete(Node<tip> *H1, int k)
 {
  Node<tip> *np = nullptr;
  int t;
  t = fibHeapDecreaseKey(H1, k, -5000);  // -5000 predstavlja -inf s pretp da value nece ici ispod -5000
  if (!t)
  {
    np = fibHeapExtractMin(H);
  }
  if (np != nullptr)
  {
    std::cout << "Čvor obrisan\n";
  }
  else
  {
    std::cout << "Čvor nije obrisan\n";
  }
}

// Po tekstu laba ne treba ali služi za ispis rezultata
int display(Node<tip> *H) 
{
 // param H -> root node stabla
  Node<tip> *p = H;
  if (p == nullptr) // ako je heap prazan ispisi da nema elemenata
   {
    std::cout << "prazan Heap\b";
    return 0;
   }
  std::cout << "Root čvorovi: \n";

  do 
  {
    std::cout << p->value;
    p = p->right;
    if (p != H) 
    {
      std::cout << "-->";
    }
  } 
  while (p != H && p->right != nullptr);
  std::cout << "\n";
}
    
};



int main() 
{
  int n, m, l;
  FibonacciHeap<int> fh;
  Node<int> *p;
  Node<int> *H;
  H = fh.initializeHeap();

  p = fh.createNode(7);
  H = fh.fibHeapInsert(H, p);
  p = fh.createNode(3);
  H = fh.fibHeapInsert(H, p);
  p = fh.createNode(17);
  H = fh.fibHeapInsert(H, p);
  p = fh.createNode(24);
  H = fh.fibHeapInsert(H, p);

  fh.display(H);

  p = fh.fibHeapExtractMin(H);
  if (p != nullptr)
  {
    std::cout << "The node with minimum key: " << p->value << "\n";
  }
  else
  {
    std::cout << "Heap is empty\n";
  }

  m = 26;
  l = 16;
  fh.fibHeapDecreaseKey(H, m, l);

  m = 16;
  fh.fibHeapDelete(H, m);
}