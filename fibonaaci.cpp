#include <iostream>

enum Color {RED, BLACK}; // pobrojani tip -> boje za cvorove

template <typename tip>
struct Node 
{
    tip data;
    Color color;
    Node *left, *right, *parent;  // pokazivaci na cvorove 
    // konstruktor klase cvor  
    Node(tip data) 
    {
        this->data = data; // popunjavanje parametra data 
        left = right = parent = nullptr; // inicijalizacija pokazivaca na null pokazivace
        this->color = RED;  // po defaultu je crvena boja
    }
    // destruktor klase cvor
    ~Node()
    {
        // brisanje pokazivaca, da nema curenja memorije
        delete left;
        left = nullptr;
        delete right;
        right = nullptr;
    }
};


template <typename tip>
class RBTree 
{
    Node<tip> *root; // jedan čvor može pokazivati na još 3 čvora tako da je moguće definirati cijelo drvo uz samo jedan čvor

    Node<tip>* insertNode(Node<tip>* &root, Node<tip> *z)
    {
        //Algoritam za umetanje cvora bez strategije za fixanje
        //https://www.andrew.cmu.edu/user/mm6/95-771/examples/RedBlackTreeProject/dist/javadoc/redblacktreeproject/RedBlackTree.html Insert function
        Node<tip> *y = nullptr;  // nill Node
        Node<tip> *x = root;

        //U ovisnosti je li podatak spasen u cvor smjesti cvor na lijevu ili desnu stranu parent node-a
        while (x != nullptr) 
        {
            y = x;
           
            if (z->data < x->data)   
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }

        // Popunjavanje podataka za umetnuti čvor (parent right left)
        z->parent = y;

        if(y == nullptr) 
        {
            root = z;
        }
        else if (z->data < y->data)
        {
            y->left = z;
        }
        else
        {
            y->right = z;
        }
        z->left = nullptr;
        z->right = nullptr;
        z->color = RED;
        // Pok na prvi cvor u stablu, jer on određuje cijelo stablo
        return root;
    }

