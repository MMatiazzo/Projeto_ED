#include "arquivo.h"
#include "Tree.h"
#include <string.h>
#include "funcoes.h"
#include <math.h>

typedef struct RAMO{
    void *data;
    int cor;
    struct RAMO *left, *right, *parent;
    int aux;
}ramo;

typedef struct arvi{
    ramo *root;
    ramo *nulo;
    int tamanho;
    int (*compare)(void*, void*);
}tree_t;

void insert_fix_up(void *raiz, void *k);

void left_rotate(tree_t *T, ramo *x);

void right_rotate(tree_t *T, ramo *y);

void percorreAux(void (*funcao)(void *, void *), ramo *no_atual, tree_t *arvore, void *auxiliar);

void delete_rb_tree(void *T, void *z);

void arvoreToArrayAux(void **vetor, ramo *no_atual, tree_t *arvore);

void delete_fix_up(tree_t *Tree, ramo *x);

ramo *rb_tree_sucessor(tree_t *T,ramo *x);

ramo *getNode(tree_t *arvore, ramo *no_atual, char * key, char* (*getId)(void*));

ramo *tree_minimum(tree_t *T,ramo *x);

int maxDepth(ramo* node,tree_t *T);
void percorreArvoreDm(void *a,void *c,void (*printaSvg)(void*,void*,FILE*,int,int,char,int),FILE *arquivoSVG,int tam,int y,int x);


void *criaArvore(int (*compare)(void*, void*)){
    tree_t *this = malloc(sizeof(tree_t));
    this->nulo = malloc(sizeof(ramo));
    this->nulo->parent = this->nulo;
    this->nulo->left = this->nulo;
    this->nulo->right = this->nulo;
    this->nulo->cor = BLACK;
    this->nulo->data = this->nulo;
    this->root = this->nulo;
    this->compare = compare;
    this->tamanho = 0;
    return (void*) this;
}

void left_rotate(tree_t *T, ramo *x){
    ramo *y = x->right;
    x->right = y->left;
    if(x->right != T->nulo){
        x->right->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == T->nulo){
        T->root = y;
    }else if(x == x->parent->left){
        x->parent->left = y;
    }else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y; 
}