    void fixTree(Node<tip>* &root, Node<tip> *z) 
    {
        // strategija za fixanje tako da je stablo očuvano RBInsertFixup sa https://www.andrew.cmu.edu/user/mm6/95-771/examples/RedBlackTreeProject/dist/javadoc/redblacktreeproject/RedBlackTree.html
        while(z != root && z->color != BLACK && z->parent->color == RED)
        {
            if(z->parent == z->parent->parent->left) //Lijeva strana 
            {
                Node<tip> *y = z->parent->parent->right;
                if(y != nullptr && y->color == RED)  // Prvi slučaj -> root ima crnu boju
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } // Kraj prvog slučaja
                else
                {
                    if(z == z->parent->right)  // Drugi slučaj
                    {
                        z = z->parent;
                        leftRotate(root, z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(root, z->parent->parent);
                }
            }
            else //Isti kod samo desna strana
            {
                Node<tip> *y = z->parent->parent->left;
                if(y != nullptr && y->color == RED)
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else 
                {
                    if(z == z->parent->left)
                    {
                        z = z->parent;
                        rightRotate(root, z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(root, z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void inOrderRun(Node<tip>* &root) 
    {
        //Služi za ispis cijelog stabla
        //Rekruzivna funkcija (kratko obj izbaci prije slanja, funkcija poziva sama sebe)
        if (root == nullptr)
        {
            return;
        }
        inOrderRun(root->left);
        std::cout << root->data << " ";
        inOrderRun(root->right);
    }

    Node<tip>* iterativeTreeSeach(Node<tip>* &x, tip key)
    {
        // x je korijen stabla (ne mora biti nužno)
        // Pretraživanje binarnog stabla (bilo kojeg, univerzalna, pseudocode ITERATIVE-TREE-SEARCH)
        while (x != nullptr && x->data != key)
        {
            if (key < x->data)
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }
        return x;
    }

    Node<tip>* treeMinimum(Node<tip>* x)
    {
        // Minimalni čvor (krajnji lijevi)
        while(x->left != nullptr)
        {
            x = x->left;
        }
        return x;
    }

    Node<tip>* treeSuccessor(Node<tip>* x)
    {
        /*Pojašnjenje: materijal koji sam poslao, Figura * slučaj 4 čvor D, lijeva slika*/
        //Briše se čvor D
        // za trazenje successora (nasljednik)
        Node<tip>* y = nullptr;
        if (x->right != nullptr)
        {
            return treeMinimum(x->right);
        }
        y = x->parent; //Čvor B
        while (y!= nullptr && x == y->right)
        {
            x = y;
            y = y->parent; 
        }
        return y;
    }
    
    void rbDeleteFixup(Node<tip>* &root, Node<tip> *x)
    {
        //Strategija za fixanje stabla prilikom brisnja čvora x 
        Node<tip>* w = nullptr;
        while (x!=root && x->color == BLACK)
        {
            if (x==x->parent->left) //Uslov da je stablo "pokvareno" lijevo orijentirano stablo
            {
                w = x->parent->right;
                if (w->color == RED) 
                {
                    // Case 1: x’s sibling is red
                    w->color = BLACK;  
                    x->parent->color = RED;
                    leftRotate(root, x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK)
                { 
                    // Case 2: x’s sibling w is black, and both w’s children are black.
                    w->color = RED;
                    x = x->parent;
                }
                else 
                {
                    if (w->right->color == BLACK)
                    {
                        // Case 3: x's sibling w is black, w's left child is red, and w's right child is black
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(root, w);
                        w = x->parent->right;
                    }
                    // Case 4: x's sibling w is black, and w's right child is red
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(root, x->parent);
                    x = root;
                }
            }
            else // desno orijentirano stablo
            {
                w = x->parent->left;
                if (w->color == RED)
                {
                    // Case 1: x’s sibling is red
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(root, x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK)
                {
                    // Case 2: x’s sibling w is black, and both w’s children are black. 
                    w->color = RED;
                    x = x->parent;
                }
                else 
                {
                    if (w->left->color == BLACK)
                    {
                        // Case 3: x's sibling w is black, w's left child is red, and w's right child is black
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(root, w);
                        w = x->parent->left;
                    }
                    // Case 4: x's sibling w is black, and w's right child is red
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(root, x->parent);
                    x = root;
                }

            }
        }
        x->color = BLACK;
    }

    Node<tip>* rbDelete(Node<tip>* &root, Node<tip> *z)
    {
        /*Glavna funkcija za brisanje, bez fixanja stabla,
        do fixanja stabla dolazi kada: The node x passed to RB-DELETE-FIXUP is one of two nodes: either the node that was y’s sole child
        before y was spliced out if y had a child that was not the sentinel NULL, or it y had no children, x is the
        sentinel NULL*/
        Node<tip>* y = nullptr; //pomocne varijable za izvršavanje algoritma
        Node<tip>* x = nullptr;
        if (z->left == nullptr || z->right == nullptr) //ako je čvor z zadnji u stablu (nil leaf)
        {
            y = z;
        }
        else
        {
            y= treeSuccessor(z);
        }

        if (y->left != nullptr)
        {
            x = y->left;
        }
        else
        {
            x = y->right;
        }

        x->parent = y->parent;
        if (y->parent == nullptr)
        {
            root = x; //modifikovanje roota stabla
        }
        else if (y == y->parent->left)
        {
            y->parent->left = x;
        }
        else
        {
            y->parent->right = x;
        }
        if (y!=z)
        {
            z->data = y->data;
        }
        if (y->color == BLACK)
        {
            rbDeleteFixup(root, x);
        }
        return y;
    }

public:
    RBTree() // Konstruktor RB stabla
    { 
        root = nullptr;
    }
    
    ~RBTree() // destrutkor  RB stabla
    {
        delete root;
        root = nullptr;
    }

    void leftRotate(Node<tip> *&root, Node<tip> *x)
    {
        // Algoritam za rotiranje čvora u lijevu stranu
        Node<tip> *y = x->right;

        x->right = y->left;

        if (y->left != nullptr) 
        {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullptr)
        {
            root = y;
        } 
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node<tip> *&root, Node<tip> *y)
    {
        // Algoirtam za rotiranje čvora u desnu stranu
        Node<tip> *x = y->left;

        y->left = x->right;

        if (y->left != nullptr)
        {
            y->left->parent = y;
        }

        x->parent = y->parent;

        if (y->parent == nullptr)
        {
            root = x;
        }

        else if (y == y->parent->left) 
        {
            y->parent->left = x;
        }

        else
        {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;
    }

    void insertNodeWithFix(tip value) 
    {
        Node<tip> *novi = new Node<tip>(value);
        // umetanje cvora
        this->root = insertNode(this->root, novi);
        // popravke
        fixTree(this->root, novi);
    }

    void inOrderIspis()
    {
        // Javna funkcija za ispis stabla
        inOrderRun(this->root);
    }

    void rbTransplant(Node<tip> *u, Node<tip> *v)
    {
        // Nije pronađeno u pseudocodovima u clrs knjizi
        if (u->parent == nullptr)
        {
            this->root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteNode(tip value)
    {
        Node<tip> *result_node = iterativeTreeSeach(this->root, value); //Pretraži stablo da li ima čvor sa vrijednosti value
        if (result_node == nullptr)
        {
            std::cout<< "Čvor nije pronađen.";
        }
        else
        {
            rbDelete(this->root, result_node);
        }
    }
};

//6,11,10,2,9,7,5,13,22,27,36,12,31
int main() 
{
    RBTree<int> stablo;
    stablo.insertNodeWithFix(6);
    stablo.insertNodeWithFix(11);
    stablo.insertNodeWithFix(10);
    stablo.insertNodeWithFix(2);
    stablo.insertNodeWithFix(9);
    stablo.insertNodeWithFix(7);
    stablo.insertNodeWithFix(5);
    stablo.insertNodeWithFix(13);
    stablo.insertNodeWithFix(22);
    stablo.insertNodeWithFix(27);
    stablo.insertNodeWithFix(36);
    stablo.insertNodeWithFix(12);
    stablo.insertNodeWithFix(31);

    stablo.inOrderIspis();

    std::cout<<"ISPIS\n";
    stablo.deleteNode(10);

    stablo.inOrderIspis();
    /*int opcija;
    do 
    {
        std::cout << "Izaberite opciju -\n1) Umetanje novog cvora\n2) Inorder ispis\n3) Izlaz\n"; // Ispis opcija korisniku (obriši komentar)
        std::cin >> opcija;  // Unos opcije (obriši komentar)
        switch (opcija)
        {
            case 1:
                int broj;
                std::cout << "Unesite cvor: ";
                std::cin >> broj;
                stablo.insertNodeWithFix(broj);
                break;
            case 2:
                std::cout << "Inorder ispis: ";
                stablo.inOrderIspis();
                std::cout << std::endl;
                break;
            case 3:
                int br;
                std::cout << "Unesi čvor za brisanje: ";
                std::cin>> br;
                stablo.deleteNode(br);
                break;
            case 4:
            default:
                break;
        }
    } 
    while (opcija != 3); */

    return 0;
}