void right_rotate(tree_t *T, ramo *y){
    ramo *x = y->left;
    y->left = x->right;
    if(y->left != T->nulo){
        y->left->parent  = y;
    }
    x->parent = y->parent;
    if(y->parent == T->nulo){
        T->root = x;
    }else if(y == y->parent->left){
        y->parent->left = x;
    }else{
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

void insertArvore(void *T, void *item){
    tree_t *raiz = (tree_t*) T;
    ramo *this = malloc(sizeof(ramo));
    ramo *y = raiz->nulo;
    ramo *x = raiz->root;

    this->data = item;

    while (x != raiz->nulo){
        y = x;
        if(raiz->compare(this->data,x->data) == -1){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    this->parent = y;
    if(y == raiz->nulo){
        raiz->root = this;
    }else if(raiz->compare(this->data,y->data) == -1){
        y->left = this;
    }else{
        y->right = this;
    }
    this->left = raiz->nulo;
    this->right = raiz->nulo;
    this->cor = RED;
    insert_fix_up(T, this);
    raiz->tamanho++;
}

void insert_fix_up(void *T, void *z){
    ramo *this = (ramo*) z;
    tree_t *raiz = (tree_t*) T;
    while(this->parent->cor == RED){
        if(this->parent == this->parent->parent->left){
            ramo *y = this->parent->parent->right;
            if(y->cor == RED){
                this->parent->cor = BLACK;
                y->cor = BLACK;
                this->parent->parent->cor = RED;
                this = this->parent->parent;
            }else{
                if(this == this->parent->right){
                    this = this->parent;
                    left_rotate((tree_t *)T,this);
                }
            this->parent->cor = BLACK;
            this->parent->parent->cor = RED;
            right_rotate((tree_t *)T,this->parent->parent); 
            }
        }else{
            ramo *y = this->parent->parent->left;
            if(y->cor == RED){
                this->parent->cor = BLACK;
                y->cor = BLACK;
                this->parent->parent->cor = RED;
                this = this->parent->parent;
            }else{
                if(this == this->parent->left){
                    this = this->parent;
                    right_rotate((tree_t *)T,this);
                }
            this->parent->cor = BLACK;
            this->parent->parent->cor = RED;
            left_rotate((tree_t *)T,this->parent->parent);
            } 
        }
    }
    raiz->root->cor = BLACK;
}

void rb_transplant(tree_t *arvore, ramo *u, ramo *v){
    if(u->parent == arvore->nulo){
        arvore->root = v;
    }else if(u == u->parent->left){
        u->parent->left = v;
    }else{
        u->parent->right = v;
    }
    v->parent = u->parent;
}

ramo *tree_minimum(tree_t *T,ramo *x){
    while (x->left != T->nulo){
        x = x->left;
    }
    return x;
}

ramo *rb_tree_sucessor(tree_t *T,ramo *x){
    if(x->right != T->nulo){
        return tree_minimum(T,x->right);
    }
    ramo *y = x->parent;
    while(y != T->nulo && x == y->right){
        x = y;
        y = y->parent;
    }
    return y;
}

void delete_fix_up(tree_t *Tree, ramo *x){
    ramo *root = Tree->root;
    while(x != root && x->cor == BLACK){
        if(x == x->parent->left){
            ramo *w = x->parent->right;
            if(w->cor == RED){
                w->cor = BLACK;
                x->parent->cor = RED;
                left_rotate(Tree, x->parent);
                w = x->parent->right;
            }

            if(w->left->cor == BLACK && w->right->cor == BLACK){
                w->cor = RED;
                x = x->parent;
            }else if(w->right->cor == BLACK){
                w->left->cor = BLACK;
                w->cor = RED;
                right_rotate(Tree, w);
                w = x->parent->right;
            }else{
                w->cor = x->parent->cor;
                x->parent->cor = BLACK;
                w->right->cor = BLACK;
                left_rotate(Tree, x->parent);
                x = root;
            }
        } else {
            ramo *w = x->parent->left;
            if(w->cor == RED){
                w->cor = BLACK;
                x->parent->cor = RED;
                right_rotate(Tree, x->parent);
                w = x->parent->left;
            }
            if(w->left->cor == BLACK && w->right->cor == BLACK){
                w->cor = RED;
                x = x->parent;
            } else if (w->left->cor == BLACK){
                w->left->cor = BLACK;
                w->cor = RED;
                left_rotate(Tree, w);
                w = x->parent->left;
            } else{
                w->cor = x->parent->cor;
                x->parent->cor = BLACK;
                w->left->cor = BLACK;
                right_rotate(Tree, x->parent);
                x = root;
            }
        }
    }
    x->cor = BLACK;
}

ramo* RBTreeSuccessor(void *raiz , void* x) {
    tree_t *this = (tree_t*) raiz;
    ramo *this_2 = (ramo*) x;
    if (this_2->right != this->nulo) {
        return tree_minimum(raiz ,this_2->right);
    }
    ramo* y = this_2->parent;
    while (y != this_2 && x == y->right) {
        this_2 = y;
        y = y->parent;
    }
    return y;
}  

void* RBTreeDelete(void* tree, void *z) {
    tree_t *raiz = (tree_t*) tree;
    ramo* this = (ramo*) z;
    ramo *x, *y;  
 
    if (this->left == raiz->nulo || this->right == raiz->nulo) {
        y = z;
    } else {
        y = RBTreeSuccessor(tree, z);
    }  
 
    if (y->left != raiz->nulo) {
        x = y->left;
    } else {
        x = y->right;
    }  

    x->parent = y->parent;  
 
    if (y->parent == raiz->nulo) {
        tree = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }  
 
    if (y != z) {
        this->data = y->data;
    }  
 
    if (y->cor == BLACK) {
        delete_fix_up(tree, x);
    }
    return y;
} 
 

void delete_rb_tree(void *T, void *z){
    tree_t *arvore = (tree_t*) T;
    ramo *this = (ramo*) z;
    
    ramo *y = this;
    ramo *x;
    int *y_original_color = &y->cor;
    if(this->left == arvore->nulo){
        x = this->right;
        rb_transplant(arvore,this,this->right);
    }else if(this->right == arvore->nulo){
        x = this->left;
        rb_transplant(arvore,this,this->left);
    }else{
        y = tree_minimum(arvore,this->right);
        y_original_color = &y->cor;
        x = y->right;
        if(y->parent == this){
            x->parent = y; 
        }else{
            rb_transplant(arvore,y,y->right);
            y->right = this->right;
            y->right->parent = y;
        } 
        rb_transplant(arvore,this,y);
        y->left = this->left;
        y->left->parent = y;
        y->cor = this->cor;
    }
    if(y_original_color == BLACK){
        delete_fix_up(arvore,x);
    }
}

void deletaArvore(void * T, char* key, char* (*getId)(void*)){
    ramo *node;
    tree_t *this = (tree_t *) T;
    node = getNode(T, this->root, key, getId);
    if(node){
        T = RBTreeDelete(T, node);
        this->tamanho--;
    }
}

ramo *getNode(tree_t *arvore, ramo *no_atual, char * key, char* (*getId)(void*)){
    ramo *retorno = NULL;
    
    if(!strcmp(getId(no_atual->data), key)){
        retorno = no_atual;
    }else {
        if(no_atual->left != arvore->nulo)
            retorno = getNode(arvore, no_atual->left, key, getId);
            if(retorno)
                return retorno;
        if(no_atual->right != arvore->nulo)
            retorno = getNode(arvore, no_atual->right, key, getId);
            if(retorno)
                return retorno;
    }
    return retorno;
}


item getNodeData(node no){
    return ((ramo *)no)->data; 
}


node getNodeLeft(node no){
    return ((ramo *)no)->left;
}

node getNodeRight(node no){
    return ((ramo *)no)->right;
}

node getNodeParent(node no){
    return ((ramo *)no)->parent;
}

node getArvoreRaiz(arvore a){
    return ((tree_t *) a)->root;
}


int isNulo(arvore a, node no){
    tree_t *this = (tree_t *) a;
    if(no == this->nulo)
        return 1;
    return 0;
    
}





void percorreArvore(arvore a, void (*funcao)(void *, void *), void *auxiliar){
    tree_t *this = (tree_t *) a;
    if(this->tamanho > 0)
        percorreAux(funcao, this->root, this, auxiliar);
    return;
}



void percorreAux(void (*funcao)(void *, void *), ramo *no_atual, tree_t *arvore, void *auxiliar){ 

    funcao(auxiliar, no_atual->data);

    if(no_atual->left != arvore->nulo)
        percorreAux(funcao, no_atual->left, arvore, auxiliar);
 
    if(no_atual->right != arvore->nulo)
        percorreAux(funcao, no_atual->right, arvore, auxiliar);
 
    return;
}


int getArvoreTamanho(arvore a){
    tree_t *this = (tree_t *) a;
    return this->tamanho;
}


void **arvoreToArray(arvore a){
    tree_t *this = (tree_t *) a;
    void **vetor = malloc(sizeof(void *) * this->tamanho);
    for(int i = 0; i < this->tamanho; i++){
        vetor[i] = NULL;
    }
    arvoreToArrayAux(vetor, this->root, this);
    return vetor;
}



void arvoreToArrayAux(void **vetor, ramo *no_atual, tree_t *arvore){
    
            
    for(int i = 0; i < arvore->tamanho; i++){
        if(vetor[i] == NULL){
            vetor[i] = no_atual->data;
            break;
        }
    }

    if(no_atual->left != arvore->nulo)
        arvoreToArrayAux(vetor, no_atual->left, arvore);

    if(no_atual->right != arvore->nulo)
        arvoreToArrayAux(vetor, no_atual->right, arvore);

    return;
}


int getNodeCor(node no){
    return ((ramo *)no)->cor;
}

int maxDepth(ramo* node,tree_t *T)  
{
   if (node==T->nulo)  
       return 0;
   else{
       int lDepth = maxDepth(node->left,T);
       int rDepth = maxDepth(node->right,T);

       if (lDepth > rDepth)  
           return(lDepth+1);
       else return(rDepth+1);
   }
}  
void percorreArvoreDm(void *a,void *c,void (*printaSvg)(void*,void*,FILE*,int,int,char,int),FILE *arquivoSVG,int tam,int y,int x){
    ramo *this = (ramo *)a;
    tree_t *b = (tree_t* )c;
    if (this->data == b->nulo){
        fprintf(arquivoSVG,"<rect x=\"%d\" y=\"%d\" width=\"10\" height=\"10\" stroke=\"black\" fill-opacity='1' fill=\"black\" stroke-width=\"1.0\"/>",x-5,y);
        return;
    }
    fprintf(arquivoSVG,"\t<line x1='%d' y1='%d' x2='%d' y2='%d' stroke-width = '2' fill='black' stroke='black'/>\n",x,y,x-tam/2,y+30);
    printaSvg(this->data,this->parent->data,arquivoSVG,x,y,this->cor,tam);
    percorreArvoreDm(this->left,b,printaSvg,arquivoSVG,tam/2,y+30,x-tam/2);
    fprintf(arquivoSVG,"\t<line x1='%d' y1='%d' x2='%d' y2='%d' stroke-width = '2' fill='black' stroke='black'/>\n",x,y,x+tam/2,y+30);
    percorreArvoreDm(this->right,b,printaSvg,arquivoSVG,tam/2,y+30,x+tam/2);
}
void arqpercorreArvoreDmAux(void *a,void (*printaSvg)(void*,void*,FILE*,int,int,char,int),FILE *arquivoSVG){
    tree_t *aux = (tree_t *)a;
    int f = maxDepth(aux->root,aux);
    int p = pow(2,f);
    int tam = p*20;
    int y = 30;
    int x = tam;
    fprintf(arquivoSVG,"\t<text x='%d' y='%d' text-anchor='middle' font-size='4px'>%s</text>\n",x,y,"RAIZ");
    percorreArvoreDm(aux->root,aux,printaSvg,arquivoSVG,tam/2,y,x);
}

void percorreToPoligon(void *a,void *x,int (*dadosNode)(void*,void**,int,void*,void*,void*),void** vetPontos,int tam,void *arquivoSVG){
    ramo *this = (ramo *)a;
    tree_t *b = (tree_t* )x;
    if (this->data == b->nulo)
        return;
    else
        percorreToPoligon(this->left,b,dadosNode,vetPontos,tam,arquivoSVG);
        dadosNode(this->data,vetPontos,tam,b,arquivoSVG,this);
        percorreToPoligon(this->right,b,dadosNode,vetPontos,tam,arquivoSVG);
}
void percorreToPoligonAux(void *a,int (*dadosNode)(void*,void**,int,void*,void*,void*),void** vetPontos,int tam,void *arquivoSVG){
    tree_t *aux = (tree_t *)a;
    percorreToPoligon(aux->root,aux,dadosNode,vetPontos,tam,arquivoSVG);
}